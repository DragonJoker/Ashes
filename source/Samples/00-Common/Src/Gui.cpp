#include "Gui.hpp"

#include "FileUtils.hpp"

#include <Buffer/Buffer.hpp>
#include <Buffer/GeometryBuffers.hpp>
#include <Buffer/VertexBuffer.hpp>
#include <Buffer/StagingBuffer.hpp>
#include <Core/Device.hpp>
#include <Descriptor/DescriptorSet.hpp>
#include <Descriptor/DescriptorSetLayout.hpp>
#include <Descriptor/DescriptorSetPool.hpp>
#include <Image/Texture.hpp>
#include <Miscellaneous/PushConstantRange.hpp>
#include <Pipeline/InputAssemblyState.hpp>
#include <Pipeline/Pipeline.hpp>
#include <Pipeline/PipelineLayout.hpp>
#include <Pipeline/Scissor.hpp>
#include <Pipeline/VertexLayout.hpp>
#include <Pipeline/Viewport.hpp>
#include <RenderPass/RenderPass.hpp>
#include <RenderPass/RenderPassState.hpp>
#include <RenderPass/RenderSubpass.hpp>
#include <RenderPass/RenderSubpassState.hpp>
#include <Shader/ShaderProgram.hpp>

#include <varargs.h>

namespace common
{
	namespace
	{
		renderer::PushConstantArray doCreateConstants()
		{
			return
			{
				{ 0u, 0u, renderer::AttributeFormat::eVec2f },
				{ 1u, 8u, renderer::AttributeFormat::eVec2f },
			};
		}
	}

	Gui::Gui( renderer::Device const & device
		, renderer::UIVec2 const & size
		, renderer::PixelFormat colourFormat
		, renderer::PixelFormat depthFormat
		, std::vector< renderer::FrameBufferPtr > const & frameBuffers )
		: m_device{ device }
		, m_frameBuffers{ frameBuffers }
		, m_size{ size }
		, m_pushConstants{ renderer::ShaderStageFlag::eVertex, doCreateConstants() }
		, m_colourFormat{ colourFormat }
		, m_depthFormat{ depthFormat }
	{
		// Init ImGui
		// Color scheme
		ImGuiStyle & style = ImGui::GetStyle();
		style.Colors[ImGuiCol_TitleBg] = ImVec4( 1.0f, 0.0f, 0.0f, 1.0f );
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4( 1.0f, 0.0f, 0.0f, 1.0f );
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4( 1.0f, 0.0f, 0.0f, 0.1f );
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4( 1.0f, 0.0f, 0.0f, 0.4f );
		style.Colors[ImGuiCol_Header] = ImVec4( 0.8f, 0.0f, 0.0f, 0.4f );
		style.Colors[ImGuiCol_HeaderActive] = ImVec4( 1.0f, 0.0f, 0.0f, 0.4f );
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4( 1.0f, 0.0f, 0.0f, 0.4f );
		style.Colors[ImGuiCol_CheckMark] = ImVec4( 1.0f, 0.0f, 0.0f, 0.8f );
		// Dimensions
		ImGuiIO & io = ImGui::GetIO();
		io.DisplaySize = ImVec2( float( size[0] ), float( size[1] ) );
		io.FontGlobalScale = 1.0f;

		m_cmdBuffers.resize( frameBuffers.size() );

		doPrepareResources();
		doPreparePipeline();
	}

	void Gui::update()
	{
		ImDrawData * imDrawData = ImGui::GetDrawData();

		if ( !imDrawData )
		{
			return;
		}

		bool updateCmdBuffers = false;
		auto vertexBufferSize = uint32_t( imDrawData->TotalVtxCount );

		if ( !m_vertexBuffer || m_vertexCount < vertexBufferSize )
		{
			if ( m_vertexBuffer )
			{
				m_vertexBuffer->unlock( m_vertexCount, true );
			}

			m_vertexBuffer.reset();
			m_vertexCount = vertexBufferSize;
			m_vertexBuffer = renderer::makeVertexBuffer< ImDrawVert >( m_device
				, m_vertexCount
				, renderer::BufferTargets{ 0u }
				, renderer::MemoryPropertyFlag::eHostVisible );

			if ( auto buffer = m_vertexBuffer->lock( 0u
				, m_vertexCount
				, renderer::MemoryMapFlag::eInvalidateRange | renderer::MemoryMapFlag::eWrite ) )
			{
				for ( int n = 0; n < imDrawData->CmdListsCount; n++ )
				{
					const ImDrawList * cmdList = imDrawData->CmdLists[n];
					memcpy( buffer, cmdList->VtxBuffer.Data, cmdList->VtxBuffer.Size * sizeof( ImDrawVert ) );
					buffer += cmdList->VtxBuffer.Size;
				}

				m_vertexBuffer->unlock( m_vertexCount, true );
			}

			updateCmdBuffers = true;
		}

		auto indexBufferSize = uint32_t( imDrawData->TotalIdxCount );

		if ( !m_indexBuffer || m_indexCount < indexBufferSize )
		{
			if ( m_indexBuffer )
			{
				m_indexBuffer->unlock( m_indexCount, true );
			}

			m_indexBuffer.reset();
			m_indexCount = indexBufferSize;
			m_indexBuffer = renderer::makeBuffer< ImDrawIdx >( m_device
				, m_indexCount
				, renderer::BufferTarget::eIndexBuffer
				, renderer::MemoryPropertyFlag::eHostVisible );

			if ( auto buffer = m_indexBuffer->lock( 0u
				, m_indexCount
				, renderer::MemoryMapFlag::eInvalidateRange | renderer::MemoryMapFlag::eWrite ) )
			{
				for ( int n = 0; n < imDrawData->CmdListsCount; n++ )
				{
					const ImDrawList * cmdList = imDrawData->CmdLists[n];
					memcpy( buffer, cmdList->IdxBuffer.Data, cmdList->IdxBuffer.Size * sizeof( ImDrawIdx ) );
					buffer += cmdList->IdxBuffer.Size;
				}

				m_indexBuffer->unlock( m_indexCount, true );
			}

			updateCmdBuffers = true;
		}

		if ( updateCmdBuffers )
		{
			doUpdateCommandBuffers();
		}
	}

	void Gui::resize( renderer::UIVec2 const & size )
	{
		ImGuiIO & io = ImGui::GetIO();
		io.DisplaySize = ImVec2( float( size[0] ), float( size[1] ) );
		m_size = size;
		doUpdateCommandBuffers();
	}

	void Gui::submit( renderer::Queue & queue
		, uint32_t bufferindex )
	{
		queue.submit( *m_cmdBuffers[bufferindex]
			, m_fence.get() );
		m_fence->wait( renderer::FenceTimeout );
		m_fence->reset();
	}

	bool Gui::header( const char *caption )
	{
		return ImGui::CollapsingHeader( caption, ImGuiTreeNodeFlags_DefaultOpen );
	}

	bool Gui::checkBox( const char *caption, bool *value )
	{
		return ImGui::Checkbox( caption, value );
	}

	bool Gui::checkBox( const char *caption, int32_t *value )
	{
		bool val = ( *value == 1 );
		bool res = ImGui::Checkbox( caption, &val );
		*value = val;
		return res;
	}

	bool Gui::inputFloat( const char *caption, float *value, float step, uint32_t precision )
	{
		return ImGui::InputFloat( caption, value, step, step * 10.0f, precision );
	}

	bool Gui::sliderFloat( const char* caption, float* value, float min, float max )
	{
		return ImGui::SliderFloat( caption, value, min, max );
	}

	bool Gui::sliderInt( const char* caption, int32_t* value, int32_t min, int32_t max )
	{
		return ImGui::SliderInt( caption, value, min, max );
	}

	bool Gui::comboBox( const char *caption, int32_t *itemindex, std::vector<std::string> items )
	{
		if ( items.empty() )
		{
			return false;
		}

		std::vector< char const * > charitems;
		charitems.reserve( items.size() );
		for ( auto & item : items )
		{
			charitems.push_back( item.c_str() );
		}

		auto itemCount = uint32_t( charitems.size() );
		return ImGui::Combo( caption, itemindex, &charitems[0], itemCount, itemCount );
	}

	bool Gui::button( const char *caption )
	{
		return ImGui::Button( caption );
	}

	void Gui::text( const char *formatstr, ... )
	{
		va_list args;
		va_start( args, formatstr );
		ImGui::TextV( formatstr, args );
		va_end( args );
	}

	void Gui::doPrepareResources()
	{
		ImGuiIO & io = ImGui::GetIO();

		// Create font texture
		unsigned char* fontData;
		int texWidth, texHeight;
		io.Fonts->GetTexDataAsRGBA32( &fontData, &texWidth, &texHeight );
		auto uploadSize = uint32_t( texWidth * texHeight * 4u * sizeof( char ) );

		m_fontImage = m_device.createTexture();
		m_fontImage->setImage( utils::PixelFormat::eR8G8B8A8
			, renderer::UIVec2{ uint32_t( texWidth ), uint32_t( texHeight ) }
		, renderer::ImageUsageFlag::eSampled | renderer::ImageUsageFlag::eTransferDst );
		m_fontView = m_fontImage->createView( m_fontImage->getType()
			, m_fontImage->getFormat() );

		auto copyCmd = m_device.getGraphicsCommandPool().createCommandBuffer();
		renderer::StagingBuffer stagingBuffer{ m_device
			, renderer::BufferTarget::eTransferSrc
			, 10000000u };
		stagingBuffer.uploadTextureData( *copyCmd
			, fontData
			, uploadSize
			, *m_fontView );

		m_sampler = m_device.createSampler( renderer::WrapMode::eClampToEdge
			, renderer::WrapMode::eClampToEdge
			, renderer::WrapMode::eClampToEdge
			, renderer::Filter::eLinear
			, renderer::Filter::eLinear
			, renderer::MipmapMode::eNone );

		m_commandPool = m_device.createCommandPool( m_device.getGraphicsQueue().getFamilyIndex()
			, renderer::CommandPoolCreateFlag::eResetCommandBuffer );

		for ( auto & commandBuffer : m_cmdBuffers )
		{
			commandBuffer = m_commandPool->createCommandBuffer();
		}

		renderer::DescriptorSetLayoutBindingArray bindings
		{
			{ 0u, renderer::DescriptorType::eCombinedImageSampler, renderer::ShaderStageFlag::eFragment }
		};
		m_descriptorSetLayout = m_device.createDescriptorSetLayout( std::move( bindings ) );
		m_descriptorPool = m_descriptorSetLayout->createPool( 2u );
		m_descriptorSet = m_descriptorPool->createDescriptorSet();

		renderer::PushConstantRange range{ renderer::ShaderStageFlag::eVertex, 0u, m_pushConstants.getSize() };
		m_pipelineLayout = m_device.createPipelineLayout( *m_descriptorSetLayout
			, range );

		m_fence = m_device.createFence();
	}

	void Gui::doPreparePipeline()
	{
		m_vertexLayout = renderer::makeLayout< ImDrawVert >( m_device, 0u );
		m_vertexLayout->createAttribute( 0u, renderer::AttributeFormat::eVec2f, offsetof( ImDrawVert, pos ) );
		m_vertexLayout->createAttribute( 1u, renderer::AttributeFormat::eVec2f, offsetof( ImDrawVert, uv ) );
		m_vertexLayout->createAttribute( 2u, renderer::AttributeFormat::eColour, offsetof( ImDrawVert, col ) );
		
		std::vector< renderer::PixelFormat > formats{ { m_colourFormat } };
		renderer::RenderPassAttachmentArray attaches
		{
			{ m_colourFormat, false },
		};
		renderer::RenderSubpassPtrArray subpasses;
		subpasses.emplace_back( m_device.createRenderSubpass( formats
			, { renderer::PipelineStageFlag::eColourAttachmentOutput, renderer::AccessFlag::eColourAttachmentRead | renderer::AccessFlag::eColourAttachmentWrite } ) );
		m_renderPass = m_device.createRenderPass( attaches
			, subpasses
			, renderer::RenderPassState{ renderer::PipelineStageFlag::eBottomOfPipe
				, renderer::AccessFlag::eMemoryRead
				, { renderer::ImageLayout::eColourAttachmentOptimal } }
			, renderer::RenderPassState{ renderer::PipelineStageFlag::eBottomOfPipe
				, renderer::AccessFlag::eMemoryRead
				, { renderer::ImageLayout::eColourAttachmentOptimal } } );

		std::string shadersFolder = getPath( getExecutableDirectory() ) / "share" / "Sample-00-Common" / "Shaders";
		m_program = m_device.createShaderProgram();
		m_program->createModule( dumpTextFile( shadersFolder / "gui.vert" ), renderer::ShaderStageFlag::eVertex );
		m_program->createModule( dumpTextFile( shadersFolder / "gui.frag" ), renderer::ShaderStageFlag::eFragment );

		m_pipeline = m_pipelineLayout->createPipeline( *m_program
			, { *m_vertexLayout }
			, *m_renderPass
			, { renderer::PrimitiveTopology::eTriangleList } );
	}

	void Gui::doUpdateCommandBuffers()
	{
		size_t index = 0u;
		ImGuiIO & io = ImGui::GetIO();
		m_pushConstants.getData()->scale = renderer::Vec2{ 2.0f / io.DisplaySize.x, 2.0f / io.DisplaySize.y };
		m_pushConstants.getData()->translate = renderer::Vec2{ -1.0f };

		for ( auto & commandBuffer : m_cmdBuffers )
		{
			if ( commandBuffer->begin() )
			{
				commandBuffer->beginRenderPass( *m_renderPass
					, *m_frameBuffers[index]
					, { renderer::RgbaColour{ 1.0, 1.0, 1.0, 1.0 } }
					, renderer::SubpassContents::eInline );
				commandBuffer->bindPipeline( *m_pipeline );
				commandBuffer->bindDescriptorSet( *m_descriptorSet
					, *m_pipelineLayout );
				commandBuffer->bindGeometryBuffers( *m_geometryBuffers );
				commandBuffer->setViewport( { uint32_t( ImGui::GetIO().DisplaySize.x )
					, uint32_t( ImGui::GetIO().DisplaySize.y )
					, 0
					, 0 } );
				commandBuffer->setScissor( { 0
					, 0 
					, uint32_t( ImGui::GetIO().DisplaySize.x )
					, uint32_t( ImGui::GetIO().DisplaySize.y ) } );
				commandBuffer->pushConstants( *m_pipelineLayout, m_pushConstants );
				ImDrawData * imDrawData = ImGui::GetDrawData();
				int32_t vertexOffset = 0;
				int32_t indexOffset = 0;

				for ( int32_t j = 0; j < imDrawData->CmdListsCount; j++ )
				{
					ImDrawList const * cmdList = imDrawData->CmdLists[j];

					for ( int32_t k = 0; k < cmdList->CmdBuffer.Size; k++ )
					{
						ImDrawCmd const * cmd = &cmdList->CmdBuffer[k];
						commandBuffer->setScissor( {
							std::max( int32_t( cmd->ClipRect.x ), 0 ),
							std::max( int32_t( cmd->ClipRect.y ), 0 ),
							uint32_t( cmd->ClipRect.z - cmd->ClipRect.x ),
							uint32_t( cmd->ClipRect.w - cmd->ClipRect.y ),
						} );
						commandBuffer->drawIndexed( cmd->ElemCount, 1u, indexOffset, vertexOffset );
						indexOffset += cmd->ElemCount;
					}

					vertexOffset += cmdList->VtxBuffer.Size;
				}

				commandBuffer->endRenderPass();
				commandBuffer->end();
			}
		}
	}
}

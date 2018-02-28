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
#include <Image/TextureView.hpp>
#include <Miscellaneous/PushConstantRange.hpp>
#include <Pipeline/DepthStencilState.hpp>
#include <Pipeline/InputAssemblyState.hpp>
#include <Pipeline/MultisampleState.hpp>
#include <Pipeline/Pipeline.hpp>
#include <Pipeline/PipelineLayout.hpp>
#include <Pipeline/Scissor.hpp>
#include <Pipeline/VertexLayout.hpp>
#include <Pipeline/Viewport.hpp>
#include <RenderPass/FrameBuffer.hpp>
#include <RenderPass/FrameBufferAttachment.hpp>
#include <RenderPass/RenderPass.hpp>
#include <RenderPass/RenderSubpass.hpp>
#include <RenderPass/RenderSubpassState.hpp>
#include <Shader/ShaderProgram.hpp>
#include <Sync/BufferMemoryBarrier.hpp>
#include <Sync/ImageMemoryBarrier.hpp>

#include <stdarg.h>

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
		, renderer::UIVec2 const & size )
		: m_device{ device }
		, m_size{ size }
		, m_pushConstants{ renderer::ShaderStageFlag::eVertex, doCreateConstants() }
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

		doPrepareResources();
	}

	void Gui::updateView( renderer::TextureView const & colourView )
	{
		if ( m_colourView != &colourView )
		{
			bool first = m_colourView == nullptr;
			m_colourView = &colourView;
			doPreparePipeline();

			if ( !first )
			{
				doUpdateCommandBuffers();
			}
		}
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

		if ( !m_vertexBuffer || m_vertexCount != vertexBufferSize )
		{
			m_vertexBuffer.reset();
			m_vertexCount = vertexBufferSize;
			m_vertexBuffer = renderer::makeVertexBuffer< ImDrawVert >( m_device
				, m_vertexCount
				, renderer::BufferTargets{ 0u }
				, renderer::MemoryPropertyFlag::eHostVisible );
			updateCmdBuffers = true;
		}

		auto indexBufferSize = uint32_t( imDrawData->TotalIdxCount );

		if ( !m_indexBuffer || m_indexCount < indexBufferSize )
		{
			m_indexBuffer.reset();
			m_indexCount = indexBufferSize;
			m_indexBuffer = renderer::makeBuffer< ImDrawIdx >( m_device
				, m_indexCount
				, renderer::BufferTarget::eIndexBuffer
				, renderer::MemoryPropertyFlag::eHostVisible );
			updateCmdBuffers = true;
		}

		if ( auto vtx = m_vertexBuffer->lock( 0u
			, m_vertexCount
			, renderer::MemoryMapFlag::eInvalidateRange | renderer::MemoryMapFlag::eWrite ) )
		{
			for ( int n = 0; n < imDrawData->CmdListsCount; n++ )
			{
				const ImDrawList * cmdList = imDrawData->CmdLists[n];
				memcpy( vtx, cmdList->VtxBuffer.Data, cmdList->VtxBuffer.Size * sizeof( ImDrawVert ) );
				vtx += cmdList->VtxBuffer.Size;
			}

			m_vertexBuffer->flush( 0u, m_vertexCount );
			m_vertexBuffer->unlock();
		}

		if ( auto idx = m_indexBuffer->lock( 0u
			, m_indexCount
			, renderer::MemoryMapFlag::eInvalidateRange | renderer::MemoryMapFlag::eWrite ) )
		{
			for ( int n = 0; n < imDrawData->CmdListsCount; n++ )
			{
				const ImDrawList * cmdList = imDrawData->CmdLists[n];
				memcpy( idx, cmdList->IdxBuffer.Data, cmdList->IdxBuffer.Size * sizeof( ImDrawIdx ) );
				idx += cmdList->IdxBuffer.Size;
			}

			m_indexBuffer->flush( 0u, m_indexCount );
			m_indexBuffer->unlock();
		}

		if ( updateCmdBuffers )
		{
			m_geometryBuffers = m_device.createGeometryBuffers( *m_vertexBuffer
				, 0u
				, *m_vertexLayout
				, m_indexBuffer->getBuffer()
				, 0u
				, renderer::IndexType::eUInt16 );
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

	void Gui::submit( renderer::Queue const & queue )
	{
		queue.submit( *m_commandBuffer
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

	bool Gui::sliderFloat( char const * caption, float* value, float min, float max )
	{
		return ImGui::SliderFloat( caption, value, min, max );
	}

	bool Gui::sliderInt( char const * caption, int32_t* value, int32_t min, int32_t max )
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
		m_commandBuffer = m_commandPool->createCommandBuffer();

		renderer::DescriptorSetLayoutBindingArray bindings
		{
			{ 0u, renderer::DescriptorType::eCombinedImageSampler, renderer::ShaderStageFlag::eFragment }
		};
		m_descriptorSetLayout = m_device.createDescriptorSetLayout( std::move( bindings ) );
		m_descriptorPool = m_descriptorSetLayout->createPool( 2u );
		m_descriptorSet = m_descriptorPool->createDescriptorSet();
		m_descriptorSet->createBinding( m_descriptorSetLayout->getBinding( 0u )
			, *m_fontView
			, *m_sampler );
		m_descriptorSet->update();

		renderer::PushConstantRange range{ renderer::ShaderStageFlag::eVertex, 0u, m_pushConstants.getSize() };
		m_pipelineLayout = m_device.createPipelineLayout( *m_descriptorSetLayout
			, range );

		m_fence = m_device.createFence();

		std::string shadersFolder = getPath( getExecutableDirectory() ) / "share" / "Sample-00-Common" / "Shaders";
		m_program = m_device.createShaderProgram();
		m_program->createModule( dumpTextFile( shadersFolder / "gui.vert" ), renderer::ShaderStageFlag::eVertex );
		m_program->createModule( dumpTextFile( shadersFolder / "gui.frag" ), renderer::ShaderStageFlag::eFragment );

		m_vertexLayout = renderer::makeLayout< ImDrawVert >( m_device, 0u );
		m_vertexLayout->createAttribute( 0u, renderer::AttributeFormat::eVec2f, offsetof( ImDrawVert, pos ) );
		m_vertexLayout->createAttribute( 1u, renderer::AttributeFormat::eVec2f, offsetof( ImDrawVert, uv ) );
		m_vertexLayout->createAttribute( 2u, renderer::AttributeFormat::eUInt, offsetof( ImDrawVert, col ) );
	}

	void Gui::doPreparePipeline()
	{
		auto dimensions = m_colourView->getTexture().getDimensions();
		auto size = renderer::UIVec2{ dimensions[0], dimensions[1] };
		m_target = m_device.createTexture();
		m_target->setImage( m_colourView->getFormat()
			, size
			, renderer::ImageUsageFlag::eColourAttachment | renderer::ImageUsageFlag::eSampled );
		m_targetView = m_target->createView( renderer::TextureType::e2D
			, m_target->getFormat() );
		
		renderer::RenderPassAttachmentArray rpAttaches
		{
			{
				0u,
				m_targetView->getFormat(),
				renderer::SampleCountFlag::e1,
				renderer::AttachmentLoadOp::eClear,
				renderer::AttachmentStoreOp::eStore,
				renderer::AttachmentLoadOp::eDontCare,
				renderer::AttachmentStoreOp::eDontCare,
				renderer::ImageLayout::eUndefined,
				renderer::ImageLayout::eShaderReadOnlyOptimal,
			}
		};
		renderer::RenderSubpassAttachmentArray subAttaches
		{
			{ 0u, renderer::ImageLayout::eColourAttachmentOptimal }
		};
		renderer::RenderSubpassPtrArray subpasses;
		subpasses.emplace_back( m_device.createRenderSubpass( renderer::PipelineBindPoint::eGraphics
			, {
				renderer::PipelineStageFlag::eColourAttachmentOutput,
				renderer::AccessFlag::eColourAttachmentRead | renderer::AccessFlag::eColourAttachmentWrite
			}
			, subAttaches ) );
		m_renderPass = m_device.createRenderPass( rpAttaches
			, std::move( subpasses )
			, renderer::RenderSubpassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
				, renderer::AccessFlag::eColourAttachmentWrite }
			, renderer::RenderSubpassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
				, renderer::AccessFlag::eShaderRead } );

		renderer::FrameBufferAttachmentArray attaches
		{
			{ *m_renderPass->begin(), *m_targetView }
		};
		m_frameBuffer = m_renderPass->createFrameBuffer( size
			, std::move( attaches ) );

		renderer::ColourBlendState cbState;
		cbState.addAttachment( renderer::ColourBlendStateAttachment
		{
			true,
			renderer::BlendFactor::eSrcAlpha,
			renderer::BlendFactor::eInvSrcAlpha,
			renderer::BlendOp::eAdd,
			renderer::BlendFactor::eSrcAlpha,
			renderer::BlendFactor::eInvSrcAlpha,
			renderer::BlendOp::eAdd
		} );

		m_pipeline = m_pipelineLayout->createPipeline(
		{
			*m_program,
			*m_renderPass,
			{ *m_vertexLayout },
			{ renderer::PrimitiveTopology::eTriangleList },
			{ 1.0f, 0u, false, false, renderer::PolygonMode::eFill, renderer::CullModeFlag::eNone },
			renderer::MultisampleState{},
			cbState,
			renderer::DepthStencilState{},
		} );
	}

	void Gui::doUpdateCommandBuffers()
	{
		size_t index = 0u;
		ImGuiIO & io = ImGui::GetIO();
		m_pushConstants.getData()->scale = renderer::Vec2{ 2.0f / io.DisplaySize.x, 2.0f / io.DisplaySize.y };
		m_pushConstants.getData()->translate = renderer::Vec2{ -1.0f };

		if ( m_commandBuffer->begin() )
		{
			m_commandBuffer->memoryBarrier( renderer::PipelineStageFlag::eTransfer
				, renderer::PipelineStageFlag::eFragmentShader
				, m_fontView->makeShaderInputResource( renderer::ImageLayout::eUndefined
					, 0u ) );
			m_commandBuffer->memoryBarrier( renderer::PipelineStageFlag::eTransfer
				, renderer::PipelineStageFlag::eFragmentShader
				, m_vertexBuffer->getBuffer().makeVertexShaderInputResource() );
			m_commandBuffer->memoryBarrier( renderer::PipelineStageFlag::eTransfer
				, renderer::PipelineStageFlag::eFragmentShader
				, m_indexBuffer->getBuffer().makeVertexShaderInputResource() );
			m_commandBuffer->beginRenderPass( *m_renderPass
				, *m_frameBuffer
				, { renderer::RgbaColour{ 1.0, 1.0, 1.0, 0.0 } }
				, renderer::SubpassContents::eInline );
			m_commandBuffer->bindPipeline( *m_pipeline );
			m_commandBuffer->bindDescriptorSet( *m_descriptorSet
				, *m_pipelineLayout );
			m_commandBuffer->bindGeometryBuffers( *m_geometryBuffers );
			m_commandBuffer->setViewport( { uint32_t( ImGui::GetIO().DisplaySize.x )
				, uint32_t( ImGui::GetIO().DisplaySize.y )
				, 0
				, 0 } );
			m_commandBuffer->setScissor( { 0
				, 0 
				, uint32_t( ImGui::GetIO().DisplaySize.x )
				, uint32_t( ImGui::GetIO().DisplaySize.y ) } );
			m_commandBuffer->pushConstants( *m_pipelineLayout, m_pushConstants );
			ImDrawData * imDrawData = ImGui::GetDrawData();
			int32_t vertexOffset = 0;
			int32_t indexOffset = 0;

			for ( int32_t j = 0; j < imDrawData->CmdListsCount; j++ )
			{
				ImDrawList const * cmdList = imDrawData->CmdLists[j];

				for ( int32_t k = 0; k < cmdList->CmdBuffer.Size; k++ )
				{
					ImDrawCmd const * cmd = &cmdList->CmdBuffer[k];
					m_commandBuffer->setScissor( {
						std::max( int32_t( cmd->ClipRect.x ), 0 ),
						std::max( int32_t( cmd->ClipRect.y ), 0 ),
						uint32_t( cmd->ClipRect.z - cmd->ClipRect.x ),
						uint32_t( cmd->ClipRect.w - cmd->ClipRect.y ),
					} );
					m_commandBuffer->drawIndexed( cmd->ElemCount, 1u, indexOffset, vertexOffset );
					indexOffset += cmd->ElemCount;
				}

				vertexOffset += cmdList->VtxBuffer.Size;
			}

			m_commandBuffer->endRenderPass();
			m_commandBuffer->end();
		}
	}
}

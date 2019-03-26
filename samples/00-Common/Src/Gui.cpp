#include "Gui.hpp"

#include "FileUtils.hpp"

#include <Ashes/Buffer/Buffer.hpp>
#include <Ashes/Buffer/VertexBuffer.hpp>
#include <Ashes/Buffer/StagingBuffer.hpp>
#include <Ashes/Core/Device.hpp>
#include <Ashes/Descriptor/DescriptorSet.hpp>
#include <Ashes/Descriptor/DescriptorSetLayout.hpp>
#include <Ashes/Descriptor/DescriptorSetPool.hpp>
#include <Ashes/Image/StagingTexture.hpp>
#include <Ashes/Image/Image.hpp>
#include <Ashes/Image/ImageView.hpp>
#include <Ashes/Miscellaneous/PushConstantRange.hpp>
#include <Ashes/Pipeline/DepthStencilState.hpp>
#include <Ashes/Pipeline/InputAssemblyState.hpp>
#include <Ashes/Pipeline/MultisampleState.hpp>
#include <Ashes/Pipeline/Pipeline.hpp>
#include <Ashes/Pipeline/PipelineLayout.hpp>
#include <Ashes/Pipeline/Scissor.hpp>
#include <Ashes/Pipeline/VertexInputState.hpp>
#include <Ashes/Pipeline/VertexLayout.hpp>
#include <Ashes/Pipeline/Viewport.hpp>
#include <Ashes/RenderPass/FrameBuffer.hpp>
#include <Ashes/RenderPass/FrameBufferAttachment.hpp>
#include <Ashes/RenderPass/RenderPass.hpp>
#include <Ashes/RenderPass/RenderSubpass.hpp>
#include <Ashes/RenderPass/RenderSubpassState.hpp>
#include <Ashes/Sync/BufferMemoryBarrier.hpp>
#include <Ashes/Sync/ImageMemoryBarrier.hpp>

#include <Utils/GlslToSpv.hpp>

#include <stdarg.h>

namespace common
{
	namespace
	{
		ashes::PushConstantArray doCreateConstants()
		{
			return
			{
				{ 0u, ashes::ConstantFormat::eVec2f },
				{ 8u, ashes::ConstantFormat::eVec2f },
			};
		}
	}

	Gui::Gui( utils::Device const & device
		, ashes::Queue const & queue
		, ashes::CommandPool const & commandPool
		, ashes::Extent2D const & size )
		: m_device{ device }
		, m_size{ size }
		, m_pushConstants{ ashes::ShaderStageFlag::eVertex, doCreateConstants() }
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
		io.DisplaySize = ImVec2( float( size.width ), float( size.height ) );
		io.FontGlobalScale = 1.0f;

		doPrepareResources( queue, commandPool );
	}

	void Gui::updateView( ashes::ImageViewPtr colourView )
	{
		if ( m_colourView != colourView )
		{
			bool first = m_colourView == nullptr;
			m_colourView = colourView;
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
			m_vertexBuffer = utils::makeVertexBuffer< ImDrawVert >( m_device
				, m_vertexCount
				, ashes::BufferTargets{ 0u }
				, ashes::MemoryPropertyFlag::eHostVisible );
			updateCmdBuffers = true;
		}

		auto indexBufferSize = uint32_t( imDrawData->TotalIdxCount );

		if ( !m_indexBuffer || m_indexCount < indexBufferSize )
		{
			m_indexBuffer.reset();
			m_indexCount = indexBufferSize;
			m_indexBuffer = utils::makeBuffer< ImDrawIdx >( m_device
				, m_indexCount
				, ashes::BufferTarget::eIndexBuffer
				, ashes::MemoryPropertyFlag::eHostVisible );
			updateCmdBuffers = true;
		}

		if ( auto vtx = m_vertexBuffer->lock( 0u
			, m_vertexCount
			, ashes::MemoryMapFlag::eInvalidateRange | ashes::MemoryMapFlag::eWrite ) )
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
			, ashes::MemoryMapFlag::eInvalidateRange | ashes::MemoryMapFlag::eWrite ) )
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
			doUpdateCommandBuffers();
		}
	}

	void Gui::resize( ashes::Extent2D const & size )
	{
		ImGuiIO & io = ImGui::GetIO();
		io.DisplaySize = ImVec2( float( size.width ), float( size.height ) );
		m_size = size;
		doUpdateCommandBuffers();
	}

	void Gui::submit( ashes::Queue const & queue )
	{
		queue.submit( *m_commandBuffer
			, m_fence.get() );
		m_fence->wait( ashes::FenceTimeout );
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

	void Gui::doPrepareResources( ashes::Queue const & queue
		, ashes::CommandPool const & commandPool )
	{
		ImGuiIO & io = ImGui::GetIO();

		// Create font texture
		unsigned char* fontData;
		int texWidth, texHeight;
		io.Fonts->GetTexDataAsRGBA32( &fontData, &texWidth, &texHeight );
		auto uploadSize = uint32_t( texWidth * texHeight * 4u * sizeof( char ) );

		m_fontImage = m_device.createImage(
			{
				0u,
				ashes::ImageType::e2D,
				ashes::Format::eR8G8B8A8_UNORM,
				ashes::Extent3D{ uint32_t( texWidth ), uint32_t( texHeight ), 1u },
				1u,
				1u,
				ashes::SampleCountFlag::e1,
				ashes::ImageTiling::eOptimal,
				ashes::ImageUsageFlag::eSampled | ashes::ImageUsageFlag::eTransferDst
			}
			, ashes::MemoryPropertyFlag::eDeviceLocal );
		m_fontView = m_fontImage->createView( ashes::ImageViewType( m_fontImage->getType() )
			, m_fontImage->getFormat() );

		auto copyCmd = commandPool.createCommandBuffer();
		auto stagingTexture = m_device.getDevice().createStagingTexture( ashes::Format::eR8G8B8A8_UNORM
			, { uint32_t( texWidth ), uint32_t( texHeight ) } );
		stagingTexture->uploadTextureData( queue
			, commandPool
			, ashes::Format::eR8G8B8A8_UNORM
			, fontData
			, *m_fontView );

		m_sampler = m_device.getDevice().createSampler( ashes::WrapMode::eClampToEdge
			, ashes::WrapMode::eClampToEdge
			, ashes::WrapMode::eClampToEdge
			, ashes::Filter::eLinear
			, ashes::Filter::eLinear
			, ashes::MipmapMode::eNone );

		m_commandPool = m_device.getDevice().createCommandPool( queue.getFamilyIndex()
			, ashes::CommandPoolCreateFlag::eResetCommandBuffer );
		m_commandBuffer = m_commandPool->createCommandBuffer();

		ashes::DescriptorSetLayoutBindingArray bindings
		{
			{ 0u, ashes::DescriptorType::eCombinedImageSampler, ashes::ShaderStageFlag::eFragment }
		};
		m_descriptorSetLayout = m_device.getDevice().createDescriptorSetLayout( std::move( bindings ) );
		m_descriptorPool = m_descriptorSetLayout->createPool( 2u );
		m_descriptorSet = m_descriptorPool->createDescriptorSet();
		m_descriptorSet->createBinding( m_descriptorSetLayout->getBinding( 0u )
			, *m_fontView
			, *m_sampler );
		m_descriptorSet->update();

		ashes::PushConstantRange range{ ashes::ShaderStageFlag::eVertex, 0u, m_pushConstants.getSize() };
		m_pipelineLayout = m_device.getDevice().createPipelineLayout( *m_descriptorSetLayout
			, range );

		m_fence = m_device.getDevice().createFence();

		m_vertexLayout = ashes::makeLayout< ImDrawVert >( 0u );
		m_vertexLayout->createAttribute( 0u
			, ashes::Format::eR32G32_SFLOAT
			, offsetof( ImDrawVert, pos ) );
		m_vertexLayout->createAttribute( 1u
			, ashes::Format::eR32G32_SFLOAT
			, offsetof( ImDrawVert, uv ) );
		m_vertexLayout->createAttribute( 2u
			, ashes::Format::eR32_UINT
			, offsetof( ImDrawVert, col ) );
	}

	void Gui::doPreparePipeline()
	{
		auto dimensions = m_colourView->getImage().getDimensions();
		auto size = ashes::Extent2D{ dimensions.width, dimensions.height };
		m_target = m_device.createImage(
			{
				0u,
				ashes::ImageType::e2D,
				m_colourView->getFormat(),
				dimensions,
				1u,
				1u,
				ashes::SampleCountFlag::e1,
				ashes::ImageTiling::eOptimal,
				ashes::ImageUsageFlag::eColourAttachment | ashes::ImageUsageFlag::eSampled | ashes::ImageUsageFlag::eTransferDst
			}
			, ashes::MemoryPropertyFlag::eDeviceLocal );
		m_targetView = m_target->createView( ashes::ImageViewType::e2D
			, m_target->getFormat() );
		
		ashes::AttachmentDescriptionArray rpAttaches
		{
			{
				m_targetView->getFormat(),
				ashes::SampleCountFlag::e1,
				ashes::AttachmentLoadOp::eClear,
				ashes::AttachmentStoreOp::eStore,
				ashes::AttachmentLoadOp::eDontCare,
				ashes::AttachmentStoreOp::eDontCare,
				ashes::ImageLayout::eUndefined,
				ashes::ImageLayout::eShaderReadOnlyOptimal,
			}
		};
		ashes::AttachmentReferenceArray subAttaches
		{
			{ 0u, ashes::ImageLayout::eColourAttachmentOptimal }
		};
		ashes::RenderSubpassPtrArray subpasses;
		subpasses.emplace_back( std::make_unique< ashes::RenderSubpass >( ashes::PipelineBindPoint::eGraphics
			, ashes::RenderSubpassState{
				ashes::PipelineStageFlag::eColourAttachmentOutput,
				ashes::AccessFlag::eColourAttachmentRead | ashes::AccessFlag::eColourAttachmentWrite
			}
			, subAttaches ) );
		m_renderPass = m_device.getDevice().createRenderPass( rpAttaches
			, std::move( subpasses )
			, ashes::RenderSubpassState{ ashes::PipelineStageFlag::eColourAttachmentOutput
				, ashes::AccessFlag::eColourAttachmentWrite }
			, ashes::RenderSubpassState{ ashes::PipelineStageFlag::eFragmentShader
				, ashes::AccessFlag::eShaderRead } );

		ashes::FrameBufferAttachmentArray attaches
		{
			{ *m_renderPass->getAttachments().begin(), m_targetView }
		};
		m_frameBuffer = m_renderPass->createFrameBuffer( size
			, std::move( attaches ) );

		ashes::ColourBlendState cbState;
		cbState.attachs.push_back( ashes::ColourBlendStateAttachment
		{
			true,
			ashes::BlendFactor::eSrcAlpha,
			ashes::BlendFactor::eInvSrcAlpha,
			ashes::BlendOp::eAdd,
			ashes::BlendFactor::eSrcAlpha,
			ashes::BlendFactor::eInvSrcAlpha,
			ashes::BlendOp::eAdd
		} );

		std::string shadersFolder = utils::getPath( utils::getExecutableDirectory() ) / "share" / "Sample-00-Common" / "Shaders";
		std::vector< ashes::ShaderStageState > shaderStages;
		shaderStages.push_back( { m_device.getDevice().createShaderModule( ashes::ShaderStageFlag::eVertex ) } );
		shaderStages.push_back( { m_device.getDevice().createShaderModule( ashes::ShaderStageFlag::eFragment ) } );
		shaderStages[0].module->loadShader( dumpShaderFile( m_device.getDevice(), ashes::ShaderStageFlag::eVertex, shadersFolder / "gui.vert" ) );
		shaderStages[1].module->loadShader( dumpShaderFile( m_device.getDevice(), ashes::ShaderStageFlag::eFragment, shadersFolder / "gui.frag" ) );

		std::vector< ashes::DynamicStateEnable > dynamicStateEnables
		{
			ashes::DynamicStateEnable::eViewport,
			ashes::DynamicStateEnable::eScissor
		};

		ashes::RasterisationState rasterisationState;
		rasterisationState.cullMode = ashes::CullModeFlag::eNone;

		m_pipeline = m_pipelineLayout->createPipeline(
		{
			std::move( shaderStages ),
			*m_renderPass,
			ashes::VertexInputState::create( *m_vertexLayout ),
			ashes::InputAssemblyState{ ashes::PrimitiveTopology::eTriangleList },
			rasterisationState,
			ashes::MultisampleState{},
			cbState,
			dynamicStateEnables,
			ashes::DepthStencilState{},
		} );
	}

	void Gui::doUpdateCommandBuffers()
	{
		size_t index = 0u;
		ImGuiIO & io = ImGui::GetIO();
		m_pushConstants.getData()->scale = utils::Vec2{ 2.0f / io.DisplaySize.x, 2.0f / io.DisplaySize.y };
		m_pushConstants.getData()->translate = utils::Vec2{ -1.0f };

		m_commandBuffer->begin();
		m_commandBuffer->memoryBarrier( ashes::PipelineStageFlag::eTransfer
			, ashes::PipelineStageFlag::eFragmentShader
			, m_fontView->makeShaderInputResource( ashes::ImageLayout::eUndefined
				, 0u ) );
		m_commandBuffer->memoryBarrier( ashes::PipelineStageFlag::eTransfer
			, ashes::PipelineStageFlag::eVertexInput
			, m_vertexBuffer->getBuffer().makeVertexShaderInputResource() );
		m_commandBuffer->memoryBarrier( ashes::PipelineStageFlag::eTransfer
			, ashes::PipelineStageFlag::eVertexInput
			, m_indexBuffer->getBuffer().makeVertexShaderInputResource() );
		m_commandBuffer->beginRenderPass( *m_renderPass
			, *m_frameBuffer
			, { ashes::ClearColorValue{ 1.0, 1.0, 1.0, 0.0 } }
			, ashes::SubpassContents::eInline );
		m_commandBuffer->bindPipeline( *m_pipeline );
		m_commandBuffer->bindDescriptorSet( *m_descriptorSet
			, *m_pipelineLayout );
		m_commandBuffer->bindVertexBuffer( 0u, m_vertexBuffer->getBuffer(), 0u );
		m_commandBuffer->bindIndexBuffer( m_indexBuffer->getBuffer(), 0u, ashes::IndexType::eUInt16 );
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
		m_commandBuffer->memoryBarrier( ashes::PipelineStageFlag::eVertexInput
			, ashes::PipelineStageFlag::eTransfer
			, m_vertexBuffer->getBuffer().makeTransferDestination() );
		m_commandBuffer->memoryBarrier( ashes::PipelineStageFlag::eVertexInput
			, ashes::PipelineStageFlag::eTransfer
			, m_indexBuffer->getBuffer().makeTransferDestination() );
		m_commandBuffer->end();
	}
}

#include "Gui.hpp"

#include "FileUtils.hpp"

#include <ashespp/Buffer/Buffer.hpp>
#include <ashespp/Buffer/VertexBuffer.hpp>
#include <ashespp/Buffer/StagingBuffer.hpp>
#include <ashespp/Core/Device.hpp>
#include <ashespp/Descriptor/DescriptorSet.hpp>
#include <ashespp/Descriptor/DescriptorSetLayout.hpp>
#include <ashespp/Descriptor/DescriptorSetPool.hpp>
#include <ashespp/Image/Image.hpp>
#include <ashespp/Image/ImageView.hpp>
#include <ashespp/Pipeline/GraphicsPipeline.hpp>
#include <ashespp/Pipeline/PipelineLayout.hpp>
#include <ashespp/RenderPass/FrameBuffer.hpp>
#include <ashespp/RenderPass/RenderPass.hpp>

#include <util/GlslToSpv.hpp>

#include <stdarg.h>

namespace common
{
	namespace
	{
		ashes::PushConstantArray doCreateConstants()
		{
			return
			{
				{ 0u, VK_FORMAT_R32G32_SFLOAT },
				{ 8u, VK_FORMAT_R32G32_SFLOAT },
			};
		}
	}

	Gui::Gui( utils::Device const & device
		, ashes::Queue const & queue
		, ashes::CommandPool const & commandPool
		, VkExtent2D const & size )
		: m_device{ device }
		, m_size{ size }
		, m_pushConstants{ VK_SHADER_STAGE_VERTEX_BIT, doCreateConstants() }
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

	void Gui::updateView( ashes::ImageView colourView )
	{
		if ( m_colourView != colourView )
		{
			bool first = m_colourView.internal == VK_NULL_HANDLE;
			m_colourView = std::move( colourView );
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
				, 0u
				, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT );
			updateCmdBuffers = true;
		}

		auto indexBufferSize = uint32_t( imDrawData->TotalIdxCount );

		if ( !m_indexBuffer || m_indexCount < indexBufferSize )
		{
			m_indexBuffer.reset();
			m_indexCount = indexBufferSize;
			m_indexBuffer = utils::makeBuffer< ImDrawIdx >( m_device
				, m_indexCount
				, VK_BUFFER_USAGE_INDEX_BUFFER_BIT
				, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT );
			updateCmdBuffers = true;
		}

		if ( auto vtx = m_vertexBuffer->lock( 0u
			, m_vertexCount
			, 0u ) )
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
			, 0u ) )
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

	void Gui::resize( VkExtent2D const & size )
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
		m_fence->wait( ashes::MaxTimeout );
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
				VK_IMAGE_TYPE_2D,
				VK_FORMAT_R8G8B8A8_UNORM,
				VkExtent3D{ uint32_t( texWidth ), uint32_t( texHeight ), 1u },
				1u,
				1u,
				VK_SAMPLE_COUNT_1_BIT,
				VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT
			}
			, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
		m_fontView = m_fontImage->createView( VkImageViewType( m_fontImage->getType() )
			, m_fontImage->getFormat() );

		auto copyCmd = commandPool.createCommandBuffer();
		auto staging = ashes::StagingBuffer{ m_device, 0u };
		staging.uploadTextureData( queue
			, commandPool
			, VK_FORMAT_R8G8B8A8_UNORM
			, fontData
			, m_fontView );

		m_sampler = m_device.getDevice().createSampler( VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
			, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
			, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
			, VK_FILTER_LINEAR
			, VK_FILTER_LINEAR
			, VK_SAMPLER_MIPMAP_MODE_NEAREST );

		m_commandPool = m_device.getDevice().createCommandPool( queue.getFamilyIndex()
			, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT );
		m_commandBuffer = m_commandPool->createCommandBuffer();

		ashes::VkDescriptorSetLayoutBindingArray bindings
		{
			{ 0u, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1u, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr }
		};
		m_descriptorSetLayout = m_device.getDevice().createDescriptorSetLayout( std::move( bindings ) );
		m_descriptorPool = m_descriptorSetLayout->createPool( 2u );
		m_descriptorSet = m_descriptorPool->createDescriptorSet();
		m_descriptorSet->createBinding( m_descriptorSetLayout->getBinding( 0u )
			, m_fontView
			, *m_sampler );
		m_descriptorSet->update();

		VkPushConstantRange range{ VK_SHADER_STAGE_VERTEX_BIT, 0u, m_pushConstants.getSize() };
		m_pipelineLayout = m_device.getDevice().createPipelineLayout( *m_descriptorSetLayout
			, range );

		m_fence = m_device.getDevice().createFence();
	}

	void Gui::doPreparePipeline()
	{
		auto dimensions = m_colourView.image->getDimensions();
		auto size = VkExtent2D{ dimensions.width, dimensions.height };
		m_target = m_device.createImage(
			{
				0u,
				VK_IMAGE_TYPE_2D,
				m_colourView.createInfo.format,
				dimensions,
				1u,
				1u,
				VK_SAMPLE_COUNT_1_BIT,
				VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT
			}
			, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
		m_targetView = m_target->createView( VK_IMAGE_VIEW_TYPE_2D
			, m_target->getFormat() );

		ashes::VkAttachmentDescriptionArray rpAttaches
		{
			{
				0u,
				m_targetView.createInfo.format,
				VK_SAMPLE_COUNT_1_BIT,
				VK_ATTACHMENT_LOAD_OP_CLEAR,
				VK_ATTACHMENT_STORE_OP_STORE,
				VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				VK_ATTACHMENT_STORE_OP_DONT_CARE,
				VK_IMAGE_LAYOUT_UNDEFINED,
				VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
			}
		};
		ashes::SubpassDescriptionArray subpasses;
		subpasses.emplace_back( ashes::SubpassDescription
			{
				0u,
				VK_PIPELINE_BIND_POINT_GRAPHICS,
				{},
				{ { 0u, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL } },
				{},
				std::nullopt,
				{},
			} );
		ashes::VkSubpassDependencyArray dependencies
		{
			{
				VK_SUBPASS_EXTERNAL,
				0u,
				VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
				VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
				VK_ACCESS_MEMORY_READ_BIT,
				VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
				VK_DEPENDENCY_BY_REGION_BIT,
			},
			{
				0u,
				VK_SUBPASS_EXTERNAL,
				VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
				VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
				VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
				VK_ACCESS_SHADER_READ_BIT,
				VK_DEPENDENCY_BY_REGION_BIT,
			}
		};
		ashes::RenderPassCreateInfo createInfo
		{
			0u,
			std::move( rpAttaches ),
			std::move( subpasses ),
			std::move( dependencies ),
		};
		m_renderPass = m_device->createRenderPass( std::move( createInfo ) );

		ashes::ImageViewArray attaches
		{
			m_targetView
		};
		m_frameBuffer = m_renderPass->createFrameBuffer( size
			, std::move( attaches ) );

		ashes::PipelineColorBlendStateCreateInfo cbState
		{
			0u,
			VK_FALSE,
			VK_LOGIC_OP_COPY,
			{
				{
					VK_TRUE,
					VK_BLEND_FACTOR_SRC_ALPHA,
					VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
					VK_BLEND_OP_ADD,
					VK_BLEND_FACTOR_SRC_ALPHA,
					VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
					VK_BLEND_OP_ADD
				}
			}
		};

		ashes::PipelineVertexInputStateCreateInfo vertexLayout
		{
			0u,
			{
				{ 0u, sizeof( ImDrawVert ), VK_VERTEX_INPUT_RATE_VERTEX },
			},
			{
				{ 0u, 0u, VK_FORMAT_R32G32_SFLOAT, offsetof( ImDrawVert, pos ) },
				{ 1u, 0u, VK_FORMAT_R32G32_SFLOAT, offsetof( ImDrawVert, uv ) },
				{ 2u, 0u, VK_FORMAT_R32_UINT, offsetof( ImDrawVert, col ) },
			},
		};

		std::string shadersFolder = ashes::getPath( ashes::getExecutableDirectory() ) / "share" / "Sample-00-Common" / "Shaders";
		ashes::PipelineShaderStageCreateInfoArray shaderStages;
		shaderStages.push_back( ashes::PipelineShaderStageCreateInfo
			{
				0u,
				VK_SHADER_STAGE_VERTEX_BIT,
				m_device->createShaderModule( dumpShaderFile( m_device
					, VK_SHADER_STAGE_VERTEX_BIT
					, shadersFolder / "gui.vert" ) ),
				"main",
				std::nullopt,
			} );
		shaderStages.push_back( ashes::PipelineShaderStageCreateInfo
			{
				0u,
				VK_SHADER_STAGE_FRAGMENT_BIT,
				m_device->createShaderModule( dumpShaderFile( m_device
					, VK_SHADER_STAGE_FRAGMENT_BIT
					, shadersFolder / "gui.frag" ) ),
				"main",
				std::nullopt,
			} );

		m_pipeline = m_device->createPipeline( ashes::GraphicsPipelineCreateInfo
			{
				0u,
				std::move( shaderStages ),
				std::move( vertexLayout ),
				ashes::PipelineInputAssemblyStateCreateInfo{ 0u, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST },
				std::nullopt,
				ashes::PipelineViewportStateCreateInfo{},
				ashes::PipelineRasterizationStateCreateInfo{ 0u, VK_FALSE, VK_FALSE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE },
				ashes::PipelineMultisampleStateCreateInfo{},
				ashes::PipelineDepthStencilStateCreateInfo{},
				std::move( cbState ),
				ashes::PipelineDynamicStateCreateInfo{ 0u, { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR } },
				*m_pipelineLayout,
				*m_renderPass,
			} );
	}

	void Gui::doUpdateCommandBuffers()
	{
		size_t index = 0u;
		ImGuiIO & io = ImGui::GetIO();
		m_pushConstants.getData()->scale = utils::Vec2{ 2.0f / io.DisplaySize.x, 2.0f / io.DisplaySize.y };
		m_pushConstants.getData()->translate = utils::Vec2{ -1.0f };

		m_commandBuffer->begin();
		m_commandBuffer->memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
			, m_fontView.makeShaderInputResource( VK_IMAGE_LAYOUT_UNDEFINED ) );
		m_commandBuffer->memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT
			, m_vertexBuffer->getBuffer().makeVertexShaderInputResource() );
		m_commandBuffer->memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT
			, m_indexBuffer->getBuffer().makeVertexShaderInputResource() );
		m_commandBuffer->beginRenderPass( *m_renderPass
			, *m_frameBuffer
			, { VkClearValue{ 1.0, 1.0, 1.0, 0.0 } }
			, VK_SUBPASS_CONTENTS_INLINE );
		m_commandBuffer->bindPipeline( *m_pipeline );
		m_commandBuffer->bindDescriptorSet( *m_descriptorSet
			, *m_pipelineLayout );
		m_commandBuffer->bindVertexBuffer( 0u, m_vertexBuffer->getBuffer(), 0u );
		m_commandBuffer->bindIndexBuffer( m_indexBuffer->getBuffer(), 0u, VK_INDEX_TYPE_UINT16 );
		m_commandBuffer->setViewport( { 0.0f, 0.0f, float( ImGui::GetIO().DisplaySize.x ), float( ImGui::GetIO().DisplaySize.y ), 0.0f, 1.0f } );
		m_commandBuffer->setScissor( { { 0, 0 }, { uint32_t( ImGui::GetIO().DisplaySize.x ), uint32_t( ImGui::GetIO().DisplaySize.y ) } } );
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
		m_commandBuffer->memoryBarrier( VK_PIPELINE_STAGE_VERTEX_INPUT_BIT
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, m_vertexBuffer->getBuffer().makeTransferDestination() );
		m_commandBuffer->memoryBarrier( VK_PIPELINE_STAGE_VERTEX_INPUT_BIT
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, m_indexBuffer->getBuffer().makeTransferDestination() );
		m_commandBuffer->end();
	}
}

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

namespace vkapp
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
		, m_context{ ImGui::CreateContext( nullptr ) }
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

		m_vertexBuffer = utils::makeVertexBuffer< ImDrawVert >( m_device
			, 1024u * 1024u
			, VK_BUFFER_USAGE_TRANSFER_DST_BIT
			, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT );
		m_indexBuffer = utils::makeBuffer< ImDrawIdx >( m_device
			, 1024u * 1024u
			, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT
			, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT );

		doPrepareResources( queue, commandPool );
	}

	Gui::~Gui()
	{
		ImGui::DestroyContext( m_context );
	}

	void Gui::initialise( ashes::RenderPass const & renderPass )
	{
		m_renderPass = &renderPass;
		doPreparePipeline();
	}

	bool Gui::update()
	{
		ImDrawData * imDrawData = ImGui::GetDrawData();

		if ( !imDrawData )
		{
			m_vertexCount = 0u;
			m_indexCount = 0u;
			return false;
		}

		bool updateCmdBuffers = false;
		auto vertexBufferSize = uint32_t( imDrawData->TotalVtxCount );
		auto indexBufferSize = uint32_t( imDrawData->TotalIdxCount );

		if ( vertexBufferSize )
		{
			updateCmdBuffers = ( m_vertexCount != vertexBufferSize )
				|| ( m_indexCount != indexBufferSize );
			m_vertexCount = vertexBufferSize;
			m_indexCount = indexBufferSize;

			if ( vertexBufferSize > m_vertexBuffer->getCount() )
			{
				m_device->waitIdle();
				m_vertexBuffer.reset();
				m_vertexBuffer = utils::makeVertexBuffer< ImDrawVert >( m_device
					, m_vertexCount
					, VK_BUFFER_USAGE_TRANSFER_DST_BIT
					, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT );
			}


			if ( indexBufferSize > m_indexBuffer->getCount() )
			{
				m_device->waitIdle();
				m_indexBuffer.reset();
				m_indexBuffer = utils::makeBuffer< ImDrawIdx >( m_device
					, m_indexCount
					, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT
					, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT );
			}

			if ( auto vtx = m_vertexBuffer->lock( 0u, m_vertexCount, 0u ) )
			{
				for ( auto & cmdList : ashes::makeArrayView( imDrawData->CmdLists, imDrawData->CmdLists + imDrawData->CmdListsCount ) )
				{
					std::copy( cmdList->VtxBuffer.Data
						, cmdList->VtxBuffer.Data + cmdList->VtxBuffer.Size
						, vtx );
					vtx += cmdList->VtxBuffer.Size;
				}

				m_vertexBuffer->flush( 0u, m_vertexCount );
				m_vertexBuffer->unlock();
			}

			if ( auto idx = m_indexBuffer->lock( 0u, m_indexCount, 0u ) )
			{
				for ( auto & cmdList : ashes::makeArrayView( imDrawData->CmdLists, imDrawData->CmdLists + imDrawData->CmdListsCount ) )
				{
					std::copy( cmdList->IdxBuffer.Data
						, cmdList->IdxBuffer.Data + cmdList->IdxBuffer.Size
						, idx );
					idx += cmdList->IdxBuffer.Size;
				}

				m_indexBuffer->flush( 0u, m_vertexCount );
				m_indexBuffer->unlock();
			}
		}
		else
		{
			updateCmdBuffers = true;
		}

		return updateCmdBuffers;
	}

	void Gui::preUpdateCommandBuffer( ashes::CommandBuffer const & commandBuffer )
	{
		if ( !m_vertexCount )
		{
			return;
		}

		ImGuiIO & io = ImGui::GetIO();
		m_pushConstants.getData()->scale = utils::Vec2{ 2.0f / io.DisplaySize.x, 2.0f / io.DisplaySize.y };
		m_pushConstants.getData()->translate = utils::Vec2{ -1.0f };

		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_HOST_BIT
			, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT
			, m_vertexBuffer->getBuffer().makeVertexShaderInputResource() );
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_HOST_BIT
			, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT
			, m_indexBuffer->getBuffer().makeVertexShaderInputResource() );
	}

	void Gui::updateCommandBuffer( ashes::CommandBuffer const & commandBuffer )
	{
		if ( !m_vertexCount )
		{
			return;
		}

		commandBuffer.bindPipeline( *m_pipeline );
		commandBuffer.bindDescriptorSet( *m_descriptorSet
			, *m_pipelineLayout );
		commandBuffer.bindVertexBuffer( 0u, m_vertexBuffer->getBuffer(), 0u );
		commandBuffer.bindIndexBuffer( m_indexBuffer->getBuffer(), 0u, VK_INDEX_TYPE_UINT16 );
		commandBuffer.setViewport( { 0.0f, 0.0f, float( ImGui::GetIO().DisplaySize.x ), float( ImGui::GetIO().DisplaySize.y ), 0.0f, 1.0f } );
		commandBuffer.setScissor( { { 0, 0 }, { uint32_t( ImGui::GetIO().DisplaySize.x ), uint32_t( ImGui::GetIO().DisplaySize.y ) } } );
		commandBuffer.pushConstants( *m_pipelineLayout, m_pushConstants );
		ImDrawData * imDrawData = ImGui::GetDrawData();
		int32_t vertexOffset = 0;
		int32_t indexOffset = 0;

		for ( int32_t j = 0; j < imDrawData->CmdListsCount; j++ )
		{
			ImDrawList const * cmdList = imDrawData->CmdLists[j];

			for ( int32_t k = 0; k < cmdList->CmdBuffer.Size; k++ )
			{
				ImDrawCmd const * cmd = &cmdList->CmdBuffer[k];
				commandBuffer.setScissor( {
					std::max( int32_t( cmd->ClipRect.x ), 0 ),
					std::max( int32_t( cmd->ClipRect.y ), 0 ),
					uint32_t( cmd->ClipRect.z - cmd->ClipRect.x ),
					uint32_t( cmd->ClipRect.w - cmd->ClipRect.y ),
					} );
				commandBuffer.drawIndexed( cmd->ElemCount, 1u, indexOffset, vertexOffset );
				indexOffset += cmd->ElemCount;
			}

			vertexOffset += cmdList->VtxBuffer.Size;
		}
	}

	void Gui::postUpdateCommandBuffer( ashes::CommandBuffer const & commandBuffer )
	{
		if ( !m_vertexCount )
		{
			return;
		}

		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_VERTEX_INPUT_BIT
			, VK_PIPELINE_STAGE_HOST_BIT
			, m_vertexBuffer->getBuffer().makeHostWrite() );
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_VERTEX_INPUT_BIT
			, VK_PIPELINE_STAGE_HOST_BIT
			, m_indexBuffer->getBuffer().makeHostWrite() );
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

		m_fontImage = m_device.createImage( { 0u
				, VK_IMAGE_TYPE_2D
				, VK_FORMAT_R8G8B8A8_UNORM
				, VkExtent3D{ uint32_t( texWidth ), uint32_t( texHeight ), 1u }
				, 1u
				, 1u
				, VK_SAMPLE_COUNT_1_BIT
				, VK_IMAGE_TILING_OPTIMAL
				, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT }
			, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
			, "FontAtlas" );
		m_fontView = m_fontImage->createView( "FontAtlas"
			, VkImageViewType( m_fontImage->getType() )
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

		ashes::VkDescriptorSetLayoutBindingArray bindings{ { 0u
			, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
			, 1u
			, VK_SHADER_STAGE_FRAGMENT_BIT
			, nullptr } };
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
	}

	void Gui::doPreparePipeline()
	{
		auto dimensions = m_size;
		auto size = VkExtent2D{ dimensions.width, dimensions.height };
		ashes::PipelineColorBlendStateCreateInfo cbState{ 0u
			, VK_FALSE
			, VK_LOGIC_OP_COPY
			, { { VK_TRUE
				, VK_BLEND_FACTOR_SRC_ALPHA
				, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA
				, VK_BLEND_OP_ADD
				, VK_BLEND_FACTOR_SRC_ALPHA
				, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA
				, VK_BLEND_OP_ADD
				, VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT } } };

		ashes::PipelineVertexInputStateCreateInfo vertexLayout{ 0u
			, { { 0u, sizeof( ImDrawVert ), VK_VERTEX_INPUT_RATE_VERTEX } }
			, { { 0u, 0u, VK_FORMAT_R32G32_SFLOAT, offsetof( ImDrawVert, pos ) }
				, { 1u, 0u, VK_FORMAT_R32G32_SFLOAT, offsetof( ImDrawVert, uv ) }
				, { 2u, 0u, VK_FORMAT_R8G8B8A8_UNORM, offsetof( ImDrawVert, col ) } },
		};

		std::string shadersFolder = ashes::getPath( ashes::getExecutableDirectory() ) / "share" / AppName / "Shaders";
		ashes::PipelineShaderStageCreateInfoArray shaderStages;
		shaderStages.push_back( ashes::PipelineShaderStageCreateInfo{ 0u
			, VK_SHADER_STAGE_VERTEX_BIT
			, m_device->createShaderModule( utils::compileGlslToSpv( m_device->getProperties()
				, VK_SHADER_STAGE_VERTEX_BIT
				, utils::dumpTextFile( shadersFolder / "gui.vert" ) ) )
			, "main"
			, ashes::nullopt } );
		shaderStages.push_back( ashes::PipelineShaderStageCreateInfo{ 0u
			, VK_SHADER_STAGE_FRAGMENT_BIT
			, m_device->createShaderModule( utils::compileGlslToSpv( m_device->getProperties()
				, VK_SHADER_STAGE_FRAGMENT_BIT
				, utils::dumpTextFile( shadersFolder / "gui.frag" ) ) )
			, "main"
			, ashes::nullopt } );

		m_pipeline = m_device->createPipeline( { 0u
			, std::move( shaderStages )
			, std::move( vertexLayout )
			, ashes::PipelineInputAssemblyStateCreateInfo{ 0u, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST }
			, ashes::nullopt
			, ashes::PipelineViewportStateCreateInfo{}
			, ashes::PipelineRasterizationStateCreateInfo{ 0u, VK_FALSE, VK_FALSE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE }
			, ashes::PipelineMultisampleStateCreateInfo{}
			, ashes::nullopt
			, std::move( cbState )
			, ashes::PipelineDynamicStateCreateInfo{ 0u, { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR } }
			, *m_pipelineLayout
			, *m_renderPass } );
	}
}

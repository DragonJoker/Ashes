/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_Pipeline_HPP___
#define ___GlRenderer_Pipeline_HPP___
#pragma once

#include "renderer/GlRenderer/Buffer/GlGeometryBuffers.hpp"
#include "renderer/GlRenderer/Core/GlContextStateStack.hpp"
#include "renderer/GlRenderer/Shader/GlShaderDesc.hpp"
#include "renderer/GlRenderer/Shader/GlShaderProgram.hpp"

#include <renderer/RendererCommon/ShaderBindings.hpp>

#include <algorithm>
#include <unordered_map>

namespace ashes::gl
{
	/**
	*\brief
	*	Un pipeline de rendu.
	*/
	class Pipeline
		: public AutoIdIcdObject< Pipeline >
	{
		friend class PipelineLayout;

	public:
		/**
		*name
		*	Construction / Destruction.
		*/
		/**@{*/
		Pipeline( VkAllocationCallbacks const * allocInfo
			, VkDevice device
			, VkGraphicsPipelineCreateInfo createInfo );
		Pipeline( VkAllocationCallbacks const * allocInfo
			, VkDevice device
			, VkComputePipelineCreateInfo createInfo );
		~Pipeline();
		GeometryBuffers * findGeometryBuffers( VboBindings const & vbos
			, IboBinding const & ibo )const;
		GeometryBuffersRef createGeometryBuffers( VboBindings vbos
			, IboBinding const & ibo
			, VkIndexType type )const;
		PushConstantsDesc findPushConstantBuffer( PushConstantsDesc const & pushConstants
			, bool isRtot )const;
		VkDescriptorSetLayoutArray const & getDescriptorsLayouts()const;
		ShaderBindings const & getDescriptorSetBindings( VkDescriptorSet descriptorSet
			, uint32_t descriptorSetIndex )const;
		ConstantsLayout const & getPushConstantsDesc( bool isRtot );
		ConstantsLayout const & getPushConstantsDesc();

		bool isCompute()const
		{
			return m_compPipeline != nullptr;
		}

		ContextState & getBackContextState()const
		{
			assert( !isCompute() );
			return m_backContextState;
		}

		ContextState & getRtotContextState()const
		{
			assert( !isCompute() );
			return m_rtotContextState;
		}

		GLuint getBackProgram()const
		{
			assert( !isCompute() );
			assert( m_backPipeline != nullptr );
			return m_backPipeline->program.program;
		}

		GLuint getRtotProgram()const
		{
			assert( !isCompute() );
			assert( m_rtotPipeline != nullptr );
			return m_rtotPipeline->program.program;
		}

		GLuint getCompProgram()const
		{
			assert( isCompute() );
			return m_compPipeline->modules.front();
		}

		auto const & getInputAssemblyState()const
		{
			assert( !isCompute() );
			return m_backContextState.iaState;
		}

		auto const & getColourBlendState()const
		{
			assert( !isCompute() );
			return m_backContextState.cbState;
		}

		auto const & getRasterisationState( bool isRtot )const
		{
			assert( !isCompute() );
			return isRtot
				? m_rtotContextState.rsState
				: m_backContextState.rsState;
		}

		auto const & getDepthStencilState()const
		{
			assert( !isCompute() );
			return m_backContextState.dsState;
		}

		auto const & getMultisampleState()const
		{
			assert( !isCompute() );
			return m_backContextState.msState;
		}

		auto const & getTessellationState()const
		{
			assert( !isCompute() );
			return m_backContextState.tsState;
		}

		auto const & getVertexInputState()const
		{
			assert( !isCompute() );
			return m_vertexInputState.value();
		}

		auto const & getViewportState()const
		{
			assert( !isCompute() );
			return m_backContextState.vpState;
		}

		bool hasViewport()const
		{
			assert( !isCompute() );
			return m_backContextState.vpState.viewportCount > 0;
		}

		bool hasScissor()const
		{
			assert( !isCompute() );
			return m_backContextState.vpState.scissorCount > 0;
		}

		ArrayView< VkViewport const > const & getViewports()const
		{
			assert( !isCompute() );
			return m_backContextState.viewportsView;
		}

		ArrayView< VkRect2D const > const & getScissors()const
		{
			assert( !isCompute() );
			return m_backContextState.scissorsView;
		}

		bool hasDynamicStateEnable( VkDynamicState state )const
		{
			assert( !isCompute() );
			auto view = makeArrayView( m_backContextState.dyState.pDynamicStates
				, m_backContextState.dyState.dynamicStateCount );
			return view.end() != std::find( view.begin()
				, view.end()
				, state );
		}

		VkPipelineLayout getLayout()const
		{
			return m_layout;
		}

		VkDevice getDevice()const
		{
			return m_device;
		}

		size_t getVertexInputStateHash()const
		{
			assert( !isCompute() );
			return m_vertexInputStateHash;
		}

	private:
		VkDevice m_device;
		VkPipelineCreateFlags m_flags{};
		VkPipelineShaderStageCreateInfoArray m_stages;
		VkVertexInputBindingDescriptionArray m_vertexBindingDescriptions;
		VkVertexInputAttributeDescriptionArray m_vertexAttributeDescriptions;
		Optional< VkPipelineVertexInputStateCreateInfo > m_vertexInputState{};
		mutable ContextState m_backContextState;
		Optional< VkPipelineRasterizationStateCreateInfo > m_rtotRasterizationState;
		mutable ContextState m_rtotContextState;
		VkPipelineLayout m_layout{};
		VkRenderPass m_renderPass{};
		uint32_t m_subpass{};
		VkPipeline m_basePipelineHandle{};
		int32_t m_basePipelineIndex{};
		ShaderProgramPtr m_backPipeline;
		ShaderProgramPtr m_rtotPipeline;
		ShaderProgramPtr m_compPipeline;
		mutable std::vector< std::pair< size_t, GeometryBuffersPtr > > m_geometryBuffers;
		mutable std::unordered_map< GLuint, DeviceMemoryDestroyConnection > m_connections;
		mutable std::unordered_map< uint64_t, ShaderBindings > m_dsBindings;
		size_t m_vertexInputStateHash;
	};
}

#endif

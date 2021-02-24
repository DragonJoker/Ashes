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

		inline bool isCompute()const
		{
			return m_compPipeline != nullptr;
		}

		inline ContextState & getBackContextState()const
		{
			assert( !isCompute() );
			return m_backContextState;
		}

		inline ContextState & getRtotContextState()const
		{
			assert( !isCompute() );
			return m_rtotContextState;
		}

		inline GLuint getBackProgram()const
		{
			assert( !isCompute() );
			assert( m_backPipeline != nullptr );
			return m_backPipeline->program.program;
		}

		inline GLuint getRtotProgram()const
		{
			assert( !isCompute() );
			assert( m_rtotPipeline != nullptr );
			return m_rtotPipeline->program.program;
		}

		inline GLuint getCompProgram()const
		{
			assert( isCompute() );
			return m_compPipeline->modules.front();
		}

		inline auto const & getInputAssemblyState()const
		{
			assert( !isCompute() );
			return m_backContextState.iaState;
		}

		inline auto const & getColourBlendState()const
		{
			assert( !isCompute() );
			return m_backContextState.cbState;
		}

		inline auto const & getRasterisationState( bool isRtot )const
		{
			assert( !isCompute() );
			return isRtot
				? m_rtotContextState.rsState
				: m_backContextState.rsState;
		}

		inline auto const & getDepthStencilState()const
		{
			assert( !isCompute() );
			return m_backContextState.dsState;
		}

		inline auto const & getMultisampleState()const
		{
			assert( !isCompute() );
			return m_backContextState.msState;
		}

		inline auto const & getTessellationState()const
		{
			assert( !isCompute() );
			return m_backContextState.tsState;
		}

		inline auto const & getVertexInputState()const
		{
			assert( !isCompute() );
			return m_vertexInputState.value();
		}

		inline auto const & getViewportState()const
		{
			assert( !isCompute() );
			return m_backContextState.vpState;
		}

		inline bool hasViewport()const
		{
			assert( !isCompute() );
			return m_backContextState.vpState.viewportCount > 0;
		}

		inline bool hasScissor()const
		{
			assert( !isCompute() );
			return m_backContextState.vpState.scissorCount > 0;
		}

		inline ArrayView< VkViewport const > const & getViewports()const
		{
			assert( !isCompute() );
			return m_backContextState.viewportsView;
		}

		inline ArrayView< VkRect2D const > const & getScissors()const
		{
			assert( !isCompute() );
			return m_backContextState.scissorsView;
		}

		inline bool hasDynamicStateEnable( VkDynamicState state )const
		{
			assert( !isCompute() );
			auto view = makeArrayView( m_backContextState.dyState.pDynamicStates
				, m_backContextState.dyState.dynamicStateCount );
			return view.end() != std::find( view.begin()
				, view.end()
				, state );
		}

		inline VkPipelineLayout getLayout()const
		{
			return m_layout;
		}

		inline VkDevice getDevice()const
		{
			return m_device;
		}

		inline size_t getVertexInputStateHash()const
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

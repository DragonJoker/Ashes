/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_Pipeline_HPP___
#define ___GlRenderer_Pipeline_HPP___
#pragma once

#include "renderer/Gl4Renderer/Buffer/GlGeometryBuffers.hpp"
#include "renderer/Gl4Renderer/Core/GlContextStateStack.hpp"
#include "renderer/Gl4Renderer/Shader/GlShaderDesc.hpp"

#include <algorithm>
#include <unordered_map>

namespace ashes::gl4
{
	/**
	*\brief
	*	Un pipeline de rendu.
	*/
	class Pipeline
	{
	public:
		/**
		*name
		*	Construction / Destruction.
		*/
		/**@{*/
		Pipeline( VkDevice device
			, VkGraphicsPipelineCreateInfo createInfo );
		Pipeline( VkDevice device
			, VkComputePipelineCreateInfo createInfo );
		GeometryBuffers * findGeometryBuffers( VboBindings const & vbos
			, IboBinding const & ibo )const;
		GeometryBuffersRef createGeometryBuffers( VboBindings vbos
			, IboBinding const & ibo
			, VkIndexType type )const;
		~Pipeline();
		PushConstantsDesc findPushConstantBuffer( PushConstantsDesc const & pushConstants
			, bool isRtot )const;
		VkDescriptorSetLayoutArray const & getDescriptorsLayouts()const;

		inline bool isCompute()const
		{
			return m_compPipeline.program.program != 0;
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
			assert( m_backPipeline.program.program != 0 );
			return m_backPipeline.program.program;
		}

		inline GLuint getRtotProgram()const
		{
			assert( !isCompute() );
			assert( m_rtotPipeline.program.program != 0 );
			return m_rtotPipeline.program.program;
		}

		inline GLuint getCompProgram()const
		{
			assert( isCompute() );
			return m_compPipeline.modules.front();
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

		inline auto const & getRasterisationState()const
		{
			assert( !isCompute() );
			return m_backContextState.rsState;
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

		inline VkViewportArray const & getViewports()const
		{
			assert( !isCompute() );
			return m_backContextState.viewports;
		}

		inline VkScissorArray const & getScissors()const
		{
			assert( !isCompute() );
			return m_backContextState.scissors;
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

		inline size_t getVertexInputStateHash()const
		{
			assert( !isCompute() );
			return m_vertexInputStateHash;
		}

	public:
		struct ProgramPipeline
		{
			PushConstantsDesc constantsPcb{};
			std::vector< GLuint > modules;
			ShaderDesc program{};
		};

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
		ProgramPipeline m_backPipeline{};
		ProgramPipeline m_rtotPipeline{};
		ProgramPipeline m_compPipeline{};
		mutable std::vector< std::pair< size_t, GeometryBuffersPtr > > m_geometryBuffers;
		mutable std::unordered_map< GLuint, DeviceMemoryDestroyConnection > m_connections;
		size_t m_vertexInputStateHash;
	};
}

#endif

/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_Pipeline_HPP___
#define ___GlRenderer_Pipeline_HPP___
#pragma once

#include "Gl4Renderer/Buffer/GlGeometryBuffers.hpp"
#include "Gl4Renderer/Core/GlContextState.hpp"
#include "Gl4Renderer/Shader/GlShaderDesc.hpp"
#include "Gl4Renderer/Shader/GlShaderProgram.hpp"

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
		PushConstantsDesc findPushConstantBuffer( PushConstantsDesc const & pushConstants )const;
		/**
		*name
		*	Getters.
		*/
		ContextState const & getContextState()const
		{
			return m_contextState;
		}

		inline auto const & getInputAssemblyState()const
		{
			return m_contextState.iaState;
		}

		inline auto const & getColourBlendState()const
		{
			return m_contextState.cbState;
		}

		inline auto const & getRasterisationState()const
		{
			return m_contextState.rsState;
		}

		inline auto const & getDepthStencilState()const
		{
			return m_contextState.dsState;
		}

		inline auto const & getMultisampleState()const
		{
			return m_contextState.msState;
		}

		inline auto const & getTessellationState()const
		{
			return m_contextState.tsState;
		}

		inline auto const & getVertexInputState()const
		{
			return m_vertexInputState.value();
		}

		inline auto const & getViewportState()const
		{
			return m_contextState.vpState;
		}

		inline bool hasViewport()const
		{
			return m_contextState.vpState.viewportCount > 0;
		}

		inline bool hasScissor()const
		{
			return m_contextState.vpState.scissorCount > 0;
		}

		inline VkViewportArray const & getViewports()const
		{
			return m_contextState.viewports;
		}

		inline VkScissorArray const & getScissors()const
		{
			return m_contextState.scissors;
		}

		inline bool hasDynamicStateEnable( VkDynamicState state )const
		{
			auto view = makeArrayView( m_contextState.dyState.pDynamicStates
				, m_contextState.dyState.dynamicStateCount );
			return view.end() != std::find( view.begin()
				, view.end()
				, state );
			return false;
		}

		inline VkPipelineLayout getLayout()const
		{
			return m_layout;
		}

		inline GLuint getProgram()const
		{
			return m_program.getProgram();
		}

		inline size_t getVertexInputStateHash()const
		{
			return m_vertexInputStateHash;
		}
		/**@}*/

	private:
		VkDevice m_device;
		VkPipelineCreateFlags m_flags;
		VkPipelineShaderStageCreateInfoArray m_stages;
		VkVertexInputBindingDescriptionArray m_vertexBindingDescriptions;
		VkVertexInputAttributeDescriptionArray m_vertexAttributeDescriptions;
		Optional< VkPipelineVertexInputStateCreateInfo > m_vertexInputState;
		ContextState m_contextState;
		VkPipelineLayout m_layout;
		VkRenderPass m_renderPass;
		uint32_t m_subpass;
		VkPipeline m_basePipelineHandle;
		int32_t m_basePipelineIndex;
		PushConstantsDesc m_constantsPcb;
		ShaderProgram m_program;
		mutable std::vector< std::pair< size_t, GeometryBuffersPtr > > m_geometryBuffers;
		mutable std::unordered_map< GLuint, BufferDestroyConnection > m_connections;
		size_t m_vertexInputStateHash;
	};
}

#endif

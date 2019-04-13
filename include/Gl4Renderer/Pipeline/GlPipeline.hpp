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
		ContextState const & getBackContextState()const
		{
			return m_backContextState;
		}

		ContextState const & getRtotContextState()const
		{
			return m_rtotContextState;
		}

		inline GLuint getBackProgram()const
		{
			assert( m_backProgram );
			return m_backProgram->getProgram();
		}

		inline GLuint getRtotProgram()const
		{
			assert( m_rtotProgram );
			return m_rtotProgram->getProgram();
		}

		inline GLuint getCompProgram()const
		{
			assert( m_compProgram );
			return m_compProgram->getProgram();
		}

		inline auto const & getInputAssemblyState()const
		{
			return m_backContextState.iaState;
		}

		inline auto const & getColourBlendState()const
		{
			return m_backContextState.cbState;
		}

		inline auto const & getRasterisationState()const
		{
			return m_backContextState.rsState;
		}

		inline auto const & getDepthStencilState()const
		{
			return m_backContextState.dsState;
		}

		inline auto const & getMultisampleState()const
		{
			return m_backContextState.msState;
		}

		inline auto const & getTessellationState()const
		{
			return m_backContextState.tsState;
		}

		inline auto const & getVertexInputState()const
		{
			return m_vertexInputState.value();
		}

		inline auto const & getViewportState()const
		{
			return m_backContextState.vpState;
		}

		inline bool hasViewport()const
		{
			return m_backContextState.vpState.viewportCount > 0;
		}

		inline bool hasScissor()const
		{
			return m_backContextState.vpState.scissorCount > 0;
		}

		inline VkViewportArray const & getViewports()const
		{
			return m_backContextState.viewports;
		}

		inline VkScissorArray const & getScissors()const
		{
			return m_backContextState.scissors;
		}

		inline bool hasDynamicStateEnable( VkDynamicState state )const
		{
			auto view = makeArrayView( m_backContextState.dyState.pDynamicStates
				, m_backContextState.dyState.dynamicStateCount );
			return view.end() != std::find( view.begin()
				, view.end()
				, state );
			return false;
		}

		inline VkPipelineLayout getLayout()const
		{
			return m_layout;
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
		ContextState m_backContextState;
		Optional< VkPipelineRasterizationStateCreateInfo > m_rtotRasterizationState;
		ContextState m_rtotContextState;
		VkPipelineLayout m_layout;
		VkRenderPass m_renderPass;
		uint32_t m_subpass;
		VkPipeline m_basePipelineHandle;
		int32_t m_basePipelineIndex;
		PushConstantsDesc m_constantsPcb;
		std::unique_ptr< ShaderProgram > m_backProgram;
		std::unique_ptr< ShaderProgram > m_rtotProgram;
		std::unique_ptr< ShaderProgram > m_compProgram;
		mutable std::vector< std::pair< size_t, GeometryBuffersPtr > > m_geometryBuffers;
		mutable std::unordered_map< GLuint, BufferDestroyConnection > m_connections;
		size_t m_vertexInputStateHash;
	};
}

#endif

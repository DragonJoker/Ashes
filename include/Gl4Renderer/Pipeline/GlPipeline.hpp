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
#include "Gl4Renderer/Shader/GlShaderDesc.hpp"
#include "Gl4Renderer/Shader/GlShaderProgram.hpp"

#include <Ashes/Buffer/PushConstantsBuffer.hpp>
#include <Ashes/Pipeline/Pipeline.hpp>
#include <Ashes/Pipeline/ColourBlendState.hpp>
#include <Ashes/Pipeline/DepthStencilState.hpp>
#include <Ashes/Pipeline/InputAssemblyState.hpp>
#include <Ashes/Pipeline/MultisampleState.hpp>
#include <Ashes/Pipeline/RasterisationState.hpp>
#include <Ashes/Pipeline/Scissor.hpp>
#include <Ashes/Pipeline/ShaderStageState.hpp>
#include <Ashes/Pipeline/TessellationState.hpp>
#include <Ashes/Pipeline/Viewport.hpp>

#include <algorithm>
#include <unordered_map>

namespace gl_renderer
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
		Pipeline( Device const & device
			, PipelineLayout const & layout
			, RenderPass const & renderPass
			, VkGraphicsPipelineCreateInfo createInfo );
		GeometryBuffers * findGeometryBuffers( VboBindings const & vbos
			, IboBinding const & ibo )const;
		GeometryBuffersRef createGeometryBuffers( VboBindings vbos
			, IboBinding const & ibo
			, ashes::IndexType type )const;
		~Pipeline();
		PushConstantsDesc findPushConstantBuffer( PushConstantsDesc const & pushConstants )const;
		/**@}*/
		/**
		*\return
		*	\p true si le Viewport est défini.
		*/
		inline bool hasViewport()const
		{
			return m_viewportState.viewportCount > 0;
		}
		/**
		*\return
		*	\p true si le Scissor est défini.
		*/
		inline bool hasScissor()const
		{
			return m_viewportState.scissorCount > 0;
		}
		/**
		*\return
		*	Le ShaderStageState.
		*/
		inline auto const & getShaderStageState()const
		{
			return m_ssState;
		}
		/**
		*\return
		*	Le InputAssemblyState.
		*/
		inline auto const & getInputAssemblyState()const
		{
			return m_iaState;
		}
		/**
		*\return
		*	Le ColourBlendState.
		*/
		inline auto const & getColourBlendState()const
		{
			return m_cbState;
		}
		/**
		*\return
		*	Le RasterisationState.
		*/
		inline auto const & getRasterisationState()const
		{
			return m_rsState;
		}
		/**
		*\return
		*	Le DepthStencilState.
		*/
		inline auto const & getDepthStencilState()const
		{
			return m_dsState;
		}
		/**
		*\return
		*	Le MultisampleState.
		*/
		inline auto const & getMultisampleState()const
		{
			return m_msState;
		}
		/**
		*\return
		*	Le TessellationState.
		*/
		inline auto const & getTessellationState()const
		{
			return m_tsState;
		}
		/**
		*\return
		*	Le VertexInputState.
		*/
		inline auto const & getVertexInputState()const
		{
			return m_vertexInputState;
		}
		/**
		*\return
		*	Le Viewport.
		*/
		inline auto const & getViewportState()const
		{
			return m_viewportState;
		}
		/**
		*\return
		*	Les Viewports.
		*/
		inline ViewportArray getViewports()const
		{
			return { m_viewportState.pViewports, m_viewportState.pViewports + m_viewportState.viewportCount };
		}
		/**
		*\return
		*	Les Scissors.
		*/
		inline ScissorArray getScissors()const
		{
			return { m_viewportState.pScissors, m_viewportState.pScissors + m_viewportState.scissorCount };
		}
		/**
		*\return
		*	Le PipelineLayout.
		*/
		inline PipelineLayout const & getLayout()const
		{
			return m_layout;
		}
		/**
		*\return
		*	Le ShaderProgram.
		*/
		inline GLuint getProgram()const
		{
			return m_program.getProgram();
		}
		/**
		*\return
		*	Dit si le pipeline a des sommets.
		*/
		inline bool hasVertexLayout()const
		{
			return m_vertexInputState.vertexAttributeDescriptionCount > 0
				&& m_vertexInputState.vertexBindingDescriptionCount > 0;
		}
		/**
		*\return
		*	Le hash du VertexInputState.
		*/
		inline size_t getVertexInputStateHash()const
		{
			return m_vertexInputStateHash;
		}
		/**
		*\return
		*	\p true si l'état dynamique est dans la liste d'états dynamiques.
		*/
		inline bool hasDynamicStateEnable( VkDynamicStateEnable state )const
		{
			if ( m_createInfo.pDynamicStateEnable )
			{
				auto end = m_createInfo.pDynamicStateEnable->pDynamicStateEnables + m_createInfo.pDynamicStateEnable->dynamicStateCount;
				return end != std::find( m_createInfo.pDynamicStateEnable->pDynamicStateEnables
					, end
					, state );
			}

			return false;
		}

	private:
		Device const & m_device;
		PipelineLayout const & m_layout;
		RenderPass const & m_renderPass;
		VkGraphicsPipelineCreateInfo m_createInfo;
		std::vector< VkPipelineShaderStageCreateInfo > m_ssState;
		VkPipelineVertexInputStateCreateInfo m_vertexInputState;
		VkPipelineInputAssemblyStateCreateInfo m_iaState;
		VkPipelineTessellationStateCreateInfo m_tsState;
		VkPipelineViewportStateCreateInfo m_viewportState;
		VkPipelineRasterizationStateCreateInfo m_rsState;
		VkPipelineMultisampleStateCreateInfo m_msState;
		VkPipelineDepthStencilStateCreateInfo m_dsState;
		VkPipelineColorBlendStateCreateInfo m_cbState;
		VkPipelineDynamicStateEnableCreateInfo m_dnState;
		PushConstantsDesc m_constantsPcb;
		ShaderProgram m_program;
		mutable std::vector< std::pair< size_t, GeometryBuffersPtr > > m_geometryBuffers;
		mutable std::unordered_map< GLuint, BufferDestroyConnection > m_connections;
		size_t m_vertexInputStateHash;
	};
}

#endif

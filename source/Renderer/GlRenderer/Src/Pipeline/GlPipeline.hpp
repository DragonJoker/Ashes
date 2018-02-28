/**
*\file
*	Texture.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_Pipeline_HPP___
#define ___GlRenderer_Pipeline_HPP___
#pragma once

#include "GlRendererPrerequisites.hpp"

#include <Buffer/PushConstantsBuffer.hpp>
#include <Pipeline/Pipeline.hpp>
#include <Pipeline/ColourBlendState.hpp>
#include <Pipeline/DepthStencilState.hpp>
#include <Pipeline/InputAssemblyState.hpp>
#include <Pipeline/MultisampleState.hpp>
#include <Pipeline/RasterisationState.hpp>
#include <Pipeline/Scissor.hpp>
#include <Pipeline/TessellationState.hpp>
#include <Pipeline/Viewport.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Un pipeline de rendu.
	*/
	class Pipeline
		: public renderer::Pipeline
	{
	public:
		/**
		*name
		*	Construction / Destruction.
		*/
		/**@{*/
		Pipeline( Device const & device
			, PipelineLayout const & layout
			, renderer::GraphicsPipelineCreateInfo && createInfo );
		/**
		*\return
		*	\p true si le Viewport est défini.
		*/
		inline bool hasViewport()const
		{
			return (bool)m_viewport;
		}
		/**
		*\return
		*	\p true si le Scissor est défini.
		*/
		inline bool hasScissor()const
		{
			return (bool)m_scissor;
		}
		/**
		*\return
		*	Le InputAssemblyState.
		*/
		inline renderer::InputAssemblyState const & getInputAssemblyState()const
		{
			return m_iaState;
		}
		/**
		*\return
		*	Le ColourBlendState.
		*/
		inline renderer::ColourBlendState const & getColourBlendState()const
		{
			return m_cbState;
		}
		/**
		*\return
		*	Le RasterisationState.
		*/
		inline renderer::RasterisationState const & getRasterisationState()const
		{
			return m_rsState;
		}
		/**
		*\return
		*	Le DepthStencilState.
		*/
		inline renderer::DepthStencilState const & getDepthStencilState()const
		{
			return m_dsState;
		}
		/**
		*\return
		*	Le MultisampleState.
		*/
		inline renderer::MultisampleState const & getMultisampleState()const
		{
			return m_msState;
		}
		/**
		*\return
		*	Le TessellationState.
		*/
		inline renderer::TessellationState const & getTessellationState()const
		{
			return m_tsState;
		}
		/**
		*\return
		*	Le Viewport.
		*/
		inline renderer::Viewport const & getViewport()const
		{
			assert( m_viewport );
			return *m_viewport;
		}
		/**
		*\return
		*	Le Scissor.
		*/
		inline renderer::Scissor const & getScissor()const
		{
			assert( m_scissor );
			return *m_scissor;
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
		inline ShaderProgram const & getProgram()const
		{
			return m_program;
		}
		/**
		*\return
		*	Dit si le pipeline a des sommets.
		*/
		inline bool hasVertexLayout()const
		{
			return !m_vertexInputState.vertexBindingDescriptions.empty()
				&& !m_vertexInputState.vertexAttributeDescriptions.empty();
		}
		/**
		*\return
		*	Le tampon de push constants correspondant aux constantes de spécialisation.
		*/
		inline std::vector< renderer::PushConstantsBufferBase > const & getConstantsPcbs()const
		{
			return m_constantsPcbs;
		}

	private:
		Device const & m_device;
		PipelineLayout const & m_layout;
		ShaderProgram const & m_program;
		renderer::VertexInputState m_vertexInputState;
		renderer::RenderPass const & m_renderPass;
		renderer::InputAssemblyState m_iaState;
		renderer::ColourBlendState m_cbState;
		renderer::RasterisationState m_rsState;
		renderer::DepthStencilState m_dsState;
		renderer::MultisampleState m_msState;
		renderer::TessellationState m_tsState;
		std::optional< renderer::Viewport > m_viewport;
		std::optional< renderer::Scissor > m_scissor;
		std::vector< renderer::PushConstantsBufferBase > m_constantsPcbs;
	};
}

#endif

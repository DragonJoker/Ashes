/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include "GlCommandBase.hpp"

namespace gl_renderer
{
	void apply( Device const & device
		, ContextLock const & context
		, renderer::ColourBlendState const & state );
	void apply( Device const & device
		, ContextLock const & context
		, renderer::RasterisationState const & state
		, bool dynamicLineWidth
		, bool dynamicDepthBias );
	void apply( Device const & device
		, ContextLock const & context
		, renderer::MultisampleState const & state );
	void apply( Device const & device
		, ContextLock const & context
		, renderer::DepthStencilState const & state );
	void apply( Device const & device
		, ContextLock const & context
		, renderer::TessellationState const & state );
	/**
	*\brief
	*	Commande d'activation d'un pipeline: shaders, tests, �tats, ...
	*/
	class BindPipelineCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] pipeline
		*	Le pipeline � activer.
		*\param[in] bindingPoint
		*	Le point d'attache du pipeline.
		*/
		BindPipelineCommand( Device const & device
			, renderer::Pipeline const & pipeline
			, renderer::PipelineBindPoint bindingPoint );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		Pipeline const & m_pipeline;
		PipelineLayout const & m_layout;
		GLuint m_program;
		renderer::PipelineBindPoint m_bindingPoint;
		bool m_dynamicLineWidth;
		bool m_dynamicDepthBias;
		bool m_dynamicScissor;
		bool m_dynamicViewport;
	};
}

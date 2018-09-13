/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "GlCommandBase.hpp"

namespace gl_renderer
{
	void apply( Device const & device
		, ContextLock const & context
		, ashes::ColourBlendState const & state );
	void apply( Device const & device
		, ContextLock const & context
		, ashes::RasterisationState const & state
		, bool dynamicLineWidth
		, bool dynamicDepthBias );
	void apply( Device const & device
		, ContextLock const & context
		, ashes::MultisampleState const & state );
	void apply( Device const & device
		, ContextLock const & context
		, ashes::DepthStencilState const & state );
	void apply( Device const & device
		, ContextLock const & context
		, ashes::TessellationState const & state );
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
			, ashes::Pipeline const & pipeline
			, ashes::PipelineBindPoint bindingPoint );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		Pipeline const & m_pipeline;
		PipelineLayout const & m_layout;
		GLuint m_program;
		ashes::PipelineBindPoint m_bindingPoint;
		bool m_dynamicLineWidth;
		bool m_dynamicDepthBias;
		bool m_dynamicScissor;
		bool m_dynamicViewport;
	};
}

/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace d3d11_renderer
{
	void apply( Device const & device
		, ashes::ColourBlendState const & state );
	void apply( Device const & device
		, ashes::RasterisationState const & state
		, bool dynamicLineWidth
		, bool dynamicDepthBias );
	void apply( Device const & device
		, ashes::MultisampleState const & state );
	void apply( Device const & device
		, ashes::DepthStencilState const & state );
	void apply( Device const & device
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

		void apply( Context const & context )const override;
		void remove( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		Pipeline const & m_pipeline;
		PipelineLayout const & m_layout;
		ashes::PipelineBindPoint m_bindingPoint;
		bool m_dynamicLineWidth;
		bool m_dynamicDepthBias;
		bool m_dynamicScissor;
		bool m_dynamicViewport;
	};
}

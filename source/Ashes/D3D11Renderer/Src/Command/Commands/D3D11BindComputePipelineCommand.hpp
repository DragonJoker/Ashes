/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11CommandBase.hpp"

namespace d3d11_renderer
{
	/**
	*\brief
	*	Commande d'activation d'un pipeline de calcul.
	*/
	class BindComputePipelineCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] pipeline
		*	Le pipeline à activer.
		*\param[in] bindingPoint
		*	Le point d'attache du pipeline.
		*/
		BindComputePipelineCommand( Device const & device
			, ashes::ComputePipeline const & pipeline
			, ashes::PipelineBindPoint bindingPoint );

		void apply( Context const & context )const override;
		void remove( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		ComputePipeline const & m_pipeline;
		PipelineLayout const & m_layout;
		ashes::ShaderStageState const & m_program;
		ashes::PipelineBindPoint m_bindingPoint;
	};
}

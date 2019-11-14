/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"
#include "renderer/D3D11Renderer/Shader/D3D11ShaderDesc.hpp"

namespace ashes::d3d11
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
		BindComputePipelineCommand( VkDevice device
			, VkPipeline pipeline
			, VkPipelineBindPoint bindingPoint );

		void apply( Context const & context )const;
		void remove( Context const & context )const;
		CommandPtr clone()const;

	private:
		VkPipeline m_pipeline;
		VkPipelineLayout m_layout;
		CompiledShaderModule const & m_program;
		VkPipelineBindPoint m_bindingPoint;
	};
}

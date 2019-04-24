/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::d3d11
{
	void apply( VkDevice device
		, VkPipelineColorBlendStateCreateInfo const & state );
	void apply( VkDevice device
		, VkPipelineRasterizationStateCreateInfo const & state
		, bool dynamicLineWidth
		, bool dynamicDepthBias );
	void apply( VkDevice device
		, VkPipelineMultisampleStateCreateInfo const & state );
	void apply( VkDevice device
		, VkPipelineDepthStencilStateCreateInfo const & state );
	void apply( VkDevice device
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
		BindPipelineCommand( VkDevice device
			, ashes::Pipeline const & pipeline
			, ashes::PipelineBindPoint bindingPoint );

		void apply( Context const & context )const;
		void remove( Context const & context )const;
		CommandPtr clone()const;

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

/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl3Renderer/Command/Commands/GlCommandBase.hpp"

namespace gl_renderer
{
	/**
	*\brief
	*	Commande d'activation d'un set de descripteurs.
	*/
	class BindDescriptorSetCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] descriptorSet
		*	Le descriptor set.
		*\param[in] layout
		*	Le layout de pipeline.
		*\param[in] bindingPoint
		*	Le point d'attache du set.
		*/
		BindDescriptorSetCommand( Device const & device
			, ashes::DescriptorSet const & descriptorSet
			, ashes::PipelineLayout const & layout
			, ashes::UInt32Array const & dynamicOffsets
			, ashes::PipelineBindPoint bindingPoint );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		DescriptorSet const & m_descriptorSet;
		PipelineLayout const & m_layout;
		ashes::PipelineBindPoint m_bindingPoint;
		ashes::UInt32Array m_dynamicOffsets;
	};
}

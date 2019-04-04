/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl4
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
		BindDescriptorSetCommand( VkDevice device
			, VkDescriptorSet descriptorSet
			, VkPipelineLayout layout
			, UInt32Array const & dynamicOffsets
			, VkPipelineBindPoint bindingPoint );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		VkDescriptorSet m_descriptorSet;
		VkPipelineLayout m_layout;
		VkPipelineBindPoint m_bindingPoint;
		UInt32Array m_dynamicOffsets;
	};
}

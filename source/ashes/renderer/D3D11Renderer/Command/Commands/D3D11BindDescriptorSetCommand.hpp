/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::D3D11_NAMESPACE
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
			, ArrayView< uint32_t const > const & dynamicOffsets
			, VkPipelineBindPoint bindingPoint );

		void apply( Context const & context )const override;
		void remove( Context const & context )const override;
		void fillContext( Context & context )const override;
		CommandPtr clone()const override;

	private:
		VkDescriptorSet m_descriptorSet;
		VkPipelineLayout m_layout;
		VkPipelineBindPoint m_bindingPoint;
		UInt32Array m_dynamicOffsets;
		std::map< UINT, ID3D11UnorderedAccessView * > m_uavs;
	};
}

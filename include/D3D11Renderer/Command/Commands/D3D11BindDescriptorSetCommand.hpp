/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace d3d11_renderer
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

		void apply( Context const & context )const override;
		void remove( Context const & context )const override;
		void fillContext( Context & context )const override;
		CommandPtr clone()const override;

	private:
		void apply11_0( Context const & context )const;
		void apply11_1( Context const & context )const;
		void remove11_0( Context const & context )const;
		void remove11_1( Context const & context )const;

	private:
		DescriptorSet const & m_descriptorSet;
		PipelineLayout const & m_layout;
		ashes::PipelineBindPoint m_bindingPoint;
		ashes::UInt32Array m_dynamicOffsets;
	};
}

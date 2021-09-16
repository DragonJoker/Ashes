/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::d3d11
{
	/**
	*\brief
	*	Commande d'application d'un scissor.
	*/
	class SetLineWidthCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] scissor
		*	Le scissor.
		*/
		SetLineWidthCommand( VkDevice device
			, float width );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		float m_width;
	};
}

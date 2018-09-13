/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11CommandBase.hpp"

#include <Pipeline/Scissor.hpp>

namespace d3d11_renderer
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
		SetLineWidthCommand( Device const & device
			, float width );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		float m_width;
	};
}

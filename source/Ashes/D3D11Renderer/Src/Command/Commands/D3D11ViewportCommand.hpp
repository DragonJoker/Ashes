/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11CommandBase.hpp"

#include <Pipeline/Viewport.hpp>

namespace d3d11_renderer
{
	/**
	*\brief
	*	Commande d'application d'un viewport.
	*/
	class ViewportCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] viewport
		*	Le viewport.
		*/
		ViewportCommand( Device const & device
			, ashes::Viewport const & viewport );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		D3D11_VIEWPORT m_viewport;
	};
}

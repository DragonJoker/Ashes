/*
This file belongs to RendererLib.
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
	class ScissorCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] scissor
		*	Le scissor.
		*/
		ScissorCommand( Device const & device
			, renderer::Scissor const & scissor );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		RECT m_scissor;
	};
}

/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

#include <Ashes/Pipeline/Viewport.hpp>

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
			, uint32_t first
			, ashes::ViewportArray const & viewports );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		std::vector< D3D11_VIEWPORT > m_viewports;
	};
}

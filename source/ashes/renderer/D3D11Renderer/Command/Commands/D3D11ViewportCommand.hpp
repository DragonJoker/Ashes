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
		ViewportCommand( VkDevice device
			, uint32_t first
			, ArrayView< VkViewport const > const & viewports );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		std::vector< D3D11_VIEWPORT > m_viewports;
	};
}

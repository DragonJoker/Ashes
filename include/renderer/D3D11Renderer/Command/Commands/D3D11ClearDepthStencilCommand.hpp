/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

#include <Ashes/RenderPass/ClearValue.hpp>

namespace ashes::d3d11
{
	/**
	*\brief
	*	Commande de vidage d'une image.
	*/
	class ClearDepthStencilCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] image
		*	L'image à vider.
		*\param[in] colour
		*	La couleur de vidage.
		*/
		ClearDepthStencilCommand( Device const & device
			, ashes::ImageView const & image
			, ashes::DepthStencilClearValue const & value );

		void apply( Context const & context )const;
		CommandPtr clone()const;

	private:
		ImageView const & m_image;
		ashes::DepthStencilClearValue m_value;
		UINT m_flags;
	};
}

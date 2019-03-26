/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

#include <Ashes/RenderPass/ClearValue.hpp>

namespace d3d11_renderer
{
	/**
	*\brief
	*	Commande de vidage d'une image.
	*/
	class ClearColourCommand
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
		ClearColourCommand( Device const & device
			, ashes::ImageView const & image
			, ashes::ClearColorValue const & colour );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		ImageView const & m_image;
		ashes::ClearColorValue m_colour;
	};
}

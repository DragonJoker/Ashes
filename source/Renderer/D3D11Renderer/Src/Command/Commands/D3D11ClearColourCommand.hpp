/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11CommandBase.hpp"

#include <RenderPass/ClearValue.hpp>

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
			, renderer::TextureView const & image
			, renderer::ClearColorValue const & colour );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		TextureView const & m_image;
		renderer::ClearColorValue m_colour;
		DXGI_FORMAT m_format;
	};
}

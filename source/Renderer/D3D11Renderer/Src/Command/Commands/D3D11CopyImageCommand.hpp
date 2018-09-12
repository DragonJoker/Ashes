/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11CommandBase.hpp"

#include <Miscellaneous/ImageCopy.hpp>

namespace d3d11_renderer
{
	/**
	*\brief
	*	Commande de copie du contenu d'une image dans une autre.
	*/
	class CopyImageCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] copyInfo
		*	Les informations de copie.
		*\param[in] src
		*	L'image source.
		*\param[in] dst
		*	L'image destination.
		*/
		CopyImageCommand( Device const & device
			, renderer::ImageCopy const & copyInfo
			, renderer::Texture const & src
			, renderer::Texture const & dst );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		Texture const & m_src;
		Texture const & m_dst;
		renderer::ImageCopy m_copyInfo;
		D3D11_BOX m_srcBox;
	};
}

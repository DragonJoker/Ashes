/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

#include <Ashes/Miscellaneous/ImageCopy.hpp>

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
			, ashes::ImageCopy const & copyInfo
			, ashes::Image const & src
			, ashes::Image const & dst );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		Image const & m_src;
		Image const & m_dst;
		ashes::ImageCopy m_copyInfo;
		D3D11_BOX m_srcBox;
		UINT m_srcSubresource;
		UINT m_dstSubresource;
	};
}

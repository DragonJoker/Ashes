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
		CopyImageCommand( VkDevice device
			, VkImageCopy const & copyInfo
			, VkImage src
			, VkImage dst );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		VkImage m_src;
		VkImage m_dst;
		VkImageCopy m_copyInfo;
		D3D11_BOX m_srcBox;
		UINT m_srcSubresource;
		UINT m_dstSubresource;
	};
}

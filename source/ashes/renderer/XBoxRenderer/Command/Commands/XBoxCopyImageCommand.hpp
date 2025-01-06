/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/XBoxRenderer/Command/Commands/XBoxCommandBase.hpp"

namespace ashes::xbox
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

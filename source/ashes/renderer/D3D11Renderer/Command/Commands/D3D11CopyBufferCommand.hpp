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
	*	Commande copie d'un vertex buffer.
	*/
	class CopyBufferCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] copyInfo
		*	Les informations de copie.
		*\param[in] src
		*	Le tampon source.
		*\param[in] dst
		*	Le tampon destination.
		*/
		CopyBufferCommand( VkDevice device
			, VkBufferCopy const & copyInfo
			, VkBuffer src
			, VkBuffer dst );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		VkBuffer m_src;
		VkBuffer m_dst;
		UINT m_dstOffset;
		D3D11_BOX m_srcBox;
	};
}

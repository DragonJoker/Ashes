/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl4
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
			, VkBufferCopy copyInfo
			, VkBuffer src
			, VkBuffer dst );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		VkBuffer m_src;
		VkBuffer m_dst;
		VkBufferCopy m_copyInfo;
	};
}

/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/Command/Commands/GlCommandBase.hpp"

#include "Gl4Renderer/Enum/GlFormat.hpp"
#include "Gl4Renderer/Enum/GlTextureType.hpp"

namespace ashes::gl4
{
	/**
	*\brief
	*	Commande de copie du contenu d'un tampon dans une image.
	*/
	class CopyBufferToImageCommand
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
		*	L'image destination.
		*/
		CopyBufferToImageCommand( VkDevice device
			, VkBufferImageCopy copyInfo
			, VkBuffer src
			, VkImage dst );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		VkBuffer m_src;
		VkImage m_dst;
		VkBufferImageCopy m_copyInfo;
		GlInternal m_internal;
		GlFormat m_format;
		GlType m_type;
		GlTextureType m_copyTarget;
	};
}

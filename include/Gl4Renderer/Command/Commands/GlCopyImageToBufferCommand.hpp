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
	*	Commande de copie du contenu d'une image dans un tampon.
	*/
	class CopyImageToBufferCommand
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
		*	Le tampon destination.
		*/
		CopyImageToBufferCommand( VkDevice device
			, VkBufferImageCopy copyInfo
			, VkImage src
			, VkBuffer dst );
		CopyImageToBufferCommand( CopyImageToBufferCommand const & rhs );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		VkImage m_src;
		VkBuffer m_dst;
		VkBufferImageCopy m_copyInfo;
		GlInternal m_internal;
		GlFormat m_format;
		GlType m_type;
		GlTextureType m_target;
		VkImageView m_view;
		GLuint m_srcFbo;
	};
}

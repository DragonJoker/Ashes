/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/Command/Commands/GlCommandBase.hpp"

#include <Ashes/Miscellaneous/BufferImageCopy.hpp>

namespace gl_renderer
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
		CopyBufferToImageCommand( Device const & device
			, ashes::BufferImageCopyArray const & copyInfo
			, ashes::BufferBase const & src
			, ashes::Texture const & dst );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		void applyOne( ContextLock const & context
			, ashes::BufferImageCopy const & copyInfo )const;

	private:
		Buffer const & m_src;
		Texture const & m_dst;
		ashes::BufferImageCopyArray m_copyInfo;
		GlInternal m_internal;
		GlFormat m_format;
		GlType m_type;
		GlTextureType m_copyTarget;
	};
}

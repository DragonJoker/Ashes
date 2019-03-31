/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl3Renderer/Command/Commands/GlCommandBase.hpp"

#include <Ashes/Miscellaneous/ImageCopy.hpp>

namespace gl_renderer
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

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		Image const & m_src;
		Image const & m_dst;
		ashes::ImageCopy m_copyInfo;
		GlInternal m_srcInternal;
		GlFormat m_srcFormat;
		GlType m_srcType;
		GlTextureType m_srcTarget;
		GlInternal m_dstInternal;
		GlFormat m_dstFormat;
		GlType m_dstType;
		GlTextureType m_dstTarget;
		mutable ashes::ByteArray m_srcData;
		mutable ashes::ByteArray m_dstData;
	};
}

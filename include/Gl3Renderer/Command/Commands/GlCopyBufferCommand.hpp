/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl3Renderer/Command/Commands/GlCommandBase.hpp"

#include <Ashes/Miscellaneous/BufferCopy.hpp>

namespace gl_renderer
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
		CopyBufferCommand( Device const & device
			, ashes::BufferCopy const & copyInfo
			, ashes::BufferBase const & src
			, ashes::BufferBase const & dst );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		Buffer const & m_src;
		Buffer const & m_dst;
		ashes::BufferCopy m_copyInfo;
	};
}

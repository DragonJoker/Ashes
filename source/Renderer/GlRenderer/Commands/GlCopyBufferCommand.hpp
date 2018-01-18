/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "GlCommandBase.hpp"

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
		*\param[in] src
		*	Le tampon source.
		*\param[in] dst
		*	Le tampon destination.
		*\param[in] size
		*	La taille des donn�es � copier.
		*/
		CopyBufferCommand( renderer::BufferBase const & src
			, renderer::BufferBase const & dst
			, uint32_t size
			, uint32_t offset );

		void apply()const override;
		CommandPtr clone()const override;

	private:
		Buffer const & m_src;
		Buffer const & m_dst;
		uint32_t m_size;
		uint32_t m_offset;
	};
}

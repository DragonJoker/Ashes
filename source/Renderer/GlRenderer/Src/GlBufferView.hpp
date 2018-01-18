/**
*\file
*	Texture.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_BufferView_HPP___
#define ___GlRenderer_BufferView_HPP___
#pragma once

#include "GlRendererPrerequisites.hpp"

#include <Buffer/BufferView.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Une vue sur un tampon.
	*/
	class BufferView
		: public renderer::BufferView
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*/
		BufferView( renderer::Device const & device
			, Buffer const & buffer
			, renderer::PixelFormat format
			, uint32_t offset
			, uint32_t range );
		/**
		*\brief
		*	Destructeur.
		*/
		~BufferView();
	};
}

#endif

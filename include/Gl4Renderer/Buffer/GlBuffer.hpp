/**
*\file
*	RenderingResources.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_Buffer_HPP___
#define ___GlRenderer_Buffer_HPP___
#pragma once

#include "Gl4Renderer/GlRendererPrerequisites.hpp"

#include <Ashes/Buffer/Buffer.hpp>

namespace gl_renderer
{
	class Buffer
		: public ashes::BufferBase
	{
	public:
		Buffer( Device const & device
			, uint32_t size
			, ashes::BufferTargets target );
		~Buffer();
		/**
		*\copydoc	ashes::BufferBase::getMemoryRequirements
		*/
		ashes::MemoryRequirements getMemoryRequirements()const override;
		/**
		*\return
		*	Le tampon.
		*/
		inline GLuint getInternal()const
		{
			assert( m_name != GL_INVALID_INDEX );
			return m_name;
		}
		/**
		*\return
		*	La cible du tampon.
		*/
		inline GlBufferTarget getTarget()const
		{
			return m_target;
		}

	private:
		void doBindMemory()override;

	public:
		mutable BufferDestroySignal onDestroy;

	private:
		Device const & m_device;
		GLuint m_name{ GL_INVALID_INDEX };
		GlBufferTarget m_target;
		mutable GlBufferTarget m_copyTarget;
	};
}

#endif

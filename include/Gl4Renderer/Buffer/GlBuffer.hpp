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
	{
	public:
		Buffer( Device const & device
			, VkBufferCreateInfo createInfo );
		~Buffer();
		/**
		*\copydoc	ashes::BufferBase::getMemoryRequirements
		*/
		VkMemoryRequirements getMemoryRequirements()const;
		/**
		*\copydoc	ashes::Image::bindMemory
		*/
		void bindMemory( DeviceMemoryPtr memory )const;
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

	public:
		mutable BufferDestroySignal onDestroy;

	private:
		Device const & m_device;
		VkBufferCreateInfo m_createInfo;
		GLuint m_name{ GL_INVALID_INDEX };
		GlBufferTarget m_target;
		mutable DeviceMemoryPtr m_storage;
		mutable GlBufferTarget m_copyTarget;
	};
}

#endif

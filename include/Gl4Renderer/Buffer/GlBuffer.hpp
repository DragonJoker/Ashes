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
#include "Gl4Renderer/Enum/GlBufferTarget.hpp"

namespace ashes::gl4
{
	class Buffer
	{
	public:
		Buffer( VkDevice device
			, VkBufferCreateInfo createInfo );
		~Buffer();
		/**
		*\copydoc	VkBuffer::getMemoryRequirements
		*/
		VkMemoryRequirements getMemoryRequirements()const;
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
		VkDevice m_device;
		VkBufferCreateFlags m_flags;
		VkDeviceSize m_size;
		VkBufferUsageFlags m_usage;
		VkSharingMode m_sharingMode;
		UInt32Array m_queueFamilyIndices;
		GLuint m_name{ GL_INVALID_INDEX };
		GlBufferTarget m_target;
		mutable VkDeviceMemory m_storage;
		mutable GlBufferTarget m_copyTarget;
	};
}

#endif

/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_BufferView_HPP___
#define ___GlRenderer_BufferView_HPP___
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"

namespace ashes::gl
{
	class BufferView
		: public IcdObject
	{
	public:
		BufferView( VkAllocationCallbacks const * allocInfo
			, VkDevice device
			, VkBufferViewCreateInfo createInfo );
		~BufferView();

		GLuint getImage()const
		{
			return getInternal();
		}

		VkDevice getDevice()const
		{
			return m_device;
		}

		VkFormat getFormat()const
		{
			return m_format;
		}

	private:
		VkDevice m_device;
		VkFormat m_format;
		VkDeviceSize m_offset;
		VkDeviceSize m_range;
	};
}

#endif

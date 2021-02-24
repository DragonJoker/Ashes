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

		inline GLuint getImage()const
		{
			return getInternal();
		}

		inline VkDevice getDevice()const
		{
			return m_device;
		}

	private:
		VkDevice m_device;
		VkFormat m_format;
		VkDeviceSize m_offset;
		VkDeviceSize m_range;
	};
}

#endif

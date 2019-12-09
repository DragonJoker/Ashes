/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_BufferView_HPP___
#define ___GlRenderer_BufferView_HPP___
#pragma once

#include "renderer/Gl4Renderer/GlRendererPrerequisites.hpp"

namespace ashes::gl4
{
	class BufferView
		: public IcdObject
	{
	public:
		BufferView( VkDevice device
			, VkBufferViewCreateInfo createInfo );
		~BufferView();

		inline GLuint getImage()const
		{
			return getInternal();
		}

	private:
		VkDevice m_device;
		VkFormat m_format;
		VkDeviceSize m_offset;
		VkDeviceSize m_range;
	};
}

#endif

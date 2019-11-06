/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_BufferView_HPP___
#define ___GlRenderer_BufferView_HPP___
#pragma once

#include "renderer/Gl3Renderer/GlRendererPrerequisites.hpp"

namespace ashes::gl3
{
	class BufferView
	{
	public:
		BufferView( VkDevice device
			, VkBufferViewCreateInfo createInfo );
		~BufferView();

		inline GLuint getImage()const
		{
			return m_name;
		}

	private:
		VkDevice m_device;
		VkFormat m_format;
		VkDeviceSize m_offset;
		VkDeviceSize m_range;
		GLuint m_name{ GL_INVALID_INDEX };
	};
}

#endif

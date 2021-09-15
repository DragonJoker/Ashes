/**
*\file
*	TestBufferView.h
*\author
*	Sylvain Doremus
*/
#ifndef ___TestRenderer_BufferView_HPP___
#define ___TestRenderer_BufferView_HPP___
#pragma once

#include "renderer/TestRenderer/TestRendererPrerequisites.hpp"

namespace ashes::test
{
	class BufferView
	{
	public:
		BufferView( VkDevice device
			, VkBufferViewCreateInfo createInfo );
		~BufferView();

		VkDevice getDevice()const
		{
			return m_device;
		}

	private:
		VkDevice m_device;
		VkBufferViewCreateInfo m_createInfo;
	};
}

#endif

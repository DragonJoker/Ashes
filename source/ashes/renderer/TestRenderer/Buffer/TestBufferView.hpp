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

		VkDevice getDevice()const noexcept
		{
			return m_device;
		}

		VkBufferViewCreateInfo const & getCreateInfo()const noexcept
		{
			return m_createInfo;
		}

	private:
		VkDevice m_device;
		VkBufferViewCreateInfo m_createInfo;
	};
}

#endif

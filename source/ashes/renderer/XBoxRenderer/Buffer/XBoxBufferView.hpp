/**
*\file
*	TestBufferView.h
*\author
*	Sylvain Doremus
*/
#ifndef ___XBOXRenderer_BufferView_HPP___
#define ___XBOXRenderer_BufferView_HPP___
#pragma once

#include "renderer/XBoxRenderer/XBoxRendererPrerequisites.hpp"

namespace ashes::xbox
{
	class BufferView
		: public NonCopyable
	{
	public:
		BufferView( VkDevice device
			, VkBufferViewCreateInfo createInfo );
		~BufferView()noexcept;

		ID3D11ShaderResourceView * getView()const
		{
			return m_view;
		}

		VkDevice getDevice()const
		{
			return m_device;
		}

	private:
		VkDevice m_device;
		VkBufferViewCreateInfo m_createInfo;
		ID3D11ShaderResourceView * m_view;
	};
}

#endif

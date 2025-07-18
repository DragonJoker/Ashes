/**
*\file
*	TestBufferView.h
*\author
*	Sylvain Doremus
*/
#ifndef ___D3D11Renderer_BufferView_HPP___
#define ___D3D11Renderer_BufferView_HPP___
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

namespace ashes::D3D11_NAMESPACE
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

/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

namespace ashes::d3d11
{
	class Framebuffer
	{
	public:
		Framebuffer( VkDevice device
			, VkFramebufferCreateInfo createInfo );
		~Framebuffer();

		inline VkExtent2D const & getDimensions()const noexcept
		{
			return m_dimensions;
		}

		inline std::vector< ID3D11View * > const & getAllViews()const
		{
			return m_allViews;
		}

		inline std::vector< ID3D11RenderTargetView * > const & getRTViews()const
		{
			return m_rtViews;
		}

		inline ID3D11DepthStencilView * getDSView()const
		{
			return m_dsView;
		}

		inline UINT getDSViewFlags()const
		{
			return m_dsViewFlags;
		}

	private:
		VkDevice m_device;
		VkFramebufferCreateInfo m_createInfo;
		VkImageViewArray m_views;
		VkExtent2D m_dimensions;
		std::vector< ID3D11View * > m_allViews;
		std::vector< ID3D11RenderTargetView * > m_rtViews;
		ID3D11DepthStencilView * m_dsView{ nullptr };
		UINT m_dsViewFlags{ 0u };
	};
}

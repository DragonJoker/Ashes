/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

namespace ashes::D3D11_NAMESPACE
{
	struct FboAttach
	{
		FboAttach( VkImageView imageView
			, ID3D11View * view
			, ID3D11Resource * resource
			, UINT subresource )
			: imageView{ imageView }
			, view{ view }
			, resource{ resource }
			, subresource{ subresource }
		{
		}

		VkImageView imageView{};
		ID3D11View * view{};
		ID3D11Resource * resource{};
		UINT subresource{};
	};
	using FboAttachPtr = std::unique_ptr< FboAttach >;

	class Framebuffer
	{
	public:
		Framebuffer( VkDevice device
			, VkFramebufferCreateInfo createInfo );

		VkExtent2D const & getDimensions()const noexcept
		{
			return m_dimensions;
		}

		std::vector< FboAttachPtr > const & getAllViews()const noexcept
		{
			return m_allViews;
		}

		std::vector< FboAttach * > const & getRTViews()const noexcept
		{
			return m_rtViews;
		}

		std::vector< FboAttach * > const & getMsRTViews()const noexcept
		{
			return m_msRtViews;
		}

		FboAttach * getDSView()const noexcept
		{
			return m_dsView;
		}

		FboAttach * getMsDSView()const noexcept
		{
			return m_msDsView;
		}

		UINT getDSViewFlags()const noexcept
		{
			return m_dsViewFlags;
		}

		bool isMultisampled()const noexcept
		{
			return m_multisampled;
		}

		VkDevice getDevice()const noexcept
		{
			return m_device;
		}

	private:
		VkDevice m_device;
		VkFramebufferCreateInfo m_createInfo;
		VkImageViewArray m_views;
		VkExtent2D m_dimensions;
		std::vector< FboAttachPtr > m_allViews;
		std::vector< FboAttach * > m_rtViews;
		std::vector< FboAttach * > m_msRtViews;
		FboAttach * m_dsView{ nullptr };
		FboAttach * m_msDsView{ nullptr };
		UINT m_dsViewFlags{ 0u };
		bool m_multisampled{ false };
	};
}

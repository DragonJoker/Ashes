/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

namespace ashes::d3d11
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

		VkImageView imageView;
		ID3D11View * view;
		ID3D11Resource * resource;
		UINT subresource;
	};
	using FboAttachPtr = std::unique_ptr< FboAttach >;

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

		inline std::vector< FboAttachPtr > const & getAllViews()const
		{
			return m_allViews;
		}

		inline std::vector< FboAttach * > const & getRTViews()const
		{
			return m_rtViews;
		}

		inline std::vector< FboAttach * > const & getMsRTViews()const
		{
			return m_msRtViews;
		}

		inline FboAttach * getDSView()const
		{
			return m_dsView;
		}

		inline FboAttach * getMsDSView()const
		{
			return m_msDsView;
		}

		inline UINT getDSViewFlags()const
		{
			return m_dsViewFlags;
		}

		bool isMultisampled()const
		{
			return m_multisampled;
		}

		VkDevice getDevice()const
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

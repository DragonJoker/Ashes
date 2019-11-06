/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/TestRendererPrerequisites.hpp"

namespace ashes::test
{
	class Framebuffer
	{
	public:
		Framebuffer( VkDevice device
			, VkFramebufferCreateInfo createInfo );

		inline VkExtent2D const & getDimensions()const noexcept
		{
			return m_dimensions;
		}

		inline VkImageViewArray const & getAllViews()const
		{
			return m_views;
		}

		inline VkImageViewArray const & getRTViews()const
		{
			return m_rtViews;
		}

		inline VkImageViewArray const & getMsRTViews()const
		{
			return m_msRtViews;
		}

		inline VkImageView getDSView()const
		{
			return m_dsView;
		}

		inline VkImageView getMsDSView()const
		{
			return m_msDsView;
		}

		bool isMultisampled()const
		{
			return m_multisampled;
		}

	private:
		VkDevice m_device;
		VkFramebufferCreateInfo m_createInfo;
		VkImageViewArray m_views;
		VkExtent2D m_dimensions;
		VkImageViewArray m_rtViews;
		VkImageViewArray m_msRtViews;
		VkImageView m_dsView{ VK_NULL_HANDLE };
		VkImageView m_msDsView{ VK_NULL_HANDLE };
		bool m_multisampled{ false };
	};
}

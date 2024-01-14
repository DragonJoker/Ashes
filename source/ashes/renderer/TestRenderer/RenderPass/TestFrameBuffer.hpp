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

		VkExtent2D const & getDimensions()const noexcept
		{
			return m_dimensions;
		}

		VkImageViewArray const & getAllViews()const noexcept
		{
			return m_views;
		}

		VkImageViewArray const & getRTViews()const noexcept
		{
			return m_rtViews;
		}

		VkImageViewArray const & getMsRTViews()const noexcept
		{
			return m_msRtViews;
		}

		VkImageView getDSView()const noexcept
		{
			return m_dsView;
		}

		VkImageView getMsDSView()const noexcept
		{
			return m_msDsView;
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
		VkImageViewArray m_rtViews;
		VkImageViewArray m_msRtViews;
		VkImageView m_dsView{};
		VkImageView m_msDsView{};
		bool m_multisampled{ false };
	};
}

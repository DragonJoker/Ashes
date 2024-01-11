/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_RenderPassCreateInfo_HPP___
#define ___AshesPP_RenderPassCreateInfo_HPP___
#pragma once

#include "SubpassDescription.hpp"

namespace ashes
{
	struct RenderPassCreateInfo
	{
		~RenderPassCreateInfo()noexcept = default;

		RenderPassCreateInfo( VkRenderPassCreateFlags flags
			, VkAttachmentDescriptionArray pattachments
			, SubpassDescriptionArray psubpasses
			, VkSubpassDependencyArray pdependencies )
			: attachments{ std::move( pattachments ) }
			, subpasses{ std::move( psubpasses ) }
			, dependencies{ std::move( pdependencies ) }
			, vkSubpasses{ makeVkArray< VkSubpassDescription >( subpasses ) }
			, vk{ VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO
				, nullptr
				, flags
				, uint32_t( attachments.size() )
				, attachments.data()
				, uint32_t( vkSubpasses.size() )
				, vkSubpasses.data()
				, uint32_t( dependencies.size() )
				, dependencies.data() }
		{
		}

		RenderPassCreateInfo( RenderPassCreateInfo const & rhs )
			: attachments{ rhs.attachments }
			, subpasses{ rhs.subpasses }
			, dependencies{ rhs.dependencies }
			, vkSubpasses{ makeVkArray< VkSubpassDescription >( subpasses ) }
			, vk{ VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO
				, rhs.vk.pNext
				, rhs.vk.flags
				, uint32_t( attachments.size() )
				, attachments.data()
				, uint32_t( vkSubpasses.size() )
				, vkSubpasses.data()
				, uint32_t( dependencies.size() )
				, dependencies.data() }
		{
		}

		RenderPassCreateInfo( RenderPassCreateInfo && rhs )noexcept
			: attachments{ std::move( rhs.attachments ) }
			, subpasses{ std::move( rhs.subpasses ) }
			, dependencies{ std::move( rhs.dependencies ) }
			, vkSubpasses{ makeVkArray< VkSubpassDescription >( subpasses ) }
			, vk{ VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO
			, rhs.vk.pNext
			, rhs.vk.flags
			, uint32_t( attachments.size() )
			, attachments.data()
			, uint32_t( vkSubpasses.size() )
			, vkSubpasses.data()
			, uint32_t( dependencies.size() )
			, dependencies.data() }
		{}

		RenderPassCreateInfo & operator=( RenderPassCreateInfo const & rhs )
		{
			attachments = rhs.attachments;
			subpasses = rhs.subpasses;
			dependencies = rhs.dependencies;
			vkSubpasses = makeVkArray< VkSubpassDescription >( subpasses );
			vk = { VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO
				, rhs.vk.pNext
				, rhs.vk.flags
				, uint32_t( attachments.size() )
				, attachments.data()
				, uint32_t( vkSubpasses.size() )
				, vkSubpasses.data()
				, uint32_t( dependencies.size() )
				, dependencies.data() };

			return *this;
		}

		RenderPassCreateInfo & operator=( RenderPassCreateInfo && rhs )noexcept
		{
			attachments = std::move( rhs.attachments );
			subpasses = std::move( rhs.subpasses );
			dependencies = std::move( rhs.dependencies );
			vkSubpasses = makeVkArray< VkSubpassDescription >( subpasses );
			vk = { VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO
				, rhs.vk.pNext
				, rhs.vk.flags
				, uint32_t( attachments.size() )
				, attachments.data()
				, uint32_t( vkSubpasses.size() )
				, vkSubpasses.data()
				, uint32_t( dependencies.size() )
				, dependencies.data() };

			return *this;
		}

		operator VkRenderPassCreateInfo const &()const noexcept
		{
			return vk;
		}

		VkRenderPassCreateInfo const * operator->()const noexcept
		{
			return &vk;
		}

		VkRenderPassCreateInfo * operator->()noexcept
		{
			return &vk;
		}

		VkAttachmentDescriptionArray attachments;
		SubpassDescriptionArray subpasses;
		VkSubpassDependencyArray dependencies;

	private:
		VkSubpassDescriptionArray vkSubpasses;
		VkRenderPassCreateInfo vk;
	};
}

#endif

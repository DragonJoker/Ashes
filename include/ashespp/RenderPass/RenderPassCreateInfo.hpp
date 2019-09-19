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
		RenderPassCreateInfo( RenderPassCreateInfo const & ) = delete;
		RenderPassCreateInfo& operator=( RenderPassCreateInfo  const & ) = delete;

		RenderPassCreateInfo( VkRenderPassCreateFlags flags
			, VkAttachmentDescriptionArray attachments
			, SubpassDescriptionArray subpasses
			, VkSubpassDependencyArray dependencies )
			: attachments{ std::move( attachments ) }
			, subpasses{ std::move( subpasses ) }
			, dependencies{ std::move( dependencies ) }
			, vkSubpasses{ makeVkArray< VkSubpassDescription >( this->subpasses ) }
			, vk
			{
				VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
				nullptr,
				flags,
				uint32_t( this->attachments.size() ),
				this->attachments.data(),
				uint32_t( this->vkSubpasses.size() ),
				this->vkSubpasses.data(),
				uint32_t( this->dependencies.size() ),
				this->dependencies.data(),
			}
		{
		}

		RenderPassCreateInfo( RenderPassCreateInfo && rhs )
			: attachments{ std::move( rhs.attachments ) }
			, subpasses{ std::move( rhs.subpasses ) }
			, dependencies{ std::move( rhs.dependencies ) }
			, vkSubpasses{ makeVkArray< VkSubpassDescription >( this->subpasses ) }
			, vk
			{
				VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
				nullptr,
				rhs.vk.flags,
				uint32_t( this->attachments.size() ),
				this->attachments.data(),
				uint32_t( this->vkSubpasses.size() ),
				this->vkSubpasses.data(),
				uint32_t( this->dependencies.size() ),
				this->dependencies.data(),
			}
		{
		}

		RenderPassCreateInfo & operator=( RenderPassCreateInfo && rhs )
		{
			attachments = std::move( rhs.attachments );
			subpasses = std::move( rhs.subpasses );
			dependencies = std::move( rhs.dependencies );
			vkSubpasses = makeVkArray< VkSubpassDescription >( this->subpasses );
			vk =
			{
				VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
				nullptr,
				rhs.vk.flags,
				uint32_t( this->attachments.size() ),
				this->attachments.data(),
				uint32_t( this->vkSubpasses.size() ),
				this->vkSubpasses.data(),
				uint32_t( this->dependencies.size() ),
				this->dependencies.data(),
			};

			return *this;
		}

		inline operator VkRenderPassCreateInfo const &()const
		{
			return vk;
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

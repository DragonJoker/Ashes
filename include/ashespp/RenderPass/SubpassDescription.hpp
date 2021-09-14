/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_SubpassDescription_HPP___
#define ___AshesPP_SubpassDescription_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
{
	struct SubpassDescription
	{
		SubpassDescription( SubpassDescription const & ) = delete;
		SubpassDescription & operator=( SubpassDescription  const & ) = delete;

		SubpassDescription( VkSubpassDescriptionFlags flags
			, VkPipelineBindPoint pipelineBindPoint
			, VkAttachmentReferenceArray pinputAttachments
			, VkAttachmentReferenceArray pcolorAttachments
			, VkAttachmentReferenceArray presolveAttachments
			, Optional< VkAttachmentReference > pdepthStencilAttachment = Optional< VkAttachmentReference >{}
			, UInt32Array preserveAttachments = {} )
			: inputAttachments{ std::move( pinputAttachments ) }
			, colorAttachments{ std::move( pcolorAttachments ) }
			, resolveAttachments{ std::move( presolveAttachments ) }
			, depthStencilAttachment{ std::move( pdepthStencilAttachment ) }
			, reserveAttachments{ std::move( preserveAttachments ) }
			, vk
			{
				flags,
				pipelineBindPoint,
				uint32_t( inputAttachments.size() ),
				inputAttachments.data(),
				uint32_t( colorAttachments.size() ),
				colorAttachments.data(),
				resolveAttachments.data(),
				bool( depthStencilAttachment ) ? &depthStencilAttachment.value() : nullptr,
				uint32_t( reserveAttachments.size() ),
				reserveAttachments.data(),
			}
		{
		}

		SubpassDescription( SubpassDescription && rhs )noexcept
			: inputAttachments{ std::move( rhs.inputAttachments ) }
			, colorAttachments{ std::move( rhs.colorAttachments ) }
			, resolveAttachments{ std::move( rhs.resolveAttachments ) }
			, depthStencilAttachment{ std::move( rhs.depthStencilAttachment ) }
			, reserveAttachments{ std::move( rhs.reserveAttachments ) }
			, vk
			{
				rhs.vk.flags,
				rhs.vk.pipelineBindPoint,
				uint32_t( inputAttachments.size() ),
				inputAttachments.data(),
				uint32_t( colorAttachments.size() ),
				colorAttachments.data(),
				resolveAttachments.data(),
				bool( depthStencilAttachment ) ? &depthStencilAttachment.value() : nullptr,
				uint32_t( reserveAttachments.size() ),
				reserveAttachments.data(),
			}
		{
		}

		SubpassDescription & operator=( SubpassDescription && rhs )noexcept
		{
			inputAttachments = std::move( rhs.inputAttachments );
			colorAttachments = std::move( rhs.colorAttachments );
			resolveAttachments = std::move( rhs.resolveAttachments );
			depthStencilAttachment = std::move( rhs.depthStencilAttachment );
			reserveAttachments = std::move( rhs.reserveAttachments );
			vk =
			{
				rhs.vk.flags,
				rhs.vk.pipelineBindPoint,
				uint32_t( inputAttachments.size() ),
				inputAttachments.data(),
				uint32_t( colorAttachments.size() ),
				colorAttachments.data(),
				resolveAttachments.data(),
				bool( depthStencilAttachment ) ? &depthStencilAttachment.value() : nullptr,
				uint32_t( reserveAttachments.size() ),
				reserveAttachments.data(),
			};

			return *this;
		}

		inline operator VkSubpassDescription const &()const
		{
			return vk;
		}

		VkAttachmentReferenceArray inputAttachments;
		VkAttachmentReferenceArray colorAttachments;
		VkAttachmentReferenceArray resolveAttachments;
		Optional< VkAttachmentReference > depthStencilAttachment;
		UInt32Array reserveAttachments;

	private:
		VkSubpassDescription vk;
	};
	using SubpassDescriptionArray = std::vector< SubpassDescription >;
}

#endif

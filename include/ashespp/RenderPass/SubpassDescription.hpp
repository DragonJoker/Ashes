/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_SubpassDescription_HPP___
#define ___Ashes_SubpassDescription_HPP___
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
			, VkAttachmentReferenceArray inputAttachments
			, VkAttachmentReferenceArray colorAttachments
			, VkAttachmentReferenceArray resolveAttachments
			, Optional< VkAttachmentReference > depthStencilAttachment = std::nullopt
			, UInt32Array reserveAttachments = {} )
			: inputAttachments{ std::move( inputAttachments ) }
			, colorAttachments{ std::move( colorAttachments ) }
			, resolveAttachments{ std::move( resolveAttachments ) }
			, depthStencilAttachment{ std::move( depthStencilAttachment ) }
			, reserveAttachments{ std::move( reserveAttachments ) }
			, vk
			{
				flags,
				pipelineBindPoint,
				uint32_t( this->inputAttachments.size() ),
				this->inputAttachments.data(),
				uint32_t( this->colorAttachments.size() ),
				this->colorAttachments.data(),
				this->resolveAttachments.data(),
				bool( this->depthStencilAttachment ) ? &this->depthStencilAttachment.value() : nullptr,
				uint32_t( this->reserveAttachments.size() ),
				this->reserveAttachments.data(),
			}
		{
		}

		SubpassDescription( SubpassDescription && rhs )
			: inputAttachments{ std::move( rhs.inputAttachments ) }
			, colorAttachments{ std::move( rhs.colorAttachments ) }
			, resolveAttachments{ std::move( rhs.resolveAttachments ) }
			, depthStencilAttachment{ std::move( rhs.depthStencilAttachment ) }
			, reserveAttachments{ std::move( rhs.reserveAttachments ) }
			, vk
			{
				rhs.vk.flags,
				rhs.vk.pipelineBindPoint,
				uint32_t( this->inputAttachments.size() ),
				this->inputAttachments.data(),
				uint32_t( this->colorAttachments.size() ),
				this->colorAttachments.data(),
				this->resolveAttachments.data(),
				bool( this->depthStencilAttachment ) ? &this->depthStencilAttachment.value() : nullptr,
				uint32_t( this->reserveAttachments.size() ),
				this->reserveAttachments.data(),
			}
		{
		}

		SubpassDescription & operator=( SubpassDescription && rhs )
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
				uint32_t( this->inputAttachments.size() ),
				this->inputAttachments.data(),
				uint32_t( this->colorAttachments.size() ),
				this->colorAttachments.data(),
				this->resolveAttachments.data(),
				bool( this->depthStencilAttachment ) ? &this->depthStencilAttachment.value() : nullptr,
				uint32_t( this->reserveAttachments.size() ),
				this->reserveAttachments.data(),
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

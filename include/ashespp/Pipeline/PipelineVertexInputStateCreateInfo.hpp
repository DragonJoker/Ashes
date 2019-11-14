/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_PipelineVertexInputStateCreateInfo_HPP___
#define ___AshesPP_PipelineVertexInputStateCreateInfo_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

#include <type_traits>
#include <vector>

namespace ashes
{
	struct PipelineVertexInputStateCreateInfo
	{
		PipelineVertexInputStateCreateInfo( VkPipelineVertexInputStateCreateFlags flags
			, VkVertexInputBindingDescriptionArray vertexBindingDescriptions
			, VkVertexInputAttributeDescriptionArray vertexAttributeDescriptions )
			: vertexBindingDescriptions{ std::move( vertexBindingDescriptions ) }
			, vertexAttributeDescriptions{ std::move( vertexAttributeDescriptions ) }
			, vk
			{
				VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
				nullptr,
				flags,
				uint32_t( this->vertexBindingDescriptions.size() ),
				this->vertexBindingDescriptions.data(),
				uint32_t( this->vertexAttributeDescriptions.size() ),
				this->vertexAttributeDescriptions.data(),
			}
		{
		}

		PipelineVertexInputStateCreateInfo( PipelineVertexInputStateCreateInfo const & rhs )
			: vertexBindingDescriptions{ rhs.vertexBindingDescriptions }
			, vertexAttributeDescriptions{ rhs.vertexAttributeDescriptions }
			, vk
			{
				VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
				nullptr,
				rhs.vk.flags,
				uint32_t( vertexBindingDescriptions.size() ),
				vertexBindingDescriptions.data(),
				uint32_t( vertexAttributeDescriptions.size() ),
				vertexAttributeDescriptions.data(),
			}
		{
		}

		PipelineVertexInputStateCreateInfo( PipelineVertexInputStateCreateInfo && rhs )
			: vertexBindingDescriptions{ std::move( rhs.vertexBindingDescriptions ) }
			, vertexAttributeDescriptions{ std::move( rhs.vertexAttributeDescriptions ) }
			, vk
			{
				VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
				nullptr,
				rhs.vk.flags,
				uint32_t( this->vertexBindingDescriptions.size() ),
				this->vertexBindingDescriptions.data(),
				uint32_t( this->vertexAttributeDescriptions.size() ),
				this->vertexAttributeDescriptions.data(),
			}
		{
		}

		PipelineVertexInputStateCreateInfo & operator=( PipelineVertexInputStateCreateInfo const & rhs )
		{
			vertexBindingDescriptions = rhs.vertexBindingDescriptions;
			vertexAttributeDescriptions = rhs.vertexAttributeDescriptions;
			vk =
			{
				VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
				nullptr,
				rhs.vk.flags,
				uint32_t( vertexBindingDescriptions.size() ),
				vertexBindingDescriptions.data(),
				uint32_t( vertexAttributeDescriptions.size() ),
				vertexAttributeDescriptions.data(),
			};

			return *this;
		}

		PipelineVertexInputStateCreateInfo & operator=( PipelineVertexInputStateCreateInfo && rhs )
		{
			vertexBindingDescriptions = std::move( rhs.vertexBindingDescriptions );
			vertexAttributeDescriptions = std::move( rhs.vertexAttributeDescriptions );
			vk =
			{
				VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
				nullptr,
				rhs.vk.flags,
				uint32_t( this->vertexBindingDescriptions.size() ),
				this->vertexBindingDescriptions.data(),
				uint32_t( this->vertexAttributeDescriptions.size() ),
				this->vertexAttributeDescriptions.data(),
			};

			return *this;
		}

		inline operator VkPipelineVertexInputStateCreateInfo const &()const
		{
			return vk;
		}

		VkVertexInputBindingDescriptionArray vertexBindingDescriptions;
		VkVertexInputAttributeDescriptionArray vertexAttributeDescriptions;

	private:
		VkPipelineVertexInputStateCreateInfo vk;
	};
}

#endif

/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_PipelineInputAssemblyStateCreateInfo_HPP___
#define ___Ashes_PipelineInputAssemblyStateCreateInfo_HPP___
#pragma once

#include "AshesPP/AshesPPPrerequisites.hpp"

#include <AshesRenderer/Helper/InputAssemblyState.hpp>

namespace ashes
{
	struct PipelineInputAssemblyStateCreateInfo
	{
		PipelineInputAssemblyStateCreateInfo( PipelineInputAssemblyStateCreateInfo const & ) = delete;
		PipelineInputAssemblyStateCreateInfo & operator=( PipelineInputAssemblyStateCreateInfo const & ) = delete;

		PipelineInputAssemblyStateCreateInfo( VkPipelineInputAssemblyStateCreateFlags flags = 0u
			, VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
			, VkBool32 primitiveRestartEnable = VK_FALSE )
			: vk
			{
				VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
				nullptr,
				flags,
				topology,
				primitiveRestartEnable,
			}
		{
		}
		
		PipelineInputAssemblyStateCreateInfo( PipelineInputAssemblyStateCreateInfo && rhs )
			: vk
			{
				VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
				nullptr,
				rhs.vk.flags,
				rhs.vk.topology,
				rhs.vk.primitiveRestartEnable,
			}
		{
		}
		
		PipelineInputAssemblyStateCreateInfo & operator=( PipelineInputAssemblyStateCreateInfo && rhs )
		{
			vk =
			{
				VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
				nullptr,
				rhs.vk.flags,
				rhs.vk.topology,
				rhs.vk.primitiveRestartEnable,
			};

			return *this;
		}

		inline operator VkPipelineInputAssemblyStateCreateInfo const &()const
		{
			return vk;
		}

	private:
		VkPipelineInputAssemblyStateCreateInfo vk;
	};
}

#endif

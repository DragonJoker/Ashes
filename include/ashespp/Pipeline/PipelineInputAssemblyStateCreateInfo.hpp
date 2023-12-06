/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_PipelineInputAssemblyStateCreateInfo_HPP___
#define ___AshesPP_PipelineInputAssemblyStateCreateInfo_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
{
	struct PipelineInputAssemblyStateCreateInfo
	{
		PipelineInputAssemblyStateCreateInfo( VkPipelineInputAssemblyStateCreateFlags flags = 0u
			, VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
			, VkBool32 primitiveRestartEnable = VK_FALSE )
			: vk{ VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO
				, nullptr
				, flags
				, topology
				, primitiveRestartEnable }
		{
		}
		
		PipelineInputAssemblyStateCreateInfo( PipelineInputAssemblyStateCreateInfo const & rhs )
			: vk{ rhs.vk }
		{
		}

		PipelineInputAssemblyStateCreateInfo( PipelineInputAssemblyStateCreateInfo && rhs )noexcept
			: vk{ std::move( rhs.vk ) }
		{
		}

		PipelineInputAssemblyStateCreateInfo & operator=( PipelineInputAssemblyStateCreateInfo const & rhs )
		{
			vk = rhs.vk;
			return *this;
		}
		
		PipelineInputAssemblyStateCreateInfo & operator=( PipelineInputAssemblyStateCreateInfo && rhs )noexcept
		{
			vk = std::move( rhs.vk );
			return *this;
		}

		operator VkPipelineInputAssemblyStateCreateInfo const &()const
		{
			return vk;
		}

		VkPipelineInputAssemblyStateCreateInfo const * operator->()const
		{
			return &vk;
		}

		VkPipelineInputAssemblyStateCreateInfo * operator->()
		{
			return &vk;
		}

	private:
		VkPipelineInputAssemblyStateCreateInfo vk;
	};
}

#endif

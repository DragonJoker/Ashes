/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_RayTracingPipelineCreateInfo_HPP___
#define ___AshesPP_RayTracingPipelineCreateInfo_HPP___
#pragma once

#include "ashespp/Pipeline/PipelineShaderStageCreateInfo.hpp"

namespace ashes
{
	using RayTracingShaderGroupCreateInfoArray = std::vector< VkRayTracingShaderGroupCreateInfoKHR >;

	struct RayTracingPipelineCreateInfo
	{
		RayTracingPipelineCreateInfo( RayTracingPipelineCreateInfo const & ) = delete;
		RayTracingPipelineCreateInfo & operator=( RayTracingPipelineCreateInfo  const & ) = delete;

		RayTracingPipelineCreateInfo( VkPipelineCreateFlags pflags
			, PipelineShaderStageCreateInfoArray pstages
			, RayTracingShaderGroupCreateInfoArray pgroups
			, uint32_t maxPipelineRayRecursionDepth
			, Optional< VkPipelineLibraryCreateInfoKHR > plibraryInfo
			, Optional< VkRayTracingPipelineInterfaceCreateInfoKHR > plibraryInterface
			, Optional< VkPipelineDynamicStateCreateInfo > pdynamicState
			, VkPipelineLayout layout
			, VkPipeline basePipelineHandle = {}
			, int32_t basePipelineIndex = {} )
			: stages{ std::move( pstages ) }
			, vkStages{ makeVkArray< VkPipelineShaderStageCreateInfo >( stages ) }
			, groups{ std::move( pgroups ) }
			, libraryInfo{ std::move( plibraryInfo ) }
			, libraryInterface{ std::move( plibraryInterface ) }
			, dynamicState{ std::move( pdynamicState ) }
			, vk{ VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_KHR
				, nullptr
				, pflags
				, uint32_t( vkStages.size() )
				, vkStages.data()
				, uint32_t( groups.size() )
				, groups.data()
				, maxPipelineRayRecursionDepth
				, convert( libraryInfo )
				, convert( libraryInterface )
				, convert( dynamicState )
				, layout
				, basePipelineHandle
				, basePipelineIndex }
		{
		}
		
		RayTracingPipelineCreateInfo( RayTracingPipelineCreateInfo && rhs )noexcept
			: RayTracingPipelineCreateInfo{ rhs.vk.flags
				, std::move( rhs.stages )
				, std::move( rhs.groups )
				, rhs.vk.maxPipelineRayRecursionDepth
				, std::move( rhs.libraryInfo )
				, std::move( rhs.libraryInterface )
				, std::move( rhs.dynamicState )
				, rhs.vk.layout
				, rhs.vk.basePipelineHandle
				, rhs.vk.basePipelineIndex }
		{
		}
		
		RayTracingPipelineCreateInfo & operator=( RayTracingPipelineCreateInfo && rhs )noexcept
		{
			stages = std::move( rhs.stages );
			vkStages = makeVkArray< VkPipelineShaderStageCreateInfo >( stages );
			groups = std::move( rhs.groups );
			libraryInfo = std::move( rhs.libraryInfo );
			libraryInterface = std::move( rhs.libraryInterface );
			dynamicState = std::move( rhs.dynamicState );
			vk = { VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_KHR
				, nullptr
				, rhs.vk.flags
				, uint32_t( vkStages.size() )
				, vkStages.data()
				, uint32_t( groups.size() )
				, groups.data()
				, rhs.vk.maxPipelineRayRecursionDepth
				, convert( libraryInfo )
				, convert( libraryInterface )
				, convert( dynamicState )
				, rhs.vk.layout };
			return *this;
		}

		operator VkRayTracingPipelineCreateInfoKHR const &()const
		{
			return vk;
		}

	private:
		PipelineShaderStageCreateInfoArray stages;
		VkPipelineShaderStageCreateInfoArray vkStages;
		RayTracingShaderGroupCreateInfoArray groups;
		Optional< VkPipelineLibraryCreateInfoKHR > libraryInfo;
		Optional< VkRayTracingPipelineInterfaceCreateInfoKHR > libraryInterface;
		Optional< VkPipelineDynamicStateCreateInfo > dynamicState;
		VkRayTracingPipelineCreateInfoKHR vk;
	};
	using RayTracingPipelineCreateInfoArray = std::vector< RayTracingPipelineCreateInfo >;

	template< typename Type >
	std::vector< Type > makeArray( std::vector< VkRayTracingPipelineCreateInfoKHR > const & values )
	{
		std::vector< Type > result;
		result.reserve( values.size() );

		for ( auto & value : values )
		{
			result.emplace_back( value );
		}

		return result;
	}

	template< typename Type >
	std::vector< Type > makeArray( VkRayTracingPipelineCreateInfoKHR const * values
		, uint32_t count )
	{
		std::vector< Type > result;
		result.reserve( count );

		for ( uint32_t i = 0u; i < count; ++i )
		{
			result.emplace_back( values[i] );
		}

		return result;
	}
}

#endif

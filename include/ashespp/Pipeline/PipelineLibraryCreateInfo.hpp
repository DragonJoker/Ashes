/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_PipelineLibraryCreateInfo_HPP___
#define ___AshesPP_PipelineLibraryCreateInfo_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
{
	using PipelineArray = std::vector< VkPipeline >;

	struct PipelineLibraryCreateInfo
	{
		PipelineLibraryCreateInfo( PipelineLibraryCreateInfo const & ) = delete;
		PipelineLibraryCreateInfo& operator=( PipelineLibraryCreateInfo  const & ) = delete;

		PipelineLibraryCreateInfo( PipelineArray plibraries )
			: libraries{ std::move( plibraries ) }
			, vk{ VK_STRUCTURE_TYPE_PIPELINE_LIBRARY_CREATE_INFO_KHR
				, nullptr
				, uint32_t( libraries.size() )
				, libraries.data() }
		{
		}
		
		PipelineLibraryCreateInfo( PipelineLibraryCreateInfo && rhs )noexcept
			: PipelineLibraryCreateInfo{ std::move( rhs.libraries ) }
		{
		}
		
		PipelineLibraryCreateInfo( VkPipelineLibraryCreateInfoKHR rhs )noexcept
			: PipelineLibraryCreateInfo{ makeArray( rhs.pLibraries, rhs.libraryCount ) }
		{
		}
		
		PipelineLibraryCreateInfo & operator=( PipelineLibraryCreateInfo && rhs )noexcept
		{
			libraries = std::move( rhs.libraries );
			vk = { VK_STRUCTURE_TYPE_PIPELINE_LIBRARY_CREATE_INFO_KHR
				, nullptr
				, uint32_t( libraries.size() )
				, libraries.data() };
			return *this;
		}

		inline operator VkPipelineLibraryCreateInfoKHR const &()const
		{
			return vk;
		}

	private:
		PipelineArray libraries;
		VkPipelineLibraryCreateInfoKHR vk;
	};
	using PipelineLibraryCreateInfoArray = std::vector< PipelineLibraryCreateInfo >;

	inline PipelineLibraryCreateInfo convert( VkPipelineLibraryCreateInfoKHR const & vk )
	{
		return PipelineLibraryCreateInfo{ vk };
	}

	template< typename Type >
	std::vector< Type > makeArray( std::vector< VkPipelineLibraryCreateInfoKHR > const & values )
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
	std::vector< Type > makeArray( VkPipelineLibraryCreateInfoKHR const * values
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

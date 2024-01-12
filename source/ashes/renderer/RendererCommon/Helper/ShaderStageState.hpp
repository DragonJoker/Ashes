/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_ShaderStageState_HPP___
#define ___Ashes_ShaderStageState_HPP___
#pragma once

#include "renderer/RendererCommon/AshesRendererPrerequisites.hpp"

namespace ashes
{
	inline bool operator==( VkSpecializationMapEntry const & lhs, VkSpecializationMapEntry const & rhs )
	{
		return lhs.constantID == rhs.constantID
			&& lhs.offset == rhs.offset
			&& lhs.size == rhs.size;
	}
	
	inline bool operator!=( VkSpecializationMapEntry const & lhs, VkSpecializationMapEntry const & rhs )
	{
		return !( lhs == rhs );
	}
	
	inline bool operator==( VkSpecializationInfo const & lhs, VkSpecializationInfo const & rhs )
	{
		auto compareEqual = []( auto const * lhsBuffer
			, auto const * rhsBuffer
			, size_t count )
			{
				bool result = true;

				while ( result && count )
				{
					result = ( *lhsBuffer == *rhsBuffer );
					++lhsBuffer;
					++rhsBuffer;
					--count;
				}

				return result;
			};

		return lhs.dataSize == rhs.dataSize
			&& compareEqual( reinterpret_cast< uint8_t const * >( lhs.pData )
				, reinterpret_cast< uint8_t const * >( rhs.pData )
				, lhs.dataSize )
			&& lhs.mapEntryCount == rhs.mapEntryCount
			&& compareEqual( lhs.pMapEntries
				, rhs.pMapEntries
				, lhs.mapEntryCount );
	}
	
	inline bool operator!=( VkSpecializationInfo const & lhs, VkSpecializationInfo const & rhs )
	{
		return !( lhs == rhs );
	}

	inline bool operator==( VkPipelineShaderStageCreateInfo const & lhs, VkPipelineShaderStageCreateInfo const & rhs )
	{
		return lhs.flags == rhs.flags
			&& lhs.module == rhs.module
			&& lhs.pName == rhs.pName
			&& lhs.pNext == rhs.pNext
			&& ( ( lhs.pSpecializationInfo == rhs.pSpecializationInfo )
				|| ( lhs.pSpecializationInfo
					&& rhs.pSpecializationInfo 
					&& *lhs.pSpecializationInfo == *rhs.pSpecializationInfo ) )
			&& lhs.stage == rhs.stage
			&& lhs.sType == rhs.sType;
	}

	inline bool operator!=( VkPipelineShaderStageCreateInfo const & lhs, VkPipelineShaderStageCreateInfo const & rhs )
	{
		return !( lhs == rhs );
	}
}

#endif

/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Buffer/PushConstantsBuffer.hpp"

#include <common/Format.hpp>

#include <algorithm>

namespace ashes
{
	namespace
	{
		uint32_t deduceOffset( PushConstantArray & variables )
		{
			std::sort( variables.begin()
				, variables.end()
				, []( PushConstant const & lhs, PushConstant const & rhs )
			{
				return lhs.offset < rhs.offset;
			} );
			return variables[0].offset;
		}

		uint32_t deduceSize( PushConstantArray const & variables
			, uint32_t baseOffset )
		{
			auto offset = baseOffset;

			for ( auto & constant : variables )
			{
				if ( offset > constant.offset )
				{
					throw std::runtime_error{ "Inconsistency detected between given and computed offsets" };
				}

				offset = constant.offset + getSize( constant.format ) * constant.arraySize;
			}

			return offset - baseOffset;
		}
	}

	PushConstantsBufferBase::PushConstantsBufferBase( VkShaderStageFlags stageFlags
		, PushConstantArray const & variables )
		: m_stageFlags{ stageFlags }
		, m_variables{ variables }
		, m_offset{ deduceOffset( m_variables ) }
		, m_data( deduceSize( m_variables, m_offset ) )
	{
		assert( ( m_offset % 4 ) == 0 );
		assert( ( m_data.size() % 4 ) == 0 );
	}
}

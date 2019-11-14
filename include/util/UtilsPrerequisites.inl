#include "UtilsPrerequisites.hpp"

namespace utils
{
	namespace details
	{
		template< typename T >
		void fillByteArray( T const & value
			, ashes::ByteArray & result )
		{
			auto size = result.size();
			result.resize( size + sizeof( T ) );
			std::memcpy( result.data() + size, &value, sizeof( T ) );
		}

		template< typename T >
		void fillByteArray( std::vector< T > const & values
			, ashes::ByteArray & result )
		{
			for ( auto & value : values )
			{
				fillByteArray( value, result );
			}
		}
	}

	template< typename T >
	ashes::ByteArray makeByteArray( T const & value )
	{
		ashes::ByteArray result;
		result.reserve( sizeof( T ) );
		details::fillByteArray( value, result );
		return result;
	}

	template< typename T >
	ashes::ByteArray makeByteArray( std::vector< T > const & values )
	{
		ashes::ByteArray result;
		result.reserve( sizeof( T ) * values.size() );
		details::fillByteArray( values, result );
		return result;
	}
}

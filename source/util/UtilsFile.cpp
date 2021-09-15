#include "util/UtilsFile.hpp"

#include <iostream>

namespace utils
{
	std::string dumpTextFile( std::string const & path )
	{
		std::ifstream file( path );

		if ( file.fail() )
		{
			throw std::runtime_error{ "Could not open file " + path };
		}

		auto begin = file.tellg();
		file.seekg( 0, std::ios::end );
		auto end = file.tellg();

		std::string result( static_cast< size_t >( end - begin ) + 1u, '\0' );
		file.seekg( 0, std::ios::beg );
		file.read( result.data(), end - begin );

		return result;
	}

	ashes::ByteArray dumpBinaryFile( std::string const & path )
	{
		std::ifstream file( path, std::ios::binary );

		if ( file.fail() )
		{
			throw std::runtime_error{ "Could not open file " + path };
		}

		auto begin = file.tellg();
		file.seekg( 0, std::ios::end );
		auto end = file.tellg();

		ashes::ByteArray result( static_cast< size_t >( end - begin ) );
		file.seekg( 0, std::ios::beg );
		file.read( reinterpret_cast< char * >( result.data() ), end - begin );

		return result;
	}
}

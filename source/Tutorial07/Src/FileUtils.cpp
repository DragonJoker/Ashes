/*
This file belongs to VkLib.
See LICENSE file in root folder.
*/
#include "Prerequisites.hpp"

#include "FileUtils.hpp"

#if defined( __linux__ )
#	include <unistd.h>
#endif

namespace vkapp
{
#if defined( _WIN32 )
	static char constexpr PathSeparator = '\\';
#else
	static char constexpr PathSeparator = '/';
#endif

	ByteArray dumpBinaryFile( std::string const & path )
	{
		std::ifstream file( path, std::ios::binary );

		if ( file.fail() )
		{
			throw std::runtime_error{ "Could not open file " + path };
		}

		auto begin = file.tellg();
		file.seekg( 0, std::ios::end );
		auto end = file.tellg();

		ByteArray result( static_cast< size_t >( end - begin ) );
		file.seekg( 0, std::ios::beg );
		file.read( reinterpret_cast< char * >( result.data() ), end - begin );

		return result;
	}

	UInt32Array dumpSpvFile( std::string const & path )
	{
		std::ifstream file( path, std::ios::binary );

		if ( file.fail() )
		{
			throw std::runtime_error{ "Could not open file " + path };
		}

		auto begin = file.tellg();
		file.seekg( 0, std::ios::end );
		auto end = file.tellg();

		UInt32Array result( static_cast< size_t >( end - begin ) / sizeof( uint32_t ) );
		file.seekg( 0, std::ios::beg );
		file.read( reinterpret_cast< char * >( result.data() ), end - begin );

		return result;
	}

	std::string getExecutableDirectory()
	{
		std::string pathReturn;

#if defined( _WIN32 )

		char path[FILENAME_MAX];
		DWORD result = ::GetModuleFileNameA( nullptr
			, path
			, sizeof( path ) );

		if ( result != 0 )
		{
			pathReturn = path;
		}

#elif defined( __linux__ )

		char path[FILENAME_MAX];
		char buffer[32];
		sprintf( buffer, "/proc/%d/exe", getpid() );
		int bytes = std::min< std::size_t >( readlink( buffer
				, path
				, sizeof( path ) )
			, sizeof( path ) - 1 );

		if ( bytes > 0 )
		{
			path[bytes] = '\0';
			pathReturn = path;
		}

#else

#	error "Unsupported platform"

#endif

		pathReturn = getPath( pathReturn );
		return pathReturn;
	}

	std::string getPath( std::string const & path )
	{
		return path.substr( 0, path.find_last_of( PathSeparator ) );
	}
}

std::string operator/( std::string const & lhs, std::string const & rhs )
{
	return lhs + vkapp::PathSeparator + rhs;
}

#include "common/FileUtils.hpp"

#ifdef _WIN32

#pragma warning( push )
#pragma warning( disable: 4365 )
#pragma warning( disable: 5262 )
#include <array>
#include <cassert>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>

#include <direct.h>
#include <ShlObj.h>
#include <Windows.h>
#pragma warning( pop )

namespace ashes
{
	static char constexpr PathSeparator = '\\';

	std::string getLastErrorText()
	{
		uint32_t errorCode = ::GetLastError();
		std::stringstream stream;
		stream.imbue( std::locale{ "C" } );
		stream << "Error Code: 0x" << std::setw( 8u ) << std::hex << errorCode << ":" << std::endl;

		if ( errorCode == ERROR_SUCCESS )
		{
			stream << " (no error)";
		}
		else
		{
			LPWSTR errorText = nullptr;

			if ( ::FormatMessageW( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
				, nullptr
				, errorCode
				, 0
				, LPWSTR( &errorText )
				, 0
				, nullptr ) == 0 )
			{
				int length = WideCharToMultiByte( CP_UTF8, 0u, errorText, -1, nullptr, 0u, nullptr, nullptr );

				if ( length > 0 )
				{
					std::string converted( size_t( length ), 0 );
					WideCharToMultiByte( CP_UTF8, 0u, errorText, -1, converted.data(), length, nullptr, nullptr );
					stream << converted.c_str() << std::endl;
				}
			}
			else
			{
				stream << " (Unable to retrieve error text)";
			}
		}

		return stream.str();
	}

	bool traverseDirectory( std::string const & folderPath
		, TraverseDirFunction const & directoryFunction
		, HitFileFunction const & fileFunction )
	{
		assert( !folderPath.empty() );

		WIN32_FIND_DATAA findData{};
		HANDLE handle = ::FindFirstFileA( ( folderPath / "*.*" ).c_str(), &findData );

		if ( handle == INVALID_HANDLE_VALUE )
		{
			return false;
		}

		bool result = true;
		std::string name = findData.cFileName;

		if ( name != "." && name != ".." )
		{
			if ( ( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) == FILE_ATTRIBUTE_DIRECTORY )
			{
				result = directoryFunction( folderPath / name );
			}
			else
			{
				fileFunction( folderPath, name );
			}
		}

		while ( result && ::FindNextFileA( handle, &findData ) == TRUE )
		{
			if ( findData.cFileName != name )
			{
				name = findData.cFileName;

				if ( name != "." && name != ".." )
				{
					if ( ( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) == FILE_ATTRIBUTE_DIRECTORY )
					{
						result = directoryFunction( folderPath / name );
					}
					else
					{
						fileFunction( folderPath, name );
					}
				}
			}
		}

		::FindClose( handle );
		return result;
	}

	std::string getPath( std::string const & path )
	{
		return path.substr( 0, path.find_last_of( PathSeparator ) );
	}

	std::string getFileName( std::string const & path )
	{
		return path.substr( path.find_last_of( PathSeparator ) + 1u );
	}

	std::string getExecutableDirectory()
	{
		std::string result;
		std::array< char, FILENAME_MAX > path{};
		
		if ( DWORD ret = ::GetModuleFileNameA( nullptr, path.data(), DWORD( sizeof( char ) * path.size() ) );
			ret != 0 )
		{
			result = path.data();
		}

		result = getPath( result );
		return result;
	}
}

std::string operator/( std::string const & lhs, std::string const & rhs )
{
	return lhs + ashes::PathSeparator + rhs;
}

#endif

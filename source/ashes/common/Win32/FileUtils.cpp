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

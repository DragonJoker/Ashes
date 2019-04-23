#include "common/FileUtils.hpp"

#ifdef _WIN32

#include <cassert>
#include <iostream>

#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>

#include <direct.h>
#include <Shlobj.h>
#include <windows.h>

namespace ashes
{
	static char constexpr PathSeparator = '\\';

	namespace
	{
		template< typename DirectoryFuncType, typename FileFuncType >
		bool TraverseDirectory( std::string const & folderPath
			, DirectoryFuncType directoryFunction
			, FileFuncType fileFunction )
		{
			assert( !folderPath.empty() );
			bool result = false;
			WIN32_FIND_DATAA findData{};
			HANDLE handle = ::FindFirstFileA( ( folderPath / "*.*" ).c_str(), &findData );

			if ( handle != INVALID_HANDLE_VALUE )
			{
				result = true;
				std::string name = findData.cFileName;

				if ( name != "." && name != ".." )
				{
					if ( ( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) == FILE_ATTRIBUTE_DIRECTORY )
					{
						result = directoryFunction( folderPath / name );
					}
					else
					{
						fileFunction( folderPath / name );
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
								fileFunction( folderPath / name );
							}
						}
					}
				}

				::FindClose( handle );
			}

			return result;
		}
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
		std::string pathReturn;
		char path[FILENAME_MAX];
		DWORD result = ::GetModuleFileNameA( nullptr
			, path
			, sizeof( path ) );

		if ( result != 0 )
		{
			pathReturn = path;
		}

		pathReturn = getPath( pathReturn );
		return pathReturn;
	}

	bool listDirectoryFiles( std::string const & folderPath
		, StringArray & files
		, bool recursive )
	{
		struct FileFunction
		{
			explicit FileFunction( StringArray & files )
				: m_files( files )
			{
			}
			void operator()( std::string const & path )
			{
				m_files.push_back( path );
			}
			StringArray & m_files;
		};

		if ( recursive )
		{
			struct DirectoryFunction
			{
				explicit DirectoryFunction( StringArray & files )
					: m_files( files )
				{
				}
				bool operator()( std::string const & path )
				{
					return TraverseDirectory( path
						, DirectoryFunction( m_files )
						, FileFunction( m_files ) );
				}
				StringArray & m_files;
			};

			return TraverseDirectory( folderPath
				, DirectoryFunction( files )
				, FileFunction( files ) );
		}
		else
		{
			struct DirectoryFunction
			{
				DirectoryFunction()
				{
				}
				bool operator()( std::string const & path )
				{
					return true;
				}
			};

			return TraverseDirectory( folderPath
				, DirectoryFunction()
				, FileFunction( files ) );
		}
	}
}

std::string operator/( std::string const & lhs, std::string const & rhs )
{
	return lhs + ashes::PathSeparator + rhs;
}

#endif

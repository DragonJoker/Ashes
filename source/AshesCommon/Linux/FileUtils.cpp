#include "AshesCommon/FileUtils.hpp"

#if defined( __linux__ )

#include <cassert>
#include <iostream>

#include <sys/stat.h>

#include <unistd.h>
#include <dirent.h>
#include <pwd.h>

namespace ashes
{
	static char constexpr PathSeparator = '/';

	namespace
	{
		template< typename DirectoryFuncType, typename FileFuncType >
		bool TraverseDirectory( std::string const & folderPath
			, DirectoryFuncType directoryFunction
			, FileFuncType fileFunction )
		{
			assert( !folderPath.empty() );
			bool result = false;
			DIR * dir;

			if ( ( dir = opendir( folderPath.c_str() ) ) == nullptr )
			{
				switch ( errno )
				{
				case EACCES:
					std::cerr << "Can't open dir : Permission denied - Directory : " << folderPath << std::endl;
					break;

				case EBADF:
					std::cerr << "Can't open dir : Invalid file descriptor - Directory : " << folderPath << std::endl;
					break;

				case EMFILE:
					std::cerr << "Can't open dir : Too many file descriptor in use - Directory : " << folderPath << std::endl;
					break;

				case ENFILE:
					std::cerr << "Can't open dir : Too many files currently open - Directory : " << folderPath << std::endl;
					break;

				case ENOENT:
					std::cerr << "Can't open dir : Directory doesn't exist - Directory : " << folderPath << std::endl;
					break;

				case ENOMEM:
					std::cerr << "Can't open dir : Insufficient memory - Directory : " << folderPath << std::endl;
					break;

				case ENOTDIR:
					std::cerr << "Can't open dir : <name> is not a directory - Directory : " << folderPath << std::endl;
					break;

				default:
					std::cerr << "Can't open dir : Unknown error - Directory : " << folderPath << std::endl;
					break;
				}

				result = false;
			}
			else
			{
				result = true;
				dirent * dirent;

				while ( result && ( dirent = readdir( dir ) ) != nullptr )
				{
					std::string name = dirent->d_name;

					if ( name != "." && name != ".." )
					{
						if ( dirent->d_type == DT_DIR )
						{
							result = directoryFunction( folderPath / name );
						}
						else
						{
							fileFunction( folderPath / name );
						}
					}
				}

				closedir( dir );
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

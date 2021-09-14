#include "ashes/common/FileUtils.hpp"

#if defined( _WIN32 )
#	include <Windows.h>
#endif

namespace ashes
{
	namespace
	{
		struct Folder
		{
			std::string path;
			bool recursive;
		};

#if defined( _WIN32 )
		std::vector< Folder > const & getSystemFolders()
		{
			static std::vector< Folder > result
			{
				{
					[]()
					{
						char tmp[MAX_PATH]{};
						GetSystemDirectoryA( tmp, MAX_PATH );
						return std::string{ tmp };
					}(),
					false,
				},
			};
			return result;
		}
#elif defined( __linux__ )
		std::vector< Folder > const & getSystemFolders()
		{
			static std::vector< Folder > result
			{
				{ "/usr/local/lib", true },
				{ "/usr/lib", true },
			};
			return result;
		}
#elif defined( __APPLE__ )
		std::vector< Folder > const & getSystemFolders()
		{
			static std::vector< Folder > result
			{
				{ "/usr/local/lib", true },
				{ "/usr/lib", true },
			};
			return result;
		}
#else
#	error Unsupported platform
#endif

		std::vector< Folder > const & getSearchFolders()
		{
			static std::vector< Folder > result = []()
			{
				std::vector< Folder > tmp
				{
					{ ashes::getExecutableDirectory(), false },
					{ ashes::getPath( ashes::getExecutableDirectory() ) / "lib", false },
				};
				auto & system = getSystemFolders();
				tmp.insert( tmp.end(), system.begin(), system.end() );
				return tmp;
			}();
			return result;
		}
	}

	StringArray filterDirectoryFiles( std::string const & folderPath
		, FilterFunction onFile
		, bool recursive )
	{
		StringArray files;
		HitFileFunction fileFunction = [&files, onFile]( std::string const & folder
			, std::string const & name )
		{
			if ( onFile( folder, name ) )
			{
				files.push_back( folder / name );
			}

			return true;
		};
		TraverseDirFunction directoryFunction;

		if ( recursive )
		{
			directoryFunction = [&fileFunction, &directoryFunction]( std::string const & path )
			{
				return traverseDirectory( path
					, directoryFunction
					, fileFunction );
			};
		}
		else
		{
			directoryFunction = []( std::string const & path )noexcept
			{
				return true;
			};
		}

		traverseDirectory( folderPath
			, directoryFunction
			, fileFunction );
		return files;
	}

	StringArray listDirectoryFiles( std::string const & folderPath
		, bool recursive )noexcept
	{
		return filterDirectoryFiles( folderPath
			, []( std::string const & folder
				, std::string const & name )noexcept
			{
				return true;
			}
			, recursive );
	}

	StringArray lookForSharedLibrary( FilterFunction onFile )
	{
		StringArray files;

		for ( auto & folder : getSearchFolders() )
		{
			if ( files.empty() )
			{
				files = ashes::filterDirectoryFiles( folder.path
					, onFile
					, folder.recursive );
			}
		}

		return files;
	}
}

#include "ashes/common/FileUtils.hpp"

#if defined( _WIN32 )
#	include <Windows.h>
#endif

#include <array>

namespace ashes
{
	namespace
	{
		struct Folder
		{
			std::string path;
			bool recursive;
		};

		inline std::vector< Folder > const SystemFolders{
#if defined( _WIN32 )
			{ []()
				{
					std::array< char, MAX_PATH > tmp{};
					GetSystemDirectoryA( tmp.data(), MAX_PATH );
					return std::string{ tmp.data() };
				}()
				, false }
#elif defined( __linux__ )
			{ "/usr/local/lib", true },
			{ "/usr/lib", true },
#elif defined( __APPLE__ )
			{ "/usr/local/lib", true },
			{ "/usr/lib", true },
#else
#	error Unsupported platform
#endif
		};

		std::vector< Folder > const & getSystemFolders()
		{
			return SystemFolders;
		}

		inline std::vector< Folder > const SearchFolders = []()
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

		std::vector< Folder > const & getSearchFolders()
		{
			return SearchFolders;
		}
	}

	StringArray filterDirectoryFiles( std::string const & folderPath
		, FilterFunction const & onFile
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
			directoryFunction = []( std::string const & )noexcept
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
			, []( std::string const &
				, std::string const & )noexcept
			{
				return true;
			}
			, recursive );
	}

	StringArray lookForSharedLibrary( FilterFunction const & onFile )
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

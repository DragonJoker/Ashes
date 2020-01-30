#include "ashes/common/FileUtils.hpp"

#if defined( _WIN32 )
#	include <Windows.h>
#endif

namespace ashes
{
	namespace
	{
#if defined( _WIN32 )
		static std::string const systemFolder = []()
		{
			char result[MAX_PATH]{};
			GetSystemDirectoryA( result, MAX_PATH );
			return std::string{ result };
		}();
		static bool recursive{ false };
#elif defined( __linux__ )
		static std::string const systemFolder{ "/usr/lib" };
		static bool recursive{ true };
#elif defined( __APPLE__ )
		static std::string const systemFolder{ "/usr/lib" };
		static bool recursive{ true };
#else
#	error Unsupported platform
#endif
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
			directoryFunction = []( std::string const & path )
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
		, bool recursive )
	{
		return filterDirectoryFiles( folderPath
			, []( std::string const & folder
				, std::string const & name )
			{
				return true;
			}
			, recursive );
	}

	StringArray lookForSharedLibrary( FilterFunction onFile )
	{
		auto binDir = ashes::getExecutableDirectory();
		auto files = ashes::filterDirectoryFiles( binDir
			, onFile
			, false );

		if ( files.empty() )
		{
			files = ashes::filterDirectoryFiles( ashes::getPath( binDir ) / "lib"
				, onFile
				, false );
		}

		if ( files.empty() )
		{
			files = ashes::filterDirectoryFiles( systemFolder
				, onFile
				, recursive );
		}

		return files;
	}
}

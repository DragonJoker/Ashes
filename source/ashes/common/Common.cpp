#include "ashes/common/FileUtils.hpp"

namespace ashes
{
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

	ashes::StringArray listDirectoryFiles( std::string const & folderPath
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
}

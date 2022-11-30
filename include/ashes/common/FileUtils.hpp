/*
See LICENSE file in root folder
*/
#ifndef ___Ashes_common_FileUtils_HPP___
#define ___Ashes_common_FileUtils_HPP___
#pragma once

#pragma warning( push )
#pragma warning( disable: 4365 )
#pragma warning( disable: 5262 )
#include <fstream>
#include <functional>
#include <set>
#include <string>
#include <vector>
#pragma warning( pop )

namespace ashes
{
	using StringArray = std::vector< std::string >;
	using TraverseDirFunction = std::function< bool( std::string const & path ) >;
	using HitFileFunction = std::function< void( std::string const & folder, std::string const & name ) >;
	using FilterFunction = std::function< bool( std::string const & folder, std::string const & name ) >;
	/**
	*\brief
	*	Traverses the files and directories of a directory.
	*\param[in] folderPath
	*	The directory path.
	*\param[in] directoryFunction
	*	returns \p true to traverse it, \p false to ignore.
	*\param[in] fileFunction
	*	Placeholder to handle a file name.
	*\return
	*	\p false if any error occured.
	*/
	bool traverseDirectory( std::string const & folderPath
		, TraverseDirFunction directoryFunction
		, HitFileFunction fileFunction );
	/**
	*\brief
	*	Filters the files in a directory, recursively or not.
	*\param[in] folderPath
	*	The directory path.
	*\param[in] onFile
	*	The filter function, returns \p true to add to the list, \p false to ignore.
	*\param[in] recursive
	*	Tells if search must be recursive.
	*\return
	*	The files list.
	*/
	StringArray filterDirectoryFiles( std::string const & folderPath
		, FilterFunction onFile
		, bool recursive = false );
	/**
	*\brief
	*	Lists all files in a directory, recursively or not.
	*\param[in] folderPath
	*	The directory path.
	*\param[in] recursive
	*	Tells if search must be recursive.
	*\return
	*	The files list.
	*/
	ashes::StringArray listDirectoryFiles( std::string const & folderPath
		, bool recursive = false )noexcept;
	/**
	*\brief
	*	Looks for a library in current bin dir, or current lib dir, or system dir.
	*\param[in] onFile
	*	Filter function to tell if a file is an appropriate library.
	*/
	ashes::StringArray lookForSharedLibrary( FilterFunction onFile );
	/**
	*\return
	*	The executable folder.
	*/
	std::string getExecutableDirectory();
	/**
	*\brief
	*	Retrieves the given path's parent folder.
	*\param[in] path
	*	The path.
	*\return
	*	The parent folder.
	*/
	std::string getPath( std::string const & path );
	/**
	*\brief
	*	Retrieves the given path's file name.
	*\param[in] path
	*	The path.
	*\return
	*	The file name.
	*/
	std::string getFileName( std::string const & path );
}
/**
*\brief
*	Concatenates two paths, adding the folder separator.
*\param[in] lhs, rhs
*	The paths to concatenate.
*\return
*	The concatenated path (lhs / rhs).
*/
std::string operator/( std::string const & lhs, std::string const & rhs );
/**
*\brief
*	Concatenates two paths, adding the folder separator.
*\param[in] lhs, rhs
*	The paths to concatenate.
*\return
*	The concatenated path (lhs / rhs).
*/
inline std::string operator/( std::string const & lhs, char const * const rhs )
{
	return lhs / std::string{ rhs };
}
/**
*\brief
*	Concatenates two paths, adding the folder separator.
*\param[in] lhs, rhs
*	The paths to concatenate.
*\return
*	The concatenated path (lhs / rhs).
*/
inline std::string operator/( char const * const lhs, std::string const & rhs )
{
	return std::string{ lhs } / rhs;
}

#endif

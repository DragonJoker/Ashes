/*
See LICENSE file in root folder
*/
#ifndef ___Ashes_common_FileUtils_HPP___
#define ___Ashes_common_FileUtils_HPP___
#pragma once

#include <fstream>
#include <string>
#include <vector>

namespace ashes
{
	using StringArray = std::vector< std::string >;
	/**
	*\brief
	*	Lists all files in a directory, recursively or not.
	*\param[in] folderPath
	*	The directory path.
	*\param[out] files
	*	Receives the files list.
	*\param[in] recursive
	*	Tells if search must be recursive.
	*\return
	*	\p true if the directory is listed.
	*/
	bool listDirectoryFiles( std::string const & folderPath
		, StringArray & files
		, bool recursive = false );
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

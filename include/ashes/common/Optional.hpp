/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#ifndef ___Ashes_common_Optional_HPP___
#define ___Ashes_common_Optional_HPP___
#pragma once

#if defined( __GNUG__ )
#	define ASHES_COMPILER_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#	if ASHES_COMPILER_VERSION < 40900
#		error "Unsupported version of GCC"
#	elif ASHES_COMPILER_VERSION < 70400
#		include <experimental/optional>
namespace ashes
{
	template< typename T >
	using Optional = std::experimental::optional< T >;
	using std::experimental::nullopt;
}
#	else
#		include <optional>
namespace ashes
{
	template< typename T >
	using Optional = std::optional< T >;
	using std::nullopt;
}
#	endif
#else
#	include <optional>
namespace ashes
{
	template< typename T >
	using Optional = std::optional< T >;
	using std::nullopt;
}
#endif

#endif

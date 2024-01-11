/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once
#ifndef ___Ashes_common_Hash_HPP___
#define ___Ashes_common_Hash_HPP___

#include <cstdint>
#include <functional>

namespace ashes
{
	/**
	*\brief
	*	Combines a hash and a value to hash.
	*\param[in,out] hash
	*	The initial hash, receives the result.
	*\param[in] v
	*	The value to hash-combine.
	*/
	template< typename T >
	void hashCombine( size_t & hash, T const & v )
	{
		uint64_t constexpr kMul = 0x9ddfea08eb382d69ULL;

		std::hash< T > hasher;
		uint64_t a = ( hasher( v ) ^ hash ) * kMul;
		a ^= ( a >> 47 );

		uint64_t b = ( hash ^ a ) * kMul;
		b ^= ( b >> 47 );

		hash = size_t( b * kMul );
	}
}

#endif

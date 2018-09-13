/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_Extent2D_HPP___
#define ___Ashes_Extent2D_HPP___
#pragma once

#include "AshesPrerequisites.hpp"

#include <algorithm>

#if defined( max )
#undef max
#endif

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Specifies a two dimensional extent.
	*\~french
	*\brief
	*	Définit une étendue en 2 dimensions.
	*/
	struct Extent2D
	{
		uint32_t width;
		uint32_t height;
	};

	inline bool operator==( Extent2D const & lhs, Extent2D const & rhs )
	{
		return lhs.width == rhs.width
			&& lhs.height == rhs.height;
	}

	inline bool operator!=( Extent2D const & lhs, Extent2D const & rhs )
	{
		return !( lhs == rhs );
	}
	/**
	*\~english
	*\brief
	*	Retrieves the minimal Extent2D for given pixel format.
	*\param[in] format
	*	The pixel format.
	*\return
	*	The Extent2D.
	*\~french
	*\brief
	*	Donne l'Extent2D minimales pour le format de pixels donné.
	*\param[in] format
	*	Le format de pixel.
	*\return
	*	L'Extent2D.
	*/
	inline Extent2D getMinimalExtent2D( Format format )noexcept
	{
		Extent2D result{ 1, 1 };

		if ( isCompressedFormat( format ) )
		{
			if ( isBCCompressedFormat( format )
				|| isETC2CompressedFormat( format )
				|| isEACCompressedFormat( format ) )
			{
				result.width = 4;
				result.height = 4;
			}
			else if ( isASTCCompressedFormat( format ) )
			{
				switch ( format )
				{
				case Format::eASTC_4x4_UNORM_BLOCK:
				case Format::eASTC_4x4_SRGB_BLOCK:
					result.width = 4;
					result.height = 4;
					break;
				case Format::eASTC_5x4_UNORM_BLOCK:
				case Format::eASTC_5x4_SRGB_BLOCK:
					result.width = 5;
					result.height = 4;
					break;
				case Format::eASTC_5x5_UNORM_BLOCK:
				case Format::eASTC_5x5_SRGB_BLOCK:
					result.width = 5;
					result.height = 5;
					break;
				case Format::eASTC_6x5_UNORM_BLOCK:
				case Format::eASTC_6x5_SRGB_BLOCK:
					result.width = 6;
					result.height = 5;
					break;
				case Format::eASTC_6x6_UNORM_BLOCK:
				case Format::eASTC_6x6_SRGB_BLOCK:
					result.width = 6;
					result.height = 6;
					break;
				case Format::eASTC_8x5_UNORM_BLOCK:
				case Format::eASTC_8x5_SRGB_BLOCK:
					result.width = 8;
					result.height = 5;
					break;
				case Format::eASTC_8x6_UNORM_BLOCK:
				case Format::eASTC_8x6_SRGB_BLOCK:
					result.width = 8;
					result.height = 6;
					break;
				case Format::eASTC_8x8_UNORM_BLOCK:
				case Format::eASTC_8x8_SRGB_BLOCK:
					result.width = 8;
					result.height = 8;
					break;
				case Format::eASTC_10x5_UNORM_BLOCK:
				case Format::eASTC_10x5_SRGB_BLOCK:
					result.width = 10;
					result.height = 5;
					break;
				case Format::eASTC_10x6_UNORM_BLOCK:
				case Format::eASTC_10x6_SRGB_BLOCK:
					result.width = 10;
					result.height = 6;
					break;
				case Format::eASTC_10x8_UNORM_BLOCK:
				case Format::eASTC_10x8_SRGB_BLOCK:
					result.width = 10;
					result.height = 8;
					break;
				case Format::eASTC_10x10_UNORM_BLOCK:
				case Format::eASTC_10x10_SRGB_BLOCK:
					result.width = 10;
					result.height = 10;
					break;
				case Format::eASTC_12x10_UNORM_BLOCK:
				case Format::eASTC_12x10_SRGB_BLOCK:
					result.width = 12;
					result.height = 10;
					break;
				case Format::eASTC_12x12_UNORM_BLOCK:
				case Format::eASTC_12x12_SRGB_BLOCK:
					result.width = 12;
					result.height = 12;
					break;
				}
			}
			else
			{
				assert( false && "Unsupported compressed format." );
			}
		}

		return result;
	}
	/**
	*\~english
	*\brief
	*	Checks if the given extent fits the given format.
	*\param[in] extent
	*	The dimensions.
	*\param[in] format
	*	The pixel format.
	*\return
	*	\p true if the extent is compatible with the format.
	*\~french
	*\brief
	*	Vérifie que les dimensiosn données sont compatibles avec le format donné.
	*\param[in] extent
	*	Les dimensions.
	*\param[in] format
	*	Le format de pixel.
	*\return
	*	\p true si les dimensiosn sont compatibles avec le format.
	*/
	inline bool checkExtent( Format format, Extent2D const & extent )
	{
		auto minimal = getMinimalExtent2D( format );
		return extent.width >= minimal.width
			&& extent.height >= minimal.height;
	}
	/**
	*\~english
	*\brief
	*	Retrieves the byte size of given pixel format and dimensions.
	*\param[in] extent
	*	The dimensions.
	*\param[in] format
	*	The pixel format.
	*\return
	*	The byte size.
	*\~french
	*\brief
	*	Donne le nombre d'octets du format de pixels et les dimensions donnés.
	*\param[in] extent
	*	Les dimensions.
	*\param[in] format
	*	Le format de pixel.
	*\return
	*	Le nombre d'octets.
	*/
	inline uint32_t getSize( Extent2D const & extent
		, Format format )noexcept
	{
		auto result = std::max( 1u, extent.width )
			* std::max( 1u, extent.height );

		if ( !isCompressedFormat( format ) )
		{
			result *= getSize( format );
		}
		else
		{
			auto minimal = getMinimalExtent2D( format );
			auto size = minimal.width * minimal.height;

			if ( ( result % size ) == 0 )
			{
				result /= size;

				if ( isBCCompressedFormat( format ) )
				{
					result *= getBCCompressedSize( format );
				}
				else if ( isETC2CompressedFormat( format ) )
				{
					result *= getETC2CompressedSize( format );
				}
				else if ( isEACCompressedFormat( format ) )
				{
					result *= getEACCompressedSize( format );
				}
			}
			else if ( result < size )
			{
				if ( isBCCompressedFormat( format ) )
				{
					result = getBCCompressedSize( format );
				}
				else if ( isETC2CompressedFormat( format ) )
				{
					result = getETC2CompressedSize( format );
				}
				else if ( isEACCompressedFormat( format ) )
				{
					result = getEACCompressedSize( format );
				}
			}
			else
			{
				assert( false );
			}
		}

		return result;
	}
}

#endif

/*
This file belongs to VkLib.
See LICENSE file in root folder
*/
#include <cassert>
#include <cstring>

namespace utils
{
	namespace details
	{
		/**
		*\brief
		*	Convertit un pixel d'un format donné dans un autre format.
		*\param[in] src
		*	Le pixel source.
		*\param[out] dst
		*	Le pixel de destination.
		*/
		template< PixelFormat SrcF, PixelFormat DstF >
		void convertPixel
			( uint8_t const * src
			, uint8_t * dst );
		/**
		*\brief
		*	Convertit un pixel d'un format donné dans un autre format.
		*\remarks
		*	Spécialisation pour la conversion de PixelFormat::eR8
		*	vers PixelFormat::eR8G8.
		*\param[in] src
		*	Le pixel source.
		*\param[out] dst
		*	Le pixel de destination.
		*/
		template<>
		inline void convertPixel< PixelFormat::eR8, PixelFormat::eR8G8 >
			( uint8_t const * src
			, uint8_t * dst )
		{
			dst[0] = src[0];
			dst[1] = 0xFF;
		}
		/**
		*\brief
		*	Convertit un pixel d'un format donné dans un autre format.
		*\remarks
		*	Spécialisation pour la conversion de PixelFormat::eR8
		*	vers PixelFormat::eR8G8B8.
		*\param[in] src
		*	Le pixel source.
		*\param[out] dst
		*	Le pixel de destination.
		*/
		template<>
		inline void convertPixel< PixelFormat::eR8, PixelFormat::eR8G8B8 >
			( uint8_t const * src
			, uint8_t * dst )
		{
			dst[0] = src[0];
			dst[1] = src[0];
			dst[2] = src[0];
		}
		/**
		*\brief
		*	Convertit un pixel d'un format donné dans un autre format.
		*\remarks
		*	Spécialisation pour la conversion de PixelFormat::eR8
		*	vers PixelFormat::eR8G8B8A8.
		*\param[in] src
		*	Le pixel source.
		*\param[out] dst
		*	Le pixel de destination.
		*/
		template<>
		inline void convertPixel< PixelFormat::eR8, PixelFormat::eR8G8B8A8 >
			( uint8_t const * src
			, uint8_t * dst )
		{
			dst[0] = src[0];
			dst[1] = src[0];
			dst[2] = src[0];
			dst[3] = 0xFF;
		}
		/**
		*\brief
		*	Convertit un pixel d'un format donné dans un autre format.
		*\remarks
		*	Spécialisation pour la conversion de PixelFormat::eR8G8
		*	vers PixelFormat::eR8.
		*\param[in] src
		*	Le pixel source.
		*\param[out] dst
		*	Le pixel de destination.
		*/
		template<>
		inline void convertPixel< PixelFormat::eR8G8, PixelFormat::eR8 >
			( uint8_t const * src
			, uint8_t * dst )
		{
			dst[0] = src[0];
		}
		/**
		*\brief
		*	Convertit un pixel d'un format donné dans un autre format.
		*\remarks
		*	Spécialisation pour la conversion de PixelFormat::eR8G8
		*	vers PixelFormat::eR8G8B8.
		*\param[in] src
		*	Le pixel source.
		*\param[out] dst
		*	Le pixel de destination.
		*/
		template<>
		inline void convertPixel< PixelFormat::eR8G8, PixelFormat::eR8G8B8 >
			( uint8_t const * src
			, uint8_t * dst )
		{
			dst[0] = src[0];
			dst[1] = src[0];
			dst[2] = src[0];
		}
		/**
		*\brief
		*	Convertit un pixel d'un format donné dans un autre format.
		*\remarks
		*	Spécialisation pour la conversion de PixelFormat::eR8G8
		*	vers PixelFormat::eR8G8B8A8.
		*\param[in] src
		*	Le pixel source.
		*\param[out] dst
		*	Le pixel de destination.
		*/
		template<>
		inline void convertPixel< PixelFormat::eR8G8, PixelFormat::eR8G8B8A8 >
			( uint8_t const * src
			, uint8_t * dst )
		{
			dst[0] = src[0];
			dst[1] = src[0];
			dst[2] = src[0];
			dst[3] = src[1];
		}
		/**
		*\brief
		*	Convertit un pixel d'un format donné dans un autre format.
		*\remarks
		*	Spécialisation pour la conversion de PixelFormat::eR8G8B8
		*	vers PixelFormat::eR8.
		*\param[in] src
		*	Le pixel source.
		*\param[out] dst
		*	Le pixel de destination.
		*/
		template<>
		inline void convertPixel< PixelFormat::eR8G8B8, PixelFormat::eR8 >
			( uint8_t const * src
			, uint8_t * dst )
		{
			struct pixel
			{
				uint8_t r;
				uint8_t g;
				uint8_t b;
			};

			auto toPixel = [src]() -> pixel const &
			{
				return *reinterpret_cast< pixel const * >( src );
			};

			*dst = uint8_t( toPixel().r * 0.30
				+ toPixel().g * 0.59
				+ toPixel().b * 0.11 );
		}
		/**
		*\brief
		*	Convertit un pixel d'un format donné dans un autre format.
		*\remarks
		*	Spécialisation pour la conversion de PixelFormat::eR8G8B8
		*	vers PixelFormat::eR8G8.
		*\param[in] src
		*	Le pixel source.
		*\param[out] dst
		*	Le pixel de destination.
		*/
		template<>
		inline void convertPixel< PixelFormat::eR8G8B8, PixelFormat::eR8G8 >
			( uint8_t const * src
			, uint8_t * dst )
		{
			struct pixel
			{
				uint8_t r;
				uint8_t g;
				uint8_t b;
			};

			auto toPixel = [src]() -> pixel const &
			{
				return *reinterpret_cast< pixel const * >( src );
			};

			dst[0] = uint8_t( toPixel().r * 0.30
				+ toPixel().g * 0.59
				+ toPixel().b * 0.11 );
			dst[1] = 0xFF;
		}
		/**
		*\brief
		*	Convertit un pixel d'un format donné dans un autre format.
		*\remarks
		*	Spécialisation pour la conversion de PixelFormat::eR8G8B8
		*	vers PixelFormat::eR8G8B8A8.
		*\param[in] src
		*	Le pixel source.
		*\param[out] dst
		*	Le pixel de destination.
		*/
		template<>
		inline void convertPixel< PixelFormat::eR8G8B8, PixelFormat::eR8G8B8A8 >
			( uint8_t const * src
			, uint8_t * dst )
		{
			dst[0] = src[0];
			dst[1] = src[1];
			dst[2] = src[2];
			dst[3] = 0xFF;
		}
		/**
		*\brief
		*	Convertit un pixel d'un format donné dans un autre format.
		*\remarks
		*	Spécialisation pour la conversion de PixelFormat::eR8G8B8A8
		*	vers PixelFormat::eR8.
		*\param[in] src
		*	Le pixel source.
		*\param[out] dst
		*	Le pixel de destination.
		*/
		template<>
		inline void convertPixel< PixelFormat::eR8G8B8A8, PixelFormat::eR8 >
			( uint8_t const * src
			, uint8_t * dst )
		{
			struct pixel
			{
				uint8_t r;
				uint8_t g;
				uint8_t b;
				uint8_t a;
			};

			auto toPixel = [src]() -> pixel const &
			{
				return *reinterpret_cast< pixel const * >( src );
			};

			*dst = uint8_t( toPixel().r * 0.30
				+ toPixel().g * 0.59
				+ toPixel().b * 0.11 );
		}
		/**
		*\brief
		*	Convertit un pixel d'un format donné dans un autre format.
		*\remarks
		*	Spécialisation pour la conversion de PixelFormat::eR8G8B8A8
		*	vers PixelFormat::eR8G8.
		*\param[in] src
		*	Le pixel source.
		*\param[out] dst
		*	Le pixel de destination.
		*/
		template<>
		inline void convertPixel< PixelFormat::eR8G8B8A8, PixelFormat::eR8G8 >
			( uint8_t const * src
			, uint8_t * dst )
		{
			struct pixel
			{
				uint8_t r;
				uint8_t g;
				uint8_t b;
				uint8_t a;
			};

			auto toPixel = [src]() -> pixel const &
			{
				return *reinterpret_cast< pixel const * >( src );
			};

			dst[0] = uint8_t( toPixel().r * 0.30
				+ toPixel().g * 0.59
				+ toPixel().b * 0.11 );
			dst[1] = toPixel().a;
		}
		/**
		*\brief
		*	Convertit un pixel d'un format donné dans un autre format.
		*\remarks
		*	Spécialisation pour la conversion de PixelFormat::eR8G8B8A8
		*	vers PixelFormat::eR8G8B8.
		*\param[in] src
		*	Le pixel source.
		*\param[out] dst
		*	Le pixel de destination.
		*/
		template<>
		inline void convertPixel< PixelFormat::eR8G8B8A8, PixelFormat::eR8G8B8 >
			( uint8_t const * src
			, uint8_t * dst )
		{
			dst[0] = src[0];
			dst[1] = src[1];
			dst[2] = src[2];
		}
		/**
		*\brief
		*	Structure de conversion de tampon de pixels.
		*\param SrcF
		*	Le format des pixels de la source.
		*\param DstF
		*	Le format des pixels de la destination.
		*/
		template< PixelFormat SrcF, PixelFormat DstF >
		struct BufferConverter
		{
			/**
			*\brief
			*	Convertit un tampon d'un format donné dans un autre format.
			*\param[in] src
			*	Le tampon source.
			*\param[in] srcs
			*	La taille du tampon source
			*\param[in,out] dst
			*	Le tampon de destination (doit déjà avoir été alloué).
			*\param[in] dsts
			*	La taille du tampon de destination.
			*/
			static inline void convert( uint8_t const * src
				, size_t srcs
				, uint8_t * dst
				, size_t dsts )
			{
				static_assert( SrcF == PixelFormat::eR8
					|| SrcF == PixelFormat::eR8G8
					|| SrcF == PixelFormat::eR8G8B8
					|| SrcF == PixelFormat::eR8G8B8A8
					, "Unsupported source format for conversion" );

				static_assert( DstF == PixelFormat::eR8
					|| DstF == PixelFormat::eR8G8
					|| DstF == PixelFormat::eR8G8B8
					|| DstF == PixelFormat::eR8G8B8A8
					, "Unsupported destination format for conversion" );

				auto srcBuf = src;
				auto srcInc = pixelSize( SrcF );
				auto dstBuf = dst;
				auto dstInc = pixelSize( DstF );
				assert( srcs / srcInc == dsts / dstInc );
				auto count = srcs / srcInc;
				auto index = 0u;

				while ( index < count )
				{
					convertPixel< SrcF, DstF >( srcBuf, dstBuf );
					++index;
					srcBuf += srcInc;
					dstBuf += dstInc;
				}
			}
		};
		/**
		*\brief
		*	Structure de conversion de tampon de pixels.
		*\param SrcF
		*	Le format des pixels de la source (et de la destination).
		*\remarks
		*	Aucune conversion.
		*/
		template< PixelFormat SrcF >
		struct BufferConverter< SrcF, SrcF >
		{
			/**
			*\brief
			*	Convertit un tampon d'un format donné dans un autre format.
			*\param[in] src
			*	Le tampon source.
			*\param[in] srcs
			*	La taille du tampon source
			*\param[in,out] dst
			*	Le tampon de destination (doit déjà avoir été alloué).
			*\param[in] dsts
			*	La taille du tampon de destination.
			*/
			static inline void convert( uint8_t const * src
				, size_t srcs
				, uint8_t * dst
				, size_t dsts )
			{
				assert( srcs == dsts );
				std::memcpy( dst, src, srcs );
			}
		};
	}

	template< PixelFormat Format >
	inline void convertBuffer( uint8_t const * src
		, size_t srcs
		, uint8_t * dst
		, PixelFormat dstf
		, size_t dsts )
	{
		switch ( dstf )
		{
		case PixelFormat::eR8:
			details::BufferConverter< Format, PixelFormat::eR8 >::convert
				( src
				, srcs
				, dst
				, dsts );
			break;

		case PixelFormat::eR8G8:
			details::BufferConverter< Format, PixelFormat::eR8G8 >::convert
				( src
				, srcs
				, dst
				, dsts );
			break;

		case PixelFormat::eR8G8B8:
			details::BufferConverter< Format, PixelFormat::eR8G8B8 >::convert
				( src
				, srcs
				, dst
				, dsts );
			break;

		case PixelFormat::eR8G8B8A8:
			details::BufferConverter< Format, PixelFormat::eR8G8B8A8 >::convert
				( src
				, srcs
				, dst
				, dsts );
			break;

		default:
			assert( false && "Unsupported pixel format for conversion" );
			break;
		}
	}
}

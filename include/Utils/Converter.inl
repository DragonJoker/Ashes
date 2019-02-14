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
		template< ashes::Format SrcF, ashes::Format DstF >
		void convertPixel
			( uint8_t const * src
			, uint8_t * dst );
		/**
		*\brief
		*	Convertit un pixel d'un format donné dans un autre format.
		*\remarks
		*	Spécialisation pour la conversion de ashes::Format::eR8_UNORM
		*	vers ashes::Format::eR8G8_UNORM.
		*\param[in] src
		*	Le pixel source.
		*\param[out] dst
		*	Le pixel de destination.
		*/
		template<>
		inline void convertPixel< ashes::Format::eR8_UNORM, ashes::Format::eR8G8_UNORM >
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
		*	Spécialisation pour la conversion de ashes::Format::eR8_UNORM
		*	vers ashes::Format::eR8G8B8_UNORM.
		*\param[in] src
		*	Le pixel source.
		*\param[out] dst
		*	Le pixel de destination.
		*/
		template<>
		inline void convertPixel< ashes::Format::eR8_UNORM, ashes::Format::eR8G8B8_UNORM >
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
		*	Spécialisation pour la conversion de ashes::Format::eR8_UNORM
		*	vers ashes::Format::eR8G8B8A8_UNORM.
		*\param[in] src
		*	Le pixel source.
		*\param[out] dst
		*	Le pixel de destination.
		*/
		template<>
		inline void convertPixel< ashes::Format::eR8_UNORM, ashes::Format::eR8G8B8A8_UNORM >
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
		*	Spécialisation pour la conversion de ashes::Format::eR8G8_UNORM
		*	vers ashes::Format::eR8_UNORM.
		*\param[in] src
		*	Le pixel source.
		*\param[out] dst
		*	Le pixel de destination.
		*/
		template<>
		inline void convertPixel< ashes::Format::eR8G8_UNORM, ashes::Format::eR8_UNORM >
			( uint8_t const * src
			, uint8_t * dst )
		{
			dst[0] = src[0];
		}
		/**
		*\brief
		*	Convertit un pixel d'un format donné dans un autre format.
		*\remarks
		*	Spécialisation pour la conversion de ashes::Format::eR8G8_UNORM
		*	vers ashes::Format::eR8G8B8_UNORM.
		*\param[in] src
		*	Le pixel source.
		*\param[out] dst
		*	Le pixel de destination.
		*/
		template<>
		inline void convertPixel< ashes::Format::eR8G8_UNORM, ashes::Format::eR8G8B8_UNORM >
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
		*	Spécialisation pour la conversion de ashes::Format::eR8G8_UNORM
		*	vers ashes::Format::eR8G8B8A8_UNORM.
		*\param[in] src
		*	Le pixel source.
		*\param[out] dst
		*	Le pixel de destination.
		*/
		template<>
		inline void convertPixel< ashes::Format::eR8G8_UNORM, ashes::Format::eR8G8B8A8_UNORM >
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
		*	Spécialisation pour la conversion de ashes::Format::eR8G8B8_UNORM
		*	vers ashes::Format::eR8_UNORM.
		*\param[in] src
		*	Le pixel source.
		*\param[out] dst
		*	Le pixel de destination.
		*/
		template<>
		inline void convertPixel< ashes::Format::eR8G8B8_UNORM, ashes::Format::eR8_UNORM >
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
		*	Spécialisation pour la conversion de ashes::Format::eR8G8B8_UNORM
		*	vers ashes::Format::eR8G8_UNORM.
		*\param[in] src
		*	Le pixel source.
		*\param[out] dst
		*	Le pixel de destination.
		*/
		template<>
		inline void convertPixel< ashes::Format::eR8G8B8_UNORM, ashes::Format::eR8G8_UNORM >
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
		*	Spécialisation pour la conversion de ashes::Format::eR8G8B8_UNORM
		*	vers ashes::Format::eR8G8B8A8_UNORM.
		*\param[in] src
		*	Le pixel source.
		*\param[out] dst
		*	Le pixel de destination.
		*/
		template<>
		inline void convertPixel< ashes::Format::eR8G8B8_UNORM, ashes::Format::eR8G8B8A8_UNORM >
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
		*	Spécialisation pour la conversion de ashes::Format::eR8G8B8A8_UNORM
		*	vers ashes::Format::eR8_UNORM.
		*\param[in] src
		*	Le pixel source.
		*\param[out] dst
		*	Le pixel de destination.
		*/
		template<>
		inline void convertPixel< ashes::Format::eR8G8B8A8_UNORM, ashes::Format::eR8_UNORM >
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
		*	Spécialisation pour la conversion de ashes::Format::eR8G8B8A8_UNORM
		*	vers ashes::Format::eR8G8_UNORM.
		*\param[in] src
		*	Le pixel source.
		*\param[out] dst
		*	Le pixel de destination.
		*/
		template<>
		inline void convertPixel< ashes::Format::eR8G8B8A8_UNORM, ashes::Format::eR8G8_UNORM >
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
		*	Spécialisation pour la conversion de ashes::Format::eR8G8B8A8_UNORM
		*	vers ashes::Format::eR8G8B8_UNORM.
		*\param[in] src
		*	Le pixel source.
		*\param[out] dst
		*	Le pixel de destination.
		*/
		template<>
		inline void convertPixel< ashes::Format::eR8G8B8A8_UNORM, ashes::Format::eR8G8B8_UNORM >
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
		template< ashes::Format SrcF, ashes::Format DstF >
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
				static_assert( SrcF == ashes::Format::eR8_UNORM
					|| SrcF == ashes::Format::eR8G8_UNORM
					|| SrcF == ashes::Format::eR8G8B8_UNORM
					|| SrcF == ashes::Format::eR8G8B8A8_UNORM
					, "Unsupported source format for conversion" );

				static_assert( DstF == ashes::Format::eR8_UNORM
					|| DstF == ashes::Format::eR8G8_UNORM
					|| DstF == ashes::Format::eR8G8B8_UNORM
					|| DstF == ashes::Format::eR8G8B8A8_UNORM
					, "Unsupported destination format for conversion" );

				auto srcBuf = src;
				auto srcInc = getSize( SrcF );
				auto dstBuf = dst;
				auto dstInc = getSize( DstF );
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
		template< ashes::Format SrcF >
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

	template< ashes::Format SrcFormat >
	inline void convertBuffer( uint8_t const * src
		, size_t srcs
		, uint8_t * dst
		, ashes::Format dstf
		, size_t dsts )
	{
		switch ( dstf )
		{
		case ashes::Format::eR8_UNORM:
			details::BufferConverter< SrcFormat, ashes::Format::eR8_UNORM >::convert
				( src
				, srcs
				, dst
				, dsts );
			break;

		case ashes::Format::eR8G8_UNORM:
			details::BufferConverter< SrcFormat, ashes::Format::eR8G8_UNORM >::convert
				( src
				, srcs
				, dst
				, dsts );
			break;

		case ashes::Format::eR8G8B8_UNORM:
			details::BufferConverter< SrcFormat, ashes::Format::eR8G8B8_UNORM >::convert
				( src
				, srcs
				, dst
				, dsts );
			break;

		case ashes::Format::eR8G8B8A8_UNORM:
			details::BufferConverter< SrcFormat, ashes::Format::eR8G8B8A8_UNORM >::convert
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

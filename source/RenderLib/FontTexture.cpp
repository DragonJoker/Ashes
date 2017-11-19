#include "FontTexture.h"

namespace render
{
	namespace
	{
		Texture doCreateTexture( Font & font )
		{
			Texture texture;
			uint32_t const maxWidth = font.maxWidth();
			uint32_t const maxHeight = font.maxHeight();
			uint32_t const count = uint32_t( std::ceil( std::distance
				( font.begin()
				, font.end() ) / 16.0 ) );
			gl::IVec2 size{ maxWidth * 16, maxHeight * count };
			auto it = font.begin();
			uint32_t const totalWidth = size.x;
			uint32_t offsetY = size.y - maxHeight;
			ByteArray data;
			data.resize( size.x * size.y );
			uint8_t * buffer = data.data();

			for ( uint32_t y = 0; y < count && it != font.end(); ++y )
			{
				uint32_t offsetX = 0;

				for ( uint32_t x = 0; x < 16 && it != font.end(); ++x )
				{
					render::Glyph & glyph = *it;
					gl::IVec2 const & glyphSize = glyph.size();
					ByteArray const & glyphBitmap = glyph.bitmap();
					uint32_t const dstLineIndex = ( totalWidth * offsetY ) + offsetX;
					uint8_t * dstLineBuffer = &buffer[dstLineIndex];

					for ( int32_t i = 0; i < glyphSize.y; ++i )
					{
						assert( dstLineIndex + glyphSize.x <= data.size() );
						memcpy( dstLineBuffer
							, &glyphBitmap[i * glyphSize.x]
							, glyphSize.x );
						dstLineBuffer += totalWidth;
					}

					glyph.clearBitmap();
					offsetX += maxWidth;
					++it;
				}

				offsetY -= maxHeight;
			}

			texture.image( gl::PixelFormat::eL8, size, data );
			return texture;
		}
	}

	FontTexture::FontTexture( FontPtr && font )
		: FontTexture{ doCreateTexture( *font ), std::move( font ) }
	{
	}

	FontTexture::FontTexture( Texture && texture, FontPtr && font )
		: m_texture{ std::move( texture ) }
		, m_font{ std::move( font ) }
	{
		uint32_t const maxWidth = m_font->maxWidth();
		uint32_t const maxHeight = m_font->maxHeight();
		uint32_t const count = uint32_t( std::ceil( std::distance
			( m_font->begin()
			, m_font->end() ) / 16.0 ) );
		gl::IVec2 size{ maxWidth * 16, maxHeight * count };
		auto it = m_font->begin();
		uint32_t offsetY = size.y - maxHeight;

		for ( uint32_t y = 0; y < count && it != m_font->end(); ++y )
		{
			uint32_t offsetX = 0;

			for ( uint32_t x = 0; x < 16 && it != m_font->end(); ++x )
			{
				render::Glyph const & glyph = *it;
				m_glyphsPositions[glyph.character()] = { offsetX
					, offsetY };
				offsetX += maxWidth;
				++it;
			}

			offsetY -= maxHeight;
		}
	}

	FontTexture::~FontTexture()
	{
	}

	gl::IVec2 const & FontTexture::glyphPosition( char c )const
	{
		auto it = m_glyphsPositions.find( c );

		if ( it == m_glyphsPositions.end() )
		{
			assert( false && "No loaded glyph for given character" );
		}

		return it->second;
	}
}

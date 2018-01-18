#include "FontTexture.h"

namespace render
{
	namespace
	{
		TexturePtr doCreateTexture( renderer::Device const & device
			, renderer::StagingBuffer const & stagingBuffer
			, renderer::CommandBuffer const & commandBuffer
			, Font & font )
		{
			TexturePtr result{ std::make_shared< Texture >( device ) };
			uint32_t const maxWidth = font.maxWidth();
			uint32_t const maxHeight = font.maxHeight();
			uint32_t const count = uint32_t( std::ceil( std::distance
				( font.begin()
				, font.end() ) / 16.0 ) );
			renderer::UIVec2 size{ maxWidth * 16, maxHeight * count };
			auto it = font.begin();
			uint32_t const totalWidth = size.x;
			uint32_t offsetY = size.y - maxHeight;
			ByteArray data( size.x * size.y * 4u, uint8_t{} );
			uint8_t * buffer = data.data();

			for ( uint32_t y = 0; y < count && it != font.end(); ++y )
			{
				uint32_t offsetX = 0;

				for ( uint32_t x = 0; x < 16 && it != font.end(); ++x )
				{
					render::Glyph & glyph = *it;
					renderer::IVec2 const & glyphSize = glyph.size();
					ByteArray const & glyphBitmap = glyph.bitmap();
					uint32_t const dstLineIndex = 4u * ( ( totalWidth * offsetY ) + offsetX );
					uint8_t * dstLineBuffer = &buffer[dstLineIndex];

					for ( int32_t y = 0; y < glyphSize.y; ++y )
					{
						assert( dstLineIndex + 4u * glyphSize.x <= data.size() );
						uint8_t * dstLine = dstLineBuffer;

						for ( int x = 0; x < glyphSize.x; ++x )
						{
							dstLine[0] = glyphBitmap[y * glyphSize.x + x];
							dstLine[1] = glyphBitmap[y * glyphSize.x + x];
							dstLine[2] = glyphBitmap[y * glyphSize.x + x];
							dstLine[3] = glyphBitmap[y * glyphSize.x + x];
							dstLine += 4u;
						}

						dstLineBuffer += totalWidth * 4u;
						//memcpy( dstLineBuffer
						//	, &glyphBitmap[y * glyphSize.x]
						//	, glyphSize.x );
						//dstLineBuffer += totalWidth;
					}

					glyph.clearBitmap();
					offsetX += maxWidth;
					++it;
				}

				offsetY -= maxHeight;
			}

			result->image( renderer::PixelFormat::eR8G8B8A8
				, size
				, data
				, stagingBuffer
				, commandBuffer );
			return result;
		}
	}

	FontTexture::FontTexture( renderer::Device const & device
		, renderer::StagingBuffer const & stagingBuffer
		, renderer::CommandBuffer const & commandBuffer
		, FontPtr && font )
		: FontTexture{ device
			, doCreateTexture( device
				, stagingBuffer
				, commandBuffer
				, *font )
			, std::move( font ) }
	{
	}

	FontTexture::FontTexture( renderer::Device const & device
		, TexturePtr texture
		, FontPtr && font )
		: m_texture{ texture }
		, m_font{ std::move( font ) }
	{
		uint32_t const maxWidth = m_font->maxWidth();
		uint32_t const maxHeight = m_font->maxHeight();
		uint32_t const count = uint32_t( std::ceil( std::distance
			( m_font->begin()
			, m_font->end() ) / 16.0 ) );
		renderer::IVec2 size{ maxWidth * 16, maxHeight * count };
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

	renderer::IVec2 const & FontTexture::glyphPosition( char c )const
	{
		auto it = m_glyphsPositions.find( c );

		if ( it == m_glyphsPositions.end() )
		{
			assert( false && "No loaded glyph for given character" );
		}

		return it->second;
	}
}

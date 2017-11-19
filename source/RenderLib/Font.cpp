#include "Font.h"

namespace render
{
	Font::Font( std::string const & name, uint32_t height )
		: m_height{ height }
		, m_faceName{ name }
	{
	}

	Font::~Font()
	{
	}

	void Font::addGlyph( Glyph const & glyph )
	{
		auto it = std::find( m_glyphs.begin()
			, m_glyphs.end()
			, glyph );

		if ( it == m_glyphs.end() )
		{
			m_glyphs.push_back( glyph );
			m_maxHeight = std::max( m_maxHeight, glyph.size().y );
			m_maxWidth = std::max( m_maxWidth, int( glyph.advance() ) );
		}
	}
}

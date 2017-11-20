#include "FontLoader.h"

#include <sstream>

#if __GNUG__
#	pragma GCC diagnostic ignored "-Wdeprecated-register"
#endif
#include <ft2build.h>

FT_BEGIN_HEADER
#	include FT_FREETYPE_H
#	include FT_GLYPH_H
#	include FT_OUTLINE_H
#	include FT_TRIGONOMETRY_H
#	include FT_TYPES_H
FT_END_HEADER

namespace utils
{
	namespace
	{
#define CHECK_FT_ERR( func, ... ) CheckErr( func( __VA_ARGS__ ), #func )

		inline bool CheckErr( FT_Error error, const char * name )
		{
			static std::map< FT_Error, std::string > MapErrors =
			{
				{ 0x0000, "Success" },
				{ 0x0001, "Invalid face handle" },
				{ 0x0002, "Invalid instance handle" },
				{ 0x0003, "Invalid glyph handle" },
				{ 0x0004, "Invalid charmap handle" },
				{ 0x0005, "Invalid result address" },
				{ 0x0006, "Invalid glyph index" },
				{ 0x0007, "Invalid argument" },
				{ 0x0008, "Could not open file" },
				{ 0x0009, "File is not collection" },
				{ 0x000A, "Table missing" },
				{ 0x000B, "Invalid horiz metrics" },
				{ 0x000C, "Invalid charmap format" },
				{ 0x000D, "Invalid ppem" },
				{ 0x0010, "Invalid file format" },
				{ 0x0020, "Invalid engine" },
				{ 0x0021, "Too many extensions" },
				{ 0x0022, "Extensions unsupported" },
				{ 0x0023, "Invalid extension id" },
				{ 0x0080, "Max profile missing" },
				{ 0x0081, "Header table missing" },
				{ 0x0082, "Horiz header missing" },
				{ 0x0083, "Locations missing" },
				{ 0x0084, "Name table missing" },
				{ 0x0085, "CMap table missing" },
				{ 0x0086, "Hmtx table missing" },
				{ 0x0087, "OS2 table missing" },
				{ 0x0088, "Post table missing" },
				{ 0x0100, "Out of memory" },
				{ 0x0200, "Invalid file offset" },
				{ 0x0201, "Invalid file read" },
				{ 0x0202, "Invalid frame access" },
				{ 0x0300, "Too many points" },
				{ 0x0301, "Too many contours" },
				{ 0x0302, "Invalid composite glyph" },
				{ 0x0303, "Too many instructions" },
				{ 0x0400, "Invalid opcode" },
				{ 0x0401, "Too few arguments" },
				{ 0x0402, "Stack overflow" },
				{ 0x0403, "Code overflow" },
				{ 0x0404, "Bad argument" },
				{ 0x0405, "Divide by zero" },
				{ 0x0406, "Storage overflow" },
				{ 0x0407, "Cvt overflow" },
				{ 0x0408, "Invalid reference" },
				{ 0x0409, "Invalid distance" },
				{ 0x040A, "Interpolate twilight" },
				{ 0x040B, "Debug opcode" },
				{ 0x040C, "ENDF in exec stream" },
				{ 0x040D, "Out of coderanges" },
				{ 0x040E, "Nested defs" },
				{ 0x040F, "Invalid coderange" },
				{ 0x0410, "Invalid displacement" },
				{ 0x0411, "Execution too long" },
				{ 0x0500, "Nested frame access" },
				{ 0x0501, "Invalid cache list" },
				{ 0x0502, "Could not find context" },
				{ 0x0503, "Unlisted object" },
				{ 0x0600, "Raster pool overflow" },
				{ 0x0601, "Raster negative height" },
				{ 0x0602, "Raster invalid value" },
				{ 0x0603, "Raster not initialized" },
				{ 0x0A00, "Invalid kerning table format" },
				{ 0x0A01, "Invalid kerning table" },
				{ 0x0B00, "Invalid post table format" },
				{ 0x0B01, "Invalid post table" },
			};

			bool ret = true;

			if ( error != 0 )
			{
				auto l_it = MapErrors.find( error );
				std::stringstream stream;
				stream << "ERROR : " << name << " failed - " << error;

				if ( l_it != MapErrors.end() )
				{
					stream << " (" << l_it->second << ")";
				}

				std::cerr << stream.str() << std::endl;
				ret = false;
			}

			return ret;
		}
	}

	FontLoader::FontLoader( std::string const & pathFile )
		: m_pathFile{ pathFile }
	{
		CHECK_FT_ERR( FT_Init_FreeType, &m_library );
	}

	FontLoader::~FontLoader()
	{
		doCleanup();

		if ( m_library )
		{
			CHECK_FT_ERR( FT_Done_FreeType, m_library );
		}
	}

	void FontLoader::doCleanup()
	{
		if ( m_face )
		{
			CHECK_FT_ERR( FT_Done_Face, m_face );
		}
	}

	void FontLoader::initialise( uint32_t height )
	{
		doCleanup();

		if ( CHECK_FT_ERR( FT_New_Face
				, m_library
				, m_pathFile.c_str()
				, 0
				, &m_face ) )
		{
			CHECK_FT_ERR( FT_Select_Charmap, m_face, FT_ENCODING_UNICODE );
			CHECK_FT_ERR( FT_Set_Pixel_Sizes, m_face, 0, height );
		}
	}

	render::Glyph FontLoader::loadGlyph( char c )const
	{
		utils::IVec2 size;
		utils::IVec2 bearing;
		uint32_t advance{ 0 };
		render::ByteArray buffer;

		if ( m_face && m_library )
		{
			FT_Glyph glyph{};
			FT_ULong const ulc( c );
			FT_UInt const index{ FT_Get_Char_Index( m_face, ulc ) };

			if ( CHECK_FT_ERR( FT_Load_Glyph, m_face, index, FT_LOAD_DEFAULT )
				&& CHECK_FT_ERR( FT_Get_Glyph, m_face->glyph, &glyph ) )
			{
				if ( CHECK_FT_ERR( FT_Glyph_To_Bitmap
					, &glyph
					, FT_RENDER_MODE_NORMAL
					, 0
					, 1 ) )
				{
					FT_BitmapGlyph const bmpGlyph = FT_BitmapGlyph( glyph );
					FT_Bitmap const & bitmap = bmpGlyph->bitmap;
					uint32_t const pitch( abs( bitmap.pitch ) );
					size = { pitch, uint32_t( bitmap.rows ) };
					bearing = { bmpGlyph->left, bmpGlyph->top };
					buffer.resize( size.x * size.y );
					advance = uint32_t( abs( glyph->advance.x ) / 65536.0 );

					if ( advance < uint32_t( size[0] ) )
					{
						advance = size[0] + bearing[0];
					}

					if ( bitmap.pitch < 0 )
					{
						uint8_t * dst = buffer.data();
						uint8_t const * src = bitmap.buffer;

						for ( uint32_t i = 0; i < uint32_t( bitmap.rows ); i++ )
						{
							memcpy( dst, src, bitmap.width );
							src += pitch;
							dst += pitch;
						}
					}
					else
					{
						uint8_t * dst = buffer.data() + buffer.size() - pitch;
						uint8_t const * src = bitmap.buffer;

						for ( uint32_t i = 0; i < uint32_t( bitmap.rows ); i++ )
						{
							memcpy( dst, src, bitmap.width );
							src += pitch;
							dst -= pitch;
						}
					}
				}

				FT_Done_Glyph( glyph );
			}
		}

		return render::Glyph{ c, size, bearing, advance, buffer };
	}
}

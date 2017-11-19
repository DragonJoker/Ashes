/**
*\file
*	LogUtils.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_LogUtils_H___
#define ___RenderLib_LogUtils_H___
#pragma once

#include <string>
#include <ostream>

namespace render
{
	/**
	*\brief
	*	Surcharge de std::streambuf, permettant de personnaliser la sortie.
	*\param LogStreambufTraits
	*	Permet la personnalisation de la sortie.\n
	*	Doit implémenter la fonction statique suivante :\n
	*	Log( string_type appName, string_type buffer )
	*/
	template< typename LogStreambufTraits >
	class LogStreambuf
		: public std::streambuf
	{
	public:
		using CharType = char;
		using string_type = std::basic_string< CharType >;
		using ostream_type = std::basic_ostream< CharType >;
		using streambuf_type = std::basic_streambuf< CharType >;
		using int_type = typename std::basic_streambuf< CharType >::int_type;
		using traits_type = typename std::basic_streambuf< CharType >::traits_type;
		/**
		*\brief
		*	Constructeur, échange le tampon du flux donné avec celui-ci.
		*\param[in] appName
		*	Le nom de l'application.
		*\param[in,out] stream
		*	Le flux recevant ce tampon de flux.
		*/
		LogStreambuf( string_type const & appName, ostream_type & stream )
			: m_stream( stream )
			, m_appName{ appName }
		{
			m_old = m_stream.rdbuf( this );
		}
		/**
		*\brief
		*	Destructeur, rétablit le tampon d'origine du flux.
		*/
		~LogStreambuf()
		{
			m_stream.rdbuf( m_old );
		}

	private:
		int_type overflow( int_type c = traits_type::eof() )override
		{
			if ( traits_type::eq_int_type( c, traits_type::eof() ) )
			{
				do_sync();
			}
			else if ( c == '\n' )
			{
				do_sync();
			}
			else if ( c == '\r' )
			{
				m_buffer += '\r';
				do_sync_no_nl();
			}
			else
			{
				m_buffer += traits_type::to_char_type( c );
			}

			return c;
		}

		int do_sync()
		{
			if ( !m_buffer.empty() )
			{
				LogStreambufTraits::Log( m_appName, m_buffer );
				m_buffer.clear();
			}

			return 0;
		}

		int do_sync_no_nl()
		{
			if ( !m_buffer.empty() )
			{
				LogStreambufTraits::Log( m_appName, m_buffer );
				m_buffer.clear();
			}

			return 0;
		}

	private:
		string_type m_buffer;
		ostream_type & m_stream;
		streambuf_type * m_old;
		string_type m_appName;
	};
}

#endif

/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Utils/LogUtils.hpp"

#include <string>
#include <ostream>

namespace utils
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
				doSync();
			}
			else if ( c == '\n' )
			{
				doSync();
			}
			else if ( c == '\r' )
			{
				m_buffer += '\r';
				doSyncNoNl();
			}
			else
			{
				m_buffer += traits_type::to_char_type( c );
			}

			return c;
		}

		int doSync()
		{
			if ( !m_buffer.empty() )
			{
				LogStreambufTraits::Log( m_appName, m_buffer );
				m_buffer.clear();
			}

			return 0;
		}

		int doSyncNoNl()
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

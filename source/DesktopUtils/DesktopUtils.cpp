#include "DesktopUtils.h"

#include <array>
#include <stdlib.h>
#include <fstream>
#include <sstream>

#include "FontLoader.h"

#ifdef _WIN32
#	include <Windows.h>
#endif

namespace utils
{
	std::string getFileTextContent( std::string const & fileName )
	{
		auto file = fopen( fileName.c_str(), "r" );
		std::string content;

		if ( file )
		{
			std::array< char, 1024 > buffer;
			size_t read = 0;

			while ( ( read = fread( buffer.data()
				, 1u
				, buffer.size()
				, file ) ) > 0 )
			{
				content += std::string{ buffer.data(), buffer.data() + read };
			}

			fclose( file );
		}

		return content;
	}

	std::vector< uint8_t > getFileBinaryContent( std::string const & fileName )
	{
		auto file = fopen( fileName.c_str(), "rb" );
		std::vector< uint8_t > content;

		if ( file )
		{
			std::array< uint8_t, 1024 > buffer;
			size_t read = 0;

			while ( ( read = fread( buffer.data()
				, 1u
				, buffer.size()
				, file ) ) > 0 )
			{
				content.insert( content.end()
					, buffer.data()
					, buffer.data() + read );
			}

			fclose( file );
		}

		return content;
	}

	void logDebugString( std::string const & log )
	{
#if _MSC_VER
		::OutputDebugStringA( log.c_str() );
#endif
		std::tm today = { 0 };
		time_t ttime;
		time( &ttime );
		localtime_s( &today, &ttime );
		char buffer[33] = { 0 };
		strftime( buffer, 32, "%Y-%m-%d %H:%M:%S", &today );
		std::string timeStamp = buffer;

		std::ofstream file{ "Debug.log", std::ios::app };

		if ( file )
		{
			file << timeStamp << " - " << log;
		}
	}
}

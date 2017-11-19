#include "StringUtils.h"

#include <algorithm>

namespace render
{
	std::string replace( std::string const & input
		, std::string const & toReplace
		, std::string const & replacement )
	{
		std::string ret;
		std::size_t currentPos = 0;
		std::size_t pos = 0;

		while ( ( pos = input.find( toReplace, currentPos ) ) != std::string::npos )
		{
			ret.append( input.substr( currentPos, pos - currentPos ) );
			ret.append( replacement );
			currentPos = pos + toReplace.size();
		}

		if ( currentPos != input.size() )
		{
			ret.append( input.substr( currentPos, pos - currentPos ) );
		}

		return ret;
	}

	std::string trimLeft( std::string const & text )
	{
		std::string ret{ text };
		size_t index = ret.find_first_not_of( " \t\r" );

		if ( index != std::string::npos )
		{
			ret = ret.substr( index );
		}

		return ret;
	}

	std::string trimRight( std::string const & text )
	{
		std::string ret{ text };
		size_t index = ret.find_last_not_of( " \t\r" );

		if ( index != std::string::npos )
		{
			ret = ret.substr( 0, index + 1 );
		}

		return ret;
	}

	std::string lowerCase( std::string const & text )
	{
		std::string result( text );
		std::transform( result.begin()
			, result.end()
			, result.begin()
			, tolower );
		return result;
	}

	std::string upperCase( std::string const & text )
	{
		std::string result( text );
		std::transform( result.begin()
			, result.end()
			, result.begin()
			, toupper );
		return result;
	}

	StringArray split( std::string const & str
		, std::string const & delims
		, uint32_t maxSplits
		, bool keepVoid )
	{
		StringArray	ret;

		if ( !str.empty() && !delims.empty() && maxSplits > 0 )
		{
			ret.reserve( maxSplits + 1 );
			std::size_t	pos = 0;
			std::size_t	start = 0;

			do
			{
				pos = str.find_first_of( delims, start );

				if ( pos == start )
				{
					start = pos + 1;

					if ( keepVoid )
					{
						ret.push_back( std::string{} );
					}
				}
				else if ( pos == std::string::npos || ret.size() == maxSplits )
				{
					std::string remnants = str.substr( start );

					if ( !remnants.empty() || keepVoid )
					{
						ret.push_back( remnants );
					}

					pos = std::string::npos;
				}
				else
				{
					ret.push_back( str.substr( start, pos - start ) );
					start = pos + 1;
				}

			}
			while ( pos != std::string::npos );
		}

		return ret;
	}
}

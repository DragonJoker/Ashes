#include "RawDataProcessor.hpp"

#include <cstdio>

namespace vkapp
{
	void readFile( std::string const & name
		, ashes::Extent3D const & size
		, ashes::ByteArray & data )
	{
		FILE * file = fopen( name.c_str(), "rb" );

		if ( !file )
		{
			throw std::runtime_error{ "Couldn't load file." };
		}

		// Holds the luminance buffer
		ashes::ByteArray luminance( size.width * size.height * size.depth );
		fread( luminance.data(), 1u, luminance.size(), file );
		fclose( file );

		// Convert the data to RGBA data.
		data.resize( size.width * size.height * size.depth * 4 );
		auto buffer = data.data();

		for ( auto & c : luminance )
		{
			buffer[0] = c;
			buffer[1] = c;
			buffer[2] = c;
			buffer[3] = c;
			buffer += 4;
		}
	}
}

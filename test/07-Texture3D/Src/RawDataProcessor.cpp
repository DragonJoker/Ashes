#include "RawDataProcessor.hpp"

#include <cstdio>

namespace vkapp
{
	void readFile( std::string const & name
		, VkExtent3D const & size
		, ashes::ByteArray & data )
	{
		FILE * file = fopen( name.c_str(), "rb" );

		if ( !file )
		{
			throw std::runtime_error{ "Couldn't load file." };
		}

		// Holds the luminance buffer
		data.resize( size.width * size.height * size.depth );
		size_t read = fread( data.data(), 1u, data.size(), file );
		assert( read == data.size() );
		fclose( file );
	}
}

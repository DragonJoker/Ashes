#include "FileUtils.hpp"

#include <ashespp/Core/Device.hpp>

#include <util/GlslToSpv.hpp>

#include <cassert>
#include <regex>

namespace common
{
	ImageData loadImage( std::string const & path )
	{
		if ( !wxFileExists( wxString( path ) ) )
		{
			throw std::runtime_error{ "Couldn't find image file." };
		}

		wxImage image{ path };

		if ( !image.IsOk() )
		{
			throw std::runtime_error{ "Couldn't load image file." };
		}

		uint8_t * data = image.GetData();
		ImageData result;
		result.format = VK_FORMAT_R8G8B8A8_UNORM;
		result.size = { uint32_t( image.GetSize().x ), uint32_t( image.GetSize().y ), 1u };
		uint32_t size = image.GetSize().x * image.GetSize().y;
		result.data.resize( size * 4 );
		auto it = result.data.begin();

		if ( image.HasAlpha() )
		{
			uint8_t * alpha = image.GetAlpha();

			for ( uint32_t i{ 0u }; i < size; ++i )
			{
				*it++ = *data++;
				*it++ = *data++;
				*it++ = *data++;
				*it++ = *alpha++;
			}
		}
		else
		{
			for ( uint32_t i{ 0u }; i < size; ++i )
			{
				*it++ = *data++;
				*it++ = *data++;
				*it++ = *data++;
				*it++ = 0xFF;
			}
		}

		return result;
	}

	ashes::UInt32Array parseShaderFile( ashes::Device const & device
		, VkShaderStageFlagBits stage
		, std::string const & path )
	{
		return utils::compileGlslToSpv( device.getProperties()
			, stage
			, utils::dumpTextFile( path ) );
	}

	ashes::UInt32Array dumpSpvFile( std::string const & path )
	{
		std::ifstream file( path, std::ios::binary );

		if ( file.fail() )
		{
			throw std::runtime_error{ "Could not open file " + path };
		}

		auto begin = file.tellg();
		file.seekg( 0, std::ios::end );
		auto end = file.tellg();

		ashes::UInt32Array result( static_cast< size_t >( end - begin ) / sizeof( uint32_t ) );
		file.seekg( 0, std::ios::beg );
		file.read( reinterpret_cast< char * >( result.data() ), end - begin );

		return result;
	}
}

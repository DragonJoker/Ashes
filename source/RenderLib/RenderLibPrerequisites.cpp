#include "RenderLibPrerequisites.h"

#include "Billboard.h"
#include "FontLoader.h"
#include "Font.h"
#include "Material.h"
#include "PolyLine.h"
#include "Texture.h"

#include <Renderer/DescriptorSetLayoutBinding.hpp>
#include <Renderer/DescriptorSet.hpp>

#include <Utils/StringUtils.hpp>
#include <Utils/Converter.hpp>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace render
{
	namespace
	{
		char getChar( std::string const & text )
		{
			return char( atoi( text.c_str() ) );
		}

		uint32_t getUInt( std::string const & text )
		{
			return uint32_t( atoi( text.c_str() ) );
		}

		utils::IVec2 getIVec2( std::string const & text )
		{
			auto infos = utils::split( text, " ", 3, false );
			utils::IVec2 ret;

			if ( infos.size() == 2 )
			{
				ret.x = atoi( infos[0].c_str() );
				ret.y = atoi( infos[1].c_str() );
			}

			return ret;
		}
	}

	RenderSubmesh::RenderSubmesh( renderer::DescriptorSetPool const & pool
		, MeshPtr mesh
		, SubmeshPtr submesh
		, MaterialPtr material
		, ObjectPtr object )
		: m_mesh{ mesh }
		, m_submesh{ submesh }
		, m_material{ material }
		, m_object{ object }
		, m_materialDescriptor{ pool }
	{
		uint32_t index = 0u;

		if ( material->hasDiffuseMap() )
		{
			m_materialDescriptor.createBinding( renderer::DescriptorSetLayoutBinding{ index++
					, renderer::DescriptorType::eSampledImage
					, renderer::ShaderStageFlag::eFragment }
				, material->diffuseMap().texture()
				, material->diffuseMap().sampler() );
		}

		if ( material->hasOpacityMap() )
		{
			m_materialDescriptor.createBinding( renderer::DescriptorSetLayoutBinding{ index++
					, renderer::DescriptorType::eSampledImage
					, renderer::ShaderStageFlag::eFragment }
				, material->opacityMap().texture()
				, material->opacityMap().sampler() );
		}
	}

	RenderBillboard::RenderBillboard( renderer::DescriptorSetPool const & pool
		, BillboardPtr billboard )
		: m_billboard{ billboard }
		, m_materialDescriptor{ pool }
	{
		auto & material = billboard->material();
		uint32_t index = 0u;

		if ( material.hasDiffuseMap() )
		{
			m_materialDescriptor.createBinding( renderer::DescriptorSetLayoutBinding{ index++
					, renderer::DescriptorType::eSampledImage
					, renderer::ShaderStageFlag::eFragment }
				, material.diffuseMap().texture()
				, material.diffuseMap().sampler() );
		}

		if ( material.hasOpacityMap() )
		{
			m_materialDescriptor.createBinding( renderer::DescriptorSetLayoutBinding{ index++
					, renderer::DescriptorType::eSampledImage
					, renderer::ShaderStageFlag::eFragment }
				, material.opacityMap().texture()
				, material.opacityMap().sampler() );
		}
	}

	RenderPolyLine::RenderPolyLine( renderer::DescriptorSetPool const & pool
		, PolyLinePtr line )
		: m_line{ line }
		, m_materialDescriptor{ pool }
	{
		auto & material = line->material();
		uint32_t index = 0u;

		if ( material.hasDiffuseMap() )
		{
			m_materialDescriptor.createBinding( renderer::DescriptorSetLayoutBinding{ index++
					, renderer::DescriptorType::eSampledImage
					, renderer::ShaderStageFlag::eFragment }
				, material.diffuseMap().texture()
				, material.diffuseMap().sampler() );
		}

		if ( material.hasOpacityMap() )
		{
			m_materialDescriptor.createBinding( renderer::DescriptorSetLayoutBinding{ index++
					, renderer::DescriptorType::eSampledImage
					, renderer::ShaderStageFlag::eFragment }
				, material.opacityMap().texture()
				, material.opacityMap().sampler() );
		}
	}

	void loadTexture( renderer::RenderingResources const & resources
		, ByteArray const & fileContent
		, Texture & texture )
	{
		int x = 0;
		int y = 0;
		int n = 0;
		uint8_t * data = stbi_load_from_memory( fileContent.data()
			, int( fileContent.size() )
			, &x
			, &y
			, &n
			, 0 );

		if ( data )
		{
			utils::PixelFormat format;

			switch ( n )
			{
			case 1:
				format = utils::PixelFormat::eL8;
				break;

			case 2:
				format = utils::PixelFormat::eL8A8;
				break;

			case 3:
				format = utils::PixelFormat::eR8G8B8;
				break;

			case 4:
				format = utils::PixelFormat::eR8G8B8A8;
				break;

			default:
				assert( "Unsupported component count" );
				format = utils::PixelFormat::eR8G8B8A8;
				break;
			}

			texture.image( format
				, utils::IVec2{ x, y }
				, ByteArray{ data, data + n * x * y }
				, resources );
			stbi_image_free( data );
		}
	}

	void loadTexture( renderer::RenderingResources const & resources
		, ByteArray const & fileContent
		, utils::PixelFormat format
		, Texture & texture )
	{
		int x = 0;
		int y = 0;
		int n = 0;
		uint8_t * data = stbi_load_from_memory( fileContent.data()
			, int( fileContent.size() )
			, &x
			, &y
			, &n
			, 0 );
		size_t bufferSize = x * y * utils::pixelSize( format );
		ByteArray buffer( bufferSize );

		if ( data )
		{
			switch ( n )
			{
			case 1:
				utils::convertBuffer< utils::PixelFormat::eL8 >( data
					, size_t( n * x * y )
					, buffer.data()
					, format
					, bufferSize );
				break;

			case 2:
				utils::convertBuffer< utils::PixelFormat::eL8A8 >( data
					, size_t( n * x * y )
					, buffer.data()
					, format
					, bufferSize );
				break;

			case 3:
				utils::convertBuffer< utils::PixelFormat::eR8G8B8 >( data
					, size_t( n * x * y )
					, buffer.data()
					, format
					, bufferSize );
				break;

			case 4:
				utils::convertBuffer< utils::PixelFormat::eR8G8B8A8 >( data
					, size_t( n * x * y )
					, buffer.data()
					, format
					, bufferSize );
				break;
			}

			texture.image( format
				, utils::IVec2{ x, y }
				, buffer
				, resources );
			stbi_image_free( data );
		}
	}

	void loadFont( std::string const & content
		, Font & font )
	{
		auto lines = utils::split( content, "\n", 0xFFFFFFFF, false );

		for ( auto & line : lines )
		{
			std::clog << line << std::endl;
			auto infos = utils::split( line, ",", 5, false );

			if ( infos.size() == 4 )
			{
				font.addGlyph( {
					getChar( infos[0] ),
					getIVec2( infos[2] ),
					getIVec2( infos[1] ),
					getUInt( infos[3] ),
					ByteArray{}
				} );
			}
		}
	}

	void loadFont( FontLoader & loader
		, Font & font )
	{
		loader.initialise( font.height() );

		for ( int c = 0; c <= 0xFF; ++c )
		{
			font.addGlyph( loader.loadGlyph( char( c ) ) );
		}
	}

}

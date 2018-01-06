#include "RenderLibPrerequisites.h"

#include "Billboard.h"
#include "FontLoader.h"
#include "Font.h"
#include "Material.h"
#include "Mesh.h"
#include "PolyLine.h"
#include "Submesh.h"
#include "Texture.h"

#include <Renderer/Buffer.hpp>
#include <Renderer/DescriptorSet.hpp>
#include <Renderer/DescriptorSetLayout.hpp>
#include <Renderer/DescriptorSetPool.hpp>
#include <Renderer/DescriptorSetLayoutBinding.hpp>
#include <Renderer/Device.hpp>
#include <Renderer/UniformBuffer.hpp>

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

	RenderSubmesh::RenderSubmesh( renderer::Device const & device
		, renderer::DescriptorSetPool const & pool
		, MeshPtr mesh
		, SubmeshPtr submesh
		, MaterialPtr material
		, ObjectPtr object
		, uint32_t index
		, renderer::UniformBuffer< MatrixUbo > const & mtxUbo
		, renderer::UniformBuffer< MaterialUbo > const & matUbo
		, renderer::VertexLayout const & posLayout
		, renderer::VertexLayoutPtr const & nmlLayout
		, renderer::VertexLayoutPtr const & texLayout )
		: m_mesh{ mesh }
		, m_submesh{ submesh }
		, m_material{ material }
		, m_object{ object }
		, m_descriptor{ pool.createDescriptorSet() }
	{
		m_descriptor->createBinding( pool.getLayout().getBinding( UberShader::UboMatrixBinding )
			, mtxUbo
			, index );
		m_descriptor->createBinding( pool.getLayout().getBinding( UberShader::UboMaterialBinding )
			, matUbo
			, index );

		if ( material->hasDiffuseMap() )
		{
			m_descriptor->createBinding( pool.getLayout().getBinding( UberShader::TextureDiffuseBinding )
				, material->diffuseMap().texture()
				, material->diffuseMap().sampler() );
		}

		if ( material->hasOpacityMap() )
		{
			m_descriptor->createBinding( pool.getLayout().getBinding( UberShader::TextureOpacityBinding )
				, material->opacityMap().texture()
				, material->opacityMap().sampler() );
		}

		m_descriptor->update();
		renderer::VertexBufferCRefArray vbos;
		renderer::VertexLayoutCRefArray layouts;
		std::vector< uint64_t > offsets;

		vbos.emplace_back( m_mesh->getPositions() );
		layouts.emplace_back( posLayout );
		offsets.emplace_back( 0ull );

		if ( nmlLayout )
		{
			vbos.emplace_back( m_mesh->getNormals() );
			layouts.emplace_back( *nmlLayout );
			offsets.emplace_back( 0ull );
		}

		if ( texLayout )
		{
			vbos.emplace_back( m_mesh->getTexCoords() );
			layouts.emplace_back( *texLayout );
			offsets.emplace_back( 0ull );
		}

		m_vao = device.createGeometryBuffers( vbos
			, offsets
			, layouts
			, m_submesh->getIbo().getBuffer()
			, 0ull
			, renderer::IndexType::eUInt16 );
	}

	RenderBillboard::RenderBillboard( renderer::Device const & device
		, renderer::DescriptorSetPool const & pool
		, BillboardPtr billboard
		, uint32_t index
		, renderer::UniformBuffer< MatrixUbo > const & mtxUbo
		, renderer::UniformBuffer< MaterialUbo > const & matUbo
		, renderer::UniformBuffer< BillboardUbo > const & billboardUbo
		, renderer::VertexLayout const & layout )
		: m_billboard{ billboard }
		, m_descriptor{ pool.createDescriptorSet() }
	{
		m_descriptor->createBinding( pool.getLayout().getBinding( UberShader::UboMatrixBinding )
			, mtxUbo
			, index );
		m_descriptor->createBinding( pool.getLayout().getBinding( UberShader::UboMaterialBinding )
			, matUbo
			, index );
		m_descriptor->createBinding( pool.getLayout().getBinding( UberShader::UboBillboardBinding )
			, billboardUbo
			, index );

		auto & material = billboard->material();

		if ( material.hasDiffuseMap() )
		{
			m_descriptor->createBinding( pool.getLayout().getBinding( UberShader::TextureDiffuseBinding )
				, material.diffuseMap().texture()
				, material.diffuseMap().sampler() );
		}

		if ( material.hasOpacityMap() )
		{
			m_descriptor->createBinding( pool.getLayout().getBinding( UberShader::TextureOpacityBinding )
				, material.opacityMap().texture()
				, material.opacityMap().sampler() );
		}

		m_descriptor->update();

		m_vao = device.createGeometryBuffers( m_billboard->buffer().vbo()
			, 0ull
			, layout );
	}

	RenderPolyLine::RenderPolyLine( renderer::Device const & device
		, renderer::DescriptorSetPool const & pool
		, PolyLinePtr line
		, uint32_t index
		, renderer::UniformBuffer< MatrixUbo > const & mtxUbo
		, renderer::UniformBuffer< MaterialUbo > const & matUbo
		, renderer::UniformBuffer< LineUbo > const & lineUbo
		, renderer::VertexLayout const & layout )
		: m_line{ line }
		, m_descriptor{ pool.createDescriptorSet() }
	{
		m_descriptor->createBinding( pool.getLayout().getBinding( UberShader::UboMatrixBinding )
			, mtxUbo
			, index );
		m_descriptor->createBinding( pool.getLayout().getBinding( UberShader::UboMaterialBinding )
			, matUbo
			, index );
		m_descriptor->createBinding( pool.getLayout().getBinding( UberShader::UboPolyLineBinding )
			, lineUbo
			, index );

		auto & material = line->material();

		if ( material.hasDiffuseMap() )
		{
			m_descriptor->createBinding( pool.getLayout().getBinding( UberShader::TextureDiffuseBinding )
				, material.diffuseMap().texture()
				, material.diffuseMap().sampler() );
		}

		if ( material.hasOpacityMap() )
		{
			m_descriptor->createBinding( pool.getLayout().getBinding( UberShader::TextureOpacityBinding )
				, material.opacityMap().texture()
				, material.opacityMap().sampler() );
		}

		m_descriptor->update();

		//m_vao = device.createGeometryBuffers( m_line->buffer()
		//	, 0ull
		//	, layout );
	}

	void loadTexture( renderer::StagingBuffer const & stagingBuffer
		, renderer::CommandBuffer const & commandBuffer
		, ByteArray const & fileContent
		, Texture & texture )
	{
		int x = 0;
		int y = 0;
		int n = 0;
		int r = 4;
		uint8_t * data = stbi_load_from_memory( fileContent.data()
			, int( fileContent.size() )
			, &x
			, &y
			, &n
			, r );

		if ( data )
		{
			utils::PixelFormat format;

			switch ( r )
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
				, ByteArray{ data, data + r * x * y }
				, stagingBuffer
				, commandBuffer );
			stbi_image_free( data );
		}
	}

	void loadTexture( renderer::StagingBuffer const & stagingBuffer
		, renderer::CommandBuffer const & commandBuffer
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
				, stagingBuffer
				, commandBuffer );
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

#include "RenderTarget.hpp"

#include "OpaqueRendering.hpp"
#include "TransparentRendering.hpp"

#include <FileUtils.hpp>
#include <OpaqueRendering.hpp>
#include <TransparentRendering.hpp>

#include <Buffer/StagingBuffer.hpp>
#include <Buffer/UniformBuffer.hpp>

#include <Utils/Transform.hpp>

namespace vkapp
{
	namespace
	{
		renderer::ShaderProgramPtr doCreateProgram( renderer::Device const & device
			, std::string const & shader )
		{
			renderer::ShaderProgramPtr result = device.createShaderProgram();
			std::string shadersFolder = common::getPath( common::getExecutableDirectory() ) / "share" / AppName / "Shaders";

			if ( !wxFileExists( shadersFolder / ( shader + ".vert" ) )
				|| !wxFileExists( shadersFolder / ( shader + ".frag" ) ) )
			{
				throw std::runtime_error{ "Shader files are missing" };
			}

			result->createModule( common::dumpTextFile( shadersFolder / ( shader + ".vert" ) )
				, renderer::ShaderStageFlag::eVertex );
			result->createModule( common::dumpTextFile( shadersFolder / ( shader + ".frag" ) )
				, renderer::ShaderStageFlag::eFragment );

			return result;
		}
	}

	RenderTarget::RenderTarget( renderer::Device const & device
		, renderer::UIVec2 const & size
		, common::Object && object
		, common::ImagePtrArray && images )
		: common::RenderTarget{ device, size, std::move( object ), std::move( images ) }
		, m_matrixUbo{ renderer::makeUniformBuffer< renderer::Mat4 >( device
			, 1u
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal ) }
		, m_objectUbo{ renderer::makeUniformBuffer< renderer::Mat4 >( device
			, 1u
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal ) }
		, m_lightsUbo{ renderer::makeUniformBuffer< common::LightsData >( device
			, 1u
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal ) }
	{
		doCreateGBuffer();
		doInitialise();
		doUpdateMatrixUbo( size );
		doInitialiseLights();
	}

	void RenderTarget::doUpdate( std::chrono::microseconds const & duration )
	{
		static renderer::Mat4 const originalTranslate = []()
		{
			renderer::Mat4 result;
			result = utils::translate( result, { 0, 0, -5 } );
			return result;
		}();
		m_rotate = utils::rotate( m_rotate
			, float( utils::DegreeToRadian ) * ( duration.count() / 20000.0f )
			, { 0, 1, 0 } );
		m_objectUbo->getData( 0 ) = originalTranslate * m_rotate;
		m_stagingBuffer->uploadUniformData( *m_updateCommandBuffer
			, m_objectUbo->getDatas()
			, *m_objectUbo
			, renderer::PipelineStageFlag::eVertexShader );
	}

	void RenderTarget::doResize( renderer::UIVec2 const & size )
	{
		doUpdateMatrixUbo( size );
		doCreateGBuffer();
	}

	common::OpaqueRenderingPtr RenderTarget::doCreateOpaqueRendering( renderer::Device const & device
		, renderer::StagingBuffer & stagingBuffer
		, renderer::TextureViewCRefArray const & views
		, common::Object const & submeshes
		, common::TextureNodePtrArray const & textureNodes )
	{
		return std::make_unique< OpaqueRendering >( std::make_unique< GeometryPass >( device
				, doCreateProgram( device, "opaque_gp" )
				, m_gbuffer
				, views[0].get().getFormat()
				, *m_matrixUbo
				, *m_objectUbo )
			, submeshes
			, stagingBuffer
			, m_gbuffer
			, views
			, textureNodes
			, *m_matrixUbo
			, *m_lightsUbo );
	}

	common::TransparentRenderingPtr RenderTarget::doCreateTransparentRendering( renderer::Device const & device
		, renderer::StagingBuffer & stagingBuffer
		, renderer::TextureViewCRefArray const & views
		, common::Object const & submeshes
		, common::TextureNodePtrArray const & textureNodes )
	{
		return std::make_unique< common::TransparentRendering >( std::make_unique< TransparentRendering >( device
				, doCreateProgram( device, "transparent" )
				, common::getFormats( views )
				, *m_matrixUbo
				, *m_objectUbo
				, *m_lightsUbo )
			, submeshes
			, stagingBuffer
			, views
			, textureNodes );
	}

	void RenderTarget::doUpdateMatrixUbo( renderer::UIVec2 const & size )
	{
#if 0
		float halfWidth = static_cast< float >( size[0] ) * 0.5f;
		float halfHeight = static_cast< float >( size[1] ) * 0.5f;
		float wRatio = 1.0f;
		float hRatio = 1.0f;

		if ( halfHeight > halfWidth )
		{
			hRatio = halfHeight / halfWidth;
		}
		else
		{
			wRatio = halfWidth / halfHeight;
		}

		m_matrixUbo->getData( 0u ) = m_device->ortho( -2.0f * wRatio
			, 2.0f * wRatio
			, -2.0f * hRatio
			, 2.0f * hRatio
			, 0.0f
			, 10.0f );
#else
		auto width = float( size[0] );
		auto height = float( size[1] );
		m_matrixUbo->getData( 0u ) = m_device.perspective( utils::toRadians( 90.0_degrees )
			, width / height
			, 0.01f
			, 100.0f );
#endif
		m_stagingBuffer->uploadUniformData( *m_updateCommandBuffer
			, m_matrixUbo->getDatas()
			, *m_matrixUbo
			, renderer::PipelineStageFlag::eVertexShader );
	}

	void RenderTarget::doInitialiseLights()
	{
		auto & lights = m_lightsUbo->getData( 0u );
		lights.lightsCount[0] = 1;
		common::DirectionalLight directional
		{
			{
				renderer::Vec4{ 1, 1, 1, 1 },
				renderer::Vec4{ 0.75, 1.0, 0.0, 0.0 }
			},
			renderer::Vec4{ 1.0, -0.25, -1.0, 0.0 }
		};
		lights.directionalLights[0] = directional;

		m_stagingBuffer->uploadUniformData( *m_updateCommandBuffer
			, m_lightsUbo->getDatas()
			, *m_lightsUbo
			, renderer::PipelineStageFlag::eFragmentShader );
	}

	void RenderTarget::doCreateGBuffer()
	{
		static renderer::PixelFormat const formats[]
		{
			renderer::PixelFormat::eR32F,
			utils::PixelFormat::eRGBA32F,
			utils::PixelFormat::eRGBA32F,
			utils::PixelFormat::eRGBA32F,
			utils::PixelFormat::eRGBA32F,
		};
		size_t index = 0u;
		renderer::UIVec2 size
		{
			getColourView().getTexture().getDimensions()[0],
			getColourView().getTexture().getDimensions()[1],
		};

		for ( auto & texture : m_gbuffer )
		{
			texture.texture = m_device.createTexture();
			texture.texture->setImage( formats[index]
				, size
				, renderer::ImageUsageFlag::eColourAttachment | renderer::ImageUsageFlag::eSampled );
			texture.view = texture.texture->createView( renderer::TextureType::e2D
				, texture.texture->getFormat() );
			++index;
		}
	}
}

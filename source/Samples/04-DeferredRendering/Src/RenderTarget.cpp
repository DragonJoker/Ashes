#include "RenderTarget.hpp"

#include "OpaqueRendering.hpp"
#include "TransparentRendering.hpp"

#include <FileUtils.hpp>
#include <OpaqueRendering.hpp>
#include <TransparentRendering.hpp>

#include <Buffer/StagingBuffer.hpp>
#include <Buffer/UniformBuffer.hpp>

#include <Transform.hpp>

namespace vkapp
{
	RenderTarget::RenderTarget( ashes::Device const & device
		, ashes::Extent2D const & size
		, common::Scene && scene
		, common::ImagePtrArray && images )
		: common::RenderTarget{ device, size, std::move( scene ), std::move( images ) }
		, m_sceneUbo{ ashes::makeUniformBuffer< common::SceneData >( device
			, 1u
			, ashes::BufferTarget::eTransferDst
			, ashes::MemoryPropertyFlag::eDeviceLocal ) }
		, m_objectUbo{ ashes::makeUniformBuffer< common::ObjectData >( device
			, 1u
			, ashes::BufferTarget::eTransferDst
			, ashes::MemoryPropertyFlag::eDeviceLocal ) }
		, m_lightsUbo{ ashes::makeUniformBuffer< common::LightsData >( device
			, 1u
			, ashes::BufferTarget::eTransferDst
			, ashes::MemoryPropertyFlag::eDeviceLocal ) }
	{
		doCreateGBuffer();
		doInitialise();
		doUpdateMatrixUbo( size );
		doInitialiseLights();
	}

	void RenderTarget::doUpdate( std::chrono::microseconds const & duration )
	{
		static utils::Mat4 const originalTranslate = []()
		{
			utils::Mat4 result;
			result = utils::translate( result, { 0, 0, -5 } );
			return result;
		}();
		m_rotate = utils::rotate( m_rotate
			, float( utils::DegreeToRadian ) * ( duration.count() / 20000.0f )
			, { 0, 1, 0 } );
		m_objectUbo->getData( 0 ).mtxModel = originalTranslate * m_rotate;
		m_stagingBuffer->uploadUniformData( *m_updateCommandBuffer
			, m_objectUbo->getDatas()
			, *m_objectUbo
			, ashes::PipelineStageFlag::eVertexShader );
	}

	void RenderTarget::doResize( ashes::Extent2D const & size )
	{
		doUpdateMatrixUbo( size );
		doCreateGBuffer();
	}

	common::OpaqueRenderingPtr RenderTarget::doCreateOpaqueRendering( ashes::Device const & device
		, ashes::StagingBuffer & stagingBuffer
		, ashes::TextureViewCRefArray const & views
		, common::Scene const & scene
		, common::TextureNodePtrArray const & textureNodes )
	{
		return std::make_unique< OpaqueRendering >( std::make_unique< GeometryPass >( device
				, common::getPath( common::getExecutableDirectory() ) / "share" / AppName / "Shaders" / "opaque_gp.frag"
				, m_gbuffer
				, views[0].get().getFormat()
				, *m_sceneUbo
				, *m_objectUbo )
			, scene
			, stagingBuffer
			, m_gbuffer
			, views
			, textureNodes
			, *m_sceneUbo
			, *m_lightsUbo );
	}

	common::TransparentRenderingPtr RenderTarget::doCreateTransparentRendering( ashes::Device const & device
		, ashes::StagingBuffer & stagingBuffer
		, ashes::TextureViewCRefArray const & views
		, common::Scene const & scene
		, common::TextureNodePtrArray const & textureNodes )
	{
		return std::make_unique< common::TransparentRendering >( std::make_unique< TransparentRendering >( device
				, common::getPath( common::getExecutableDirectory() ) / "share" / AppName / "Shaders" / "transparent.frag"
				, common::getFormats( views )
				, *m_sceneUbo
				, *m_objectUbo
				, *m_lightsUbo )
			, scene
			, stagingBuffer
			, views
			, textureNodes );
	}

	void RenderTarget::doUpdateMatrixUbo( ashes::Extent2D const & size )
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

		m_sceneUbo->getData( 0u ).mtxProjection = utils::Mat4{ m_device->ortho( -2.0f * wRatio
			, 2.0f * wRatio
			, -2.0f * hRatio
			, 2.0f * hRatio
			, 0.0f
			, 10.0f ) };
#else
		auto width = float( size.width );
		auto height = float( size.height );
		m_sceneUbo->getData( 0u ).mtxProjection = utils::Mat4{ m_device.perspective( float( utils::toRadians( 90.0_degrees ) )
			, width / height
			, 0.01f
			, 100.0f ) };
#endif
		m_stagingBuffer->uploadUniformData( *m_updateCommandBuffer
			, m_sceneUbo->getDatas()
			, *m_sceneUbo
			, ashes::PipelineStageFlag::eVertexShader );
	}

	void RenderTarget::doInitialiseLights()
	{
		auto & lights = m_lightsUbo->getData( 0u );
		lights.lightsCount[0] = 1;
		common::DirectionalLight directional
		{
			{
				utils::Vec4{ 1, 1, 1, 1 },
				utils::Vec4{ 0.75, 1.0, 0.0, 0.0 }
			},
			utils::Vec4{ 1.0, -0.25, -1.0, 0.0 }
		};
		lights.directionalLights[0] = directional;

		m_stagingBuffer->uploadUniformData( *m_updateCommandBuffer
			, m_lightsUbo->getDatas()
			, *m_lightsUbo
			, ashes::PipelineStageFlag::eFragmentShader );
	}

	void RenderTarget::doCreateGBuffer()
	{
		static ashes::Format const formats[]
		{
			ashes::Format::eR32_SFLOAT,
			ashes::Format::eR32G32B32A32_SFLOAT,
			ashes::Format::eR32G32B32A32_SFLOAT,
			ashes::Format::eR32G32B32A32_SFLOAT,
			ashes::Format::eR32G32B32A32_SFLOAT,
		};
		size_t index = 0u;
		ashes::Extent2D size
		{
			getColourView().getTexture().getDimensions().width,
			getColourView().getTexture().getDimensions().height,
		};

		for ( auto & texture : m_gbuffer )
		{
			texture.texture = m_device.createTexture(
				{
					0,
					ashes::TextureType::e2D,
					formats[index],
					getColourView().getTexture().getDimensions(),
					1u,
					1u,
					ashes::SampleCountFlag::e1,
					ashes::ImageTiling::eOptimal,
					ashes::ImageUsageFlag::eColourAttachment | ashes::ImageUsageFlag::eSampled
				}
				, ashes::MemoryPropertyFlag::eDeviceLocal );
			texture.view = texture.texture->createView( ashes::TextureViewType::e2D
				, texture.texture->getFormat() );
			++index;
		}
	}
}

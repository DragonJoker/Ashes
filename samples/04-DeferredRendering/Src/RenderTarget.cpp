#include "RenderTarget.hpp"

#include "OpaqueRendering.hpp"
#include "TransparentRendering.hpp"

#include <FileUtils.hpp>
#include <OpaqueRendering.hpp>
#include <TransparentRendering.hpp>

#include <ashespp/Buffer/StagingBuffer.hpp>
#include <ashespp/Buffer/UniformBuffer.hpp>

#include <util/Transform.hpp>

namespace vkapp
{
	RenderTarget::RenderTarget( utils::Device const & device
		, ashes::CommandPool const & commandPool
		, ashes::Queue const & transferQueue
		, VkExtent2D const & size
		, common::Scene scene
		, common::ImagePtrArray images )
		: common::RenderTarget{ device, commandPool, transferQueue, size, std::move( scene ), std::move( images ) }
		, m_sceneUbo{ utils::makeUniformBuffer< common::SceneData >( device
			, 1u
			, VK_BUFFER_USAGE_TRANSFER_DST_BIT
			, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT ) }
		, m_objectUbo{ utils::makeUniformBuffer< common::ObjectData >( device
			, 1u
			, VK_BUFFER_USAGE_TRANSFER_DST_BIT
			, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT ) }
		, m_lightsUbo{ utils::makeUniformBuffer< common::LightsData >( device
			, 1u
			, VK_BUFFER_USAGE_TRANSFER_DST_BIT
			, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT ) }
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
		m_stagingBuffer->uploadUniformData( m_transferQueue
			, m_commandPool
			, m_objectUbo->getDatas()
			, *m_objectUbo
			, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT );
	}

	void RenderTarget::doResize( VkExtent2D const & size )
	{
		doUpdateMatrixUbo( size );
		doCreateGBuffer();
	}

	common::OpaqueRenderingPtr RenderTarget::doCreateOpaqueRendering( utils::Device const & device
		, ashes::StagingBuffer & stagingBuffer
		, ashes::ImageViewArray views
		, common::Scene const & scene
		, common::TextureNodePtrArray const & textureNodes )
	{
		return std::make_unique< OpaqueRendering >( std::make_unique< GeometryPass >( device
				, m_commandPool
				, m_transferQueue
				, ashes::getPath( ashes::getExecutableDirectory() ) / "share" / AppName / "Shaders" / "opaque_gp.frag"
				, m_gbuffer
				, views[0]->getFormat()
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

	common::TransparentRenderingPtr RenderTarget::doCreateTransparentRendering( utils::Device const & device
		, ashes::StagingBuffer & stagingBuffer
		, ashes::ImageViewArray views
		, common::Scene const & scene
		, common::TextureNodePtrArray const & textureNodes )
	{
		return std::make_unique< common::TransparentRendering >( std::make_unique< TransparentRendering >( device
				, m_commandPool
				, m_transferQueue
				, ashes::getPath( ashes::getExecutableDirectory() ) / "share" / AppName / "Shaders" / "transparent.frag"
				, common::getFormats( views )
				, *m_sceneUbo
				, *m_objectUbo
				, *m_lightsUbo )
			, scene
			, stagingBuffer
			, views
			, textureNodes );
	}

	void RenderTarget::doUpdateMatrixUbo( VkExtent2D const & size )
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

		m_sceneUbo->getData( 0u ).mtxProjection = utils::Mat4{ m_device->getDevice().ortho( -2.0f * wRatio
			, 2.0f * wRatio
			, -2.0f * hRatio
			, 2.0f * hRatio
			, 0.0f
			, 10.0f ) };
#else
		auto width = float( size.width );
		auto height = float( size.height );
		m_sceneUbo->getData( 0u ).mtxProjection = utils::Mat4{ m_device.getDevice().perspective( float( utils::toRadians( 90.0_degrees ) )
			, width / height
			, 0.01f
			, 100.0f ) };
#endif
		m_stagingBuffer->uploadUniformData( m_transferQueue
			, m_commandPool
			, m_sceneUbo->getDatas()
			, *m_sceneUbo
			, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT );
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

		m_stagingBuffer->uploadUniformData( m_transferQueue
			, m_commandPool
			, m_lightsUbo->getDatas()
			, *m_lightsUbo
			, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT );
	}

	void RenderTarget::doCreateGBuffer()
	{
		static VkFormat const formats[]
		{
			VK_FORMAT_R32_SFLOAT,
			VK_FORMAT_R32G32B32A32_SFLOAT,
			VK_FORMAT_R32G32B32A32_SFLOAT,
			VK_FORMAT_R32G32B32A32_SFLOAT,
			VK_FORMAT_R32G32B32A32_SFLOAT,
		};
		size_t index = 0u;
		VkExtent2D size
		{
			getColourView()->getImage().getDimensions().width,
			getColourView()->getImage().getDimensions().height,
		};

		for ( auto & texture : m_gbuffer )
		{
			texture.texture = m_device.createImage(
				{
					0,
					VK_IMAGE_TYPE_2D,
					formats[index],
					getColourView()->getImage().getDimensions(),
					1u,
					1u,
					VK_SAMPLE_COUNT_1_BIT,
					VK_IMAGE_TILING_OPTIMAL,
					VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT
				}
				, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
			texture.view = texture.texture->createView( VK_IMAGE_VIEW_TYPE_2D
				, texture.texture->getFormat() );
			++index;
		}
	}
}

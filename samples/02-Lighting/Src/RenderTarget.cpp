#include "RenderTarget.hpp"

#include "NodesRenderer.hpp"

#include <FileUtils.hpp>
#include <OpaqueRendering.hpp>
#include <TransparentRendering.hpp>

#include <AshesPP/Buffer/StagingBuffer.hpp>
#include <AshesPP/Buffer/UniformBuffer.hpp>

#include <Utils/Transform.hpp>

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
	}

	common::OpaqueRenderingPtr RenderTarget::doCreateOpaqueRendering( utils::Device const & device
		, ashes::StagingBuffer & stagingBuffer
		, ashes::ImageViewPtrArray views
		, common::Scene const & scene
		, common::TextureNodePtrArray const & textureNodes )
	{
		return std::make_unique< common::OpaqueRendering >( std::make_unique< NodesRenderer >( device
				, m_commandPool
				, m_transferQueue
				, ashes::getPath( ashes::getExecutableDirectory() ) / "share" / AppName / "Shaders" / "offscreen.frag"
				, common::getFormats( views )
				, true
				, true
				, *m_sceneUbo
				, *m_objectUbo
				, *m_lightsUbo )
			, scene
			, stagingBuffer
			, views
			, textureNodes );
	}

	common::TransparentRenderingPtr RenderTarget::doCreateTransparentRendering( utils::Device const & device
		, ashes::StagingBuffer & stagingBuffer
		, ashes::ImageViewPtrArray views
		, common::Scene const & scene
		, common::TextureNodePtrArray const & textureNodes )
	{
		return std::make_unique< common::TransparentRendering >( std::make_unique< NodesRenderer >( device
				, m_commandPool
				, m_transferQueue
				, ashes::getPath( ashes::getExecutableDirectory() ) / "share" / AppName / "Shaders" / "offscreen.frag"
				, common::getFormats( views )
				, false
				, false
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
		auto width = float( size.width );
		auto height = float( size.height );
		m_sceneUbo->getData( 0u ).mtxProjection = utils::Mat4{ m_device.getDevice().perspective( float( utils::toRadians( 90.0_degrees ) )
			, width / height
			, 0.01f
			, 100.0f ) };
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
}

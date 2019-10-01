#include "RenderTarget.hpp"

#include "NodesRenderer.hpp"

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
		, m_sceneUbo{ utils::makeUniformBuffer( device
			, 1u
			, uint32_t( sizeof( common::SceneData ) )
			, VK_BUFFER_USAGE_TRANSFER_DST_BIT
			, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT ) }
		, m_sceneData{ 1u }
	{
		doInitialise();
		doUpdateProjection( size );
		m_camera.update();
	}

	void RenderTarget::doUpdateProjection( VkExtent2D const & size )
	{
		auto width = float( size.width );
		auto height = float( size.height );
		m_sceneData[0].mtxProjection = utils::Mat4{ m_device.getDevice().perspective( float( utils::toRadians( 90.0_degrees ) )
			, width / height
			, 0.01f
			, 100.0f ) };
	}

	void RenderTarget::doUpdate( std::chrono::microseconds const & duration )
	{
		if ( m_currentMousePosition != m_previousMousePosition
			&& m_moveCamera )
		{
			VkOffset2D delta = {
				m_currentMousePosition.x - m_previousMousePosition.x,
				m_currentMousePosition.y - m_previousMousePosition.y,
			};
			auto & result = m_camera.getRotation();
			result = utils::pitch( result, utils::Radians{ float( -delta.y ) / m_size.width } );
			result = utils::yaw( result, utils::Radians{ float( delta.x ) / m_size.height } );
			m_camera.update();
		}

		m_previousMousePosition = m_currentMousePosition;
		auto & data = m_sceneData[0];
		data.mtxView = m_camera.getView();
		auto & pos = m_camera.getPosition();
		data.cameraPosition = utils::Vec4{ pos[0], pos[1], pos[2], 0.0f };
		m_stagingBuffer->uploadUniformData( m_transferQueue
			, m_commandPool
			, m_sceneData
			, *m_sceneUbo
			, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT );
	}

	void RenderTarget::doResize( VkExtent2D const & size )
	{
		doUpdateProjection( size );
	}

	common::OpaqueRenderingPtr RenderTarget::doCreateOpaqueRendering( utils::Device const & device
		, ashes::StagingBuffer & stagingBuffer
		, ashes::ImageViewArray views
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
				, *m_sceneUbo )
			, scene
			, stagingBuffer
			, views
			, textureNodes );
	}

	common::TransparentRenderingPtr RenderTarget::doCreateTransparentRendering( utils::Device const & device
		, ashes::StagingBuffer & stagingBuffer
		, ashes::ImageViewArray views
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
				, *m_sceneUbo )
			, scene
			, stagingBuffer
			, views
			, textureNodes );
	}
}

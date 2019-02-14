#include "RenderTarget.hpp"

#include "NodesRenderer.hpp"

#include <FileUtils.hpp>
#include <OpaqueRendering.hpp>
#include <TransparentRendering.hpp>

#include <Ashes/Buffer/StagingBuffer.hpp>
#include <Ashes/Buffer/UniformBuffer.hpp>

#include <Utils/Transform.hpp>

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
	{
		doInitialise();
		doUpdateProjection( size );
		m_camera.update();
	}

	void RenderTarget::doUpdateProjection( ashes::Extent2D const & size )
	{
		auto width = float( size.width );
		auto height = float( size.height );
		m_sceneUbo->getData( 0u ).mtxProjection = utils::Mat4{ m_device.perspective( float( utils::toRadians( 90.0_degrees ) )
			, width / height
			, 0.01f
			, 100.0f ) };
	}

	void RenderTarget::doUpdate( std::chrono::microseconds const & duration )
	{
		if ( m_currentMousePosition != m_previousMousePosition
			&& m_moveCamera )
		{
			ashes::Offset2D delta = {
				m_currentMousePosition.x - m_previousMousePosition.x,
				m_currentMousePosition.y - m_previousMousePosition.y,
			};
			auto & result = m_camera.getRotation();
			result = utils::pitch( result, utils::Radians{ float( delta.x ) / m_size.width } );
			result = utils::yaw( result, utils::Radians{ float( -delta.y ) / m_size.height } );
			m_camera.update();
		}

		m_previousMousePosition = m_currentMousePosition;
		auto & data = m_sceneUbo->getData( 0u );
		data.mtxView = m_camera.getView();
		auto & pos = m_camera.getPosition();
		data.cameraPosition = utils::Vec4{ pos[0], pos[1], pos[2], 0.0f };
		m_stagingBuffer->uploadUniformData( *m_updateCommandBuffer
			, m_sceneUbo->getDatas()
			, *m_sceneUbo
			, ashes::PipelineStageFlag::eVertexShader );
	}

	void RenderTarget::doResize( ashes::Extent2D const & size )
	{
		doUpdateProjection( size );
	}

	common::OpaqueRenderingPtr RenderTarget::doCreateOpaqueRendering( ashes::Device const & device
		, ashes::StagingBuffer & stagingBuffer
		, ashes::TextureViewCRefArray const & views
		, common::Scene const & scene
		, common::TextureNodePtrArray const & textureNodes )
	{
		return std::make_unique< common::OpaqueRendering >( std::make_unique< NodesRenderer >( device
				, common::getPath( common::getExecutableDirectory() ) / "share" / AppName / "Shaders" / "offscreen.frag"
				, common::getFormats( views )
				, true
				, true
				, *m_sceneUbo )
			, scene
			, stagingBuffer
			, views
			, textureNodes );
	}

	common::TransparentRenderingPtr RenderTarget::doCreateTransparentRendering( ashes::Device const & device
		, ashes::StagingBuffer & stagingBuffer
		, ashes::TextureViewCRefArray const & views
		, common::Scene const & scene
		, common::TextureNodePtrArray const & textureNodes )
	{
		return std::make_unique< common::TransparentRendering >( std::make_unique< NodesRenderer >( device
				, common::getPath( common::getExecutableDirectory() ) / "share" / AppName / "Shaders" / "offscreen.frag"
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

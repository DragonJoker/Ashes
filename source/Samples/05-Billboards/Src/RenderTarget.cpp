#include "RenderTarget.hpp"

#include "NodesRenderer.hpp"

#include <FileUtils.hpp>
#include <OpaqueRendering.hpp>
#include <TransparentRendering.hpp>

#include <Buffer/StagingBuffer.hpp>
#include <Buffer/UniformBuffer.hpp>

#include <Utils/Transform.hpp>

namespace vkapp
{
	RenderTarget::RenderTarget( renderer::Device const & device
		, renderer::Extent2D const & size
		, common::Scene && scene
		, common::ImagePtrArray && images )
		: common::RenderTarget{ device, size, std::move( scene ), std::move( images ) }
		, m_sceneUbo{ renderer::makeUniformBuffer< common::SceneData >( device
			, 1u
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal ) }
	{
		doInitialise();
		doUpdateProjection( size );
		m_camera.update();
	}

	void RenderTarget::doUpdateProjection( renderer::Extent2D const & size )
	{
		auto width = float( size.width );
		auto height = float( size.height );
		m_sceneUbo->getData( 0u ).mtxProjection = m_device.perspective( utils::toRadians( 90.0_degrees )
			, width / height
			, 0.01f
			, 100.0f );
	}

	void RenderTarget::doUpdate( std::chrono::microseconds const & duration )
	{
		if ( m_currentMousePosition != m_previousMousePosition
			&& m_moveCamera )
		{
			renderer::Offset2D delta = {
				m_currentMousePosition.x - m_previousMousePosition.x,
				m_currentMousePosition.y - m_previousMousePosition.y,
			};
			auto & result = m_camera.getRotation();
			result = utils::pitch( result, renderer::Radians{ float( delta.x ) / m_size.width } );
			result = utils::yaw( result, renderer::Radians{ float( -delta.y ) / m_size.height } );
			m_camera.update();
		}

		m_previousMousePosition = m_currentMousePosition;
		auto & data = m_sceneUbo->getData( 0u );
		data.mtxView = m_camera.getView();
		auto & pos = m_camera.getPosition();
		data.cameraPosition = renderer::Vec4{ pos[0], pos[1], pos[2], 0.0f };
		m_stagingBuffer->uploadUniformData( *m_updateCommandBuffer
			, m_sceneUbo->getDatas()
			, *m_sceneUbo
			, renderer::PipelineStageFlag::eVertexShader );
	}

	void RenderTarget::doResize( renderer::Extent2D const & size )
	{
		doUpdateProjection( size );
	}

	common::OpaqueRenderingPtr RenderTarget::doCreateOpaqueRendering( renderer::Device const & device
		, renderer::StagingBuffer & stagingBuffer
		, renderer::TextureViewCRefArray const & views
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

	common::TransparentRenderingPtr RenderTarget::doCreateTransparentRendering( renderer::Device const & device
		, renderer::StagingBuffer & stagingBuffer
		, renderer::TextureViewCRefArray const & views
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

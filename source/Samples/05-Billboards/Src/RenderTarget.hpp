#pragma once

#include "Prerequisites.hpp"

#include <Camera.hpp>
#include <RenderTarget.hpp>

namespace vkapp
{
	class RenderTarget
		: public common::RenderTarget
	{
	public:
		RenderTarget( renderer::Device const & device
			, renderer::Extent2D const & size
			, common::Scene && scene
			, common::ImagePtrArray && images );

		inline void enableMoveCamera( bool enable )
		{
			m_moveCamera = enable;
		}

		inline void updateMousePosition( renderer::Offset2D const & position )
		{
			m_currentMousePosition = position;
		}

	private:
		void doUpdateProjection( renderer::Extent2D const & size );
		void doUpdate( std::chrono::microseconds const & duration )override;
		virtual void doResize( renderer::Extent2D const & size )override;
		common::OpaqueRenderingPtr doCreateOpaqueRendering( renderer::Device const & device
			, renderer::StagingBuffer & stagingBuffer
			, renderer::TextureViewCRefArray const & views
			, common::Scene const & scene
			, common::TextureNodePtrArray const & textureNodes )override;
		common::TransparentRenderingPtr doCreateTransparentRendering( renderer::Device const & device
			, renderer::StagingBuffer & stagingBuffer
			, renderer::TextureViewCRefArray const & views
			, common::Scene const & scene
			, common::TextureNodePtrArray const & textureNodes )override;

	private:
		renderer::UniformBufferPtr< common::SceneData > m_sceneUbo;
		renderer::Offset2D m_previousMousePosition;
		renderer::Offset2D m_currentMousePosition;
		common::Camera m_camera;
		bool m_moveCamera{ false };
	};
}

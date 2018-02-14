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
			, renderer::UIVec2 const & size
			, common::Scene && scene
			, common::ImagePtrArray && images );

		inline void enableMoveCamera( bool enable )
		{
			m_moveCamera = enable;
		}

		inline void updateMousePosition( renderer::IVec2 const & position )
		{
			m_currentMousePosition = position;
		}

	private:
		void doUpdateProjection( renderer::UIVec2 const & size );
		void doUpdate( std::chrono::microseconds const & duration )override;
		virtual void doResize( renderer::UIVec2 const & size )override;
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
		renderer::IVec2 m_previousMousePosition;
		renderer::IVec2 m_currentMousePosition;
		common::Camera m_camera;
		bool m_moveCamera{ false };
	};
}

#pragma once

#include "Prerequisites.hpp"

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

	private:
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
		void doUpdateMatrixUbo( renderer::UIVec2 const & size );

	private:
		renderer::UniformBufferPtr< common::SceneData > m_sceneUbo;
		renderer::UniformBufferPtr< common::ObjectData > m_objectUbo;
		renderer::Mat4 m_rotate;
	};
}

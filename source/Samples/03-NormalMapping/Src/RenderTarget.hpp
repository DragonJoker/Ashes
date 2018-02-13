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
			, common::Object && object
			, common::ImagePtrArray && images );

	private:
		void doUpdate( std::chrono::microseconds const & duration )override;
		virtual void doResize( renderer::UIVec2 const & size )override;
		common::OpaqueRenderingPtr doCreateOpaqueRendering( renderer::Device const & device
			, renderer::StagingBuffer & stagingBuffer
			, renderer::TextureViewCRefArray const & views
			, common::Object const & submeshes
			, common::TextureNodePtrArray const & textureNodes )override;
		common::TransparentRenderingPtr doCreateTransparentRendering( renderer::Device const & device
			, renderer::StagingBuffer & stagingBuffer
			, renderer::TextureViewCRefArray const & views
			, common::Object const & submeshes
			, common::TextureNodePtrArray const & textureNodes )override;
		void doUpdateMatrixUbo( renderer::UIVec2 const & size );
		void doInitialiseLights();

	private:
		renderer::UniformBufferPtr< renderer::Mat4 > m_matrixUbo;
		renderer::UniformBufferPtr< renderer::Mat4 > m_objectUbo;
		renderer::UniformBufferPtr< common::LightsData > m_lightsUbo;
		renderer::Mat4 m_rotate;
	};
}

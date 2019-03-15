#pragma once

#include "Prerequisites.hpp"

#include <RenderTarget.hpp>

namespace vkapp
{
	class RenderTarget
		: public common::RenderTarget
	{
	public:
		RenderTarget( ashes::Device const & device
			, ashes::CommandPool const & commandPool
			, ashes::Queue const & transferQueue
			, ashes::Extent2D const & size
			, common::Scene scene
			, common::ImagePtrArray images );

	private:
		void doUpdate( std::chrono::microseconds const & duration )override;
		virtual void doResize( ashes::Extent2D const & size )override;
		common::OpaqueRenderingPtr doCreateOpaqueRendering( ashes::Device const & device
			, ashes::StagingBuffer & stagingBuffer
			, ashes::TextureViewCRefArray const & views
			, common::Scene const & scene
			, common::TextureNodePtrArray const & textureNodes )override;
		common::TransparentRenderingPtr doCreateTransparentRendering( ashes::Device const & device
			, ashes::StagingBuffer & stagingBuffer
			, ashes::TextureViewCRefArray const & views
			, common::Scene const & scene
			, common::TextureNodePtrArray const & textureNodes )override;
		void doUpdateMatrixUbo( ashes::Extent2D const & size );

	private:
		ashes::UniformBufferPtr< common::SceneData > m_sceneUbo;
		ashes::UniformBufferPtr< common::ObjectData > m_objectUbo;
		utils::Mat4 m_rotate;
	};
}

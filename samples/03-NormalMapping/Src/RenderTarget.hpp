#pragma once

#include "Prerequisites.hpp"

#include <RenderTarget.hpp>

namespace vkapp
{
	class RenderTarget
		: public common::RenderTarget
	{
	public:
		RenderTarget( utils::Device const & device
			, ashes::CommandPool const & commandPool
			, ashes::Queue const & transferQueue
			, VkExtent2D const & size
			, common::Scene scene
			, common::ImagePtrArray images );

	private:
		void doUpdate( std::chrono::microseconds const & duration )override;
		virtual void doResize( VkExtent2D const & size )override;
		common::OpaqueRenderingPtr doCreateOpaqueRendering( utils::Device const & device
			, ashes::StagingBuffer & stagingBuffer
			, ashes::ImageViewPtrArray views
			, common::Scene const & scene
			, common::TextureNodePtrArray const & textureNodes )override;
		common::TransparentRenderingPtr doCreateTransparentRendering( utils::Device const & device
			, ashes::StagingBuffer & stagingBuffer
			, ashes::ImageViewPtrArray views
			, common::Scene const & scene
			, common::TextureNodePtrArray const & textureNodes )override;
		void doUpdateMatrixUbo( VkExtent2D const & size );
		void doInitialiseLights();

	private:
		ashes::UniformBufferPtr< common::SceneData > m_sceneUbo;
		ashes::UniformBufferPtr< common::ObjectData > m_objectUbo;
		ashes::UniformBufferPtr< common::LightsData > m_lightsUbo;
		utils::Mat4 m_rotate;
	};
}

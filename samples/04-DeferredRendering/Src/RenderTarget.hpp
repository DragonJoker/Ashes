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

		inline GeometryPassResult const & getGBuffer()const
		{
			return m_gbuffer;
		}

	private:
		void doUpdate( std::chrono::microseconds const & duration )override;
		void doResize( VkExtent2D const & size )override;
		common::OpaqueRenderingPtr doCreateOpaqueRendering( utils::Device const & device
			, ashes::StagingBuffer & stagingBuffer
			, ashes::ImageViewArray views
			, common::Scene const & scene
			, common::TextureNodePtrArray const & textureNodes )override;
		common::TransparentRenderingPtr doCreateTransparentRendering( utils::Device const & device
			, ashes::StagingBuffer & stagingBuffer
			, ashes::ImageViewArray views
			, common::Scene const & scene
			, common::TextureNodePtrArray const & textureNodes )override;
		void doUpdateMatrixUbo( VkExtent2D const & size );
		void doInitialiseLights();
		void doCreateGBuffer();

	private:
		ashes::UniformBufferPtr m_sceneUbo;
		std::vector< common::SceneData > m_sceneData{ 1u };
		ashes::UniformBufferPtr m_objectUbo;
		std::vector< common::ObjectData > m_objectData{ 1u };
		ashes::UniformBufferPtr m_lightsUbo;
		std::vector< common::LightsData > m_lightsData{ 1u };
		utils::Mat4 m_rotate;
		GeometryPassResult m_gbuffer;
	};
}

#pragma once

#include "Prerequisites.hpp"

#include <Camera.hpp>
#include <RenderTarget.hpp>

#include <Ashes/Miscellaneous/Offset2D.hpp>

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

		inline void enableMoveCamera( bool enable )
		{
			m_moveCamera = enable;
		}

		inline void updateMousePosition( VkOffset2D const & position )
		{
			m_currentMousePosition = position;
		}

	private:
		void doUpdateProjection( VkExtent2D const & size );
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

	private:
		ashes::UniformBufferPtr< common::SceneData > m_sceneUbo;
		VkOffset2D m_previousMousePosition;
		VkOffset2D m_currentMousePosition;
		common::Camera m_camera;
		bool m_moveCamera{ false };
	};
}

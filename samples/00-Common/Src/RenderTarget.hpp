#pragma once

#include "Scene.hpp"

#include <Ashes/Core/Device.hpp>
#include <Ashes/Miscellaneous/Extent2D.hpp>

namespace common
{
	class RenderTarget
	{
	public:
		RenderTarget( ashes::Device const & device
			, ashes::Extent2D const & size
			, Scene && scene
			, ImagePtrArray && images );
		virtual ~RenderTarget();
		void resize( ashes::Extent2D const & size );
		void update( std::chrono::microseconds const & duration );
		void draw( std::chrono::microseconds & gpu );

		inline ashes::TextureView const & getColourView()const
		{
			return *m_colourView;
		}

		inline ashes::TextureView const & getDepthView()const
		{
			return *m_depthView;
		}

	protected:
		void doInitialise();

		inline OpaqueRendering const & getOpaqueRendering()const
		{
			assert( m_opaque );
			return *m_opaque;
		}

		inline TransparentRendering const & getTransparentRendering()const
		{
			assert( m_transparent );
			return *m_transparent;
		}

	private:
		void doCleanup();
		void doCreateStagingBuffer();
		void doCreateTextures();
		void doCreateRenderPass();
		void doUpdateRenderViews();

		virtual void doUpdate( std::chrono::microseconds const & duration ) = 0;
		virtual void doResize( ashes::Extent2D const & size ) = 0;

		virtual OpaqueRenderingPtr doCreateOpaqueRendering( ashes::Device const & device
			, ashes::StagingBuffer & stagingBuffer
			, ashes::TextureViewCRefArray const & views
			, Scene const & scene
			, TextureNodePtrArray const & textureNodes ) = 0;
		virtual TransparentRenderingPtr doCreateTransparentRendering( ashes::Device const & device
			, ashes::StagingBuffer & stagingBuffer
			, ashes::TextureViewCRefArray const & views
			, Scene const & scene
			, TextureNodePtrArray const & textureNodes ) = 0;

	protected:
		ashes::Device const & m_device;
		ashes::StagingBufferPtr m_stagingBuffer;
		ashes::CommandBufferPtr m_updateCommandBuffer;
		ashes::Extent2D m_size;

	private:
		ImagePtrArray m_images;
		Scene m_scene;
		TextureNodePtrArray m_textureNodes;
		utils::Mat4 m_rotate;
		ashes::TexturePtr m_colour;
		ashes::TextureViewPtr m_colourView;
		ashes::TexturePtr m_depth;
		ashes::TextureViewPtr m_depthView;
		ashes::CommandBufferPtr m_commandBuffer;
		std::shared_ptr< OpaqueRendering > m_opaque;
		std::shared_ptr< TransparentRendering > m_transparent;
	};
}

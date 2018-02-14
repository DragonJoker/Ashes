#pragma once

#include "Scene.hpp"

#include <Core/Device.hpp>

namespace common
{
	class RenderTarget
	{
	public:
		RenderTarget( renderer::Device const & device
			, renderer::UIVec2 const & size
			, Scene && scene
			, ImagePtrArray && images );
		~RenderTarget();
		void resize( renderer::UIVec2 const & size );
		void update( std::chrono::microseconds const & duration );
		bool draw( std::chrono::microseconds & gpu );

		inline renderer::TextureView const & getColourView()const
		{
			return *m_colourView;
		}

		inline renderer::TextureView const & getDepthView()const
		{
			return *m_depthView;
		}

	protected:
		void doInitialise();

	private:
		void doCleanup();
		void doCreateStagingBuffer();
		void doCreateTextures();
		void doCreateRenderPass();
		void doUpdateRenderViews();

		virtual void doUpdate( std::chrono::microseconds const & duration ) = 0;
		virtual void doResize( renderer::UIVec2 const & size ) = 0;

		virtual OpaqueRenderingPtr doCreateOpaqueRendering( renderer::Device const & device
			, renderer::StagingBuffer & stagingBuffer
			, renderer::TextureViewCRefArray const & views
			, Scene const & scene
			, TextureNodePtrArray const & textureNodes ) = 0;
		virtual TransparentRenderingPtr doCreateTransparentRendering( renderer::Device const & device
			, renderer::StagingBuffer & stagingBuffer
			, renderer::TextureViewCRefArray const & views
			, Scene const & scene
			, TextureNodePtrArray const & textureNodes ) = 0;

	protected:
		renderer::Device const & m_device;
		renderer::StagingBufferPtr m_stagingBuffer;
		renderer::CommandBufferPtr m_updateCommandBuffer;

	private:
		renderer::UIVec2 m_size;
		ImagePtrArray m_images;
		Scene m_scene;
		TextureNodePtrArray m_textureNodes;
		renderer::Mat4 m_rotate;
		renderer::TexturePtr m_colour;
		renderer::TextureViewPtr m_colourView;
		renderer::TexturePtr m_depth;
		renderer::TextureViewPtr m_depthView;
		renderer::CommandBufferPtr m_commandBuffer;
		std::shared_ptr< OpaqueRendering > m_opaque;
		std::shared_ptr< TransparentRendering > m_transparent;
	};
}

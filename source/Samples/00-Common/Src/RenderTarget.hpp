#pragma once

#include "Prerequisites.hpp"

#include <Core/Device.hpp>

namespace common
{
	class RenderTarget
	{
	public:
		RenderTarget( renderer::Device const & device
			, renderer::UIVec2 const & size
			, Object && object
			, ImagePtrArray && images );
		~RenderTarget();
		void resize( renderer::UIVec2 const & size );
		void update();
		bool draw( std::chrono::microseconds & gpu );

		inline renderer::TextureView const & getColourView()const
		{
			return *m_colourView;
		}

	protected:
		void doInitialise();

	private:
		void doCleanup();
		void doCreateStagingBuffer();
		void doCreateUniformBuffer();
		void doCreateTextures();
		void doCreateRenderPass();
		void doUpdateMatrixUbo();
		void doUpdateRenderViews();

		virtual void doCreateOpaqueRendering() = 0;
		virtual void doCreateTransparentRendering() = 0;

	private:
		renderer::Device const & m_device;
		renderer::UIVec2 m_size;
		ImagePtrArray m_images;
		Object m_object;
		TextureNodePtrArray m_textureNodes;
		renderer::Mat4 m_rotate;
		renderer::StagingBufferPtr m_stagingBuffer;
		renderer::TexturePtr m_colour;
		renderer::TextureViewPtr m_colourView;
		renderer::TexturePtr m_depth;
		renderer::TextureViewPtr m_depthView;
		renderer::UniformBufferPtr< renderer::Mat4 > m_matrixUbo;
		renderer::UniformBufferPtr< renderer::Mat4 > m_objectUbo;
		renderer::CommandBufferPtr m_updateCommandBuffer;
		renderer::CommandBufferPtr m_commandBuffer;
		std::shared_ptr< OpaqueRendering > m_opaque;
		std::shared_ptr< TransparentRendering > m_transparent;
	};
}

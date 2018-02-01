#pragma once

#include "TransparentRendering.hpp"

#include <Core/Device.hpp>

namespace vkapp
{
	class RenderTarget
	{
	public:
		RenderTarget( renderer::Device const & device
			, renderer::UniformBuffer< common::LightsData > const & lightsUbo
			, renderer::UIVec2 const & size
			, common::Object && object );
		~RenderTarget();
		void resize( renderer::UIVec2 const & size );
		void update();
		void draw();

		inline renderer::TextureView const & getColourView()const
		{
			return *m_colourView;
		}

	private:
		void doCleanup();
		void doCreateStagingBuffer();
		void doCreateUniformBuffer();
		void doCreateRenderPass();
		void doCreateFrameBuffer();
		void doPrepareFrame();

	private:
		renderer::Device const & m_device;
		renderer::UniformBuffer< common::LightsData > const & m_lightsUbo;
		renderer::UIVec2 m_size;
		common::Object m_object;
		renderer::Mat4 m_rotate;
		renderer::StagingBufferPtr m_stagingBuffer;
		renderer::TexturePtr m_colour;
		renderer::TextureViewPtr m_colourView;
		renderer::TexturePtr m_depth;
		renderer::TextureViewPtr m_depthView;
		renderer::RenderPassPtr m_renderPass;
		renderer::FrameBufferPtr m_frameBuffer;
		renderer::UniformBufferPtr< renderer::Mat4 > m_matrixUbo;
		renderer::UniformBufferPtr< renderer::Mat4 > m_objectUbo;
		renderer::CommandBufferPtr m_updateCommandBuffer;
		renderer::CommandBufferPtr m_commandBuffer;
	};
}

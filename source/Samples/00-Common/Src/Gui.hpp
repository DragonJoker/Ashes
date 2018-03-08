#pragma once

#include "Prerequisites.hpp"
#include "imgui.h"

#include <Buffer/PushConstantsBuffer.hpp>

namespace common
{
	class Gui
	{
	public:
		Gui( renderer::Device const & device
			, renderer::Extent2D const & size );
		void updateView( renderer::TextureView const & colourView );
		void update();
		void resize( renderer::Extent2D const & size );
		void submit( renderer::Queue const & queue );

		bool header( char const * caption );
		bool checkBox( char const * caption, bool * value );
		bool checkBox( char const * caption, int32_t * value );
		bool inputFloat( char const * caption, float * value, float step, uint32_t precision );
		bool sliderFloat( char const * caption, float * value, float min, float max );
		bool sliderInt( char const * caption, int32_t * value, int32_t min, int32_t max );
		bool comboBox( char const * caption, int32_t * itemindex, std::vector< std::string > items );
		bool button( char const * caption );
		void text( char const * formatstr, ... );

		inline renderer::TextureView const & getTargetView()const
		{
			return *m_targetView;
		}

	private:
		void doPrepareResources();
		void doPreparePipeline();
		void doUpdateCommandBuffers();

	private:
		struct PushConstBlock
		{
			renderer::Vec2 scale;
			renderer::Vec2 translate;
		};

		renderer::Device const & m_device;
		renderer::TextureView const * m_colourView{ nullptr };
		renderer::Extent2D m_size;
		renderer::PushConstantsBuffer< PushConstBlock > m_pushConstants;
		renderer::VertexBufferPtr< ImDrawVert > m_vertexBuffer;
		renderer::BufferPtr< ImDrawIdx > m_indexBuffer;
		renderer::TexturePtr m_target;
		renderer::TextureViewPtr m_targetView;
		uint32_t m_vertexCount = 0;
		uint32_t m_indexCount = 0;

		renderer::DescriptorSetLayoutPtr m_descriptorSetLayout;
		renderer::DescriptorSetPoolPtr m_descriptorPool;
		renderer::DescriptorSetPtr m_descriptorSet;
		renderer::PipelineLayoutPtr m_pipelineLayout;
		renderer::RenderPassPtr m_renderPass;
		renderer::FrameBufferPtr m_frameBuffer;
		renderer::VertexLayoutPtr m_vertexLayout;
		renderer::PipelinePtr m_pipeline;
		renderer::CommandPoolPtr m_commandPool;
		renderer::CommandBufferPtr m_commandBuffer;
		renderer::FencePtr m_fence;

		renderer::TexturePtr m_fontImage;
		renderer::TextureViewPtr m_fontView;
		renderer::SamplerPtr m_sampler;
	};
}

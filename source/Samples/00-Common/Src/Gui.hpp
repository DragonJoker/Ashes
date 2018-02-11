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
			, renderer::UIVec2 const & size
			, renderer::PixelFormat colourFormat
			, renderer::PixelFormat depthFormat
			, std::vector< renderer::FrameBufferPtr > const & frameBuffers );
		void update();
		void resize( renderer::UIVec2 const & size );
		void submit( renderer::Queue & queue
			, uint32_t bufferindex );

		bool header( const char* caption );
		bool checkBox( const char* caption, bool* value );
		bool checkBox( const char* caption, int32_t* value );
		bool inputFloat( const char* caption, float* value, float step, uint32_t precision );
		bool sliderFloat( const char* caption, float* value, float min, float max );
		bool sliderInt( const char* caption, int32_t* value, int32_t min, int32_t max );
		bool comboBox( const char* caption, int32_t* itemindex, std::vector<std::string> items );
		bool button( const char* caption );
		void text( const char* formatstr, ... );

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
		std::vector< renderer::FrameBufferPtr > const & m_frameBuffers;
		renderer::UIVec2 m_size;
		renderer::PushConstantsBuffer< PushConstBlock > m_pushConstants;
		std::vector< renderer::CommandBufferPtr > m_cmdBuffers;
		renderer::VertexBufferPtr< ImDrawVert > m_vertexBuffer;
		renderer::BufferPtr< ImDrawIdx > m_indexBuffer;
		renderer::GeometryBuffersPtr m_geometryBuffers;
		uint32_t m_vertexCount = 0;
		uint32_t m_indexCount = 0;
		renderer::PixelFormat m_colourFormat;
		renderer::PixelFormat m_depthFormat;

		renderer::DescriptorSetLayoutPtr m_descriptorSetLayout;
		renderer::DescriptorSetPoolPtr m_descriptorPool;
		renderer::DescriptorSetPtr m_descriptorSet;
		renderer::PipelineLayoutPtr m_pipelineLayout;
		renderer::RenderPassPtr m_renderPass;
		renderer::VertexLayoutPtr m_vertexLayout;
		renderer::ShaderProgramPtr m_program;
		renderer::PipelinePtr m_pipeline;
		renderer::CommandPoolPtr m_commandPool;
		renderer::FencePtr m_fence;

		renderer::TexturePtr m_fontImage;
		renderer::TextureViewPtr m_fontView;
		renderer::SamplerPtr m_sampler;
	};
}

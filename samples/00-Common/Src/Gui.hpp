#pragma once

#include "Prerequisites.hpp"
#include "imgui.h"

#include <Ashes/Buffer/PushConstantsBuffer.hpp>
#include <Ashes/Image/Sampler.hpp>

namespace common
{
	class Gui
	{
	public:
		Gui( utils::Device const & device
			, ashes::Queue const & queue
			, ashes::CommandPool const & commandPool
			, ashes::Extent2D const & size );
		void updateView( ashes::TextureView const & colourView );
		void update();
		void resize( ashes::Extent2D const & size );
		void submit( ashes::Queue const & queue );

		bool header( char const * caption );
		bool checkBox( char const * caption, bool * value );
		bool checkBox( char const * caption, int32_t * value );
		bool inputFloat( char const * caption, float * value, float step, uint32_t precision );
		bool sliderFloat( char const * caption, float * value, float min, float max );
		bool sliderInt( char const * caption, int32_t * value, int32_t min, int32_t max );
		bool comboBox( char const * caption, int32_t * itemindex, std::vector< std::string > items );
		bool button( char const * caption );
		void text( char const * formatstr, ... );

		inline ashes::TextureView const & getTargetView()const
		{
			return *m_targetView;
		}

	private:
		void doPrepareResources( ashes::Queue const & queue
			, ashes::CommandPool const & commandPool );
		void doPreparePipeline();
		void doUpdateCommandBuffers();

	private:
		struct PushConstBlock
		{
			utils::Vec2 scale;
			utils::Vec2 translate;
		};

		utils::Device const & m_device;
		ashes::TextureView const * m_colourView{ nullptr };
		ashes::Extent2D m_size;
		ashes::PushConstantsBuffer< PushConstBlock > m_pushConstants;
		ashes::VertexBufferPtr< ImDrawVert > m_vertexBuffer;
		ashes::BufferPtr< ImDrawIdx > m_indexBuffer;
		ashes::TexturePtr m_target;
		ashes::TextureViewPtr m_targetView;
		uint32_t m_vertexCount = 0;
		uint32_t m_indexCount = 0;

		ashes::DescriptorSetLayoutPtr m_descriptorSetLayout;
		ashes::DescriptorSetPoolPtr m_descriptorPool;
		ashes::DescriptorSetPtr m_descriptorSet;
		ashes::PipelineLayoutPtr m_pipelineLayout;
		ashes::RenderPassPtr m_renderPass;
		ashes::FrameBufferPtr m_frameBuffer;
		ashes::VertexLayoutPtr m_vertexLayout;
		ashes::PipelinePtr m_pipeline;
		ashes::CommandPoolPtr m_commandPool;
		ashes::CommandBufferPtr m_commandBuffer;
		ashes::FencePtr m_fence;

		ashes::TexturePtr m_fontImage;
		ashes::TextureViewPtr m_fontView;
		ashes::SamplerPtr m_sampler;
	};
}

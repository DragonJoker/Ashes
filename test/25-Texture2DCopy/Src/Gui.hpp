#pragma once

#include "Prerequisites.hpp"
#include "imgui.h"

#include <ashespp/Buffer/PushConstantsBuffer.hpp>
#include <ashespp/Image/Sampler.hpp>

namespace vkapp
{
	class Gui
	{
	public:
		Gui( utils::Device const & device
			, ashes::Queue const & queue
			, ashes::CommandPool const & commandPool
			, VkExtent2D const & size );
		~Gui();
		void initialise( ashes::RenderPass const & renderPass );
		//\return \p true if the command buffers needs be rewritten.
		bool update();

		void preUpdateCommandBuffer( ashes::CommandBuffer const & commandBuffer );
		void updateCommandBuffer( ashes::CommandBuffer const & commandBuffer );
		void postUpdateCommandBuffer( ashes::CommandBuffer const & commandBuffer );

		bool header( char const * caption );
		bool checkBox( char const * caption, bool * value );
		bool checkBox( char const * caption, int32_t * value );
		bool inputFloat( char const * caption, float * value, float step, uint32_t precision );
		bool sliderFloat( char const * caption, float * value, float min, float max );
		bool sliderInt( char const * caption, int32_t * value, int32_t min, int32_t max );
		bool comboBox( char const * caption, int32_t * itemindex, std::vector< std::string > items );
		bool button( char const * caption );
		void text( char const * formatstr, ... );

	private:
		void doPrepareResources( ashes::Queue const & queue
			, ashes::CommandPool const & commandPool );
		void doPreparePipeline();

	private:
		struct PushConstBlock
		{
			utils::Vec2 scale;
			utils::Vec2 translate;
		};

		utils::Device const & m_device;
		VkExtent2D m_size;
		ashes::PushConstantsBuffer< PushConstBlock > m_pushConstants;
		ashes::VertexBufferPtr< ImDrawVert > m_vertexBuffer;
		ashes::BufferPtr< ImDrawIdx > m_indexBuffer;
		uint32_t m_vertexCount = 0;
		uint32_t m_indexCount = 0;
		ashes::RenderPass const * m_renderPass;

		ashes::DescriptorSetLayoutPtr m_descriptorSetLayout;
		ashes::DescriptorSetPoolPtr m_descriptorPool;
		ashes::DescriptorSetPtr m_descriptorSet;
		ashes::PipelineLayoutPtr m_pipelineLayout;
		ashes::GraphicsPipelinePtr m_pipeline;

		ashes::ImagePtr m_fontImage;
		ashes::ImageView m_fontView;
		ashes::SamplerPtr m_sampler;

		ImGuiContext * m_context;
	};
}

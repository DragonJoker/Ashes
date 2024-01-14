#pragma once

#include "Prerequisites.hpp"

namespace vkapp
{
	class LightingPass
	{
	public:
		LightingPass( utils::Device const & device
			, ashes::CommandPool const & commandPool
			, ashes::Queue const & transferQueue
			, ashes::UniformBuffer const & lightsUbo
			, ashes::StagingBuffer const & stagingBuffer
			, ashes::ImageViewArray views );
		void update( common::SceneData const & sceneData
			, ashes::StagingBuffer const & stagingBuffer
			, ashes::ImageViewArray views
			, GeometryPassResult const & geometryBuffers );
		void draw( ashes::Queue const & queue
			, std::chrono::nanoseconds & gpu )const;

	private:
		utils::Device const & m_device;
		ashes::CommandPool const & m_commandPool;
		ashes::Queue const & m_transferQueue;
		ashes::UniformBuffer const & m_lightsUbo;
		ashes::ImageView m_colourView;
		ashes::ImageView m_depthView;
		GeometryPassResult const * m_geometryBuffers{ nullptr };

		ashes::CommandBufferPtr m_commandBuffer;
		ashes::UniformBufferPtr m_sceneUbo;
		std::vector< common::SceneData > m_sceneData{ 1u };
		ashes::DescriptorSetLayoutPtr m_uboDescriptorLayout;
		ashes::DescriptorSetPoolPtr m_uboDescriptorPool;
		ashes::DescriptorSetPtr m_uboDescriptorSet;
		ashes::DescriptorSetLayoutPtr m_gbufferDescriptorLayout;
		ashes::DescriptorSetPoolPtr m_gbufferDescriptorPool;
		ashes::DescriptorSetPtr m_gbufferDescriptorSet;
		ashes::RenderPassPtr m_renderPass;
		ashes::SamplerPtr m_sampler;
		ashes::VertexBufferPtr< common::TexturedVertexData > m_vertexBuffer;
		ashes::PipelineLayoutPtr m_pipelineLayout;
		ashes::GraphicsPipelinePtr m_pipeline;
		ashes::FrameBufferPtr m_frameBuffer;
		ashes::QueryPoolPtr m_queryPool;
	};
}

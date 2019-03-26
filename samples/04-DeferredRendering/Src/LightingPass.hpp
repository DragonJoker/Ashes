#pragma once

#include "Prerequisites.hpp"

#include <Ashes/Pipeline/VertexLayout.hpp>

namespace vkapp
{
	class LightingPass
	{
	public:
		LightingPass( utils::Device const & device
			, ashes::CommandPool const & commandPool
			, ashes::Queue const & transferQueue
			, ashes::UniformBuffer< common::LightsData > const & lightsUbo
			, ashes::StagingBuffer & stagingBuffer
			, ashes::ImageViewPtrArray views );
		void update( common::SceneData const & sceneData
			, ashes::StagingBuffer & stagingBuffer
			, ashes::ImageViewPtrArray views
			, GeometryPassResult const & geometryBuffers );
		void draw( ashes::Queue const & queue
			, std::chrono::nanoseconds & gpu )const;

	private:
		utils::Device const & m_device;
		ashes::CommandPool const & m_commandPool;
		ashes::Queue const & m_transferQueue;
		ashes::UniformBuffer< common::LightsData > const & m_lightsUbo;
		ashes::ImageViewPtr m_colourView;
		ashes::ImageViewPtr m_depthView;
		GeometryPassResult const * m_geometryBuffers{ nullptr };

		ashes::CommandBufferPtr m_commandBuffer;
		ashes::UniformBufferPtr< common::SceneData > m_sceneUbo;
		ashes::DescriptorSetLayoutPtr m_uboDescriptorLayout;
		ashes::DescriptorSetPoolPtr m_uboDescriptorPool;
		ashes::DescriptorSetPtr m_uboDescriptorSet;
		ashes::DescriptorSetLayoutPtr m_gbufferDescriptorLayout;
		ashes::DescriptorSetPoolPtr m_gbufferDescriptorPool;
		ashes::DescriptorSetPtr m_gbufferDescriptorSet;
		ashes::RenderPassPtr m_renderPass;
		ashes::SamplerPtr m_sampler;
		ashes::VertexBufferPtr< common::TexturedVertexData > m_vertexBuffer;
		ashes::VertexLayoutPtr m_vertexLayout;
		ashes::PipelineLayoutPtr m_pipelineLayout;
		ashes::PipelinePtr m_pipeline;
		ashes::FrameBufferPtr m_frameBuffer;
		ashes::QueryPoolPtr m_queryPool;
	};
}

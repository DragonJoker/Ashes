#pragma once

#include "Prerequisites.hpp"

#include <Pipeline/VertexLayout.hpp>

namespace vkapp
{
	class LightingPass
	{
	public:
		LightingPass( ashes::Device const & device
			, ashes::UniformBuffer< common::LightsData > const & lightsUbo
			, ashes::StagingBuffer & stagingBuffer
			, ashes::TextureViewCRefArray const & views );
		void update( common::SceneData const & sceneData
			, ashes::StagingBuffer & stagingBuffer
			, ashes::TextureViewCRefArray const & views
			, GeometryPassResult const & geometryBuffers );
		void draw( std::chrono::nanoseconds & gpu )const;

	private:
		ashes::Device const & m_device;
		ashes::UniformBuffer< common::LightsData > const & m_lightsUbo;
		ashes::TextureView const * m_colourView{ nullptr };
		ashes::TextureView const * m_depthView{ nullptr };
		GeometryPassResult const * m_geometryBuffers{ nullptr };

		ashes::CommandBufferPtr m_updateCommandBuffer;
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

#pragma once

#include "Prerequisites.hpp"

#include <Pipeline/VertexLayout.hpp>

namespace vkapp
{
	class LightingPass
	{
	public:
		LightingPass( renderer::Device const & device
			, renderer::UniformBuffer< common::LightsData > const & lightsUbo
			, renderer::StagingBuffer & stagingBuffer
			, renderer::TextureViewCRefArray const & views );
		void update( common::SceneData const & sceneData
			, renderer::StagingBuffer & stagingBuffer
			, renderer::TextureViewCRefArray const & views
			, GeometryPassResult const & geometryBuffers );
		bool draw( std::chrono::nanoseconds & gpu )const;

	private:
		renderer::Device const & m_device;
		renderer::UniformBuffer< common::LightsData > const & m_lightsUbo;
		renderer::TextureView const * m_colourView{ nullptr };
		renderer::TextureView const * m_depthView{ nullptr };
		GeometryPassResult const * m_geometryBuffers{ nullptr };

		renderer::CommandBufferPtr m_updateCommandBuffer;
		renderer::CommandBufferPtr m_commandBuffer;
		renderer::UniformBufferPtr< common::SceneData > m_sceneUbo;
		renderer::DescriptorSetLayoutPtr m_uboDescriptorLayout;
		renderer::DescriptorSetPoolPtr m_uboDescriptorPool;
		renderer::DescriptorSetPtr m_uboDescriptorSet;
		renderer::DescriptorSetLayoutPtr m_gbufferDescriptorLayout;
		renderer::DescriptorSetPoolPtr m_gbufferDescriptorPool;
		renderer::DescriptorSetPtr m_gbufferDescriptorSet;
		renderer::ShaderProgramPtr m_program;
		renderer::RenderPassPtr m_renderPass;
		renderer::SamplerPtr m_sampler;
		renderer::VertexBufferPtr< common::TexturedVertexData > m_vertexBuffer;
		renderer::VertexLayoutPtr m_vertexLayout;
		renderer::PipelineLayoutPtr m_pipelineLayout;
		renderer::PipelinePtr m_pipeline;
		renderer::FrameBufferPtr m_frameBuffer;
		renderer::QueryPoolPtr m_queryPool;
	};
}

#pragma once

#include "Prerequisites.hpp"

#include <Buffer/GeometryBuffers.hpp>
#include <Pipeline/VertexLayout.hpp>

namespace vkapp
{
	class LightingPass
	{
	public:
		LightingPass( renderer::Device const & device
			, renderer::UniformBuffer< common::LightsData > const & lightsUbo
			, renderer::StagingBuffer & stagingBuffer
			, renderer::TextureView const & depthView
			, renderer::TextureView const & colourView );
		void update( renderer::Mat4 const & viewProj
			, renderer::StagingBuffer & stagingBuffer
			, renderer::TextureView const & colourView
			, renderer::TextureView const & depthView
			, GeometryPassResult const & geometryBuffers );
		bool draw()const;

	private:
		renderer::Device const & m_device;
		renderer::UniformBuffer< common::LightsData > const & m_lightsUbo;
		renderer::TextureView const * m_colourView{ nullptr };
		renderer::TextureView const * m_depthView{ nullptr };
		GeometryPassResult const * m_geometryBuffers{ nullptr };

		renderer::CommandBufferPtr m_updateCommandBuffer;
		renderer::CommandBufferPtr m_commandBuffer;
		renderer::UniformBufferPtr< renderer::Mat4 > m_matrixUbo;
		renderer::DescriptorSetLayoutPtr m_uboDescriptorLayout;
		renderer::DescriptorSetPoolPtr m_uboDescriptorPool;
		renderer::DescriptorSetPtr m_uboDescriptorSet;
		renderer::DescriptorSetLayoutPtr m_gbufferDescriptorLayout;
		renderer::DescriptorSetPoolPtr m_gbufferDescriptorPool;
		renderer::DescriptorSetPtr m_gbufferDescriptorSet;
		renderer::ShaderProgramPtr m_program;
		renderer::RenderPassPtr m_renderPass;
		renderer::SamplerPtr m_sampler;
		renderer::VertexBufferPtr< TexturedVertexData > m_vertexBuffer;
		renderer::VertexLayoutPtr m_vertexLayout;
		renderer::GeometryBuffersPtr m_vao;
		renderer::PipelineLayoutPtr m_pipelineLayout;
		renderer::PipelinePtr m_pipeline;
		renderer::FrameBufferPtr m_frameBuffer;
	};
}

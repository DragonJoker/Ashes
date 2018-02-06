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
			, renderer::UniformBuffer< renderer::Mat4 > const & matrixUbo
			, renderer::UniformBuffer< common::LightsData > const & lightsUbo
			, renderer::StagingBuffer & stagingBuffer
			, renderer::TextureView const & colourView );
		void update( renderer::TextureView const & colourView
			, renderer::TextureView const & depthView
			, GeometryPassResult const & geometryBuffers );
		void draw()const;

	private:
		renderer::Device const & m_device;
		renderer::UniformBuffer< renderer::Mat4 > const & m_matrixUbo;
		renderer::UniformBuffer< common::LightsData > const & m_lightsUbo;
		renderer::TextureView const * m_colourView{ nullptr };
		renderer::TextureView const * m_depthView{ nullptr };
		GeometryPassResult const * m_geometryBuffers{ nullptr };

		renderer::CommandBufferPtr m_updateCommandBuffer;
		renderer::CommandBufferPtr m_commandBuffer;
		renderer::DescriptorSetLayoutPtr m_descriptorLayout;
		renderer::DescriptorSetPoolPtr m_descriptorPool;
		renderer::DescriptorSetPtr m_descriptorSet;
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

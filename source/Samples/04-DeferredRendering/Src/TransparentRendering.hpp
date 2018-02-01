#pragma once

#include "Prerequisites.hpp"

namespace vkapp
{
	class TransparentRendering
	{
	public:
		TransparentRendering( renderer::Device const & device
			, common::Object const & submeshes
			, renderer::UniformBuffer< renderer::Mat4 > const & matrixUbo
			, renderer::UniformBuffer< renderer::Mat4 > const & objectUbo
			, renderer::UniformBuffer< common::LightsData > const & lightsUbo
			, renderer::RenderPass const & renderPass
			, renderer::UIVec2 const & size );
		void update( renderer::UIVec2 const & size );

		renderer::CommandBuffer const & getCommandBuffer()const
		{
			return *m_commandBuffer;
		}

	private:
		renderer::Device const & m_device;
		renderer::UniformBuffer< renderer::Mat4 > const & m_matrixUbo;
		renderer::UniformBuffer< renderer::Mat4 > const & m_objectUbo;
		renderer::UniformBuffer< common::LightsData > const & m_lightsUbo;
		common::SubmeshNodes m_submeshNodes;
		common::ObjectNodes m_renderNodes;
		renderer::SamplerPtr m_sampler;
		renderer::StagingBufferPtr m_stagingBuffer;
		renderer::CommandBufferPtr m_commandBuffer;
		renderer::UniformBufferPtr< common::MaterialData > m_materialsUbo;
		renderer::DescriptorSetLayoutPtr m_descriptorLayout;
		renderer::DescriptorSetPoolPtr m_descriptorPool;
		renderer::ShaderProgramPtr m_program;
		renderer::UIVec2 m_size;
		uint32_t m_nodesCount;
	};
}

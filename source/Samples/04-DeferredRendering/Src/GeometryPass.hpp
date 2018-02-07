#pragma once

#include "Prerequisites.hpp"

namespace vkapp
{
	class GeometryPass
	{
	public:
		GeometryPass( renderer::Device const & device
			, common::Object const & submeshes
			, renderer::UniformBuffer< renderer::Mat4 > const & matrixUbo
			, renderer::UniformBuffer< renderer::Mat4 > const & objectUbo
			, renderer::StagingBuffer & stagingBuffer
			, renderer::TextureView const & depthView
			, common::TextureNodePtrArray const & textureNodes );
		void update( renderer::TextureView const & depthView );
		bool draw()const;

		inline GeometryPassResult const & getResult()
		{
			return m_result;
		}

	private:
		void doCreateResult();

	private:
		renderer::Device const & m_device;
		renderer::UniformBuffer< renderer::Mat4 > const & m_matrixUbo;
		renderer::UniformBuffer< renderer::Mat4 > const & m_objectUbo;
		renderer::SamplerPtr m_sampler;
		renderer::CommandBufferPtr m_updateCommandBuffer;
		renderer::CommandBufferPtr m_commandBuffer;
		renderer::UniformBufferPtr< common::MaterialData > m_materialsUbo;
		renderer::DescriptorSetLayoutPtr m_descriptorLayout;
		renderer::DescriptorSetPoolPtr m_descriptorPool;
		renderer::ShaderProgramPtr m_program;
		GeometryPassResult m_result;
		renderer::RenderPassPtr m_renderPass;
		renderer::UIVec2 m_size;
		renderer::FrameBufferPtr m_frameBuffer;
		common::SubmeshNodes m_submeshNodes;
		common::ObjectNodes m_renderNodes;
		uint32_t m_nodesCount;
	};
}

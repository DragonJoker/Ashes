#pragma once

#include "Prerequisites.hpp"

namespace vkapp
{
	class OpaqueRendering
	{
	public:
		OpaqueRendering( renderer::Device const & device
			, common::Object const & submeshes
			, renderer::UniformBuffer< renderer::Mat4 > const & matrixUbo
			, renderer::UniformBuffer< renderer::Mat4 > const & objectUbo
			, renderer::UniformBuffer< common::LightsData > const & lightsUbo
			, renderer::StagingBuffer & stagingBuffer
			, renderer::TextureView const & colourView
			, renderer::TextureView const & depthView
			, common::TextureNodePtrArray const & textureNodes );
		void update( renderer::TextureView const & colourView
			, renderer::TextureView const & depthView );
		void draw()const;

	private:
		renderer::Device const & m_device;
		renderer::UniformBuffer< renderer::Mat4 > const & m_matrixUbo;
		renderer::UniformBuffer< renderer::Mat4 > const & m_objectUbo;
		renderer::UniformBuffer< common::LightsData > const & m_lightsUbo;
		renderer::TextureView const * m_colourView{ nullptr };
		renderer::TextureView const * m_depthView{ nullptr };
		renderer::TexturePtr m_diffuse{ nullptr };
		renderer::TextureViewPtr m_diffuseView{ nullptr };
		renderer::TexturePtr m_specular{ nullptr };
		renderer::TextureViewPtr m_specularView{ nullptr };
		renderer::TexturePtr m_emissive{ nullptr };
		renderer::TextureViewPtr m_emissiveView{ nullptr };
		renderer::TexturePtr m_normal{ nullptr };
		renderer::TextureViewPtr m_normalView{ nullptr };
		renderer::SamplerPtr m_sampler;
		renderer::CommandBufferPtr m_updateCommandBuffer;
		renderer::CommandBufferPtr m_commandBuffer;
		renderer::UniformBufferPtr< common::MaterialData > m_materialsUbo;
		renderer::DescriptorSetLayoutPtr m_descriptorLayout;
		renderer::DescriptorSetPoolPtr m_descriptorPool;
		renderer::ShaderProgramPtr m_program;
		renderer::RenderPassPtr m_renderPass;
		renderer::FrameBufferPtr m_frameBuffer;
		common::SubmeshNodes m_submeshNodes;
		common::ObjectNodes m_renderNodes;
		uint32_t m_nodesCount;
	};
}

#pragma once

#include "Prerequisites.hpp"

#include <Buffer/UniformBuffer.hpp>
#include <Command/CommandBuffer.hpp>
#include <Descriptor/DescriptorSet.hpp>
#include <Descriptor/DescriptorSetLayout.hpp>
#include <Descriptor/DescriptorSetPool.hpp>
#include <Image/Sampler.hpp>
#include <Miscellaneous/QueryPool.hpp>
#include <Pipeline/Pipeline.hpp>
#include <Pipeline/PipelineLayout.hpp>
#include <RenderPass/RenderPass.hpp>
#include <RenderPass/FrameBuffer.hpp>
#include <RenderPass/RenderPass.hpp>
#include <Shader/ShaderProgram.hpp>

namespace common
{
	class NodesRenderer
	{
	public:
		NodesRenderer( renderer::Device const & device
			, renderer::ShaderProgramPtr && program
			, std::vector< renderer::PixelFormat > const & formats
			, bool clearViews
			, bool opaqueNodes );
		void update( renderer::TextureViewCRefArray const & views );
		bool draw( std::chrono::nanoseconds & gpu )const;
		void initialise( Object const & submeshes
			, renderer::StagingBuffer & stagingBuffer
			, renderer::TextureViewCRefArray const & views
			, common::TextureNodePtrArray const & textureNodes );

	private:
		virtual void doFillDescriptorLayoutBindings( renderer::DescriptorSetLayoutBindingArray & bindings ) = 0;
		virtual void doFillDescriptorSet( renderer::DescriptorSetLayout & descriptorLayout
			, renderer::DescriptorSet & descriptorSet ) = 0;

	protected:
		renderer::Device const & m_device;
		bool m_opaqueNodes;
		std::vector< renderer::TextureView const * > m_views;
		renderer::SamplerPtr m_sampler;
		renderer::CommandBufferPtr m_updateCommandBuffer;
		renderer::CommandBufferPtr m_commandBuffer;
		renderer::UniformBufferPtr< MaterialData > m_materialsUbo;
		renderer::DescriptorSetLayoutPtr m_descriptorLayout;
		renderer::DescriptorSetPoolPtr m_descriptorPool;
		renderer::ShaderProgramPtr m_program;
		renderer::RenderPassPtr m_renderPass;
		renderer::FrameBufferPtr m_frameBuffer;
		renderer::QueryPoolPtr m_queryPool;
		SubmeshNodes m_submeshNodes;
		ObjectNodes m_renderNodes;
		uint32_t m_nodesCount;
	};
}

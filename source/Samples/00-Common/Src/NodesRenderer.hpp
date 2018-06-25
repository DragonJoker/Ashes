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
			, std::string const & fragmentShaderFile
			, std::vector< renderer::Format > const & formats
			, bool clearViews
			, bool opaqueNodes );
		virtual ~NodesRenderer() = default;
		virtual void update( RenderTarget const & target );
		void draw( std::chrono::nanoseconds & gpu )const;
		void initialise( Scene const & scene
			, renderer::StagingBuffer & stagingBuffer
			, renderer::TextureViewCRefArray const & views
			, TextureNodePtrArray const & textureNodes );

		inline renderer::Device const & getDevice()const
		{
			return m_device;
		}

	protected:
		void doUpdate( renderer::TextureViewCRefArray const & views );

	private:
		void doInitialiseObject( Object const & object
			, renderer::StagingBuffer & stagingBuffer
			, TextureNodePtrArray const & textureNodes
			, uint32_t & matIndex );
		void doInitialiseBillboard( Billboard const & billboard
			, renderer::StagingBuffer & stagingBuffer
			, TextureNodePtrArray const & textureNodes
			, uint32_t & matIndex );

		virtual void doFillObjectDescriptorLayoutBindings( renderer::DescriptorSetLayoutBindingArray & bindings )
		{
		}

		virtual void doFillObjectDescriptorSet( renderer::DescriptorSetLayout & descriptorLayout
			, renderer::DescriptorSet & descriptorSet )
		{
		}

		virtual void doFillBillboardDescriptorLayoutBindings( renderer::DescriptorSetLayoutBindingArray & bindings )
		{
		}

		virtual void doFillBillboardDescriptorSet( renderer::DescriptorSetLayout & descriptorLayout
			, renderer::DescriptorSet & descriptorSet )
		{
		}

	protected:
		renderer::Device const & m_device;
		bool m_opaqueNodes;
		renderer::Extent2D m_size;
		std::string m_fragmentShaderFile;
		std::vector< renderer::TextureView const * > m_views;
		renderer::SamplerPtr m_sampler;
		renderer::CommandBufferPtr m_updateCommandBuffer;
		renderer::CommandBufferPtr m_commandBuffer;
		renderer::UniformBufferPtr< MaterialData > m_materialsUbo;

		renderer::DescriptorSetLayoutPtr m_objectDescriptorLayout;
		renderer::DescriptorSetPoolPtr m_objectDescriptorPool;
		renderer::VertexLayoutPtr m_objectVertexLayout;

		renderer::DescriptorSetLayoutPtr m_billboardDescriptorLayout;
		renderer::DescriptorSetPoolPtr m_billboardDescriptorPool;
		renderer::VertexLayoutPtr m_billboardVertexLayout;
		renderer::VertexLayoutPtr m_billboardInstanceLayout;

		renderer::RenderPassPtr m_renderPass;
		renderer::FrameBufferPtr m_frameBuffer;
		renderer::QueryPoolPtr m_queryPool;
		SubmeshNodes m_submeshNodes;
		ObjectNodes m_submeshRenderNodes;
		BillboardNodes m_billboardNodes;
		BillboardListNodes m_billboardRenderNodes;
		uint32_t m_objectsCount;
		uint32_t m_billboardsCount;
	};
}

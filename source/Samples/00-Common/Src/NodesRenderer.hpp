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
		NodesRenderer( ashes::Device const & device
			, std::string const & fragmentShaderFile
			, std::vector< ashes::Format > const & formats
			, bool clearViews
			, bool opaqueNodes );
		virtual ~NodesRenderer() = default;
		virtual void update( RenderTarget const & target );
		void draw( std::chrono::nanoseconds & gpu )const;
		void initialise( Scene const & scene
			, ashes::StagingBuffer & stagingBuffer
			, ashes::TextureViewCRefArray const & views
			, TextureNodePtrArray const & textureNodes );

		inline ashes::Device const & getDevice()const
		{
			return m_device;
		}

	protected:
		void doUpdate( ashes::TextureViewCRefArray const & views );

	private:
		void doInitialiseObject( Object const & object
			, ashes::StagingBuffer & stagingBuffer
			, TextureNodePtrArray const & textureNodes
			, uint32_t & matIndex );
		void doInitialiseBillboard( Billboard const & billboard
			, ashes::StagingBuffer & stagingBuffer
			, TextureNodePtrArray const & textureNodes
			, uint32_t & matIndex );

		virtual void doFillObjectDescriptorLayoutBindings( ashes::DescriptorSetLayoutBindingArray & bindings )
		{
		}

		virtual void doFillObjectDescriptorSet( ashes::DescriptorSetLayout & descriptorLayout
			, ashes::DescriptorSet & descriptorSet )
		{
		}

		virtual void doFillBillboardDescriptorLayoutBindings( ashes::DescriptorSetLayoutBindingArray & bindings )
		{
		}

		virtual void doFillBillboardDescriptorSet( ashes::DescriptorSetLayout & descriptorLayout
			, ashes::DescriptorSet & descriptorSet )
		{
		}

	protected:
		ashes::Device const & m_device;
		bool m_opaqueNodes;
		ashes::Extent2D m_size;
		std::string m_fragmentShaderFile;
		std::vector< ashes::TextureView const * > m_views;
		ashes::SamplerPtr m_sampler;
		ashes::CommandBufferPtr m_updateCommandBuffer;
		ashes::CommandBufferPtr m_commandBuffer;
		ashes::UniformBufferPtr< MaterialData > m_materialsUbo;

		ashes::DescriptorSetLayoutPtr m_objectDescriptorLayout;
		ashes::DescriptorSetPoolPtr m_objectDescriptorPool;
		ashes::VertexLayoutPtr m_objectVertexLayout;

		ashes::DescriptorSetLayoutPtr m_billboardDescriptorLayout;
		ashes::DescriptorSetPoolPtr m_billboardDescriptorPool;
		ashes::VertexLayoutPtr m_billboardVertexLayout;
		ashes::VertexLayoutPtr m_billboardInstanceLayout;

		ashes::RenderPassPtr m_renderPass;
		ashes::FrameBufferPtr m_frameBuffer;
		ashes::QueryPoolPtr m_queryPool;
		SubmeshNodes m_submeshNodes;
		ObjectNodes m_submeshRenderNodes;
		BillboardNodes m_billboardNodes;
		BillboardListNodes m_billboardRenderNodes;
		uint32_t m_objectsCount;
		uint32_t m_billboardsCount;
	};
}

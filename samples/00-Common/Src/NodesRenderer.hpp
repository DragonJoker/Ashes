#pragma once

#include "Prerequisites.hpp"

#include <Utils/GlslToSpv.hpp>
#include <Utils/UtilsDevice.hpp>

#include <Ashes/Buffer/UniformBuffer.hpp>
#include <Ashes/Command/CommandBuffer.hpp>
#include <Ashes/Descriptor/DescriptorSet.hpp>
#include <Ashes/Descriptor/DescriptorSetLayout.hpp>
#include <Ashes/Descriptor/DescriptorSetPool.hpp>
#include <Ashes/Image/Sampler.hpp>
#include <Ashes/Miscellaneous/QueryPool.hpp>
#include <Ashes/Pipeline/Pipeline.hpp>
#include <Ashes/Pipeline/PipelineLayout.hpp>
#include <Ashes/RenderPass/RenderPass.hpp>
#include <Ashes/RenderPass/FrameBuffer.hpp>
#include <Ashes/RenderPass/RenderPass.hpp>

namespace common
{
	class NodesRenderer
	{
	public:
		NodesRenderer( utils::Device const & device
			, ashes::CommandPool const & commandPool
			, ashes::Queue const & transferQueue
			, std::string const & fragmentShaderFile
			, std::vector< VkFormat > const & formats
			, bool clearViews
			, bool opaqueNodes );
		virtual ~NodesRenderer() = default;
		virtual void update( RenderTarget const & target );
		void draw( ashes::Queue const & queue
			, std::chrono::nanoseconds & gpu )const;
		void initialise( Scene const & scene
			, ashes::StagingBuffer & stagingBuffer
			, ashes::ImageViewPtrArray views
			, TextureNodePtrArray const & textureNodes );

		inline utils::Device const & getDevice()const
		{
			return m_device;
		}

		inline ashes::CommandPool const & getCommandPool()const
		{
			return m_commandPool;
		}

		inline ashes::Queue const & getTransferQueue()const
		{
			return m_transferQueue;
		}

	protected:
		void doUpdate( ashes::ImageViewPtrArray views );

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
		utils::Device const & m_device;
		ashes::CommandPool const & m_commandPool;
		ashes::Queue const & m_transferQueue;
		bool m_opaqueNodes;
		VkExtent2D m_size;
		std::string m_fragmentShaderFile;
		ashes::SamplerPtr m_sampler;
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

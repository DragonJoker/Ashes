#pragma once

#include "Prerequisites.hpp"

#include <Utils/GlslToSpv.hpp>
#include <Utils/UtilsDevice.hpp>

#include <AshesPP/Buffer/UniformBuffer.hpp>
#include <AshesPP/Command/CommandBuffer.hpp>
#include <AshesPP/Descriptor/DescriptorSet.hpp>
#include <AshesPP/Descriptor/DescriptorSetLayout.hpp>
#include <AshesPP/Descriptor/DescriptorSetPool.hpp>
#include <AshesPP/Image/Sampler.hpp>
#include <AshesPP/Miscellaneous/QueryPool.hpp>
#include <AshesPP/Pipeline/GraphicsPipeline.hpp>
#include <AshesPP/Pipeline/PipelineLayout.hpp>
#include <AshesPP/RenderPass/RenderPass.hpp>
#include <AshesPP/RenderPass/FrameBuffer.hpp>
#include <AshesPP/RenderPass/RenderPass.hpp>

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
			, ashes::ImageViewArray views
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
		void doUpdate( ashes::ImageViewArray views );

	private:
		void doInitialiseObject( Object const & object
			, ashes::StagingBuffer & stagingBuffer
			, TextureNodePtrArray const & textureNodes
			, uint32_t & matIndex );
		void doInitialiseBillboard( Billboard const & billboard
			, ashes::StagingBuffer & stagingBuffer
			, TextureNodePtrArray const & textureNodes
			, uint32_t & matIndex );

		virtual void doFillObjectDescriptorLayoutBindings( ashes::VkDescriptorSetLayoutBindingArray & bindings )
		{
		}

		virtual void doFillObjectDescriptorSet( ashes::DescriptorSetLayout & descriptorLayout
			, ashes::DescriptorSet & descriptorSet )
		{
		}

		virtual void doFillBillboardDescriptorLayoutBindings( ashes::VkDescriptorSetLayoutBindingArray & bindings )
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

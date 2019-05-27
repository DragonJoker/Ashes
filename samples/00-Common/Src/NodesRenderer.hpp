#pragma once

#include "Prerequisites.hpp"

#include <util/GlslToSpv.hpp>
#include <util/UtilsDevice.hpp>

#include <ashespp/Buffer/UniformBuffer.hpp>
#include <ashespp/Command/CommandBuffer.hpp>
#include <ashespp/Descriptor/DescriptorSet.hpp>
#include <ashespp/Descriptor/DescriptorSetLayout.hpp>
#include <ashespp/Descriptor/DescriptorSetPool.hpp>
#include <ashespp/Image/Sampler.hpp>
#include <ashespp/Miscellaneous/QueryPool.hpp>
#include <ashespp/Pipeline/GraphicsPipeline.hpp>
#include <ashespp/Pipeline/PipelineLayout.hpp>
#include <ashespp/RenderPass/RenderPass.hpp>
#include <ashespp/RenderPass/FrameBuffer.hpp>
#include <ashespp/RenderPass/RenderPass.hpp>

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
		void doFillTextures( ashes::DescriptorSetLayout const & layout
			, ashes::DescriptorSet & descriptorSet
			, TextureNodePtrArray const & textures );
		ashes::GraphicsPipelinePtr doCreatePipeline( ashes::PipelineLayout const & pipelineLayout
			, ashes::PipelineVertexInputStateCreateInfo vertexLayout
			, VkPrimitiveTopology topology
			, VkCullModeFlagBits cullMode );

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
		ashes::ImagePtr m_dummyImage;
		ashes::ImageView m_dummyView;
		bool m_opaqueNodes;
		VkExtent2D m_size;
		std::string m_fragmentShaderFile;
		ashes::SamplerPtr m_sampler;
		ashes::CommandBufferPtr m_commandBuffer;
		ashes::UniformBufferPtr< MaterialData > m_materialsUbo;

		ashes::DescriptorSetLayoutPtr m_objectDescriptorLayout;
		ashes::DescriptorSetPoolPtr m_objectDescriptorPool;

		ashes::DescriptorSetLayoutPtr m_billboardDescriptorLayout;
		ashes::DescriptorSetPoolPtr m_billboardDescriptorPool;

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

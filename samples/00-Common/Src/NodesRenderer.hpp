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
		virtual ~NodesRenderer()noexcept = default;
		virtual void update( RenderTarget const & target );
		void draw( ashes::Queue const & queue
			, std::chrono::nanoseconds & gpu )const;
		void initialise( Scene const & scene
			, ashes::StagingBuffer const & stagingBuffer
			, ashes::ImageViewArray views
			, TextureNodePtrArray const & textureNodes );

		utils::Device const & getDevice()const noexcept
		{
			return m_device;
		}

		ashes::CommandPool const & getCommandPool()const noexcept
		{
			return m_commandPool;
		}

		ashes::Queue const & getTransferQueue()const noexcept
		{
			return m_transferQueue;
		}

	protected:
		void doUpdate( ashes::ImageViewArray views );

	private:
		void doInitialiseObject( Object const & object
			, ashes::StagingBuffer const & stagingBuffer
			, TextureNodePtrArray const & textureNodes
			, uint32_t & matIndex );
		void doInitialiseSubmesh( Submesh const & submesh
			, ashes::StagingBuffer const & stagingBuffer
			, TextureNodePtrArray const & textureNodes
			, uint32_t & matIndex );
		void doInitialiseBillboard( Billboard const & billboard
			, ashes::StagingBuffer const & stagingBuffer
			, TextureNodePtrArray const & textureNodes
			, uint32_t & matIndex );
		void doFillTextures( ashes::DescriptorSetLayout const & layout
			, ashes::DescriptorSet & descriptorSet
			, TextureNodePtrArray const & textures )const;
		ashes::GraphicsPipelinePtr doCreatePipeline( ashes::PipelineLayout const & pipelineLayout
			, ashes::PipelineShaderStageCreateInfoArray shaderStages
			, ashes::PipelineVertexInputStateCreateInfo vertexLayout
			, VkPrimitiveTopology topology
			, VkCullModeFlagBits cullMode )const;

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
		ashes::Image const & getDummyImage()const noexcept
		{
			assert( m_dummyImage );
			return *m_dummyImage;
		}

		ashes::ImageView getDummyView()const noexcept
		{
			return m_dummyView;
		}

		bool getOpaqueNodes()const noexcept
		{
			return m_opaqueNodes;
		}

		VkExtent2D getSize()const noexcept
		{
			return m_size;
		}

		std::string getFragmentShaderFile()const noexcept
		{
			return m_fragmentShaderFile;
		}

		ashes::Sampler const & getSampler()const noexcept
		{
			assert( m_sampler );
			return *m_sampler;
		}

		ashes::CommandBuffer const & getCommandBuffer()const noexcept
		{
			assert( m_commandBuffer );
			return *m_commandBuffer;
		}

		ashes::UniformBuffer const & getMaterialsUbo()const noexcept
		{
			assert( m_materialsUbo );
			return *m_materialsUbo;
		}

		std::vector< common::MaterialData > const & getMaterialsData()const noexcept
		{
			return m_materialsData;
		}

		ashes::DescriptorSetLayout const & getObjectDescriptorLayout()const noexcept
		{
			assert( m_objectDescriptorLayout );
			return *m_objectDescriptorLayout;
		}

		ashes::DescriptorSetPool const & getObjectDescriptorPool()const noexcept
		{
			assert( m_objectDescriptorPool );
			return *m_objectDescriptorPool;
		}

		ashes::DescriptorSetLayout const & getBillboardDescriptorLayout()const noexcept
		{
			assert( m_billboardDescriptorLayout );
			return *m_billboardDescriptorLayout;
		}

		ashes::DescriptorSetPool const & getBillboardDescriptorPool()const noexcept
		{
			assert( m_billboardDescriptorPool );
			return *m_billboardDescriptorPool;
		}

		ashes::RenderPass const & getRenderPass()const noexcept
		{
			assert( m_renderPass );
			return *m_renderPass;
		}

		ashes::FrameBuffer const & getFrameBuffer()const noexcept
		{
			assert( m_frameBuffer );
			return *m_frameBuffer;
		}

		ashes::QueryPool const & getQueryPool()const noexcept
		{
			assert( m_queryPool );
			return *m_queryPool;
		}

		SubmeshNodes const & getSubmeshNodes()const noexcept
		{
			return m_submeshNodes;
		}

		ObjectNodes const & getSubmeshRenderNodes()const noexcept
		{
			return m_submeshRenderNodes;
		}

		BillboardNodes const & getBillboardNodes()const noexcept
		{
			return m_billboardNodes;
		}

		BillboardListNodes const & getBillboardRenderNodes()const noexcept
		{
			return m_billboardRenderNodes;
		}

		uint32_t getObjectsCount()const noexcept
		{
			return m_objectsCount;
		}

		uint32_t getBillboardsCount()const noexcept
		{
			return m_billboardsCount;
		}

	private:
		utils::Device const & m_device;
		ashes::CommandPool const & m_commandPool;
		ashes::Queue const & m_transferQueue;
		ashes::ImagePtr m_dummyImage{};
		ashes::ImageView m_dummyView{};
		bool m_opaqueNodes{};
		VkExtent2D m_size{};
		std::string m_fragmentShaderFile{};
		ashes::SamplerPtr m_sampler{};
		ashes::CommandBufferPtr m_commandBuffer{};
		ashes::UniformBufferPtr m_materialsUbo{};
		std::vector< common::MaterialData > m_materialsData{};

		ashes::DescriptorSetLayoutPtr m_objectDescriptorLayout{};
		ashes::DescriptorSetPoolPtr m_objectDescriptorPool{};

		ashes::DescriptorSetLayoutPtr m_billboardDescriptorLayout{};
		ashes::DescriptorSetPoolPtr m_billboardDescriptorPool{};

		ashes::RenderPassPtr m_renderPass{};
		ashes::FrameBufferPtr m_frameBuffer{};
		ashes::QueryPoolPtr m_queryPool{};
		SubmeshNodes m_submeshNodes{};
		ObjectNodes m_submeshRenderNodes{};
		BillboardNodes m_billboardNodes{};
		BillboardListNodes m_billboardRenderNodes{};
		uint32_t m_objectsCount{};
		uint32_t m_billboardsCount{};
	};
}

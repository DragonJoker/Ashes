/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/TestDevice.hpp"

#include "Buffer/TestBuffer.hpp"
#include "Buffer/TestBufferView.hpp"
#include "Buffer/TestUniformBuffer.hpp"
#include "Command/TestCommandPool.hpp"
#include "Command/TestQueue.hpp"
#include "Core/TestConnection.hpp"
#include "Core/TestPhysicalDevice.hpp"
#include "Core/TestRenderer.hpp"
#include "Core/TestSwapChain.hpp"
#include "Descriptor/TestDescriptorPool.hpp"
#include "Descriptor/TestDescriptorSetLayout.hpp"
#include "Image/TestSampler.hpp"
#include "Image/TestStagingTexture.hpp"
#include "Image/TestTexture.hpp"
#include "Image/TestTextureView.hpp"
#include "Miscellaneous/TestDeviceMemory.hpp"
#include "Miscellaneous/TestQueryPool.hpp"
#include "Pipeline/TestPipelineLayout.hpp"
#include "RenderPass/TestRenderPass.hpp"
#include "Shader/TestShaderModule.hpp"
#include "Sync/TestEvent.hpp"
#include "Sync/TestFence.hpp"
#include "Sync/TestSemaphore.hpp"

#include <Miscellaneous/MemoryRequirements.hpp>
#include <RenderPass/RenderPassCreateInfo.hpp>

namespace test_renderer
{
	Device::Device( Renderer const & renderer
		, ashes::ConnectionPtr && connection )
		: ashes::Device{ renderer, connection->getGpu(), *connection }
		, m_renderer{ renderer }
		, m_connection{ static_cast< Connection * >( connection.release() ) }
		, m_gpu{ static_cast< PhysicalDevice const & >( ashes::Device::getPhysicalDevice() ) }
	{
		m_timestampPeriod = m_gpu.getProperties().limits.timestampPeriod;
		m_presentQueue = std::make_unique< Queue >( *this, m_connection->getPresentQueueFamilyIndex() );
		m_presentCommandPool = std::make_unique< CommandPool >( *this
			, m_presentQueue->getFamilyIndex()
			, ashes::CommandPoolCreateFlag::eResetCommandBuffer | ashes::CommandPoolCreateFlag::eTransient );

		if ( m_connection->getGraphicsQueueFamilyIndex() != m_connection->getPresentQueueFamilyIndex() )
		{
			m_graphicsQueue = std::make_unique< Queue >( *this, m_connection->getPresentQueueFamilyIndex() );
		}
		else
		{
			m_graphicsQueue = std::make_unique< Queue >( *this, m_connection->getGraphicsQueueFamilyIndex() );
		}

		m_graphicsCommandPool = std::make_unique< CommandPool >( *this
			, m_graphicsQueue->getFamilyIndex()
			, ashes::CommandPoolCreateFlag::eResetCommandBuffer | ashes::CommandPoolCreateFlag::eTransient );

		if ( m_connection->getGraphicsQueueFamilyIndex() != m_connection->getComputeQueueFamilyIndex() )
		{
			m_computeQueue = std::make_unique< Queue >( *this, m_connection->getComputeQueueFamilyIndex() );
		}
		else
		{
			m_computeQueue = std::make_unique< Queue >( *this, m_connection->getGraphicsQueueFamilyIndex() );
		}

		m_computeCommandPool = std::make_unique< CommandPool >( *this
			, m_computeQueue->getFamilyIndex()
			, ashes::CommandPoolCreateFlag::eResetCommandBuffer | ashes::CommandPoolCreateFlag::eTransient );
	}

	Device::~Device()
	{
		m_graphicsCommandPool.reset();
		m_graphicsQueue.reset();
		m_presentCommandPool.reset();
		m_presentQueue.reset();
		m_computeCommandPool.reset();
		m_computeQueue.reset();
	}

	ashes::StagingTexturePtr Device::createStagingTexture( ashes::Format format
		, ashes::Extent3D const & extent )const
	{
		return std::make_unique< StagingTexture >( *this, format, extent );
	}

	ashes::RenderPassPtr Device::createRenderPass( ashes::RenderPassCreateInfo createInfo )const
	{
		return std::make_unique< RenderPass >( *this, std::move( createInfo ) );
	}

	ashes::PipelineLayoutPtr Device::createPipelineLayout( ashes::DescriptorSetLayoutCRefArray const & setLayouts
			, ashes::PushConstantRangeCRefArray const & pushConstantRanges )const
	{
		return std::make_unique< PipelineLayout >( *this
			, setLayouts
			, pushConstantRanges );
	}

	ashes::DescriptorSetLayoutPtr Device::createDescriptorSetLayout( ashes::DescriptorSetLayoutBindingArray && bindings )const
	{
		return std::make_unique< DescriptorSetLayout >( *this, std::move( bindings ) );
	}

	ashes::DescriptorPoolPtr Device::createDescriptorPool( ashes::DescriptorPoolCreateFlags flags
		, uint32_t maxSets
		, ashes::DescriptorPoolSizeArray poolSizes )const
	{
		return std::make_unique< DescriptorPool >( *this, flags, maxSets, poolSizes );
	}

	ashes::DeviceMemoryPtr Device::allocateMemory( ashes::MemoryRequirements const & requirements
		, ashes::MemoryPropertyFlags flags )const
	{
		return std::make_unique< DeviceMemory >( *this
			, requirements
			, flags );
	}

	ashes::TexturePtr Device::createTexture( ashes::ImageCreateInfo const & createInfo )const
	{
		return std::make_unique< Texture >( *this, createInfo );
	}

	void Device::getImageSubresourceLayout( ashes::Texture const & image
		, ashes::ImageSubresource const & subresource
		, ashes::SubresourceLayout & layout )const
	{
	}

	ashes::SamplerPtr Device::createSampler( ashes::SamplerCreateInfo const & createInfo )const
	{
		return std::make_unique< Sampler >( *this, createInfo );
	}

	ashes::BufferBasePtr Device::createBuffer( uint32_t size
		, ashes::BufferTargets target )const
	{
		return std::make_unique< Buffer >( *this
			, size
			, target );
	}

	ashes::BufferViewPtr Device::createBufferView( ashes::BufferBase const & buffer
		, ashes::Format format
		, uint32_t offset
		, uint32_t range )const
	{
		return std::make_unique< BufferView >( *this
			, static_cast< Buffer const & >( buffer )
			, format
			, offset
			, range );
	}

	ashes::UniformBufferBasePtr Device::createUniformBuffer( uint32_t count
		, uint32_t size
		, ashes::BufferTargets target
		, ashes::MemoryPropertyFlags memoryFlags )const
	{
		return std::make_unique< UniformBuffer >( *this
			, count
			, size
			, target
			, memoryFlags );
	}

	ashes::SwapChainPtr Device::createSwapChain( ashes::Extent2D const & size )const
	{
		ashes::SwapChainPtr result;

		try
		{
			result = std::make_unique< SwapChain >( *this, size );
		}
		catch ( std::exception & exc )
		{
			ashes::Logger::logError( std::string{ "Could not create the swap chain:\n" } + exc.what() );
		}
		catch ( ... )
		{
			ashes::Logger::logError( "Could not create the swap chain:\nUnknown error" );
		}

		return result;
	}

	ashes::SemaphorePtr Device::createSemaphore()const
	{
		return std::make_unique< Semaphore >( *this );
	}

	ashes::FencePtr Device::createFence( ashes::FenceCreateFlags flags )const
	{
		return std::make_unique< Fence >( *this, flags );
	}

	ashes::EventPtr Device::createEvent()const
	{
		return std::make_unique< Event >( *this );
	}

	ashes::CommandPoolPtr Device::createCommandPool( uint32_t queueFamilyIndex
		, ashes::CommandPoolCreateFlags const & flags )const
	{
		return std::make_unique< CommandPool >( *this
			, queueFamilyIndex
			, flags );
	}

	ashes::ShaderModulePtr Device::createShaderModule( ashes::ShaderStageFlag stage )const
	{
		return std::make_shared< ShaderModule >( *this, stage );
	}

	ashes::QueryPoolPtr Device::createQueryPool( ashes::QueryType type
		, uint32_t count
		, ashes::QueryPipelineStatisticFlags pipelineStatistics )const
	{
		return std::make_unique< QueryPool >( *this
			, type
			, count
			, pipelineStatistics );
	}

	void Device::waitIdle()const
	{
	}
}

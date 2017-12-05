/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "VkDevice.hpp"

#include "VkBuffer.hpp"
#include "VkCommandPool.hpp"
#include "VkConnection.hpp"
#include "VkDescriptorSetLayout.hpp"
#include "VkPipeline.hpp"
#include "VkPipelineLayout.hpp"
#include "VkQueue.hpp"
#include "VkRenderer.hpp"
#include "VkRenderPass.hpp"
#include "VkRenderSubpass.hpp"
#include "VkSampler.hpp"
#include "VkSemaphore.hpp"
#include "VkShaderProgram.hpp"
#include "VkStagingBuffer.hpp"
#include "VkSwapChain.hpp"
#include "VkTexture.hpp"
#include "VkUniformBuffer.hpp"
#include "VkVertexLayout.hpp"

namespace vk_renderer
{
	Device::Device( renderer::Renderer const & renderer
		, renderer::ConnectionPtr && connection )
		: renderer::Device{ renderer, *connection }
		, m_device{ static_cast< Renderer const & >( renderer ).getInstance()
			, static_cast< Renderer const & >( renderer ).getPhysicalDevice()
			, std::move( static_cast< Connection & >( *connection ).getConnection() ) }
		, m_version{ "Vulkan 1.0.0" }
	{
		m_presentQueue = std::make_unique< Queue >( m_device.getPresentQueue() );
		m_graphicsQueue = std::make_unique< Queue >( m_device.getGraphicsQueue() );
		m_presentCommandPool = std::make_unique< CommandPool >( *this, m_device.getPresentCommandPool() );
		m_graphicsCommandPool = std::make_unique< CommandPool >( *this, m_device.getGraphicsCommandPool() );
	}

	renderer::RenderPassPtr Device::createRenderPass( std::vector< utils::PixelFormat > const & formats
		, renderer::RenderSubpassPtrArray const & subpasses
		, renderer::RenderPassState const & initialState
		, renderer::RenderPassState const & finalState
		, bool clear
		, renderer::SampleCountFlag samplesCount )const
	{
		return std::make_unique< RenderPass >( *this
			, formats
			, subpasses
			, initialState
			, finalState
			, clear
			, samplesCount );
	}

	renderer::RenderSubpassPtr Device::createRenderSubpass( std::vector< utils::PixelFormat > const & formats
		, renderer::RenderSubpassState const & neededState )const
	{
		return std::make_unique< RenderSubpass >( *this
			, formats
			, neededState );
	}

	renderer::VertexLayoutPtr Device::createVertexLayout( uint32_t bindingSlot
		, uint32_t stride )const
	{
		return std::make_unique< VertexLayout >( bindingSlot
			, stride );
	}

	renderer::PipelineLayoutPtr Device::createPipelineLayout( renderer::DescriptorSetLayout const & layout )const
	{
		return std::make_unique< PipelineLayout >( *this
			, &layout );
	}

	renderer::PipelinePtr Device::createPipeline( renderer::PipelineLayout const & layout
		, renderer::ShaderProgram const & program
		, renderer::VertexLayoutCRefArray const & vertexLayouts
		, renderer::RenderPass const & renderPass
		, renderer::PrimitiveTopology topology
		, renderer::RasterisationState const & rasterisationState
		, renderer::ColourBlendState const & colourBlendState )const
	{
		return std::make_shared< Pipeline >( *this
			, layout
			, program
			, vertexLayouts
			, renderPass
			, topology
			, rasterisationState
			, colourBlendState );
	}

	renderer::DescriptorSetLayoutPtr Device::createDescriptorSetLayout( renderer::DescriptorSetLayoutBindingArray && bindings )const
	{
		return std::make_unique< DescriptorSetLayout >( *this, std::move( bindings ) );
	}

	renderer::TexturePtr Device::createTexture()const
	{
		return std::make_shared< Texture >( *this );
	}

	renderer::SamplerPtr Device::createSampler( renderer::WrapMode wrapS
		, renderer::WrapMode wrapT
		, renderer::WrapMode wrapR
		, renderer::Filter minFilter
		, renderer::Filter magFilter
		, renderer::MipmapMode mipFilter )const
	{
		return std::make_unique< Sampler >( *this
			, wrapS
			, wrapT
			, wrapR
			, minFilter
			, magFilter
			, mipFilter );
	}

	renderer::BufferBasePtr Device::createBuffer( uint32_t size
		, renderer::BufferTargets target
		, renderer::MemoryPropertyFlags memoryFlags )const
	{
		return std::make_unique< BufferBase >( *this
			, size
			, target
			, memoryFlags );
	}

	renderer::StagingBufferPtr Device::createStagingBuffer( uint32_t size )const
	{
		return std::make_shared< StagingBuffer >( *this, size );
	}

	renderer::UniformBufferBasePtr Device::createUniformBuffer( uint32_t count
		, uint32_t size
		, renderer::BufferTargets target
		, renderer::MemoryPropertyFlags memoryFlags )const
	{
		return std::make_unique< UniformBufferBase >( *this
			, count
			, size
			, target
			, memoryFlags );
	}

	renderer::SwapChainPtr Device::createSwapChain( utils::IVec2 const & size )const
	{
		renderer::SwapChainPtr result;

		try
		{
			result = std::make_unique< SwapChain >( *this, size );
		}
		catch ( std::exception & exc )
		{
			std::cerr << "Could not create the swap chain:\n" << exc.what() << std::endl;
		}
		catch ( ... )
		{
			std::cerr << "Could not create the swap chain:\nUnknown error" << std::endl;
		}

		return result;
	}

	renderer::SemaphorePtr Device::createSemaphore()const
	{
		return std::make_unique< Semaphore >( *this );
	}

	renderer::CommandPoolPtr Device::createCommandPool( uint32_t queueFamilyIndex
		, renderer::CommandPoolCreateFlags const & flags )const
	{
		return std::make_unique< CommandPool >( *this
			, queueFamilyIndex
			, flags );
	}

	renderer::ShaderProgramPtr Device::createShaderProgram()const
	{
		return std::make_unique< ShaderProgram >( *this );
	}

	void Device::waitIdle()const
	{
		m_device.waitIdle();
	}
}

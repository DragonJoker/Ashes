/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "VkDevice.hpp"

#include "VkBuffer.hpp"
#include "VkCommandPool.hpp"
#include "VkConnection.hpp"
#include "VkDescriptorSetLayout.hpp"
#include "VkGeometryBuffers.hpp"
#include "VkPipeline.hpp"
#include "VkPipelineLayout.hpp"
#include "VkQueue.hpp"
#include "VkRenderer.hpp"
#include "VkRenderPass.hpp"
#include "VkRenderSubpass.hpp"
#include "VkSampler.hpp"
#include "VkSemaphore.hpp"
#include "VkShaderProgram.hpp"
#include "VkSwapChain.hpp"
#include "VkTexture.hpp"
#include "VkUniformBuffer.hpp"
#include "VkVertexLayout.hpp"

namespace vk_renderer
{
	Device::Device( Renderer const & renderer
		, renderer::ConnectionPtr && connection )
		: renderer::Device{ renderer, *connection }
		, m_renderer{ renderer }
		, m_connection{ static_cast< Connection * >( connection.release() ) }
		, m_gpu{ m_connection->getGpu() }
		, m_version{ "Vulkan 1.0.0" }
	{
		std::vector< VkDeviceQueueCreateInfo > queueCreateInfos;
		std::vector< float > queuePriorities = { 1.0f };

		queueCreateInfos.push_back(
		{
			VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,                 // sType
			nullptr,                                                    // pNext
			0,                                                          // flags
			m_connection->getGraphicsQueueFamilyIndex(),                 // queueFamilyIndex
			static_cast< uint32_t >( queuePriorities.size() ),          // queueCount
			queuePriorities.data()                                      // pQueuePriorities
		} );

		if ( m_connection->getPresentQueueFamilyIndex() != m_connection->getGraphicsQueueFamilyIndex() )
		{
			queueCreateInfos.push_back(
			{
				VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,             // sType
				nullptr,                                                // pNext
				0,                                                      // flags
				m_connection->getPresentQueueFamilyIndex(),              // queueFamilyIndex
				static_cast< uint32_t >( queuePriorities.size() ),      // queueCount
				queuePriorities.data()                                  // pQueuePriorities
			} );
		}

		VkDeviceCreateInfo deviceInfo
		{
			VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			nullptr,
			0,                                                                               // flags
			static_cast< uint32_t >( queueCreateInfos.size() ),                              // queueCreateInfoCount
			queueCreateInfos.data(),                                                         // pQueueCreateInfos
			static_cast< uint32_t >( m_gpu.getLayerNames().size() ),                         // enabledLayerCount
			m_gpu.getLayerNames().empty() ? nullptr : m_gpu.getLayerNames().data(),          // ppEnabledLayerNames
			static_cast< uint32_t >( m_gpu.getExtensionNames().size() ),                     // enabledExtensionCount
			m_gpu.getExtensionNames().empty() ? nullptr : m_gpu.getExtensionNames().data(),  // ppEnabledExtensionNames
			nullptr                                                                          // pEnabledFeatures
		};
		DEBUG_DUMP( deviceInfo );

		auto res = CreateDevice( m_gpu, &deviceInfo, nullptr, &m_device );

		if ( !checkError( res ) )
		{
			throw std::runtime_error{ "LogicalDevice creation failed: " + getLastError() };
		}
		
		m_presentQueue = std::make_unique< Queue >( *this, m_connection->getPresentQueueFamilyIndex() );
		m_presentCommandPool = std::make_unique< CommandPool >( *this
			, m_presentQueue->getFamilyIndex()
			, renderer::CommandPoolCreateFlag::eResetCommandBuffer | renderer::CommandPoolCreateFlag::eTransient );

		if ( m_connection->getGraphicsQueueFamilyIndex() != m_connection->getPresentQueueFamilyIndex() )
		{
			m_graphicsQueue = std::make_unique< Queue >( *this, m_connection->getGraphicsQueueFamilyIndex() );
		}
		else
		{
			m_graphicsQueue = std::make_unique< Queue >( *this, m_connection->getPresentQueueFamilyIndex() );
		}

		m_graphicsCommandPool = std::make_unique< CommandPool >( *this
			, m_graphicsQueue->getFamilyIndex()
			, renderer::CommandPoolCreateFlag::eResetCommandBuffer | renderer::CommandPoolCreateFlag::eTransient );
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

	renderer::GeometryBuffersPtr Device::createGeometryBuffers( renderer::VertexBufferBase const & vbo
		, uint64_t vboOffset
		, renderer::VertexLayout const & layout )const
	{
		return std::make_unique< GeometryBuffers >( vbo
			, vboOffset
			, layout );
	}

	renderer::GeometryBuffersPtr Device::createGeometryBuffers( renderer::VertexBufferBase const & vbo
		, uint64_t vboOffset
		, renderer::VertexLayout const & layout
		, renderer::BufferBase const & ibo
		, uint64_t iboOffset
		, renderer::IndexType type )const
	{
		return std::make_unique< GeometryBuffers >( vbo
			, vboOffset
			, layout
			, ibo
			, iboOffset
			, type );
	}

	renderer::GeometryBuffersPtr Device::createGeometryBuffers( renderer::VertexBufferCRefArray const & vbos
		, std::vector< uint64_t > vboOffsets
		, renderer::VertexLayoutCRefArray const & layouts )const
	{
		return std::make_unique< GeometryBuffers >( vbos
			, vboOffsets
			, layouts );
	}

	renderer::GeometryBuffersPtr Device::createGeometryBuffers( renderer::VertexBufferCRefArray const & vbos
		, std::vector< uint64_t > vboOffsets
		, renderer::VertexLayoutCRefArray const & layouts
		, renderer::BufferBase const & ibo
		, uint64_t iboOffset
		, renderer::IndexType type )const
	{
		return std::make_unique< GeometryBuffers >( vbos
			, vboOffsets
			, layouts
			, ibo
			, iboOffset
			, type );
	}

	renderer::PipelineLayoutPtr Device::createPipelineLayout()const
	{
		return std::make_unique< PipelineLayout >( *this
			, nullptr );
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
		return std::make_unique< Buffer >( *this
			, size
			, target
			, memoryFlags );
	}

	renderer::UniformBufferBasePtr Device::createUniformBuffer( uint32_t count
		, uint32_t size
		, renderer::BufferTargets target
		, renderer::MemoryPropertyFlags memoryFlags )const
	{
		return std::make_unique< UniformBuffer >( *this
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
		DeviceWaitIdle( m_device );
	}

	VkMemoryRequirements Device::getBufferMemoryRequirements( VkBuffer buffer )const
	{
		VkMemoryRequirements requirements;
		GetBufferMemoryRequirements( m_device
			, buffer
			, &requirements );
		return requirements;
	}

	VkMemoryRequirements Device::getImageMemoryRequirements( VkImage image )const
	{
		VkMemoryRequirements requirements;
		GetImageMemoryRequirements( m_device
			, image
			, &requirements );
		return requirements;
	}
}

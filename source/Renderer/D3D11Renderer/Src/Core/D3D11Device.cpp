/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Core/D3D11Device.hpp"

#include "Buffer/D3D11Buffer.hpp"
#include "Buffer/D3D11BufferView.hpp"
#include "Buffer/D3D11PushConstantsBuffer.hpp"
#include "Buffer/D3D11UniformBuffer.hpp"
#include "Command/D3D11CommandPool.hpp"
#include "Command/D3D11Queue.hpp"
#include "Core/D3D11Connection.hpp"
#include "Core/D3D11DummyIndexBuffer.hpp"
#include "Core/D3D11PhysicalDevice.hpp"
#include "Core/D3D11Renderer.hpp"
#include "Core/D3D11SwapChain.hpp"
#include "Descriptor/D3D11DescriptorPool.hpp"
#include "Descriptor/D3D11DescriptorSetLayout.hpp"
#include "Image/D3D11Sampler.hpp"
#include "Image/D3D11StagingTexture.hpp"
#include "Image/D3D11Texture.hpp"
#include "Image/D3D11TextureView.hpp"
#include "Miscellaneous/D3D11DeviceMemory.hpp"
#include "Miscellaneous/D3D11QueryPool.hpp"
#include "Pipeline/D3D11PipelineLayout.hpp"
#include "RenderPass/D3D11RenderPass.hpp"
#include "Shader/D3D11ShaderModule.hpp"
#include "Sync/D3D11Event.hpp"
#include "Sync/D3D11Fence.hpp"
#include "Sync/D3D11Semaphore.hpp"

#include <Core/PlatformWindowHandle.hpp>
#include <Miscellaneous/MemoryRequirements.hpp>
#include <RenderPass/RenderPassCreateInfo.hpp>

namespace d3d11_renderer
{
	Device::Device( Renderer const & renderer
		, renderer::ConnectionPtr && connection )
		: renderer::Device{ renderer, connection->getGpu(), *connection }
		, m_renderer{ renderer }
		, m_connection{ static_cast< Connection * >( connection.release() ) }
		, m_gpu{ static_cast< PhysicalDevice const & >( renderer::Device::getPhysicalDevice() ) }
	{
		doCreateD3D11Device();
		m_timestampPeriod = m_gpu.getProperties().limits.timestampPeriod;

		m_presentQueue = std::make_unique< Queue >( *this
			, m_connection->getPresentQueueFamilyIndex() );
		m_presentCommandPool = std::make_unique< CommandPool >( *this
			, m_presentQueue->getFamilyIndex()
			, renderer::CommandPoolCreateFlag::eResetCommandBuffer | renderer::CommandPoolCreateFlag::eTransient );

		m_graphicsQueue = std::make_unique< Queue >( *this
			, m_connection->getPresentQueueFamilyIndex() );
		m_graphicsCommandPool = std::make_unique< CommandPool >( *this
			, m_graphicsQueue->getFamilyIndex()
			, renderer::CommandPoolCreateFlag::eResetCommandBuffer | renderer::CommandPoolCreateFlag::eTransient );

		m_computeQueue = std::make_unique< Queue >( *this
			, m_connection->getComputeQueueFamilyIndex() );
		m_computeCommandPool = std::make_unique< CommandPool >( *this
			, m_computeQueue->getFamilyIndex()
			, renderer::CommandPoolCreateFlag::eResetCommandBuffer | renderer::CommandPoolCreateFlag::eTransient );

		auto count = uint32_t( sizeof( dummyIndex ) / sizeof( dummyIndex[0] ) );
		m_dummyIndexed = renderer::makeBuffer< uint32_t >( *this
			, count
			, renderer::BufferTarget::eIndexBuffer | renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eHostVisible );

		if ( auto * buffer = m_dummyIndexed->lock( 0u
			, count
			, renderer::MemoryMapFlag::eWrite ) )
		{
			std::copy( dummyIndex, dummyIndex + count, buffer );
			m_dummyIndexed->flush( 0, count );
			m_dummyIndexed->unlock();
		}
	}

	Device::~Device()
	{
		m_graphicsCommandPool.reset();
		m_graphicsQueue.reset();
		m_presentCommandPool.reset();
		m_presentQueue.reset();
		m_computeCommandPool.reset();
		m_computeQueue.reset();
		m_dummyIndexed.reset();

		m_deviceContext->ClearState();
		m_deviceContext->Flush();
		safeRelease( m_waitIdleQuery );
		safeRelease( m_deviceContext );
		safeRelease( m_device2 );
		safeRelease( m_device1 );
		safeRelease( m_device );

#if !defined( NDEBUG )

		m_debug->ReportLiveDeviceObjects( D3D11_RLDO_DETAIL | D3D11_RLDO_IGNORE_INTERNAL );
		safeRelease( m_debug );

#endif
	}

	renderer::StagingTexturePtr Device::createStagingTexture( renderer::Format format
		, renderer::Extent3D const & extent )const
	{
		return std::make_unique< StagingTexture >( *this, format, extent );
	}

	renderer::PushConstantsBufferPtr Device::createPushConstantsBuffer( uint32_t location
		, renderer::ShaderStageFlags stageFlags
		, renderer::PushConstantArray const & variables )const
	{
		return std::make_unique< PushConstantsBuffer >( *this
			, location
			, stageFlags
			, variables );
	}

	renderer::RenderPassPtr Device::createRenderPass( renderer::RenderPassCreateInfo createInfo )const
	{
		return std::make_unique< RenderPass >( *this, std::move( createInfo ) );
	}

	renderer::PipelineLayoutPtr Device::createPipelineLayout( renderer::DescriptorSetLayoutCRefArray const & setLayouts
			, renderer::PushConstantRangeCRefArray const & pushConstantRanges )const
	{
		return std::make_unique< PipelineLayout >( *this
			, setLayouts
			, pushConstantRanges );
	}

	renderer::DescriptorSetLayoutPtr Device::createDescriptorSetLayout( renderer::DescriptorSetLayoutBindingArray && bindings )const
	{
		return std::make_unique< DescriptorSetLayout >( *this, std::move( bindings ) );
	}

	renderer::DescriptorPoolPtr Device::createDescriptorPool( renderer::DescriptorPoolCreateFlags flags
		, uint32_t maxSets
		, renderer::DescriptorPoolSizeArray poolSizes )const
	{
		return std::make_unique< DescriptorPool >( *this, flags, maxSets, poolSizes );
	}

	renderer::DeviceMemoryPtr Device::allocateMemory( renderer::MemoryRequirements const & requirements
		, renderer::MemoryPropertyFlags flags )const
	{
		return std::make_unique< DeviceMemory >( *this
			, requirements
			, flags );
	}

	renderer::TexturePtr Device::createTexture( renderer::ImageCreateInfo const & createInfo )const
	{
		return std::make_unique< Texture >( *this, createInfo );
	}

	void Device::getImageSubresourceLayout( renderer::Texture const & image
		, renderer::ImageSubresource const & subresource
		, renderer::SubresourceLayout & layout )const
	{
	}

	renderer::SamplerPtr Device::createSampler( renderer::SamplerCreateInfo const & createInfo )const
	{
		return std::make_unique< Sampler >( *this, createInfo );
	}

	renderer::BufferBasePtr Device::createBuffer( uint32_t size
		, renderer::BufferTargets target )const
	{
		return std::make_unique< Buffer >( *this
			, size
			, target );
	}

	renderer::BufferViewPtr Device::createBufferView( renderer::BufferBase const & buffer
		, renderer::Format format
		, uint32_t offset
		, uint32_t range )const
	{
		return std::make_unique< BufferView >( *this
			, static_cast< Buffer const & >( buffer )
			, format
			, offset
			, range );
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

	renderer::SwapChainPtr Device::createSwapChain( renderer::Extent2D const & size )const
	{
		renderer::SwapChainPtr result;

		try
		{
			result = std::make_unique< SwapChain >( *this, size );
		}
		catch ( std::exception & exc )
		{
			renderer::Logger::logError( std::string{ "Could not create the swap chain:\n" } + exc.what() );
		}
		catch ( ... )
		{
			renderer::Logger::logError( "Could not create the swap chain:\nUnknown error" );
		}

		return result;
	}

	renderer::SemaphorePtr Device::createSemaphore()const
	{
		return std::make_unique< Semaphore >( *this );
	}

	renderer::FencePtr Device::createFence( renderer::FenceCreateFlags flags )const
	{
		return std::make_unique< Fence >( *this, flags );
	}

	renderer::EventPtr Device::createEvent()const
	{
		return std::make_unique< Event >( *this );
	}

	renderer::CommandPoolPtr Device::createCommandPool( uint32_t queueFamilyIndex
		, renderer::CommandPoolCreateFlags const & flags )const
	{
		return std::make_unique< CommandPool >( *this
			, queueFamilyIndex
			, flags );
	}

	renderer::ShaderModulePtr Device::createShaderModule( renderer::ShaderStageFlag stage )const
	{
		return std::make_shared< ShaderModule >( *this, stage );
	}

	renderer::QueryPoolPtr Device::createQueryPool( renderer::QueryType type
		, uint32_t count
		, renderer::QueryPipelineStatisticFlags pipelineStatistics )const
	{
		return std::make_unique< QueryPool >( *this
			, type
			, count
			, pipelineStatistics );
	}

	void Device::waitIdle()const
	{
		m_deviceContext->End( m_waitIdleQuery );
		m_deviceContext->Flush();
		BOOL data{ FALSE };

		while ( ( S_FALSE == m_deviceContext->GetData( m_waitIdleQuery
				, &data
				, UINT( sizeof( data ) )
				, 0u ) )
			&& !data )
		{
			std::this_thread::sleep_for( std::chrono::microseconds{ 1ull } );
		}
	}

	void Device::doCreateD3D11Device()
	{
		auto factory = m_renderer.getDXGIFactory();

		std::vector< D3D_FEATURE_LEVEL > requestedFeatureLevels
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1,
		};
		D3D_FEATURE_LEVEL supportedFeatureLevel;
		HRESULT hr;
		HWND hWnd = m_connection->getHandle().getInternal< renderer::IMswWindowHandle >().getHwnd();
		hr = factory->MakeWindowAssociation( hWnd, 0 );

		if ( SUCCEEDED( hr ) )
		{
			HWND hWnd2;
			hr = factory->GetWindowAssociation( &hWnd2 );

			if ( hWnd2 )
			{
				hWnd = hWnd2;
			}
		}

		UINT flags = 0;
		auto adapter = m_gpu.getAdapter();

#if !defined( NDEBUG )
		flags = D3D11_CREATE_DEVICE_DEBUG;
#endif

		// First me check max supported feature level
		hr = D3D11CreateDevice( nullptr
			, D3D_DRIVER_TYPE_HARDWARE
			, nullptr
			, flags
			, requestedFeatureLevels.data()
			, UINT( requestedFeatureLevels.size() )
			, D3D11_SDK_VERSION
			, nullptr
			, &supportedFeatureLevel
			, nullptr );

		if ( hr == S_FALSE )
		{
			hr = D3D11CreateDevice( nullptr
				, D3D_DRIVER_TYPE_HARDWARE
				, nullptr
				, flags
				, &supportedFeatureLevel
				, 1
				, D3D11_SDK_VERSION
				, &m_device
				, &m_featureLevel
				, &m_deviceContext );
		}
		else
		{
			// First me check max supported feature level
			hr = D3D11CreateDevice( adapter
				, D3D_DRIVER_TYPE_HARDWARE
				, nullptr
				, flags
				, requestedFeatureLevels.data()
				, UINT( requestedFeatureLevels.size() )
				, D3D11_SDK_VERSION
				, nullptr
				, &supportedFeatureLevel
				, nullptr );

			if ( hr == S_FALSE )
			{
				hr = D3D11CreateDevice( adapter
					, D3D_DRIVER_TYPE_HARDWARE
					, nullptr
					, flags
					, &supportedFeatureLevel
					, 1
					, D3D11_SDK_VERSION
					, &m_device
					, &m_featureLevel
					, &m_deviceContext );
			}
		}

		if ( m_device )
		{
#if !defined( NDEBUG )

			m_device->QueryInterface( __uuidof( ID3D11Debug ), reinterpret_cast< void ** >( &m_debug ) );

#endif

			dxDebugName( m_device, Device );

			if ( S_OK == m_device->QueryInterface( __uuidof( ID3D11Device1 ), reinterpret_cast< void ** >( &m_device1 ) ) )
			{
				dxDebugName( m_device1, Device1 );
			}

			if ( S_OK == m_device->QueryInterface( __uuidof( ID3D11Device2 ), reinterpret_cast< void ** >( &m_device2 ) ) )
			{
				dxDebugName( m_device2, Device2 );
			}

			D3D11_QUERY_DESC desc{};
			desc.Query = D3D11_QUERY_EVENT;
			m_device->CreateQuery( &desc, &m_waitIdleQuery );
		}
	}
}

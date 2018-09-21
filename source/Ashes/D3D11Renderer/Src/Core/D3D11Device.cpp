/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/D3D11Device.hpp"

#include "Buffer/D3D11Buffer.hpp"
#include "Buffer/D3D11BufferView.hpp"
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
		, ashes::ConnectionPtr && connection )
		: ashes::Device{ renderer, connection->getGpu(), *connection }
		, m_renderer{ renderer }
		, m_connection{ static_cast< Connection * >( connection.release() ) }
		, m_gpu{ static_cast< PhysicalDevice const & >( ashes::Device::getPhysicalDevice() ) }
	{
		doCreateD3D11Device();
		m_timestampPeriod = m_gpu.getProperties().limits.timestampPeriod;

		m_presentQueue = std::make_unique< Queue >( *this
			, m_connection->getPresentQueueFamilyIndex() );
		m_presentCommandPool = std::make_unique< CommandPool >( *this
			, m_presentQueue->getFamilyIndex()
			, ashes::CommandPoolCreateFlag::eResetCommandBuffer | ashes::CommandPoolCreateFlag::eTransient );

		m_graphicsQueue = std::make_unique< Queue >( *this
			, m_connection->getPresentQueueFamilyIndex() );
		m_graphicsCommandPool = std::make_unique< CommandPool >( *this
			, m_graphicsQueue->getFamilyIndex()
			, ashes::CommandPoolCreateFlag::eResetCommandBuffer | ashes::CommandPoolCreateFlag::eTransient );

		m_computeQueue = std::make_unique< Queue >( *this
			, m_connection->getComputeQueueFamilyIndex() );
		m_computeCommandPool = std::make_unique< CommandPool >( *this
			, m_computeQueue->getFamilyIndex()
			, ashes::CommandPoolCreateFlag::eResetCommandBuffer | ashes::CommandPoolCreateFlag::eTransient );

		auto count = uint32_t( sizeof( dummyIndex ) / sizeof( dummyIndex[0] ) );
		m_dummyIndexed = ashes::makeBuffer< uint32_t >( *this
			, count
			, ashes::BufferTarget::eIndexBuffer | ashes::BufferTarget::eTransferDst
			, ashes::MemoryPropertyFlag::eHostVisible );

		if ( auto * buffer = m_dummyIndexed->lock( 0u
			, count
			, ashes::MemoryMapFlag::eWrite ) )
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
		safeRelease( m_device );

#if !defined( NDEBUG )

		m_debug->ReportLiveDeviceObjects( D3D11_RLDO_DETAIL | D3D11_RLDO_IGNORE_INTERNAL );
		safeRelease( m_debug );

#endif
	}

	ashes::StagingTexturePtr Device::createStagingTexture( ashes::Format format
		, ashes::Extent2D const & extent )const
	{
		return std::make_unique< StagingTexture >( *this, format, extent );
	}

	ashes::RenderPassPtr Device::createRenderPass( ashes::RenderPassCreateInfo createInfo )const
	{
		return std::make_unique< RenderPass >( *this, std::move( createInfo ) );
	}

	ashes::PipelineLayoutPtr Device::createPipelineLayout( ashes::DescriptorSetLayoutCRefArray const & setLayouts
			, ashes::PushConstantRangeArray const & pushConstantRanges )const
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
		HWND hWnd = m_connection->getHandle().getInternal< ashes::IMswWindowHandle >().getHwnd();
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

			D3D11_QUERY_DESC desc{};
			desc.Query = D3D11_QUERY_EVENT;
			m_device->CreateQuery( &desc, &m_waitIdleQuery );
		}
	}
}

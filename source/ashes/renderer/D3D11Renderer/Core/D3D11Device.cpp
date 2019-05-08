/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/D3D11Device.hpp"

#include "Buffer/D3D11Buffer.hpp"
#include "Buffer/D3D11BufferView.hpp"
#include "Command/D3D11CommandPool.hpp"
#include "Command/D3D11Queue.hpp"
#include "Core/D3D11Surface.hpp"
#include "Core/D3D11DummyIndexBuffer.hpp"
#include "Core/D3D11PhysicalDevice.hpp"
#include "Core/D3D11Instance.hpp"
#include "Core/D3D11SwapChain.hpp"
#include "Descriptor/D3D11DescriptorPool.hpp"
#include "Descriptor/D3D11DescriptorSetLayout.hpp"
#include "Image/D3D11Sampler.hpp"
#include "Image/D3D11Image.hpp"
#include "Image/D3D11ImageView.hpp"
#include "Miscellaneous/D3D11DeviceMemory.hpp"
#include "Miscellaneous/D3D11QueryPool.hpp"
#include "Pipeline/D3D11PipelineLayout.hpp"
#include "RenderPass/D3D11RenderPass.hpp"
#include "Shader/D3D11ShaderModule.hpp"
#include "Sync/D3D11Event.hpp"
#include "Sync/D3D11Fence.hpp"
#include "Sync/D3D11Semaphore.hpp"

#include <D3DCommon.h>

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
{
	namespace
	{
		ID3D11DeviceChild * getResource( VkDevice device
			, VkDebugReportObjectTypeEXT objectType
			, void const * object )
		{
			ID3D11DeviceChild * result = nullptr;

			switch ( objectType )
			{
			case VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT:
				break;
			case VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT:
				result = reinterpret_cast< Buffer const * >( object )->getBuffer();
				break;
			case VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT:
				result = reinterpret_cast< Image const * >( object )->getResource();
				break;
			case VK_DEBUG_REPORT_OBJECT_TYPE_QUERY_POOL_EXT:
				result = ( *reinterpret_cast< QueryPool const * >( object )->begin() );
				break;
			case VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_VIEW_EXT:
				result = reinterpret_cast< BufferView const * >( object )->getView();
				break;
			case VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_EXT:
				result = reinterpret_cast< Sampler const * >( object )->getSampler();
				break;
			default:
				break;
			}
			return result;
		}

		template< typename T, typename U >
		T getAligned( T value, U align )
		{
			return T( ( value + align - 1 ) & ~( align - 1 ) );
		};

		template< typename T >
		T getSubresourceValue( T value, uint32_t mipLevel )
		{
			return T( value >> mipLevel );
		};

		VkExtent3D getTexelBlockExtent( VkFormat format )
		{
			VkExtent3D texelBlockExtent{ 1u, 1u, 1u };

			if ( ashes::isCompressedFormat( format ) )
			{
				auto extent = ashes::getMinimalExtent2D( format );
				texelBlockExtent.width = extent.width;
				texelBlockExtent.height = extent.height;
			}
			else
			{
				texelBlockExtent.width = 1u;
			}

			return texelBlockExtent;
		}

		uint32_t getTexelBlockByteSize( VkExtent3D const & texelBlockExtent
			, VkFormat format )
		{
			VkDeviceSize texelBlockSize;

			if ( !isDepthStencilFormat( format ) )
			{
				texelBlockSize = getSize( texelBlockExtent, format );
			}
			else
			{
				texelBlockSize = texelBlockExtent.width;
			}

			return uint32_t( texelBlockSize );
		}
	}

	Device::Device( VkInstance instance
		, VkPhysicalDevice physicalDevice
		, VkDeviceCreateInfo createInfos )
		: m_instance{ instance }
		, m_physicalDevice{ physicalDevice }
		, m_createInfos{ std::move( createInfos ) }
	{
		doCreateD3D11Device();
		doCreateDummyIndexBuffer();
		doCreateQueues();
	}

	Device::~Device()
	{
		for ( auto creates : m_queues )
		{
			for ( auto queue : creates.second.queues )
			{
				deallocate( queue, nullptr );
			}
		}

		deallocate( m_dummyIndexed.memory, nullptr );
		deallocate( m_dummyIndexed.buffer, nullptr );

		m_deviceContext->ClearState();
		m_deviceContext->Flush();
		safeRelease( m_waitIdleQuery );
		safeRelease( m_deviceContext );
		safeRelease( m_d3dDevice );

#if !defined( NDEBUG )

		m_debug->ReportLiveDeviceObjects( D3D11_RLDO_DETAIL );
		safeRelease( m_debug );

#endif
	}

	void Device::getImageSubresourceLayout( VkImage image
		, VkImageSubresource const & subresource
		, VkSubresourceLayout & layout )const
	{
		auto extent = getTexelBlockExtent( get( image )->getFormat() );
		auto byteSize = getTexelBlockByteSize( extent, get( image )->getFormat() );
		auto mipWidth = getSubresourceValue( get( image )->getDimensions().width, subresource.mipLevel );
		auto mipHeight = getSubresourceValue( get( image )->getDimensions().height, subresource.mipLevel );
		layout.rowPitch = byteSize * mipWidth / ( extent.width * extent.height * extent.depth );
		layout.arrayPitch = layout.rowPitch * mipHeight * extent.height / ( extent.width * extent.depth );
		layout.depthPitch = layout.arrayPitch;
		layout.offset = subresource.arrayLayer * layout.arrayPitch;
		layout.size = layout.arrayPitch * get( image )->getDimensions().depth;
	}

	void Device::debugMarkerSetObjectName( VkDebugMarkerObjectNameInfoEXT const & nameInfo )const
	{
#if !defined( NDEBUG )
		auto * resource = getResource( get( this )
			, nameInfo.objectType
			, (void*)nameInfo.object );

		if ( resource )
		{
			resource->SetPrivateData( WKPDID_D3DDebugObjectName
				, UINT( strnlen( nameInfo.pObjectName, 256 ) )
				, nameInfo.pObjectName );
		}
#endif
	}

	VkQueue Device::getQueue( uint32_t familyIndex
		, uint32_t index )const
	{
		auto it = m_queues.find( familyIndex );

		if ( it == m_queues.end() )
		{
			throw ashes::Exception{ VK_ERROR_INITIALIZATION_FAILED, "Couldn't find family index within created queues" };
		}

		if ( it->second.queues.size() <= index )
		{
			throw ashes::Exception{ VK_ERROR_INITIALIZATION_FAILED, "Couldn't find queue with wanted index within its family" };
		}

		return it->second.queues[index];
	}

	VkResult Device::waitIdle()const
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

		return data
			? VK_SUCCESS
			: VK_TIMEOUT;
	}

	bool Device::onCopyToImageCommand( VkCommandBuffer cmd
		, VkBufferImageCopyArray const & copyInfo
		, VkBuffer src
		, VkImage dst )const
	{
		return get( m_instance )->onCopyToImageCommand( cmd, copyInfo, src, dst );
	}

	bool Device::onCheckHResultCommand( HRESULT hresult
		, std::string message )const
	{
		return get( m_instance )->onCheckHResultCommand( hresult, std::move( message ) );
	}

	void Device::doCreateD3D11Device()
	{
		auto factory = get( m_instance )->getDXGIFactory();

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
		HRESULT hr;
		UINT flags = 0;
		auto adapter = get( m_physicalDevice )->getAdapter();

#if !defined( NDEBUG )
		flags = D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_FEATURE_LEVEL supportedFeatureLevel = get( m_physicalDevice )->getFeatureLevel();
		hr = D3D11CreateDevice( nullptr
			, D3D_DRIVER_TYPE_HARDWARE
			, nullptr
			, flags
			, &supportedFeatureLevel
			, 1
			, D3D11_SDK_VERSION
			, &m_d3dDevice
			, &m_featureLevel
			, &m_deviceContext );

		if ( m_d3dDevice )
		{
#if !defined( NDEBUG )

			m_d3dDevice->QueryInterface( __uuidof( ID3D11Debug ), reinterpret_cast< void ** >( &m_debug ) );

#endif

			dxDebugName( m_d3dDevice, Device );

			D3D11_QUERY_DESC desc{};
			desc.Query = D3D11_QUERY_EVENT;
			m_d3dDevice->CreateQuery( &desc, &m_waitIdleQuery );
		}
	}

	void Device::doCreateDummyIndexBuffer()
	{
		auto size = uint32_t( sizeof( dummyIndex ) );
		auto count = uint32_t( size / sizeof( dummyIndex[0] ) );
		allocate( m_dummyIndexed.buffer
			, nullptr
			, get( this )
			, VkBufferCreateInfo
			{
				VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
				nullptr,
				0u,
				size,
				VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
				VK_SHARING_MODE_EXCLUSIVE,
				0u,
				nullptr,
			} );
		auto requirements = get( m_dummyIndexed.buffer )->getMemoryRequirements();
		auto deduced = deduceMemoryType( requirements.memoryTypeBits
			, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT );
		allocate( m_dummyIndexed.memory
			, nullptr
			, get( this )
			, VkMemoryAllocateInfo
			{
				VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
				nullptr,
				requirements.size,
				deduced
			} );
		get( m_dummyIndexed.buffer )->bindMemory( m_dummyIndexed.memory, 0u );
		uint32_t * buffer;

		if ( VK_SUCCESS == get( m_dummyIndexed.memory )->lock( 0u
			, size
			, 0u
			, reinterpret_cast< void ** >( &buffer ) ) )
		{
			std::copy( dummyIndex, dummyIndex + count, buffer );
			get( m_dummyIndexed.memory )->flush( 0, size );
			get( m_dummyIndexed.memory )->unlock();
		}
	}

	void Device::doCreateQueues()
	{
		for ( auto & queueCreateInfo : makeArrayView( m_createInfos.pQueueCreateInfos, m_createInfos.queueCreateInfoCount ) )
		{
			auto it = m_queues.emplace( queueCreateInfo.queueFamilyIndex
				, QueueCreates{ queueCreateInfo, {} } ).first;

			VkQueue queue;
			allocate( queue
				, nullptr
				, get( this )
				, it->second.createInfo
				, uint32_t( it->second.queues.size() ) );
			it->second.queues.emplace_back( queue );
		}
	}
}

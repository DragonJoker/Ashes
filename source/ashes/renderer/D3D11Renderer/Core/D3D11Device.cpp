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

#include <common/Hash.hpp>

#include <iomanip>

namespace ashes::d3d11
{
	namespace
	{
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

		size_t makeKey( VkImageType type
			, VkFormat format
			, VkExtent3D const & extent
			, uint32_t mipLevels )
		{
			auto result = std::hash< VkImageType >{}( type );
			ashes::hashCombine( result, format );
			ashes::hashCombine( result, extent.width );
			ashes::hashCombine( result, extent.height );
			ashes::hashCombine( result, extent.depth );
			ashes::hashCombine( result, mipLevels );
			return result;
		}

		void doCheckEnabledExtensions( ashes::ArrayView< char const * const > const & extensions )
		{
			auto & available = getSupportedDeviceExtensions();

			for ( auto & extension : extensions )
			{
				if ( available.end() == std::find_if( available.begin()
					, available.end()
					, [&extension]( VkExtensionProperties const & lookup )
					{
						return lookup.extensionName == std::string{ extension };
					} ) )
				{
					throw ashes::Exception{ VK_ERROR_EXTENSION_NOT_PRESENT, extension };
				}
			}
		}
	}

	Device::Device( VkInstance instance
		, VkPhysicalDevice physicalDevice
		, VkDeviceCreateInfo createInfos )
		: m_instance{ instance }
		, m_physicalDevice{ physicalDevice }
		, m_createInfos{ std::move( createInfos ) }
	{
		doCheckEnabledExtensions( ashes::makeArrayView( m_createInfos.ppEnabledExtensionNames, m_createInfos.enabledExtensionCount ) );
		doCreateD3D11Device();
		doCreateDummyIndexBuffer();
		doCreateQueues();
		allocate( m_sampler
			, nullptr
			, get( this )
			, VkSamplerCreateInfo
			{
				VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
				nullptr,
				0u,
				VK_FILTER_NEAREST,
				VK_FILTER_NEAREST,
				VK_SAMPLER_MIPMAP_MODE_NEAREST,
				VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
				VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
				VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
				0.0f,
				VK_FALSE,
				1.0f,
				VK_FALSE,
				VK_COMPARE_OP_ALWAYS,
				0.0f,
				1.0f,
				VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK,
				VK_FALSE,
			} );
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

		for ( auto & it : m_stagingTextures )
		{
			deallocate( it.second.first, nullptr );
			deallocate( it.second.second, nullptr );
		}

		deallocate( m_sampler, nullptr );
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

	VkPhysicalDeviceLimits const & Device::getLimits()const
	{
		return get( m_physicalDevice )->getProperties().limits;
	}

	VkImage Device::getStagingImage( VkImage image
		, VkDeviceMemory & memory )
	{
		auto key = makeKey( get( image )->getType()
			, get( image )->getFormat()
			, get( image )->getDimensions()
			, get( image )->getMipmapLevels() );
		auto it = m_stagingTextures.find( key );

		if ( it == m_stagingTextures.end() )
		{
			VkImage result;
			allocate( result
				, nullptr
				, get( this )
				, VkImageCreateInfo
				{
					VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
					nullptr,
					0u,
					get( image )->getType(),
					get( image )->getFormat(),
					get( image )->getDimensions(),
					get( image )->getMipmapLevels(),
					1u, // layerCount, we process images layer per layer.
					VK_SAMPLE_COUNT_1_BIT,
					VK_IMAGE_TILING_LINEAR,
					VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
					VK_SHARING_MODE_EXCLUSIVE,
					0u,
					nullptr,
					VK_IMAGE_LAYOUT_UNDEFINED,
				} );
			auto requirements = get( result )->getMemoryRequirements();
			uint32_t deduced = deduceMemoryType( requirements.memoryTypeBits
				, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT );
			VkDeviceMemory resMem;
			allocate( resMem
				, nullptr
				, get( this )
				, VkMemoryAllocateInfo
				{
					VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
					nullptr,
					requirements.size,
					deduced
				} );
			get( result )->bindMemory( resMem, 0u );
			it = m_stagingTextures.emplace( key, std::make_pair( result, resMem ) ).first;
		}

		memory = it->second.second;
		return it->second.first;
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

#if VK_EXT_debug_utils

	VkResult Device::setDebugUtilsObjectName( VkDebugUtilsObjectNameInfoEXT const & nameInfo )const
	{
		HRESULT hr = S_OK;

		switch ( nameInfo.objectType )
		{
		case VK_OBJECT_TYPE_DEVICE:
			hr = get( VkDevice( nameInfo.objectHandle ) )->getDevice()->SetPrivateData( WKPDID_D3DDebugObjectName
				, UINT( strlen( nameInfo.pObjectName ) )
				, nameInfo.pObjectName );
			break;
		case VK_OBJECT_TYPE_SWAPCHAIN_KHR:
			hr = get( VkSwapchainKHR( nameInfo.objectHandle ) )->getSwapChain()->SetPrivateData( WKPDID_D3DDebugObjectName
				, UINT( strlen( nameInfo.pObjectName ) )
				, nameInfo.pObjectName );
			break;
		case VK_OBJECT_TYPE_BUFFER:
			get( VkBuffer( nameInfo.objectHandle ) )->setDebugName( nameInfo.pObjectName );
			break;
		case VK_OBJECT_TYPE_IMAGE:
			get( VkImage( nameInfo.objectHandle ) )->setDebugName( nameInfo.pObjectName );
			break;
		default:
			{
				auto object = getObject( nameInfo.objectHandle, nameInfo.objectType );

				if ( object )
				{
					hr = object->SetPrivateData( WKPDID_D3DDebugObjectName
						, UINT( strlen( nameInfo.pObjectName ) )
						, nameInfo.pObjectName );
				}
			}
			break;
		}

		if ( hr == S_OK )
		{
			std::clog << "0x" << std::hex << std::setfill( '0' ) << std::setw( 16u ) << nameInfo.objectHandle << " - " << nameInfo.pObjectName << "\n";
		}

		return checkError( get( this ), hr, "SetPrivateData" )
			? VK_SUCCESS
			: VK_ERROR_INVALID_DEVICE_ADDRESS_EXT;
	}

	VkResult Device::setDebugUtilsObjectTag( VkDebugUtilsObjectTagInfoEXT const & tagInfo )const
	{
		return VK_SUCCESS;
	}

	void Device::submitDebugUtilsMessenger( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity
		, VkDebugUtilsMessageTypeFlagsEXT messageTypes
		, VkDebugUtilsMessengerCallbackDataEXT const & callbackData )const
	{
		get( m_instance )->submitDebugUtilsMessenger( messageSeverity
			, messageTypes
			, callbackData );
	}

	void Device::onSubmitDebugUtilsMessenger( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity
		, VkDebugUtilsMessageTypeFlagsEXT messageTypes
		, VkDebugUtilsMessengerCallbackDataEXT const & callbackData )const
	{
		get( m_instance )->onSubmitDebugUtilsMessenger( messageSeverity
			, messageTypes
			, callbackData );
	}

#endif
#if VK_EXT_debug_marker

	VkResult Device::debugMarkerSetObjectTag( VkDebugMarkerObjectTagInfoEXT const & tagInfo )const
	{
		return VK_SUCCESS;
	}

	VkResult Device::debugMarkerSetObjectName( VkDebugMarkerObjectNameInfoEXT const & nameInfo )const
	{
		HRESULT hr = S_OK;

		switch ( nameInfo.objectType )
		{
		case VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_EXT:
			hr = get( VkDevice( nameInfo.object ) )->getDevice()->SetPrivateData( WKPDID_D3DDebugObjectName
				, UINT( strlen( nameInfo.pObjectName ) )
				, nameInfo.pObjectName );
			break;
		case VK_DEBUG_REPORT_OBJECT_TYPE_SWAPCHAIN_KHR_EXT:
			hr = get( VkSwapchainKHR( nameInfo.object ) )->getSwapChain()->SetPrivateData( WKPDID_D3DDebugObjectName
				, UINT( strlen( nameInfo.pObjectName ) )
				, nameInfo.pObjectName );
			break;
		case VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT:
			get( VkBuffer( nameInfo.object ) )->setDebugName( nameInfo.pObjectName );
			break;
		case VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT:
			get( VkImage( nameInfo.object ) )->setDebugName( nameInfo.pObjectName );
			break;
		default:
			{
				auto object = getObject( nameInfo.object, nameInfo.objectType );

				if ( object )
				{
					hr = object->SetPrivateData( WKPDID_D3DDebugObjectName
						, UINT( strlen( nameInfo.pObjectName ) )
						, nameInfo.pObjectName );
				}
			}
			break;
		}

		if ( hr == S_OK )
		{
			std::clog << "0x" << std::hex << std::setfill( '0' ) << std::setw( 16u ) << nameInfo.object << " - " << nameInfo.pObjectName << "\n";
		}

		return checkError( get( this ), hr, "SetPrivateData" )
			? VK_SUCCESS
			: VK_ERROR_INVALID_DEVICE_ADDRESS_EXT;
	}

#endif
#if VK_EXT_debug_report

	void Device::reportMessage( VkDebugReportFlagsEXT flags
		, VkDebugReportObjectTypeEXT objectType
		, uint64_t object
		, size_t location
		, int32_t messageCode
		, const char * pLayerPrefix
		, const char * pMessage )
	{
		get( m_instance )->reportMessage( flags
			, objectType
			, object
			, location
			, messageCode
			, pLayerPrefix
			, pMessage );
	}

	void Device::onReportMessage( VkDebugReportFlagsEXT flags
		, VkDebugReportObjectTypeEXT objectType
		, uint64_t object
		, size_t location
		, int32_t messageCode
		, const char * pLayerPrefix
		, const char * pMessage )
	{
		get( m_instance )->onReportMessage( flags
			, objectType
			, object
			, location
			, messageCode
			, pLayerPrefix
			, pMessage );
	}

#endif

	VkQueue Device::getQueue( uint32_t familyIndex
		, uint32_t index )const
	{
		auto it = m_queues.find( familyIndex );

		if ( it == m_queues.end() )
		{
			get( m_instance )->onReportMessage( VK_DEBUG_REPORT_ERROR_BIT_EXT
				, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_EXT
				, uint64_t( get( this ) )
				, 0u
				, VK_ERROR_INCOMPATIBLE_DRIVER
				, "Direct3D11"
				, "Couldn't find family index within created queues" );
			return VK_NULL_HANDLE;
		}

		if ( it->second.queues.size() <= index )
		{
			get( m_instance )->onReportMessage( VK_DEBUG_REPORT_ERROR_BIT_EXT
				, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_EXT
				, uint64_t( get( this ) )
				, 0u
				, VK_ERROR_INCOMPATIBLE_DRIVER
				, "Direct3D11"
				, "Couldn't find queue with wanted index within its family" );
			return VK_NULL_HANDLE;
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

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

#include <Ashes/Core/PlatformWindowHandle.hpp>
#include <Ashes/Image/ImageSubresource.hpp>
#include <Ashes/Image/SubresourceLayout.hpp>
#include <Ashes/Miscellaneous/MemoryRequirements.hpp>
#include <Ashes/RenderPass/RenderPassCreateInfo.hpp>

#include <D3DCommon.h>

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
			case VkDebugReportObjectTypeEXT::eDeviceMemory:
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
			case VkDebugReportObjectTypeEXT::eBufferView:
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
			uint32_t texelBlockSize;

			if ( !isDepthStencilFormat( format ) )
			{
				texelBlockSize = getSize( texelBlockExtent, format );
			}
			else
			{
				texelBlockSize = texelBlockExtent.width;
			}

			return texelBlockSize;
		}
	}

	Device::Device( Instance const & instance
		, PhysicalDevice const & physicalDevice
		, VkDeviceCreateInfo createInfos )
		: ashes::Device{ instance
			, physicalDevice
			, std::move( createInfos ) }
		, m_instance{ instance }
		, m_physicalDevice{ physicalDevice }
	{
		doCreateD3D11Device();
		m_timestampPeriod = m_gpu.getProperties().limits.timestampPeriod;
		doCreateDummyIndexBuffer();
		doCreateQueues();
	}

	Device::~Device()
	{
		m_dummyIndexed.reset();

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

	VkRenderPass Device::createRenderPass( VkRenderPassCreateInfo createInfo )const
	{
		return std::make_unique< RenderPass >( *this, std::move( createInfo ) );
	}

	VkPipelineLayout Device::createPipelineLayout( ashes::VkDescriptorSetLayoutArray const & setLayouts
			, ashes::VkPushConstantRangeArray const & pushConstantRanges )const
	{
		return std::make_unique< PipelineLayout >( *this
			, setLayouts
			, pushConstantRanges );
	}

	VkDescriptorSetLayout Device::createDescriptorSetLayout( VkDescriptorSetLayoutBindingArray bindings )const
	{
		return std::make_unique< DescriptorSetLayout >( *this, std::move( bindings ) );
	}

	VkDescriptorPool Device::createDescriptorPool( VkDescriptorPoolCreateFlags flags
		, uint32_t maxSets
		, ashes::VkDescriptorPoolSizeArray poolSizes )const
	{
		return std::make_unique< DescriptorPool >( *this, flags, maxSets, poolSizes );
	}

	VkDeviceMemory Device::allocateMemory( VkMemoryAllocateInfo allocateInfo )const
	{
		return std::make_unique< DeviceMemory >( *this
			, std::move( allocateInfo ) );
	}

	VkImage Device::createImage( VkImageCreateInfo const & createInfo )const
	{
		return std::make_unique< Image >( *this, createInfo );
	}

	void Device::getImageSubresourceLayout( VkImage image
		, VkImageSubresource const & subresource
		, VkSubresourceLayout & layout )const
	{
		auto extent = getTexelBlockExtent( image.getFormat() );
		auto byteSize = getTexelBlockByteSize( extent, image.getFormat() );
		auto mipWidth = getSubresourceValue( image.getDimensions().width, subresource.mipLevel );
		auto mipHeight = getSubresourceValue( image.getDimensions().height, subresource.mipLevel );
		layout.rowPitch = byteSize * mipWidth / ( extent.width * extent.height * extent.depth );
		layout.arrayPitch = layout.rowPitch * mipHeight * extent.height / ( extent.width * extent.depth );
		layout.depthPitch = layout.arrayPitch;
		layout.offset = subresource.arrayLayer * layout.arrayPitch * byteSize;
		layout.size = layout.arrayPitch * image.getDimensions().depth;
	}

	VkSampler Device::createSampler( VkSamplerCreateInfo const & createInfo )const
	{
		return std::make_unique< Sampler >( *this, createInfo );
	}

	VkBuffer Device::createBuffer( uint32_t size
		, VkBufferUsageFlags target )const
	{
		return std::make_unique< Buffer >( *this
			, size
			, target );
	}

	VkBufferView Device::createBufferView( VkBuffer buffer
		, VkFormat format
		, uint32_t offset
		, uint32_t range )const
	{
		return std::make_unique< BufferView >( *this
			, static_cast< Buffer const & >( buffer )
			, format
			, offset
			, range );
	}

	VkSwapchainKHR Device::createSwapChain( VkSwapchainCreateInfoKHR createInfo )const
	{
		VkSwapchainKHR result;

		try
		{
			result = std::make_unique< SwapChain >( *this, std::move( createInfo ) );
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

	VkSemaphore Device::createSemaphore()const
	{
		return std::make_unique< Semaphore >( *this );
	}

	VkFence Device::createFence( VkFenceCreateFlags flags )const
	{
		return std::make_unique< Fence >( *this, flags );
	}

	VkEvent Device::createEvent()const
	{
		return std::make_unique< Event >( *this );
	}

	VkCommandPool Device::createCommandPool( uint32_t queueFamilyIndex
		, VkCommandPoolCreateFlags const & flags )const
	{
		return std::make_unique< CommandPool >( *this
			, queueFamilyIndex
			, flags );
	}

	VkShaderModule Device::createShaderModule( VkShaderStageFlagBits stage )const
	{
		return std::make_shared< ShaderModule >( *this, stage );
	}

	VkQueryPool Device::createQueryPool( VkQueryType type
		, uint32_t count
		, VkQueryPipelineStatisticFlags pipelineStatistics )const
	{
		return std::make_unique< QueryPool >( *this
			, type
			, count
			, pipelineStatistics );
	}

	void Device::debugMarkerSetObjectName( VkDebugMarkerObjectNameInfoEXT const & nameInfo )const
	{
#if !defined( NDEBUG )
		auto * resource = getResource( *this, nameInfo.objectType, nameInfo.object );

		if ( resource )
		{
			resource->SetPrivateData( WKPDID_D3DDebugObjectName
				, UINT( nameInfo.objectName.size() )
				, nameInfo.objectName.c_str() );
		}
#endif
	}

	VkQueue Device::getQueue( uint32_t familyIndex
		, uint32_t index )const
	{
		auto it = m_queues.find( familyIndex );

		if ( it == m_queues.end() )
		{
			throw ashes::Exception{ ashes::Result::eErrorRenderer, "Couldn't find family index within created queues" };
		}

		if ( it->second.second <= index )
		{
			throw ashes::Exception{ ashes::Result::eErrorRenderer, "Couldn't find queue with wanted index within its family" };
		}

		return std::make_unique< Queue >( *this, it->second.first, index );
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

	bool Device::onCopyToImageCommand( VkCommandBuffer cmd
		, ashes::VkBufferImageCopyArray const & copyInfo
		, VkBuffer src
		, VkImage dst )const
	{
		return m_instance.onCopyToImageCommand( cmd, copyInfo, src, dst );
	}

	bool Device::onCheckHResultCommand( HRESULT hresult
		, std::string message )const
	{
		return m_instance.onCheckHResultCommand( hresult, std::move( message ) );
	}

	void Device::doCreateD3D11Device()
	{
		auto factory = m_instance.getDXGIFactory();

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
		//HWND hWnd = m_surface->getHandle().getInternal< ashes::IMswWindowHandle >().getHwnd();
		//hr = factory->MakeWindowAssociation( hWnd, 0 );

		//if ( SUCCEEDED( hr ) )
		//{
		//	HWND hWnd2;
		//	hr = factory->GetWindowAssociation( &hWnd2 );

		//	if ( hWnd2 )
		//	{
		//		hWnd = hWnd2;
		//	}
		//}

		UINT flags = 0;
		auto adapter = m_physicalDevice.getAdapter();

#if !defined( NDEBUG )
		flags = D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_FEATURE_LEVEL supportedFeatureLevel = m_physicalDevice.getFeatureLevel();
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
		auto count = uint32_t( sizeof( dummyIndex ) / sizeof( dummyIndex[0] ) );
		m_dummyIndexed = ashes::makeBuffer< uint32_t >( *this
			, count
			, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT );
		auto requirements = m_dummyIndexed->getBuffer().getMemoryRequirements();
		auto deduced = deduceMemoryType( requirements.memoryTypeBits
			, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT );
		m_dummyIndexed->bindMemory( allocateMemory( { requirements.size, deduced } ) );

		if ( auto * buffer = m_dummyIndexed->lock( 0u
			, count
			, VkMemoryMapFlagBits::eWrite ) )
		{
			std::copy( dummyIndex, dummyIndex + count, buffer );
			m_dummyIndexed->flush( 0, count );
			m_dummyIndexed->unlock();
		}
	}

	void Device::doCreateQueues()
	{
		for ( auto & queueCreateInfo : m_createInfos.queueCreateInfos )
		{
			auto it = m_queues.emplace( queueCreateInfo.queueFamilyIndex
				, QueueCreateCount{ queueCreateInfo, 0u } ).first;
			it->second.second++;
		}
	}
}

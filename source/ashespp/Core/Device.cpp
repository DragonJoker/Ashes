/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Core/Device.hpp"

#include "ashespp/AccelerationStructure/AccelerationStructure.hpp"
#include "ashespp/Buffer/Buffer.hpp"
#include "ashespp/Buffer/BufferView.hpp"
#include "ashespp/Core/Instance.hpp"
#include "ashespp/Core/SwapChain.hpp"
#include "ashespp/Descriptor/DescriptorPool.hpp"
#include "ashespp/Descriptor/DescriptorSetLayout.hpp"
#include "ashespp/Image/Image.hpp"
#include "ashespp/Image/Sampler.hpp"
#include "ashespp/Image/StagingTexture.hpp"
#include "ashespp/Miscellaneous/DeferredOperation.hpp"
#include "ashespp/Miscellaneous/QueryPool.hpp"
#include "ashespp/Pipeline/PipelineLayout.hpp"
#include "ashespp/Pipeline/RayTracingPipeline.hpp"
#include "ashespp/RenderPass/RenderPass.hpp"
#include "ashespp/Shader/ShaderModule.hpp"
#include "ashespp/Sync/Event.hpp"
#include "ashespp/Sync/Fence.hpp"
#include "ashespp/Sync/Semaphore.hpp"

#include <ashes/common/Exception.hpp>

#pragma warning( disable: 4191 )

namespace ashes
{
	Device::Device( Instance const & instance
		, VkAllocationCallbacks const * callbacks
		, PhysicalDevice const & physicalDevice
		, ashes::DeviceCreateInfo createInfos )
		: m_instance{ instance }
		, m_callbacks{ callbacks }
		, m_physicalDevice{ physicalDevice }
		, m_createInfos{ std::move( createInfos ) }
		, m_memoryProperties{ m_physicalDevice.getMemoryProperties() }
		, m_properties{ m_physicalDevice.getProperties() }
		, m_features{ m_physicalDevice.getFeatures() }
		, m_queueFamilyProperties{ m_physicalDevice.getQueueFamilyProperties() }
	{
		m_timestampPeriod = m_physicalDevice.getProperties().limits.timestampPeriod;
		DEBUG_DUMP( m_createInfos );
		auto res = m_physicalDevice.vkCreateDevice( m_physicalDevice
			, &static_cast< VkDeviceCreateInfo const & >( m_createInfos )
			, nullptr
			, &m_internal );
		checkError( res, "LogicalDevice creation" );

		vkGetDeviceProcAddr = reinterpret_cast< PFN_vkGetDeviceProcAddr >( m_instance.vkGetDeviceProcAddr( m_internal, "vkGetDeviceProcAddr" ) );
#define VK_LIB_DEVICE_FUNCTION( ver, fun )\
		vk##fun = reinterpret_cast< PFN_vk##fun >( m_instance.vkGetDeviceProcAddr( m_internal, "vk"#fun ) );
#define VK_LIB_DEVICE_FUNCTION_EXT( ver, ext, fun )\
		vk##fun = reinterpret_cast< PFN_vk##fun >( m_instance.vkGetDeviceProcAddr( m_internal, "vk"#fun ) );
#include <ashes/ashes_functions_list.hpp>
	}

	Device::~Device()
	{
		reportRegisteredObjects();
		vkDestroyDevice( m_internal, nullptr );
	}

	std::array< float, 16u > Device::frustum( float left
		, float right
		, float bottom
		, float top
		, float zNear
		, float zFar )const
	{
		return m_instance.frustum( left, right, bottom, top, zNear, zFar );
	}

	std::array< float, 16u > Device::perspective( float radiansFovY
		, float aspect
		, float zNear
		, float zFar )const
	{
		return m_instance.perspective( radiansFovY, aspect, zNear, zFar );
	}

	std::array< float, 16u > Device::ortho( float left
		, float right
		, float bottom
		, float top
		, float zNear
		, float zFar )const
	{
		return m_instance.ortho( left, right, bottom, top, zNear, zFar );
	}

	std::array< float, 16u > Device::infinitePerspective( float radiansFovY
		, float aspect
		, float zNear )const
	{
		return m_instance.infinitePerspective( radiansFovY, aspect, zNear );
	}

	StagingTexturePtr Device::createStagingTexture( VkFormat format
		, VkExtent2D const & extent
		, uint32_t mipLevels )const
	{
		return std::make_unique< StagingTexture >( *this
			, format
			, extent
			, mipLevels );
	}

	StagingTexturePtr Device::createStagingTexture( VkFormat format
		, VkExtent3D const & extent
		, uint32_t mipLevels )const
	{
		return std::make_unique< StagingTexture >( *this
			, format
			, extent
			, mipLevels );
	}

	StagingTexturePtr Device::createStagingTexture( std::string const & debugName
		, VkFormat format
		, VkExtent2D const & extent
		, uint32_t mipLevels )const
	{
		return std::make_unique< StagingTexture >( *this
			, debugName
			, format
			, extent
			, mipLevels );
	}

	StagingTexturePtr Device::createStagingTexture( std::string const & debugName
		, VkFormat format
		, VkExtent3D const & extent
		, uint32_t mipLevels )const
	{
		return std::make_unique< StagingTexture >( *this
			, debugName
			, format
			, extent
			, mipLevels );
	}

	RenderPassPtr Device::createRenderPass( RenderPassCreateInfo createInfo )const
	{
		return std::make_unique< RenderPass >( *this
			, std::move( createInfo ) );
	}

	RenderPassPtr Device::createRenderPass( std::string const & debugName
		, RenderPassCreateInfo createInfo )const
	{
		return std::make_unique< RenderPass >( *this
			, debugName
			, std::move( createInfo ) );
	}

	GraphicsPipelinePtr Device::createPipeline( GraphicsPipelineCreateInfo createInfo )const
	{
		return std::make_unique< GraphicsPipeline >( *this
			, std::move( createInfo ) );
	}

	GraphicsPipelinePtr Device::createPipeline( std::string const & debugName
		, GraphicsPipelineCreateInfo createInfo )const
	{
		return std::make_unique< GraphicsPipeline >( *this
			, debugName
			, std::move( createInfo ) );
	}

	ComputePipelinePtr Device::createPipeline( ComputePipelineCreateInfo createInfo )const
	{
		return std::make_unique< ComputePipeline >( *this
			, std::move( createInfo ) );
	}

	ComputePipelinePtr Device::createPipeline( std::string const & debugName
		, ComputePipelineCreateInfo createInfo )const
	{
		return std::make_unique< ComputePipeline >( *this
			, debugName
			, std::move( createInfo ) );
	}

#if VK_KHR_ray_tracing_pipeline

	RayTracingPipelinePtr Device::createPipeline( DeferredOperation const & deferredOperation
		, RayTracingPipelineCreateInfoArray createInfos )const
	{
		return std::make_unique< RayTracingPipeline >( *this
			, deferredOperation
			, std::move( createInfos ) );
	}

	RayTracingPipelinePtr Device::createPipeline( std::string const & debugName
		, DeferredOperation const & deferredOperation
		, RayTracingPipelineCreateInfoArray createInfos )const
	{
		return std::make_unique< RayTracingPipeline >( *this
			, debugName
			, deferredOperation
			, std::move( createInfos ) );
	}

#endif

	PipelineLayoutPtr Device::createPipelineLayout( DescriptorSetLayoutCRefArray const & setLayouts
		, VkPushConstantRangeArray const & pushConstantRanges )const
	{
		return std::make_unique< PipelineLayout >( *this
			, setLayouts
			, pushConstantRanges );
	}

	PipelineLayoutPtr Device::createPipelineLayout( std::string const & debugName
		, DescriptorSetLayoutCRefArray const & setLayouts
		, VkPushConstantRangeArray const & pushConstantRanges )const
	{
		return std::make_unique< PipelineLayout >( *this
			, debugName
			, setLayouts
			, pushConstantRanges );
	}

	DescriptorSetLayoutPtr Device::createDescriptorSetLayout( std::string const & debugName
		, VkDescriptorSetLayoutCreateInfo createInfo )const
	{
		return std::make_unique< DescriptorSetLayout >( *this
			, debugName
			, std::move( createInfo ) );
	}

	DescriptorSetLayoutPtr Device::createDescriptorSetLayout( VkDescriptorSetLayoutBindingArray bindings
		, VkDescriptorBindingFlags flags )const
	{
		return std::make_unique< DescriptorSetLayout >( *this
			, std::move( bindings )
			, flags );
	}

	DescriptorSetLayoutPtr Device::createDescriptorSetLayout( std::string const & debugName
		, VkDescriptorSetLayoutBindingArray bindings
		, VkDescriptorBindingFlags flags )const
	{
		return std::make_unique< DescriptorSetLayout >( *this
			, debugName
			, std::move( bindings )
			, flags );
	}

	DescriptorPoolPtr Device::createDescriptorPool( VkDescriptorPoolCreateFlags flags
		, uint32_t maxSets
		, VkDescriptorPoolSizeArray poolSizes )const
	{
		return std::make_unique< DescriptorPool >( *this
			, flags
			, maxSets
			, poolSizes );
	}

	DescriptorPoolPtr Device::createDescriptorPool( std::string const & debugName
		, VkDescriptorPoolCreateFlags flags
		, uint32_t maxSets
		, VkDescriptorPoolSizeArray poolSizes )const
	{
		return std::make_unique< DescriptorPool >( *this
			, debugName
			, flags
			, maxSets
			, poolSizes );
	}

	DeviceMemoryPtr Device::allocateMemory( VkMemoryAllocateInfo allocateInfo )const
	{
		return std::make_unique< DeviceMemory >( *this
			, std::move( allocateInfo ) );
	}

	DeviceMemoryPtr Device::allocateMemory( std::string const & debugName
		, VkMemoryAllocateInfo allocateInfo )const
	{
		return std::make_unique< DeviceMemory >( *this
			, debugName
			, std::move( allocateInfo ) );
	}

	ImagePtr Device::createImage( ImageCreateInfo createInfo )const
	{
		return std::make_unique< Image >( *this
			, std::move( createInfo ) );
	}

	ImagePtr Device::createImage( std::string const & debugName
		, ImageCreateInfo createInfo )const
	{
		return std::make_unique< Image >( *this
			, debugName
			, std::move( createInfo ) );
	}

	void Device::getImageSubresourceLayout( Image const & image
		, VkImageSubresource const & subresource
		, VkSubresourceLayout & layout )const
	{
		vkGetImageSubresourceLayout( m_internal
			, image
			, &subresource
			, &layout );
	}

	SamplerPtr Device::createSampler( SamplerCreateInfo createInfo )const
	{
		return std::make_unique< Sampler >( *this
			, std::move( createInfo ) );
	}

	SamplerPtr Device::createSampler( std::string const & debugName
		, SamplerCreateInfo createInfo )const
	{
		return std::make_unique< Sampler >( *this
			, debugName
			, std::move( createInfo ) );
	}

	SamplerPtr Device::createSampler( VkSamplerCreateInfo createInfo )const
	{
		return std::make_unique< Sampler >( *this
			, std::move( createInfo ) );
	}

	SamplerPtr Device::createSampler( std::string const & debugName
		, VkSamplerCreateInfo createInfo )const
	{
		return std::make_unique< Sampler >( *this
			, debugName
			, std::move( createInfo ) );
	}

	BufferBasePtr Device::createBuffer( VkDeviceSize size
		, VkBufferUsageFlags usage
		, QueueShare sharingMode )const
	{
		return std::make_unique< BufferBase >( *this
			, size
			, usage
			, std::move( sharingMode ) );
	}

	BufferBasePtr Device::createBuffer( std::string const & debugName
		, VkDeviceSize size
		, VkBufferUsageFlags usage
		, QueueShare sharingMode )const
	{
		return std::make_unique< BufferBase >( *this
			, debugName
			, size
			, usage
			, std::move( sharingMode ) );
	}

	BufferViewPtr Device::createBufferView( BufferBase const & buffer
		, VkFormat format
		, VkDeviceSize offset
		, VkDeviceSize range )const
	{
		return std::make_unique< BufferView >( *this
			, buffer
			, format
			, offset
			, range );
	}

	BufferViewPtr Device::createBufferView( std::string const & debugName
		, BufferBase const & buffer
		, VkFormat format
		, VkDeviceSize offset
		, VkDeviceSize range )const
	{
		return std::make_unique< BufferView >( *this
			, debugName
			, buffer
			, format
			, offset
			, range );
	}

	SwapChainPtr Device::createSwapChain( VkSwapchainCreateInfoKHR createInfo )const
	{
		SwapChainPtr result;

		try
		{
			result = std::make_unique< SwapChain >( *this
				, std::move( createInfo ) );
		}
		catch ( Exception & exc )
		{
			log::error << "Could not create the swap chain:\n" << exc.what() << "\n";
		}
		catch ( ... )
		{
			log::error << "Could not create the swap chain:\nUnknown error\n";
		}

		return result;
	}

	SwapChainPtr Device::createSwapChain( std::string const & debugName
		, VkSwapchainCreateInfoKHR createInfo )const
	{
		SwapChainPtr result;

		try
		{
			result = std::make_unique< SwapChain >( *this
				, debugName
				, std::move( createInfo ) );
		}
		catch ( Exception & exc )
		{
			log::error << "Could not create the swap chain:\n" << exc.what() << "\n";
		}
		catch ( ... )
		{
			log::error << "Could not create the swap chain:\nUnknown error\n";
		}

		return result;
	}

	SemaphorePtr Device::createSemaphore()const
	{
		return std::make_unique< Semaphore >( *this );
	}

	SemaphorePtr Device::createSemaphore( std::string const & debugName )const
	{
		return std::make_unique< Semaphore >( *this
				, debugName );
	}

	FencePtr Device::createFence( VkFenceCreateFlags flags )const
	{
		return std::make_unique< Fence >( *this
			, flags );
	}

	FencePtr Device::createFence( std::string const & debugName
		, VkFenceCreateFlags flags )const
	{
		return std::make_unique< Fence >( *this
			, debugName
			, flags );
	}

	EventPtr Device::createEvent()const
	{
		return std::make_unique< Event >( *this );
	}

	EventPtr Device::createEvent( std::string const & debugName )const
	{
		return std::make_unique< Event >( *this
				, debugName );
	}

	CommandPoolPtr Device::createCommandPool( uint32_t queueFamilyIndex
		, VkCommandPoolCreateFlags const & flags )const
	{
		return std::make_unique< CommandPool >( *this
			, queueFamilyIndex
			, flags );
	}

	CommandPoolPtr Device::createCommandPool( std::string const & debugName
		, uint32_t queueFamilyIndex
		, VkCommandPoolCreateFlags const & flags )const
	{
		return std::make_unique< CommandPool >( *this
			, debugName
			, queueFamilyIndex
			, flags );
	}

	ShaderModulePtr Device::createShaderModule( UInt32Array const & shader )const
	{
		return std::make_shared< ShaderModule >( *this
			, shader );
	}

	ShaderModulePtr Device::createShaderModule( std::string const & debugName
		, UInt32Array const & shader )const
	{
		return std::make_shared< ShaderModule >( *this
			, debugName
			, shader );
	}

	QueryPoolPtr Device::createQueryPool( VkQueryType type
		, uint32_t count
		, VkQueryPipelineStatisticFlags pipelineStatistics )const
	{
		return std::make_unique< QueryPool >( *this
			, type
			, count
			, pipelineStatistics );
	}

	QueryPoolPtr Device::createQueryPool( std::string const & debugName
		, VkQueryType type
		, uint32_t count
		, VkQueryPipelineStatisticFlags pipelineStatistics )const
	{
		return std::make_unique< QueryPool >( *this
			, debugName
			, type
			, count
			, pipelineStatistics );
	}

#if VK_KHR_deferred_host_operations

	DeferredOperationPtr Device::createDeferredOperation( std::string const & debugName )const
	{
		return std::make_unique< DeferredOperation >( *this
			, debugName );
	}

	DeferredOperationPtr Device::createDeferredOperation()const
	{
		return std::make_unique< DeferredOperation >( *this );
	}

#endif
#if VK_KHR_acceleration_structure

	AccelerationStructurePtr Device::createAccelerationStructure( std::string const & debugName
		, VkAccelerationStructureCreateInfoKHR infos )const
	{
		return std::make_unique< AccelerationStructure >( *this
			, debugName
			, std::move( infos ) );
	}

	AccelerationStructurePtr Device::createAccelerationStructure( VkAccelerationStructureCreateInfoKHR infos )const
	{
		return std::make_unique< AccelerationStructure >( *this
			, std::move( infos ) );
	}

	void Device::buildAccelerationStructuresKHR( VkDeferredOperationKHR deferredOperation
		, VkAccelerationStructureBuildGeometryInfoArray const & infos
		, VkAccelerationStructureBuildRangeInfoPtrArray const & buildRangeInfos )const
	{
		auto res = vkBuildAccelerationStructuresKHR( m_internal
			, deferredOperation
			, uint32_t( infos.size() )
			, infos.data()
			, buildRangeInfos.data() );
		checkError( res, "vkBuildAccelerationStructuresKHR" );
	}

	void Device::writeAccelerationStructuresPropertiesKHR( VkAccelerationStructureArray const & accelerationStructures
		, VkQueryType queryType
		, ByteArray data
		, size_t stride )const
	{
		auto res = vkWriteAccelerationStructuresPropertiesKHR( m_internal
			, uint32_t( accelerationStructures.size() )
			, accelerationStructures.data()
			, queryType
			, data.size()
			, data.data()
			, stride );
		checkError( res, "vkWriteAccelerationStructuresPropertiesKHR" );
	}

	VkAccelerationStructureCompatibilityKHR Device::getDeviceAccelerationStructureCompatibilityKHR( VkAccelerationStructureVersionInfoKHR const & versionInfo )const
	{
		VkAccelerationStructureCompatibilityKHR result;
		vkGetDeviceAccelerationStructureCompatibilityKHR( m_internal
			, &versionInfo
			, &result );
		return result;
	}

	VkAccelerationStructureBuildSizesInfoKHR Device::getAccelerationStructureBuildSizesKHR( VkAccelerationStructureBuildTypeKHR buildType
		, VkAccelerationStructureBuildGeometryInfoKHR const & buildInfo
		, UInt32Array const & maxPrimitiveCounts )const
	{
		VkAccelerationStructureBuildSizesInfoKHR result{ VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR
			, nullptr
			, {}
			, {}
			, {} };
		vkGetAccelerationStructureBuildSizesKHR( m_internal
			, buildType
			, &buildInfo
			, maxPrimitiveCounts.empty() ? nullptr : maxPrimitiveCounts.data()
			, &result );
		return result;
	}

#endif
#if VK_EXT_device_fault
	DeviceFaultInfo Device::getDeviceFaultInfo()const
	{
		DeviceFaultInfo result;

		if ( vkGetDeviceFaultInfoEXT )
		{
			VkDeviceFaultCountsEXT counts{};
			vkGetDeviceFaultInfoEXT( m_internal, &counts, nullptr );

			VkDeviceFaultInfoEXT info{};
			if ( counts.addressInfoCount )
			{
				result.addressInfos.resize( counts.addressInfoCount );
				info.pAddressInfos = result.addressInfos.data();
			}
			if ( counts.vendorInfoCount )
			{
				result.vendorInfos.resize( counts.vendorInfoCount );
				info.pVendorInfos = result.vendorInfos.data();
			}
			if ( counts.vendorBinarySize )
			{
				result.vendorBinaryData.resize( counts.vendorBinarySize );
				info.pVendorBinaryData = result.vendorBinaryData.data();
			}
			vkGetDeviceFaultInfoEXT( m_internal, &counts, &info );
			result.description = info.description;
		}

		return result;
	}
#endif

	QueuePtr Device::getQueue( std::string const & debugName
		, uint32_t familyIndex
		, uint32_t index )const
	{
		return std::make_unique< Queue >( *this
			, debugName
			, familyIndex
			, index );
	}

	void Device::waitIdle()const
	{
		checkError( vkDeviceWaitIdle( m_internal ), "Device wait idle" );
	}

	VkMemoryRequirements Device::getBufferMemoryRequirements( VkBuffer buffer )const
	{
		VkMemoryRequirements result;
		vkGetBufferMemoryRequirements( m_internal
			, buffer
			, &result );
		return result;
	}

	VkMemoryRequirements Device::getImageMemoryRequirements( VkImage image )const
	{
		VkMemoryRequirements result;
		vkGetImageMemoryRequirements( m_internal
			, image
			, &result );
		return result;
	}

	uint32_t Device::deduceMemoryType( uint32_t typeBits
		, VkMemoryPropertyFlags requirements )const
	{
		return ashes::deduceMemoryType( typeBits
			, requirements
			, m_memoryProperties );
	}

	PipelineLayoutPtr Device::createPipelineLayout()const
	{
		return createPipelineLayout( DescriptorSetLayoutCRefArray{}
			, VkPushConstantRangeArray{} );
	}

	PipelineLayoutPtr Device::createPipelineLayout( std::string const & debugName )const
	{
		return createPipelineLayout( debugName
			, DescriptorSetLayoutCRefArray{}
			, VkPushConstantRangeArray{} );
	}

	PipelineLayoutPtr Device::createPipelineLayout( DescriptorSetLayout const & layout )const
	{
		return createPipelineLayout( DescriptorSetLayoutCRefArray{ layout }
			, VkPushConstantRangeArray{} );
	}

	PipelineLayoutPtr Device::createPipelineLayout( std::string const & debugName
		, DescriptorSetLayout const & layout )const
	{
		return createPipelineLayout( debugName
			, DescriptorSetLayoutCRefArray{ layout }
			, VkPushConstantRangeArray{} );
	}

	PipelineLayoutPtr Device::createPipelineLayout( VkPushConstantRange const & pushConstantRange )const
	{
		return createPipelineLayout( DescriptorSetLayoutCRefArray{}
			, VkPushConstantRangeArray{ 1u, pushConstantRange } );
	}

	PipelineLayoutPtr Device::createPipelineLayout( std::string const & debugName
		, VkPushConstantRange const & pushConstantRange )const
	{
		return createPipelineLayout( debugName
			, DescriptorSetLayoutCRefArray{}
			, VkPushConstantRangeArray{ 1u, pushConstantRange } );
	}

	PipelineLayoutPtr Device::createPipelineLayout( DescriptorSetLayout const & layout
		, VkPushConstantRange const & pushConstantRange )const
	{
		return createPipelineLayout( DescriptorSetLayoutCRefArray{ layout }
			, VkPushConstantRangeArray{ 1u, pushConstantRange } );
	}

	PipelineLayoutPtr Device::createPipelineLayout( std::string const & debugName
		, DescriptorSetLayout const & layout
		, VkPushConstantRange const & pushConstantRange )const
	{
		return createPipelineLayout( debugName
			, DescriptorSetLayoutCRefArray{ layout }
			, VkPushConstantRangeArray{ 1u, pushConstantRange } );
	}

	PipelineLayoutPtr Device::createPipelineLayout( DescriptorSetLayoutCRefArray const & layouts )const
	{
		return createPipelineLayout( layouts
			, VkPushConstantRangeArray{} );
	}

	PipelineLayoutPtr Device::createPipelineLayout( std::string const & debugName
		, DescriptorSetLayoutCRefArray const & layouts )const
	{
		return createPipelineLayout( debugName
			, layouts
			, VkPushConstantRangeArray{} );
	}

	PipelineLayoutPtr Device::createPipelineLayout( VkPushConstantRangeArray const & pushConstantRanges )const
	{
		return createPipelineLayout( DescriptorSetLayoutCRefArray{}
			, pushConstantRanges );
	}

	PipelineLayoutPtr Device::createPipelineLayout( std::string const & debugName
		, VkPushConstantRangeArray const & pushConstantRanges )const
	{
		return createPipelineLayout( debugName
			, DescriptorSetLayoutCRefArray{}
			, pushConstantRanges );
	}

	SamplerPtr Device::createSampler( VkSamplerAddressMode wrapS
		, VkSamplerAddressMode wrapT
		, VkSamplerAddressMode wrapR
		, VkFilter minFilter
		, VkFilter magFilter
		, VkSamplerMipmapMode mipFilter
		, float minLod
		, float maxLod
		, float lodBias
		, VkBorderColor borderColour
		, float maxAnisotropy
		, VkCompareOp compareOp )const
	{
		return createSampler( SamplerCreateInfo
			{
				0u,
				minFilter,
				magFilter,
				mipFilter,
				wrapR,
				wrapT,
				wrapS,
				lodBias,
				maxAnisotropy != 1.0f,
				maxAnisotropy,
				compareOp != VK_COMPARE_OP_ALWAYS ? 1u : 0u,
				compareOp,
				minLod,
				maxLod,
				borderColour,
				false
			} );
	}

	SamplerPtr Device::createSampler( std::string const & debugName
		, VkSamplerAddressMode wrapS
		, VkSamplerAddressMode wrapT
		, VkSamplerAddressMode wrapR
		, VkFilter minFilter
		, VkFilter magFilter
		, VkSamplerMipmapMode mipFilter
		, float minLod
		, float maxLod
		, float lodBias
		, VkBorderColor borderColour
		, float maxAnisotropy
		, VkCompareOp compareOp )const
	{
		return createSampler( debugName
			, SamplerCreateInfo
			{
				0u,
				minFilter,
				magFilter,
				mipFilter,
				wrapR,
				wrapT,
				wrapS,
				lodBias,
				maxAnisotropy != 1.0f,
				maxAnisotropy,
				compareOp != VK_COMPARE_OP_ALWAYS ? 1u : 0u,
				compareOp,
				minLod,
				maxLod,
				borderColour,
				false
			} );
	}

#if VK_EXT_debug_utils || VK_EXT_debug_marker

	void Device::doRegisterObject( uint64_t object
		, uint32_t objectType
		, std::string const & objectName
		, std::string const & typeName )const
	{
		std::stringstream stream;
		stream.imbue( std::locale{ "C" } );
		stream << "Created [0x" << std::hex << std::setw( 8u ) << std::setfill( '0' ) << object << "]"
			<< " - " << typeName
			<< " - " << objectName;
		std::stringstream callStack;

		if ( m_callstackCallback )
		{
			callStack << m_callstackCallback();
		}

		{
			lock_type lock{ m_allocationMutex };
#	if VK_EXT_debug_utils
			if ( m_instance.checkExtension( VK_EXT_DEBUG_UTILS_EXTENSION_NAME ) )
			{
				setDebugUtilsObjectName(
					{
						VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
						nullptr,
						VkObjectType( objectType ),
						object,
						objectName.c_str(),
					} );
			}
#	endif
#	if VK_EXT_debug_marker
			if ( m_instance.checkExtension( VK_EXT_DEBUG_MARKER_EXTENSION_NAME ) )
			{
				debugMarkerSetObjectName(
					{
						VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT,
						nullptr,
						VkDebugReportObjectTypeEXT( objectType ),
						object,
						objectName.c_str(),
					} );
			}
#	endif

			if ( !m_allocated.try_emplace( object
				, typeName, objectName, callStack.str() ).second )
			{
				stream << " - Already found in allocated objects ?";
			}
		}

		Logger::logTrace( stream );
	}

	void Device::doUnregisterObject( uint64_t object )const
	{
		lock_type lock{ m_allocationMutex };
		auto it = m_allocated.find( size_t( object ) );
		std::stringstream stream;
		stream.imbue( std::locale{ "C" } );
		stream << "Destroyed [0x" << std::hex << std::setw( 8u ) << std::setfill( '0' ) << object << "]";

		if ( it != m_allocated.end() )
		{
			stream << " - " << it->second.type
				<< " - " << it->second.name;
			m_allocated.erase( it );
		}
		else
		{
			stream << " - Not found in allocated objects ?";
		}

		Logger::logTrace( stream );
	}

	void Device::doReportRegisteredObjects()const
	{
		lock_type lock{ m_allocationMutex };

		for ( auto const & [size, alloc] : m_allocated )
		{
			std::stringstream stream;
			stream << "Leaked [" << alloc.type << "](" << alloc.name << "), allocation stack:\n";
			stream << alloc.callstack;
			log::error << stream.str() << "\n";
		}
	}

#endif

	bool Device::doCheckExtension( std::string const & name )const
	{
		auto result = m_createInfos.enabledExtensionNames.end() != std::find( m_createInfos.enabledExtensionNames.begin()
			, m_createInfos.enabledExtensionNames.end()
			, name );

		if ( !result )
		{
			result = m_instance.checkExtension( name );
		}

		return result;
	}
}

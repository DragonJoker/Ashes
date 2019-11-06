/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_Device_HPP___
#define ___AshesPP_Device_HPP___
#pragma once

#include "ashespp/Command/CommandPool.hpp"
#include "ashespp/Command/Queue.hpp"
#include "ashespp/Core/DeviceCreateInfo.hpp"
#include "ashespp/Core/Surface.hpp"
#include "ashespp/Core/PhysicalDevice.hpp"
#include "ashespp/Image/ImageCreateInfo.hpp"
#include "ashespp/Image/SamplerCreateInfo.hpp"
#include "ashespp/Miscellaneous/QueueShare.hpp"
#include "ashespp/Pipeline/ComputePipelineCreateInfo.hpp"
#include "ashespp/Pipeline/GraphicsPipelineCreateInfo.hpp"
#include "ashespp/RenderPass/RenderPassCreateInfo.hpp"

#include <string>
#include <sstream>
#include <unordered_map>

namespace ashes
{
	/**
	*\brief
	*	The class containing the informations related to the logical device.
	*\remarks
	*	It creates most of the rendering API objects.
	*/
	class Device
	{
	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] instance
		*	The Instance instance.
		*\param[in] physicalDevice
		*	The physical device.
		*\param[in] createInfos
		*	The creation informations.
		*/
		Device( Instance const & instance
			, PhysicalDevice const & physicalDevice
			, ashes::DeviceCreateInfo createInfos );
		/**
		*\brief
		*	Destructor.
		*/
		~Device();
		/**
		*\brief
		*	Computes an frustum projection matrix.
		*\param[in] left, right
		*	The left and right planes position.
		*\param[in] top, bottom
		*	The top and bottom planes position.
		*\param[in] zNear, zFar
		*	The near and far planes position.
		*\return
		*	The computed matrix in column major order.
		*/
		std::array< float, 16u > frustum( float left
			, float right
			, float bottom
			, float top
			, float zNear
			, float zFar )const;
		/**
		*\brief
		*	Computes a perspective projection matrix.
		*\param[in] radiansFovY
		*	The vertical aperture angle.
		*\param[in] aspect
		*	The width / height ratio.
		*\param[in] zNear
		*	The near plane position.
		*\param[in] zFar
		*	The far plane position.
		*\return
		*	The computed matrix in column major order.
		*/
		std::array< float, 16u > perspective( float radiansFovY
			, float aspect
			, float zNear
			, float zFar )const;
		/**
		*\brief
		*	Computes an orthographic projection matrix.
		*\param[in] left, right
		*	The left and right planes position.
		*\param[in] top, bottom
		*	The top and bottom planes position.
		*\param[in] zNear, zFar
		*	The near and far planes position.
		*\return
		*	The computed matrix in column major order.
		*/
		std::array< float, 16u > ortho( float left
			, float right
			, float bottom
			, float top
			, float zNear
			, float zFar )const;
		/**
		*\brief
		*	Computes a perspective projection matrix with no far plane clipping.
		*\param[in] radiansFovY
		*	The vertical aperture angle.
		*\param[in] aspect
		*	The width / height ratio.
		*\param[in] zNear
		*	The near plane position.
		*\return
		*	The computed matrix in column major order.
		*/
		std::array< float, 16u > infinitePerspective( float radiansFovY
			, float aspect
			, float zNear )const;
		/**
		*\brief
		*	Creates a staging texture.
		*\param[in] extent
		*	The texture dimensions.
		*\return
		*	The created staging texture.
		*/
		StagingTexturePtr createStagingTexture( VkFormat format
			, VkExtent2D const & extent )const;
		/*
		*\brief
		*	Creates a render pass.
		*\param[in] createInfo
		*	The creation informations.
		*\return
		*	The created pass.
		*/
		RenderPassPtr createRenderPass( RenderPassCreateInfo createInfo )const;
		/**
		*\brief
		*	Creates a graphics pipeline using this layout.
		*\param[in] createInfo
		*	The creation informations.
		*\return
		*	The created pipeline.
		*/
		GraphicsPipelinePtr createPipeline( GraphicsPipelineCreateInfo createInfo )const;
		/**
		*\brief
		*	Creates a compute pipeline using this layout.
		*\param[in] createInfo
		*	The creation informations.
		*\return
		*	The created pipeline.
		*/
		ComputePipelinePtr createPipeline( ComputePipelineCreateInfo createInfo )const;
		/**
		*\brief
		*	Creates a pipeline layout.
		*\param[in] setLayouts
		*	The descriptor sets layouts.
		*\param[in] pushConstantRanges
		*	The push constants ranges.
		*\return
		*	The created layout.
		*/
		PipelineLayoutPtr createPipelineLayout( DescriptorSetLayoutCRefArray const & setLayouts
			, VkPushConstantRangeArray const & pushConstantRanges )const;
		/**
		*\brief
		*	Creates a descriptor set layout.
		*\param[in] bindings
		*	The layout bindings.
		*\return
		*	The created layout.
		*/
		DescriptorSetLayoutPtr createDescriptorSetLayout( VkDescriptorSetLayoutBindingArray bindings )const;
		/**
		*\brief
		*	Creates a descriptor pool.
		*\param[in] flags
		*	Bitmask specifying certain supported operations on a descriptor pool.
		*\param[in] maxSets
		*	The maximum number of descriptor sets that can be allocated from the pool.
		*\param[in] poolSizes
		*	The array of DescriptorPoolSize describing the type and count of descriptors to be allocated in the pool.
		*\return
		*	The created pool.
		*/
		DescriptorPoolPtr createDescriptorPool( VkDescriptorPoolCreateFlags flags
			, uint32_t maxSets
			, VkDescriptorPoolSizeArray poolSizes )const;
		/**
		*\brief
		*	Allocates memory on the device.
		*\param[in] allocateInfo
		*	The memory allocation requirements.
		*\return
		*	The DeviceMemory object holding the allocated memory.
		*/
		DeviceMemoryPtr allocateMemory( VkMemoryAllocateInfo allocateInfo )const;
		/**
		*\brief
		*	Creates a texture.
		*\param[in] createInfo
		*	The creation informations.
		*/
		ImagePtr createImage( ImageCreateInfo createInfo )const;
		/**
		*\brief
		*	Creates a texture.
		*\param[in] image
		*	The image whose layout is being queried.
		*\param[out] subresource
		*	Receives the image subresource.
		*\param[out] layout
		*	Receives the subresource layout.
		*/
		void getImageSubresourceLayout( Image const & image
			, VkImageSubresource const & subresource
			, VkSubresourceLayout & layout )const;
		/**
		*\brief
		*	Creates a sampler.
		*\param[in] createInfo
		*	The creation informations.
		*/
		SamplerPtr createSampler( SamplerCreateInfo createInfo )const;
		/**
		*\brief
		*	Creates a GPU buffer.
		*\param[in] size
		*	The buffer size.
		*\param[in] usage
		*	The buffer usage flags.
		*/
		BufferBasePtr createBuffer( VkDeviceSize size
			, VkBufferUsageFlags usage
			, QueueShare sharingMode = {} )const;
		/**
		*\brief
		*	Crée une vue sur un tampon GPU.
		*\param[in] buffer
		*	Le tampon sur lequel la vue est créée.
		*\param[in] format
		*	Le format des pixels de la vue.
		*\param[in] offset
		*	Le décalage dans le tampon.
		*\param[in] range
		*	Le nombre d'éléments dans le tampon.
		*/
		BufferViewPtr createBufferView( BufferBase const & buffer
			, VkFormat format
			, VkDeviceSize offset
			, VkDeviceSize range )const;
		/**
		*\brief
		*	Creates a swap chain.
		*\param[in] createInfo
		*	The creation info.
		*/
		SwapChainPtr createSwapChain( VkSwapchainCreateInfoKHR createInfo )const;
		/**
		*\brief
		*	Creates a semaphore.
		*/
		SemaphorePtr createSemaphore()const;
		/**
		*\brief
		*	Creates a fence.
		*\param[in] flags
		*	The fence creation flags.
		*/
		FencePtr createFence( VkFenceCreateFlags flags = 0 )const;
		/**
		*\brief
		*	Creates an event.
		*/
		EventPtr createEvent()const;
		/**
		*\brief
		*	Creates a command buffer pool.
		*\param[in] queueFamilyIndex
		*	The family index of the queue to which the pool belongs.
		*\param[in] flags
		*	The command pool creation flags.
		*/
		CommandPoolPtr createCommandPool( uint32_t queueFamilyIndex
			, VkCommandPoolCreateFlags const & flags = 0 )const;
		/**
		*\brief
		*	Creates a shader module.
		*\param[in] shader
		*	The module's SPIR-V code.
		*/
		ShaderModulePtr createShaderModule( UInt32Array const & shader )const;
		/**
		*\brief
		*	Creates a query pool.
		*\param[in] type
		*	The query type.
		*\param[in] count
		*	The number of queries managed by the pool.
		*\param[in] pipelineStatistics
		*	Specifies the counters to be returned in queries.
		*/
		QueryPoolPtr createQueryPool( VkQueryType type
			, uint32_t count
			, VkQueryPipelineStatisticFlags pipelineStatistics )const;

#if VK_EXT_debug_utils

		inline bool hasDebugUtils()const
		{
			return vkSetDebugUtilsObjectNameEXT
				&& vkSetDebugUtilsObjectTagEXT;
		}
		/**
		*\brief
		*	Gives an object a user-friendly name.
		*\param[in] nameInfo
		*	The parameters of the name to set on the object.
		*/
		inline VkResult setDebugUtilsObjectName( VkDebugUtilsObjectNameInfoEXT const & nameInfo )const
		{
			VkResult result = VK_ERROR_EXTENSION_NOT_PRESENT;

			if ( vkSetDebugUtilsObjectNameEXT )
			{
				result = vkSetDebugUtilsObjectNameEXT( m_internal, &nameInfo );
			}

			return result;
		}
		/**
		*\brief
		*	Gives an object a tag.
		*\param[in] tagInfo
		*	The parameters of the tag to set on the object.
		*/
		inline VkResult setDebugUtilsObjectTag( VkDebugUtilsObjectTagInfoEXT const & tagInfo )const
		{
			VkResult result = VK_ERROR_EXTENSION_NOT_PRESENT;

			if ( vkSetDebugUtilsObjectTagEXT )
			{
				vkSetDebugUtilsObjectTagEXT( m_internal, &tagInfo );
			}

			return result;
		}

#endif
#if VK_EXT_debug_marker

		inline bool hasDebugMarker()const
		{
			return vkDebugMarkerSetObjectTagEXT
				&& vkDebugMarkerSetObjectNameEXT;
		}
		/**
		*\brief
		*	Gives an object a tag.
		*\param[in] nameInfo
		*	The parameters of the tag to set on the object.
		*/
		inline VkResult debugMarkerSetObjectTag( VkDebugMarkerObjectTagInfoEXT const & tagInfo )const
		{
			VkResult result = VK_ERROR_EXTENSION_NOT_PRESENT;

			if ( vkDebugMarkerSetObjectTagEXT )
			{
				vkDebugMarkerSetObjectTagEXT( m_internal, &tagInfo );
			}

			return result;
		}
		/**
		*\brief
		*	Gives an object a user-friendly name.
		*\param[in] nameInfo
		*	The parameters of the name to set on the object.
		*/
		inline VkResult debugMarkerSetObjectName( VkDebugMarkerObjectNameInfoEXT const & nameInfo )const
		{
			VkResult result = VK_ERROR_EXTENSION_NOT_PRESENT;

			if ( vkDebugMarkerSetObjectNameEXT )
			{
				vkDebugMarkerSetObjectNameEXT( m_internal, &nameInfo );
			}

			return result;
		}

#endif

		/**
		*\brief
		*	Retrieves a device queue with given queue family index.
		*\param[in] familyIndex
		*	The device queue family index.
		*\param[in] index
		*	The index within this queue family of the queue to retrieve.
		*\return
		*	The device queue.
		*/
		QueuePtr getQueue( uint32_t familyIndex
			, uint32_t index )const;
		/**
		*\brief
		*	Creates a pipeline layout.
		*\return
		*	The created layout.
		*/
		PipelineLayoutPtr createPipelineLayout()const;
		/**
		*\brief
		*	Creates a pipeline layout.
		*\param[in] layout
		*	The descriptor set layout.
		*\return
		*	The created layout.
		*/
		PipelineLayoutPtr createPipelineLayout( DescriptorSetLayout const & layout )const;
		/**
		*\brief
		*	Creates a pipeline layout.
		*\param[in] pushConstantRange
		*	The push constants range.
		*\return
		*	The created layout.
		*\return
		*	Le layout créé.
		*/
		PipelineLayoutPtr createPipelineLayout( VkPushConstantRange const & pushConstantRange )const;
		/**
		*\brief
		*	Creates a pipeline layout.
		*\param[in] layout
		*	The descriptor set layout.
		*\param[in] pushConstantRange
		*	The push constants range.
		*\return
		*	The created layout.
		*/
		PipelineLayoutPtr createPipelineLayout( DescriptorSetLayout const & layout
			, VkPushConstantRange const & pushConstantRange )const;
		/**
		*\brief
		*	Creates a pipeline layout.
		*\param[in] layouts
		*	The descriptor sets layouts.
		*\return
		*	The created layout.
		*/
		PipelineLayoutPtr createPipelineLayout( DescriptorSetLayoutCRefArray const & layouts )const;
		/**
		*\brief
		*	Creates a pipeline layout.
		*\param[in] pushConstantRanges
		*	The push constants ranges.
		*\return
		*	The created layout.
		*/
		PipelineLayoutPtr createPipelineLayout( VkPushConstantRangeArray const & pushConstantRanges )const;
		/**
		*\brief
		*	Creates a sampler.
		*\param[in] wrapS, wrapT, wrapR
		*	The texture wrap modes.
		*\param[in] minFilter, magFilter
		*	The minification and magnification filters.
		*\param[in] mipFilter
		*	The mipmap filter.
		*\param[in] minLod
		*	Minimal LOD Level.
		*\param[in] maxLod
		*	Maximal LOD Level.
		*\param[in] lodBias
		*	The texture LOD offset.
		*\param[in] borderColour
		*	Image border colour.
		*\param[in] maxAnisotropy
		*	Maximal anisotropic filtering value.
		*\param[in] compareOp
		*	The comparison operator, for depth maps.
		*/
		SamplerPtr createSampler( VkSamplerAddressMode wrapS
			, VkSamplerAddressMode wrapT
			, VkSamplerAddressMode wrapR
			, VkFilter minFilter
			, VkFilter magFilter
			, VkSamplerMipmapMode mipFilter = VkSamplerMipmapMode( 0u )
			, float minLod = -1000.0f
			, float maxLod = 1000.0f
			, float lodBias = 0.0f
			, VkBorderColor borderColour = VkBorderColor::VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK
			, float maxAnisotropy = 1.0f
			, VkCompareOp compareOp = VkCompareOp::VK_COMPARE_OP_ALWAYS )const;
		/**
		*\brief
		*	Waits for the device to be idle.
		*/
		void waitIdle()const;
		/**
		*\brief
		*	Retrieves the memory requirements for given buffer.
		*\param[in] buffer
		*	The buffer.
		*return
		*	The memory requirements.
		*/
		VkMemoryRequirements getBufferMemoryRequirements( VkBuffer buffer )const;
		/**
		*\brief
		*	Retrieves the memory requirements for given buffer.
		*\param[in] image
		*	The image.
		*return
		*	The memory requirements.
		*/
		VkMemoryRequirements getImageMemoryRequirements( VkImage image )const;

		uint32_t deduceMemoryType( uint32_t typeBits
			, VkMemoryPropertyFlags requirements )const;
		/**
		*\name
		*	Getters.
		*/
		/**@{*/
		inline uint32_t getShaderVersion()const
		{
			return m_physicalDevice.getShaderVersion();
		}

		inline Instance const & getInstance()const
		{
			return m_instance;
		}

		inline PhysicalDevice const & getPhysicalDevice()const
		{
			return m_physicalDevice;
		}

		inline float getTimestampPeriod()const
		{
			return m_timestampPeriod;
		}

		inline VkPhysicalDeviceMemoryProperties const & getMemoryProperties()const
		{
			return m_memoryProperties;
		}

		inline VkPhysicalDeviceProperties const & getProperties()const
		{
			return m_properties;
		}

		inline VkPhysicalDeviceFeatures const & getFeatures()const
		{
			return m_features;
		}

		inline VkQueueFamilyPropertiesArray const & getQueueFamilyProperties()const
		{
			return m_queueFamilyProperties;
		}
		/**@}*/
		/**
		*\brief
		*	VkDevice implicit cast operator.
		*/
		inline operator VkDevice const & ()const
		{
			return m_internal;
		}

#define VK_LIB_DEVICE_FUNCTION( fun ) PFN_vk##fun vk##fun{ nullptr };
#	include <ashes/ashes_functions_list.hpp>

	protected:
		Instance const & m_instance;
		PhysicalDevice const & m_physicalDevice;
		ashes::DeviceCreateInfo m_createInfos;
		VkDevice m_internal{ VK_NULL_HANDLE };
		float m_timestampPeriod;
		uint32_t m_shaderVersion;
		VkPhysicalDeviceMemoryProperties m_memoryProperties;
		VkPhysicalDeviceProperties m_properties;
		VkPhysicalDeviceFeatures m_features;
		VkQueueFamilyPropertiesArray m_queueFamilyProperties;

#ifndef NDEBUG
		struct ObjectAllocation
		{
			std::string type;
			std::string callstack;
		};

		mutable std::unordered_map< size_t, ObjectAllocation > m_allocated;

	public:
		static inline void stRegisterObject( Device const & device, char const * const type, void * object )
		{
			device.doRegisterObject( type, object );
		}

		static inline void stUnregisterObject( Device const & device, void * object )
		{
			device.doUnregisterObject( object );
		}

	private:
		void doRegisterObject( char const * const type, void * object )const;
		void doUnregisterObject( void * object )const;
		void doReportRegisteredObjects()const;

#	define registerObject( Dev, TypeName, Object )\
	Device::stRegisterObject( Dev, TypeName, Object )
#	define unregisterObject( Dev, Object )\
	Device::stUnregisterObject( Dev, Object )
#	define reportRegisteredObjects()\
	doReportRegisteredObjects()
#else
#	define registerObject( Dev, TypeName, Object )
#	define unregisterObject( Dev, Object )
#	define reportRegisteredObjects()
#endif

	private:
		bool doCheckExtension( std::string const & name )const;
	};
}

#endif

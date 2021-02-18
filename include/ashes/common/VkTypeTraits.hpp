/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#ifndef ___Ashes_common_VkTypeTraits_HPP___
#define ___Ashes_common_VkTypeTraits_HPP___
#pragma once

#ifndef VK_NO_PROTOTYPES
#	define VK_NO_PROTOTYPES
#endif
#include <vulkan/vulkan.h>

#include <cassert>
#include <functional>
#include <set>
#include <map>
#include <mutex>

namespace ashes
{
	template< typename AshesType >
	struct VkTypeTraits;

#if VK_EXT_debug_utils
	template<>
	struct VkTypeTraits< VkDebugUtilsMessengerEXT >
	{
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT;
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT_EXT;
#endif
		static constexpr VkSystemAllocationScope Scope = VK_SYSTEM_ALLOCATION_SCOPE_INSTANCE;

		static std::string const & getName()
		{
			static std::string result{ "VkDebugUtilsMessengerEXT" };
			return result;
		}
	};
#endif
#if VK_EXT_debug_report
	template<>
	struct VkTypeTraits< VkDebugReportCallbackEXT >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT;
#endif
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT_EXT;
		static constexpr VkSystemAllocationScope Scope = VK_SYSTEM_ALLOCATION_SCOPE_INSTANCE;

		static std::string const & getName()
		{
			static std::string result{ "VkDebugReportCallbackEXT" };
			return result;
		}
	};
#endif

	template<>
	struct VkTypeTraits< VkInstance >
	{
		static constexpr VkSystemAllocationScope Scope = VK_SYSTEM_ALLOCATION_SCOPE_INSTANCE;
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_INSTANCE;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_INSTANCE_EXT;
#endif
		static std::string const & getName()
		{
			static std::string result{ "VkInstance" };
			return result;
		}
	};

	template<>
	struct VkTypeTraits< VkPhysicalDevice >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_PHYSICAL_DEVICE;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_PHYSICAL_DEVICE_EXT;
#endif
		static std::string const & getName()
		{
			static std::string result{ "VkPhysicalDevice" };
			return result;
		}
	};

	template<>
	struct VkTypeTraits< VkDevice >
	{
		static constexpr VkSystemAllocationScope Scope = VK_SYSTEM_ALLOCATION_SCOPE_INSTANCE;
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_DEVICE;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_EXT;
#endif
		static std::string const & getName()
		{
			static std::string result{ "VkDevice" };
			return result;
		}
	};

	template<>
	struct VkTypeTraits< VkQueue >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_QUEUE;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_QUEUE_EXT;
#endif
		static std::string const & getName()
		{
			static std::string result{ "VkQueue" };
			return result;
		}
	};

	template<>
	struct VkTypeTraits< VkSemaphore >
	{
		static constexpr VkSystemAllocationScope Scope = VK_SYSTEM_ALLOCATION_SCOPE_DEVICE;
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_SEMAPHORE;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_SEMAPHORE_EXT;
#endif
		static std::string const & getName()
		{
			static std::string result{ "VkSemaphore" };
			return result;
		}
	};

	template<>
	struct VkTypeTraits< VkCommandBuffer >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_COMMAND_BUFFER;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT;
#endif
		static std::string const & getName()
		{
			static std::string result{ "VkCommandBuffer" };
			return result;
		}
	};

	template<>
	struct VkTypeTraits< VkFence >
	{
		static constexpr VkSystemAllocationScope Scope = VK_SYSTEM_ALLOCATION_SCOPE_DEVICE;
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_FENCE;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_FENCE_EXT;
#endif
		static std::string const & getName()
		{
			static std::string result{ "VkFence" };
			return result;
		}
	};

	template<>
	struct VkTypeTraits< VkDeviceMemory >
	{
		static constexpr VkSystemAllocationScope Scope = VK_SYSTEM_ALLOCATION_SCOPE_DEVICE;
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_DEVICE_MEMORY;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT;
#endif
		static std::string const & getName()
		{
			static std::string result{ "VkDeviceMemory" };
			return result;
		}
	};

	template<>
	struct VkTypeTraits< VkBuffer >
	{
		static constexpr VkSystemAllocationScope Scope = VK_SYSTEM_ALLOCATION_SCOPE_DEVICE;
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_BUFFER;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT;
#endif
		static std::string const & getName()
		{
			static std::string result{ "VkBuffer" };
			return result;
		}
	};

	template<>
	struct VkTypeTraits< VkImage >
	{
		static constexpr VkSystemAllocationScope Scope = VK_SYSTEM_ALLOCATION_SCOPE_DEVICE;
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_IMAGE;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT;
#endif
		static std::string const & getName()
		{
			static std::string result{ "VkImage" };
			return result;
		}
	};

	template<>
	struct VkTypeTraits< VkEvent >
	{
		static constexpr VkSystemAllocationScope Scope = VK_SYSTEM_ALLOCATION_SCOPE_DEVICE;
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_EVENT;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_EVENT_EXT;
#endif
		static std::string const & getName()
		{
			static std::string result{ "VkEvent" };
			return result;
		}
	};

	template<>
	struct VkTypeTraits< VkQueryPool >
	{
		static constexpr VkSystemAllocationScope Scope = VK_SYSTEM_ALLOCATION_SCOPE_DEVICE;
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_QUERY_POOL;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_QUERY_POOL_EXT;
#endif
		static std::string const & getName()
		{
			static std::string result{ "VkQueryPool" };
			return result;
		}
	};

	template<>
	struct VkTypeTraits< VkBufferView >
	{
		static constexpr VkSystemAllocationScope Scope = VK_SYSTEM_ALLOCATION_SCOPE_DEVICE;
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_BUFFER_VIEW;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_VIEW_EXT;
#endif
		static std::string const & getName()
		{
			static std::string result{ "VkBufferView" };
			return result;
		}
	};

	template<>
	struct VkTypeTraits< VkImageView >
	{
		static constexpr VkSystemAllocationScope Scope = VK_SYSTEM_ALLOCATION_SCOPE_DEVICE;
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_IMAGE_VIEW;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_VIEW_EXT;
#endif
		static std::string const & getName()
		{
			static std::string result{ "VkImageView" };
			return result;
		}
	};

	template<>
	struct VkTypeTraits< VkShaderModule >
	{
		static constexpr VkSystemAllocationScope Scope = VK_SYSTEM_ALLOCATION_SCOPE_DEVICE;
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_SHADER_MODULE;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT;
#endif
		static std::string const & getName()
		{
			static std::string result{ "VkShaderModule" };
			return result;
		}
	};

	template<>
	struct VkTypeTraits< VkPipelineLayout >
	{
		static constexpr VkSystemAllocationScope Scope = VK_SYSTEM_ALLOCATION_SCOPE_DEVICE;
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_PIPELINE_LAYOUT;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_LAYOUT_EXT;
#endif
		static std::string const & getName()
		{
			static std::string result{ "VkPipelineLayout" };
			return result;
		}
	};

	template<>
	struct VkTypeTraits< VkRenderPass >
	{
		static constexpr VkSystemAllocationScope Scope = VK_SYSTEM_ALLOCATION_SCOPE_DEVICE;
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_RENDER_PASS;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_RENDER_PASS_EXT;
#endif
		static std::string const & getName()
		{
			static std::string result{ "VkRenderPass" };
			return result;
		}
	};

	template<>
	struct VkTypeTraits< VkPipeline >
	{
		static constexpr VkSystemAllocationScope Scope = VK_SYSTEM_ALLOCATION_SCOPE_DEVICE;
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_PIPELINE;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT;
#endif
		static std::string const & getName()
		{
			static std::string result{ "VkPipeline" };
			return result;
		}
	};

	template<>
	struct VkTypeTraits< VkPipelineCache >
	{
		static constexpr VkSystemAllocationScope Scope = VK_SYSTEM_ALLOCATION_SCOPE_DEVICE;
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_PIPELINE_CACHE;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_CACHE_EXT;
#endif
		static std::string const & getName()
		{
			static std::string result{ "VkPipelineCache" };
			return result;
		}
	};

	template<>
	struct VkTypeTraits< VkDescriptorSetLayout >
	{
		static constexpr VkSystemAllocationScope Scope = VK_SYSTEM_ALLOCATION_SCOPE_DEVICE;
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT_EXT;
#endif
		static std::string const & getName()
		{
			static std::string result{ "VkDescriptorSetLayout" };
			return result;
		}
	};

	template<>
	struct VkTypeTraits< VkSampler >
	{
		static constexpr VkSystemAllocationScope Scope = VK_SYSTEM_ALLOCATION_SCOPE_DEVICE;
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_SAMPLER;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_EXT;
#endif
		static std::string const & getName()
		{
			static std::string result{ "VkSampler" };
			return result;
		}
	};

	template<>
	struct VkTypeTraits< VkDescriptorPool >
	{
		static constexpr VkSystemAllocationScope Scope = VK_SYSTEM_ALLOCATION_SCOPE_DEVICE;
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_DESCRIPTOR_POOL;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_POOL_EXT;
#endif
		static std::string const & getName()
		{
			static std::string result{ "VkDecriptorPool" };
			return result;
		}
	};

	template<>
	struct VkTypeTraits< VkDescriptorSet >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_DESCRIPTOR_SET;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_EXT;
#endif
		static std::string const & getName()
		{
			static std::string result{ "VkDescriptorSet" };
			return result;
		}
	};

	template<>
	struct VkTypeTraits< VkFramebuffer >
	{
		static constexpr VkSystemAllocationScope Scope = VK_SYSTEM_ALLOCATION_SCOPE_DEVICE;
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_FRAMEBUFFER;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_FRAMEBUFFER_EXT;
#endif
		static std::string const & getName()
		{
			static std::string result{ "VkFramebuffer" };
			return result;
		}
	};

	template<>
	struct VkTypeTraits< VkCommandPool >
	{
		static constexpr VkSystemAllocationScope Scope = VK_SYSTEM_ALLOCATION_SCOPE_DEVICE;
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_COMMAND_POOL;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_POOL_EXT;
#endif
		static std::string const & getName()
		{
			static std::string result{ "VkCommandPool" };
			return result;
		}
	};

	template<>
	struct VkTypeTraits< VkSurfaceKHR >
	{
		static constexpr VkSystemAllocationScope Scope = VK_SYSTEM_ALLOCATION_SCOPE_INSTANCE;
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_SURFACE_KHR;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_SURFACE_KHR_EXT;
#endif
		static std::string const & getName()
		{
			static std::string result{ "VkSurfaceKHR" };
			return result;
		}
	};

	template<>
	struct VkTypeTraits< VkSwapchainKHR >
	{
		static constexpr VkSystemAllocationScope Scope = VK_SYSTEM_ALLOCATION_SCOPE_DEVICE;
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_SWAPCHAIN_KHR;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_SWAPCHAIN_KHR_EXT;
#endif
		static std::string const & getName()
		{
			static std::string result{ "VkSwapchainKHR" };
			return result;
		}
	};

	template<>
	struct VkTypeTraits< VkDisplayKHR >
	{
		static constexpr VkSystemAllocationScope Scope = VK_SYSTEM_ALLOCATION_SCOPE_DEVICE;
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_DISPLAY_KHR;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_DISPLAY_KHR_EXT;
#endif
		static std::string const & getName()
		{
			static std::string result{ "VkDisplayKHR" };
			return result;
		}
	};

	template<>
	struct VkTypeTraits< VkDisplayModeKHR >
	{
		static constexpr VkSystemAllocationScope Scope = VK_SYSTEM_ALLOCATION_SCOPE_DEVICE;
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_DISPLAY_MODE_KHR;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_DISPLAY_MODE_KHR_EXT;
#endif
		static std::string const & getName()
		{
			static std::string result{ "VkDisplayModeKHR" };
			return result;
		}
	};

	template< typename VkStructure >
	struct VkStructureTypeTraits;

#if VK_EXT_inline_uniform_block

	template<>
	struct VkStructureTypeTraits< VkWriteDescriptorSetInlineUniformBlockEXT >
	{
		static VkStructureType constexpr TypeValue = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_INLINE_UNIFORM_BLOCK_EXT;
	};
	
	template<>
	struct VkStructureTypeTraits< VkDescriptorPoolInlineUniformBlockCreateInfoEXT >
	{
		static VkStructureType constexpr TypeValue = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_INLINE_UNIFORM_BLOCK_CREATE_INFO_EXT;
	};

#endif

	template< typename VkType >
	static constexpr VkSystemAllocationScope allocationScopeT = VkTypeTraits< VkType >::Scope;

	template< typename T >
	T const * tryGet( void const * next )
	{
		while ( next )
		{
			auto vkStruct = reinterpret_cast< T const * >( next );

			if ( vkStruct->sType == VkStructureTypeTraits< T >::TypeValue )
			{
				return reinterpret_cast< T const * >( vkStruct );
			}

			next = vkStruct->pNext;
		}

		return nullptr;
	}

	struct CompareImageViewCreate
	{
		inline bool lower( VkComponentMapping const & lhs
			, VkComponentMapping const & rhs )const
		{
			return lhs.r < rhs.r
				|| ( lhs.r == rhs.r
					&& ( lhs.g < rhs.g
						|| ( lhs.g == rhs.g
							&& ( lhs.b < rhs.b
								|| ( lhs.b == rhs.b
									&& lhs.a < rhs.a
									)
								)
							)
						)
					);
		}

		inline bool equal( VkComponentMapping const & lhs
			, VkComponentMapping const & rhs )const
		{
			return lhs.r == rhs.r
				&& lhs.g == rhs.g
				&& lhs.b == rhs.b
				&& lhs.a == rhs.a;
		}

		inline bool lower( VkImageSubresourceRange const & lhs
			, VkImageSubresourceRange const & rhs )const
		{
			return lhs.aspectMask < rhs.aspectMask
				|| ( lhs.aspectMask == rhs.aspectMask
					&& ( lhs.baseArrayLayer < rhs.baseArrayLayer
						|| ( lhs.baseArrayLayer == rhs.baseArrayLayer
							&& ( lhs.layerCount < rhs.layerCount
								|| ( lhs.layerCount == rhs.layerCount
									&& ( lhs.baseMipLevel < rhs.baseMipLevel
										|| ( lhs.baseMipLevel == rhs.baseMipLevel
											&& lhs.levelCount < rhs.levelCount
											)
										)
									)
								)
							)
						)
					);
		}

		inline bool operator()( VkImageViewCreateInfo const & lhs
			, VkImageViewCreateInfo const & rhs )const
		{
			// LOL
			return lhs.flags < rhs.flags
				|| ( lhs.flags == rhs.flags
					&& ( lhs.format < rhs.format
						|| ( lhs.format == rhs.format
							&& ( lhs.image < rhs.image
								|| ( lhs.image == rhs.image
									&& ( lhs.viewType < rhs.viewType
										|| ( lhs.viewType == rhs.viewType
											&& ( lower( lhs.components, rhs.components )
												|| ( equal( lhs.components, rhs.components )
													&& lower( lhs.subresourceRange, rhs.subresourceRange )
													)
												)
											)
										)
									)
								)
							)
						)
					);
		}
	};

	using ImageViewCache = std::map< VkImageViewCreateInfo, VkImageView, CompareImageViewCreate >;
}

#endif

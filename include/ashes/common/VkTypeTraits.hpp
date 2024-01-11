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
		static inline std::string Name{ "VkInstance" };

		static std::string const & getName()
		{
			return Name;
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
		static inline std::string Name{ "VkPhysicalDevice" };
		static std::string const & getName()
		{
			return Name;
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
		static inline std::string Name{ "VkDevice" };
		static std::string const & getName()
		{
			return Name;
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
		static inline std::string Name{ "VkQueue" };
		static std::string const & getName()
		{
			return Name;
		}
	};

#if VK_EXT_debug_utils
	template<>
	struct VkTypeTraits< VkDebugUtilsMessengerEXT >
	{
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT;
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT_EXT;
#endif
		static constexpr VkSystemAllocationScope Scope = VK_SYSTEM_ALLOCATION_SCOPE_INSTANCE;

		static inline std::string Name{ "VkDebugUtilsMessengerEXT" };
		static std::string const & getName()
		{
			return Name;
		}
	};
#endif

	template<>
	struct VkTypeTraits< VkCommandBuffer >
	{
		static constexpr VkSystemAllocationScope Scope = VK_SYSTEM_ALLOCATION_SCOPE_OBJECT;
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_COMMAND_BUFFER;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT;
#endif
		static inline std::string Name{ "VkCommandBuffer" };
		static std::string const & getName()
		{
			return Name;
		}
	};

#if ( VK_USE_64_BIT_PTR_DEFINES == 1 )

#if VK_EXT_debug_report
	template<>
	struct VkTypeTraits< VkDebugReportCallbackEXT >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT;
#endif
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT_EXT;
		static constexpr VkSystemAllocationScope Scope = VK_SYSTEM_ALLOCATION_SCOPE_INSTANCE;

		static inline std::string Name{ "VkDebugReportCallbackEXT" };
		static std::string const & getName()
		{
			return Name;
		}
	};
#endif

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
		static inline std::string Name{ "VkSemaphore" };
		static std::string const & getName()
		{
			return Name;
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
		static inline std::string Name{ "VkFence" };
		static std::string const & getName()
		{
			return Name;
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
		static inline std::string Name{ "VkDeviceMemory" };
		static std::string const & getName()
		{
			return Name;
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
		static inline std::string Name{ "VkBuffer" };
		static std::string const & getName()
		{
			return Name;
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
		static inline std::string Name{ "VkImage" };
		static std::string const & getName()
		{
			return Name;
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
		static inline std::string Name{ "VkEvent" };
		static std::string const & getName()
		{
			return Name;
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
		static inline std::string Name{ "VkQueryPool" };
		static std::string const & getName()
		{
			return Name;
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
		static inline std::string Name{ "VkBufferView" };
		static std::string const & getName()
		{
			return Name;
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
		static inline std::string Name{ "VkImageView" };
		static std::string const & getName()
		{
			return Name;
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
		static inline std::string Name{ "VkShaderModule" };
		static std::string const & getName()
		{
			return Name;
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
		static inline std::string Name{ "VkPipelineLayout" };
		static std::string const & getName()
		{
			return Name;
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
		static inline std::string Name{ "VkRenderPass" };
		static std::string const & getName()
		{
			return Name;
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
		static inline std::string Name{ "VkPipeline" };
		static std::string const & getName()
		{
			return Name;
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
		static inline std::string Name{ "VkPipelineCache" };
		static std::string const & getName()
		{
			return Name;
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
		static inline std::string Name{ "VkDescriptorSetLayout" };
		static std::string const & getName()
		{
			return Name;
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
		static inline std::string Name{ "VkSampler" };
		static std::string const & getName()
		{
			return Name;
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
		static inline std::string Name{ "VkDecriptorPool" };
		static std::string const & getName()
		{
			return Name;
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
		static inline std::string Name{ "VkDescriptorSet" };
		static std::string const & getName()
		{
			return Name;
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
		static inline std::string Name{ "VkFramebuffer" };
		static std::string const & getName()
		{
			return Name;
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
		static inline std::string Name{ "VkCommandPool" };
		static std::string const & getName()
		{
			return Name;
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
		static inline std::string Name{ "VkSurfaceKHR" };
		static std::string const & getName()
		{
			return Name;
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
		static inline std::string Name{ "VkSwapchainKHR" };
		static std::string const & getName()
		{
			return Name;
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
		static inline std::string Name{ "VkDisplayKHR" };
		static std::string const & getName()
		{
			return Name;
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
		static inline std::string Name{ "VkDisplayModeKHR" };
		static std::string const & getName()
		{
			return Name;
		}
	};

#endif // ( VK_USE_64_BIT_PTR_DEFINES == 1 )

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

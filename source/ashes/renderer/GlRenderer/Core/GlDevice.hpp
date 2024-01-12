/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/Command/GlCommandBuffer.hpp"
#include "renderer/GlRenderer/Core/GlContextLock.hpp"
#include "renderer/GlRenderer/Core/GlPhysicalDevice.hpp"

#include <unordered_map>

namespace ashes::gl
{
	template< typename AshesType >
	struct AshesTypeTraits;

	template<>
	struct AshesTypeTraits< Instance >
	{
		using VkType = VkInstance;
	};

	template<>
	struct AshesTypeTraits< PhysicalDevice >
	{
		using VkType = VkPhysicalDevice;
	};

	template<>
	struct AshesTypeTraits< Device >
	{
		using VkType = VkDevice;
	};

	template<>
	struct AshesTypeTraits< Queue >
	{
		using VkType = VkQueue;
	};

	template<>
	struct AshesTypeTraits< Semaphore >
	{
		using VkType = VkSemaphore;
	};

	template<>
	struct AshesTypeTraits< CommandBuffer >
	{
		using VkType = VkCommandBuffer;
	};

	template<>
	struct AshesTypeTraits< Fence >
	{
		using VkType = VkFence;
	};

	template<>
	struct AshesTypeTraits< DeviceMemory >
	{
		using VkType = VkDeviceMemory;
	};

	template<>
	struct AshesTypeTraits< Buffer >
	{
		using VkType = VkBuffer;
	};

	template<>
	struct AshesTypeTraits< Event >
	{
		using VkType = VkEvent;
	};

	template<>
	struct AshesTypeTraits< Image >
	{
		using VkType = VkImage;
	};

	template<>
	struct AshesTypeTraits< QueryPool >
	{
		using VkType = VkQueryPool;
	};

	template<>
	struct AshesTypeTraits< BufferView >
	{
		using VkType = VkBufferView;
	};

	template<>
	struct AshesTypeTraits< ImageView >
	{
		using VkType = VkImageView;
	};

	template<>
	struct AshesTypeTraits< VkImageView >
	{
		using VkType = VkImageView;
	};

	template<>
	struct AshesTypeTraits< ShaderModule >
	{
		using VkType = VkShaderModule;
	};

	template<>
	struct AshesTypeTraits< PipelineLayout >
	{
		using VkType = VkPipelineLayout;
	};

	template<>
	struct AshesTypeTraits< RenderPass >
	{
		using VkType = VkRenderPass;
	};

	template<>
	struct AshesTypeTraits< Pipeline >
	{
		using VkType = VkPipeline;
	};

	template<>
	struct AshesTypeTraits< PipelineCache >
	{
		using VkType = VkPipelineCache;
	};

	template<>
	struct AshesTypeTraits< DescriptorSetLayout >
	{
		using VkType = VkDescriptorSetLayout;
	};

	template<>
	struct AshesTypeTraits< Sampler >
	{
		using VkType = VkSampler;
	};

	template<>
	struct AshesTypeTraits< DescriptorPool >
	{
		using VkType = VkDescriptorPool;
	};

	template<>
	struct AshesTypeTraits< DescriptorSet >
	{
		using VkType = VkDescriptorSet;
	};

	template<>
	struct AshesTypeTraits< Framebuffer >
	{
		using VkType = VkFramebuffer;
	};

	template<>
	struct AshesTypeTraits< CommandPool >
	{
		using VkType = VkCommandPool;
	};

	template<>
	struct AshesTypeTraits< SurfaceKHR >
	{
		using VkType = VkSurfaceKHR;
	};

	template<>
	struct AshesTypeTraits< SwapchainKHR >
	{
		using VkType = VkSwapchainKHR;
	};

	template< typename AshesType >
	struct AshesDebugTypeTraits;

	template<>
	struct AshesDebugTypeTraits< Instance >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_INSTANCE;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_INSTANCE_EXT;
#endif
		static inline std::string const Name{ "VkInstance" };

		static std::string const & getName()
		{
			return Name;
		}
	};

	template<>
	struct AshesDebugTypeTraits< PhysicalDevice >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_PHYSICAL_DEVICE;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_PHYSICAL_DEVICE_EXT;
#endif
		static inline std::string const Name{ "VkPhysicalDevice" };

		static std::string const & getName()
		{
			return Name;
		}
	};

	template<>
	struct AshesDebugTypeTraits< Device >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_PHYSICAL_DEVICE;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_PHYSICAL_DEVICE_EXT;
#endif
		static inline std::string const Name{ "VkDevice" };

		static std::string const & getName()
		{
			return Name;
		}
	};

	template<>
	struct AshesDebugTypeTraits< Queue >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_QUEUE;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_QUEUE_EXT;
#endif
		static inline std::string const Name{ "VkQueue" };

		static std::string const & getName()
		{
			return Name;
		}
	};

	template<>
	struct AshesDebugTypeTraits< Semaphore >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_SEMAPHORE;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_SEMAPHORE_EXT;
#endif
		static inline std::string const Name{ "VkSemaphore" };

		static std::string const & getName()
		{
			return Name;
		}
	};

	template<>
	struct AshesDebugTypeTraits< CommandBuffer >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_COMMAND_BUFFER;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT;
#endif
		static inline std::string const Name{ "VkCommandBuffer" };

		static std::string const & getName()
		{
			return Name;
		}
	};

	template<>
	struct AshesDebugTypeTraits< Event >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_EVENT;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_EVENT_EXT;
#endif
		static inline std::string const Name{ "VkEvent" };

		static std::string const & getName()
		{
			return Name;
		}
	};

	template<>
	struct AshesDebugTypeTraits< Fence >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_FENCE;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_FENCE_EXT;
#endif
		static inline std::string const Name{ "VkFence" };

		static std::string const & getName()
		{
			return Name;
		}
	};

	template<>
	struct AshesDebugTypeTraits< DeviceMemory >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_DEVICE_MEMORY;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT;
#endif
		static inline std::string const Name{ "VkDeviceMemory" };

		static std::string const & getName()
		{
			return Name;
		}
	};

	template<>
	struct AshesDebugTypeTraits< Buffer >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_BUFFER;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT;
#endif
		static inline std::string const Name{ "VkBuffer" };

		static std::string const & getName()
		{
			return Name;
		}
	};

	template<>
	struct AshesDebugTypeTraits< Image >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_IMAGE;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT;
#endif
		static inline std::string const Name{ "VkImage" };

		static std::string const & getName()
		{
			return Name;
		}
	};

	template<>
	struct AshesDebugTypeTraits< QueryPool >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_QUERY_POOL;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_QUERY_POOL_EXT;
#endif
		static inline std::string const Name{ "VkQueryPool" };

		static std::string const & getName()
		{
			return Name;
		}
	};

	template<>
	struct AshesDebugTypeTraits< BufferView >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_BUFFER_VIEW;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_VIEW_EXT;
#endif
		static inline std::string const Name{ "VkBufferView" };

		static std::string const & getName()
		{
			return Name;
		}
	};

	template<>
	struct AshesDebugTypeTraits< ImageView >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_IMAGE_VIEW;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_VIEW_EXT;
#endif
		static inline std::string const Name{ "VkImageView" };

		static std::string const & getName()
		{
			return Name;
		}
	};

	template<>
	struct AshesDebugTypeTraits< VkImageView >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_IMAGE_VIEW;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_VIEW_EXT;
#endif
		static inline std::string const Name{ "VkImageView" };

		static std::string const & getName()
		{
			return Name;
		}
	};

	template<>
	struct AshesDebugTypeTraits< ShaderModule >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_SHADER_MODULE;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT;
#endif
		static inline std::string const Name{ "VkShaderModule" };

		static std::string const & getName()
		{
			return Name;
		}
	};

	template<>
	struct AshesDebugTypeTraits< PipelineLayout >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_PIPELINE_LAYOUT;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_LAYOUT_EXT;
#endif
		static inline std::string const Name{ "VkPipelineLayout" };

		static std::string const & getName()
		{
			return Name;
		}
	};

	template<>
	struct AshesDebugTypeTraits< RenderPass >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_RENDER_PASS;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_RENDER_PASS_EXT;
#endif
		static inline std::string const Name{ "VkRenderPass" };

		static std::string const & getName()
		{
			return Name;
		}
	};

	template<>
	struct AshesDebugTypeTraits< Pipeline >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_PIPELINE;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT;
#endif
		static inline std::string const Name{ "VkPipeline" };

		static std::string const & getName()
		{
			return Name;
		}
	};

	template<>
	struct AshesDebugTypeTraits< PipelineCache >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_PIPELINE_CACHE;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_CACHE_EXT;
#endif
		static inline std::string const Name{ "VkPipelineCache" };

		static std::string const & getName()
		{
			return Name;
		}
	};

	template<>
	struct AshesDebugTypeTraits< DescriptorSetLayout >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT_EXT;
#endif
		static inline std::string const Name{ "VkDescriptorSetLayout" };

		static std::string const & getName()
		{
			return Name;
		}
	};

	template<>
	struct AshesDebugTypeTraits< Sampler >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_SAMPLER;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_EXT;
#endif
		static inline std::string const Name{ "VkSampler" };

		static std::string const & getName()
		{
			return Name;
		}
	};

	template<>
	struct AshesDebugTypeTraits< DescriptorPool >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_DESCRIPTOR_POOL;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_POOL_EXT;
#endif
		static inline std::string const Name{ "VkDecriptorPool" };

		static std::string const & getName()
		{
			return Name;
		}
	};

	template<>
	struct AshesDebugTypeTraits< DescriptorSet >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_DESCRIPTOR_SET;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_EXT;
#endif
		static inline std::string const Name{ "VkDescriptorSet" };

		static std::string const & getName()
		{
			return Name;
		}
	};

	template<>
	struct AshesDebugTypeTraits< Framebuffer >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_FRAMEBUFFER;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_FRAMEBUFFER_EXT;
#endif
		static inline std::string const Name{ "VkFramebuffer" };

		static std::string const & getName()
		{
			return Name;
		}
	};

	template<>
	struct AshesDebugTypeTraits< CommandPool >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_COMMAND_POOL;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_POOL_EXT;
#endif
		static inline std::string const Name{ "VkCommandPool" };

		static std::string const & getName()
		{
			return Name;
		}
	};

	template<>
	struct AshesDebugTypeTraits< SurfaceKHR >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_SURFACE_KHR;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_SURFACE_KHR_EXT;
#endif
		static inline std::string const Name{ "VkSurfaceKHR" };

		static std::string const & getName()
		{
			return Name;
		}
	};

	template<>
	struct AshesDebugTypeTraits< SwapchainKHR >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_SWAPCHAIN_KHR;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_SWAPCHAIN_KHR_EXT;
#endif
		static inline std::string const Name{ "VkSwapchainKHR" };

		static std::string const & getName()
		{
			return Name;
		}
	};

	class Device
		: public ashes::IcdObject
	{
		friend class Instance;

	public:
		Device( VkAllocationCallbacks const * callbacks
			, VkInstance instance
			, VkPhysicalDevice gpu
			, VkDeviceCreateInfo createInfos );
		~Device()noexcept;

		void cleanupContextDependent( Context const & context )noexcept;
		void cleanupBlitSrcFbo()noexcept;
		void cleanupBlitDstFbo()noexcept;
		bool hasExtension( std::string_view extension )const;
		VkPhysicalDeviceLimits const & getLimits()const;
		void getImageSubresourceLayout( VkImage image
			, VkImageSubresource const & subresource
			, VkSubresourceLayout & layout )const;
		VkResult waitIdle()const;
#if VK_EXT_debug_utils
		VkResult setDebugUtilsObjectName( VkDebugUtilsObjectNameInfoEXT const & nameInfo )const;
		VkResult setDebugUtilsObjectTag( VkDebugUtilsObjectTagInfoEXT const & tagInfo )const;
		void submitDebugUtilsMessenger( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity
			, VkDebugUtilsMessageTypeFlagsEXT messageTypes
			, VkDebugUtilsMessengerCallbackDataEXT const & callbackData )const;
#endif
#if VK_EXT_debug_marker
		VkResult debugMarkerSetObjectTag( VkDebugMarkerObjectTagInfoEXT const & nameInfo )const;
		VkResult debugMarkerSetObjectName( VkDebugMarkerObjectNameInfoEXT const & nameInfo )const;
#endif
#if VK_EXT_debug_report
		void reportMessage( VkDebugReportFlagsEXT flags
			, VkDebugReportObjectTypeEXT objectType
			, uint64_t object
			, size_t location
			, int32_t messageCode
			, const char * pLayerPrefix
			, const char * pMessage );
#endif
		VkQueue getQueue( uint32_t familyIndex
			, uint32_t index )const;
		void swapBuffers()const;

		void link( VkSurfaceKHR surface );
		void unlink()noexcept;
		ContextLock getContext()const noexcept;

		VkPhysicalDeviceFeatures const & getEnabledFeatures()const noexcept
		{
			return m_enabledFeatures;
		}

		GeometryBuffers & getEmptyIndexedVao()const noexcept
		{
			doInitialiseDummy();
			return *m_dummyIndexed.geometryBuffers;
		}

		VkBuffer getEmptyIndexedVaoIdx()const noexcept
		{
			doInitialiseDummy();
			return m_dummyIndexed.indexBuffer;
		}

		VkFramebuffer getBlitSrcFbo()const noexcept
		{
			return m_blitFbos[0];
		}

		VkFramebuffer getBlitDstFbo()const noexcept
		{
			return m_blitFbos[1];
		}

		VkInstance getInstance()const noexcept
		{
			return m_instance;
		}

		VkPhysicalDevice getPhysicalDevice()const noexcept
		{
			return m_physicalDevice;
		}
		
		bool hasMemoryBarrier()const noexcept
		{
			return m_currentContext->hasMemoryBarrier();
		}

		VkSampler getSampler()const noexcept
		{
			return m_sampler;
		}

		VkAllocationCallbacks const * getAllocationCallbacks()const noexcept
		{
			return m_callbacks;
		}

	private:
		void doInitialiseQueues();
		void doInitialiseDummy()const;
		void doInitialiseContextDependent();
		void doCleanupContextDependent()noexcept;

	private:
		struct QueueCreates
		{
			QueueCreates( VkDeviceQueueCreateInfo pcreateInfo
				, std::vector< VkQueue > pqueues = {} )
				: createInfo{ std::move( pcreateInfo ) }
				, queues{ std::move( pqueues ) }
			{
			}

			VkDeviceQueueCreateInfo createInfo;
			std::vector< VkQueue > queues;
		};
		using QueueCreateCountMap = std::map< uint32_t, QueueCreates >;

	private:
		VkInstance m_instance;
		VkPhysicalDevice m_physicalDevice;
		VkAllocationCallbacks const * m_callbacks;
		StringArray m_enabledExtensions;
		VkDeviceCreateInfo m_createInfos;
		VkPhysicalDeviceFeatures m_enabledFeatures{};
		QueueCreateCountMap m_queues;
		mutable ContextPtr m_ownContext;
		mutable Context * m_currentContext;
		struct Vertex
		{
			float x;
			float y;
			float z;
		};
		// Mimic the behavior in Vulkan, when no IBO nor VBO is bound.
		mutable struct
		{
			VkBuffer indexBuffer{};
			VkBuffer vertexBuffer{};
			VkDeviceMemory indexMemory{};
			VkDeviceMemory vertexMemory{};
			GeometryBuffersPtr geometryBuffers;
		} m_dummyIndexed;
		mutable std::array< VkFramebuffer, 2u > m_blitFbos{};
		mutable VkSampler m_sampler{};
		VkPipelineColorBlendAttachmentStateArray m_cbStateAttachments;
		VkDynamicStateArray m_dyState{ VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_VIEWPORT };

		struct ObjectAllocation
		{
			explicit ObjectAllocation( std::string ptype )
				: type{ std::move( ptype ) }
			{
			}

			std::string type;
		};

		mutable std::unordered_map< size_t, ObjectAllocation > m_allocated;

	public:
		template< typename AshesType >
		static inline void stRegisterObject( VkDevice device
			, AshesType & object )
		{
			using DebugTypeTraits = AshesDebugTypeTraits< AshesType >;
			getDevice( device )->doRegisterObject( uint64_t( get( &object ) )
#if VK_EXT_debug_utils
				, uint32_t( DebugTypeTraits::UtilsValue )
#elif VK_EXT_debug_marker
				, uint32_t( DebugTypeTraits::ReportValue )
#endif
				, DebugTypeTraits::getName() );
		}

		template< typename AshesType >
		static inline void stUnregisterObject( VkDevice device
			, AshesType & object )noexcept
		{
			getDevice( device )->doUnregisterObject( uint64_t( get( &object ) ) );
		}

	private:
		static Device * getDevice( VkDevice device );
		void doRegisterObject( uint64_t object
			, uint32_t objectType
			, std::string const & typeName )const;
		void doUnregisterObject( uint64_t object )const noexcept;
		void doReportRegisteredObjects()const noexcept;

#ifndef NDEBUG
#	define reportRegisteredObjects()\
	doReportRegisteredObjects()
#else
#	define reportRegisteredObjects()
#endif
	};

#ifndef NDEBUG
#	define registerObject( Dev, Object )\
	Device::stRegisterObject( Dev, Object )
#	define unregisterObject( Dev, Object )\
	Device::stUnregisterObject( Dev, Object )
#else
#	define registerObject( Dev, Object )
#	define unregisterObject( Dev, Object )
#endif


	bool has420PackExtensions( VkDevice device )noexcept;
	bool hasCopyImage( VkDevice device )noexcept;
	bool hasProgramPipelines( VkDevice device )noexcept;
	bool hasSamplerAnisotropy( VkDevice device )noexcept;
	bool hasTextureStorage( VkDevice device )noexcept;
	bool hasTextureViews( VkDevice device )noexcept;
	bool hasViewportArrays( VkDevice device )noexcept;
	bool hasProgramInterfaceQuery( VkDevice device )noexcept;
}

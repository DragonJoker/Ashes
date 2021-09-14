/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_Device_HPP___
#define ___AshesPP_Device_HPP___
#pragma once

#include "ashespp/Command/CommandPool.hpp"
#include "ashespp/Sync/Queue.hpp"
#include "ashespp/Core/DeviceCreateInfo.hpp"
#include "ashespp/Core/Surface.hpp"
#include "ashespp/Core/PhysicalDevice.hpp"
#include "ashespp/Image/ImageCreateInfo.hpp"
#include "ashespp/Image/SamplerCreateInfo.hpp"
#include "ashespp/Miscellaneous/QueueShare.hpp"
#include "ashespp/Pipeline/ComputePipelineCreateInfo.hpp"
#include "ashespp/Pipeline/GraphicsPipelineCreateInfo.hpp"
#include "ashespp/RenderPass/RenderPassCreateInfo.hpp"

#include <mutex>
#include <string>
#include <sstream>
#include <unordered_map>

namespace ashes
{
	template< typename AshesType >
	struct AshesTypeTraits;

	template<>
	struct AshesTypeTraits< ashes::Instance >
	{
		using VkType = VkInstance;
	};

	template<>
	struct AshesTypeTraits< ashes::PhysicalDevice >
	{
		using VkType = VkPhysicalDevice;
	};

	template<>
	struct AshesTypeTraits< ashes::Device >
	{
		using VkType = VkDevice;
	};

	template<>
	struct AshesTypeTraits< ashes::Queue >
	{
		using VkType = VkQueue;
	};

	template<>
	struct AshesTypeTraits< ashes::Semaphore >
	{
		using VkType = VkSemaphore;
	};

	template<>
	struct AshesTypeTraits< ashes::CommandBuffer >
	{
		using VkType = VkCommandBuffer;
	};

	template<>
	struct AshesTypeTraits< ashes::Fence >
	{
		using VkType = VkFence;
	};

	template<>
	struct AshesTypeTraits< ashes::DeviceMemory >
	{
		using VkType = VkDeviceMemory;
	};

	template<>
	struct AshesTypeTraits< ashes::BufferBase >
	{
		using VkType = VkBuffer;
	};

	template<>
	struct AshesTypeTraits< ashes::UniformBuffer >
	{
		using VkType = VkBuffer;
	};

	template<>
	struct AshesTypeTraits< ashes::VertexBufferBase >
	{
		using VkType = VkBuffer;
	};

	template<>
	struct AshesTypeTraits< ashes::Image >
	{
		using VkType = VkImage;
	};

	template<>
	struct AshesTypeTraits< ashes::Event >
	{
		using VkType = VkEvent;
	};

	template<>
	struct AshesTypeTraits< ashes::QueryPool >
	{
		using VkType = VkQueryPool;
	};

	template<>
	struct AshesTypeTraits< ashes::BufferView >
	{
		using VkType = VkBufferView;
	};

	template<>
	struct AshesTypeTraits< ashes::ImageView >
	{
		using VkType = VkImageView;
	};

	template<>
	struct AshesTypeTraits< VkImageView >
	{
		using VkType = VkImageView;
	};

	template<>
	struct AshesTypeTraits< ashes::ShaderModule >
	{
		using VkType = VkShaderModule;
	};

	template<>
	struct AshesTypeTraits< ashes::PipelineLayout >
	{
		using VkType = VkPipelineLayout;
	};

	template<>
	struct AshesTypeTraits< ashes::RenderPass >
	{
		using VkType = VkRenderPass;
	};

	template<>
	struct AshesTypeTraits< ashes::Pipeline >
	{
		using VkType = VkPipeline;
	};

	template<>
	struct AshesTypeTraits< ashes::DescriptorSetLayout >
	{
		using VkType = VkDescriptorSetLayout;
	};

	template<>
	struct AshesTypeTraits< ashes::Sampler >
	{
		using VkType = VkSampler;
	};

	template<>
	struct AshesTypeTraits< ashes::DescriptorSetPool >
	{
		using VkType = VkDescriptorPool;
	};

	template<>
	struct AshesTypeTraits< ashes::DescriptorPool >
	{
		using VkType = VkDescriptorPool;
	};

	template<>
	struct AshesTypeTraits< ashes::DescriptorSet >
	{
		using VkType = VkDescriptorSet;
	};

	template<>
	struct AshesTypeTraits< ashes::FrameBuffer >
	{
		using VkType = VkFramebuffer;
	};

	template<>
	struct AshesTypeTraits< ashes::CommandPool >
	{
		using VkType = VkCommandPool;
	};

	template<>
	struct AshesTypeTraits< ashes::Surface >
	{
		using VkType = VkSurfaceKHR;
	};

	template<>
	struct AshesTypeTraits< ashes::SwapChain >
	{
		using VkType = VkSwapchainKHR;
	};

	template< typename AshesType >
	struct AshesDebugTypeTraits;

	template<>
	struct AshesDebugTypeTraits< ashes::Instance >
	{
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
	struct AshesDebugTypeTraits< ashes::PhysicalDevice >
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
	struct AshesDebugTypeTraits< ashes::Device >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_PHYSICAL_DEVICE;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_PHYSICAL_DEVICE_EXT;
#endif
		static std::string const & getName()
		{
			static std::string result{ "VkDevice" };
			return result;
		}
	};

	template<>
	struct AshesDebugTypeTraits< ashes::Queue >
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
	struct AshesDebugTypeTraits< ashes::Semaphore >
	{
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
	struct AshesDebugTypeTraits< ashes::CommandBuffer >
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
	struct AshesDebugTypeTraits< ashes::Fence >
	{
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
	struct AshesDebugTypeTraits< ashes::DeviceMemory >
	{
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
	struct AshesDebugTypeTraits< ashes::BufferBase >
	{
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
	struct AshesDebugTypeTraits< ashes::UniformBuffer >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_BUFFER;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT;
#endif
		static std::string const & getName()
		{
			static std::string result{ "VkUniformBuffer" };
			return result;
		}
	};

	template<>
	struct AshesDebugTypeTraits< ashes::VertexBufferBase >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_BUFFER;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT;
#endif
		static std::string const & getName()
		{
			static std::string result{ "VkVertexBuffer" };
			return result;
		}
	};

	template<>
	struct AshesDebugTypeTraits< ashes::Image >
	{
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
	struct AshesDebugTypeTraits< ashes::Event >
	{
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
	struct AshesDebugTypeTraits< ashes::QueryPool >
	{
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
	struct AshesDebugTypeTraits< ashes::BufferView >
	{
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
	struct AshesDebugTypeTraits< ashes::ImageView >
	{
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
	struct AshesDebugTypeTraits< VkImageView >
	{
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
	struct AshesDebugTypeTraits< ashes::ShaderModule >
	{
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
	struct AshesDebugTypeTraits< ashes::PipelineLayout >
	{
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
	struct AshesDebugTypeTraits< ashes::RenderPass >
	{
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
	struct AshesDebugTypeTraits< ashes::Pipeline >
	{
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
	struct AshesDebugTypeTraits< ashes::DescriptorSetLayout >
	{
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
	struct AshesDebugTypeTraits< ashes::Sampler >
	{
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
	struct AshesDebugTypeTraits< ashes::DescriptorSetPool >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_DESCRIPTOR_POOL;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_POOL_EXT;
#endif
		static std::string const & getName()
		{
			static std::string result{ "VkDescriptorSetPool" };
			return result;
		}
	};

	template<>
	struct AshesDebugTypeTraits< ashes::DescriptorPool >
	{
#if VK_EXT_debug_utils
		static VkObjectType constexpr UtilsValue = VK_OBJECT_TYPE_DESCRIPTOR_POOL;
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
		static VkDebugReportObjectTypeEXT constexpr ReportValue = VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_POOL_EXT;
#endif
		static std::string const & getName()
		{
			static std::string result{ "VkDescriptorPool" };
			return result;
		}
	};

	template<>
	struct AshesDebugTypeTraits< ashes::DescriptorSet >
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
	struct AshesDebugTypeTraits< ashes::FrameBuffer >
	{
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
	struct AshesDebugTypeTraits< ashes::CommandPool >
	{
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
	struct AshesDebugTypeTraits< ashes::Surface >
	{
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
	struct AshesDebugTypeTraits< ashes::SwapChain >
	{
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
			, VkAllocationCallbacks const * callbacks
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
			, VkExtent2D const & extent
			, uint32_t mipLevels = 1u )const;
		/**
		*\brief
		*	Creates a staging texture.
		*\param[in] extent
		*	The texture dimensions.
		*\return
		*	The created staging texture.
		*/
		StagingTexturePtr createStagingTexture( std::string const & debugName
			, VkFormat format
			, VkExtent2D const & extent
			, uint32_t mipLevels = 1u )const;
		/*
		*\brief
		*	Creates a render pass.
		*\param[in] createInfo
		*	The creation informations.
		*\return
		*	The created pass.
		*/
		RenderPassPtr createRenderPass( RenderPassCreateInfo createInfo )const;
		/*
		*\brief
		*	Creates a render pass.
		*\param[in] createInfo
		*	The creation informations.
		*\return
		*	The created pass.
		*/
		RenderPassPtr createRenderPass( std::string const & debugName
			, RenderPassCreateInfo createInfo )const;
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
		*	Creates a graphics pipeline using this layout.
		*\param[in] createInfo
		*	The creation informations.
		*\return
		*	The created pipeline.
		*/
		GraphicsPipelinePtr createPipeline( std::string const & debugName
			, GraphicsPipelineCreateInfo createInfo )const;
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
		*	Creates a compute pipeline using this layout.
		*\param[in] createInfo
		*	The creation informations.
		*\return
		*	The created pipeline.
		*/
		ComputePipelinePtr createPipeline( std::string const & debugName
			, ComputePipelineCreateInfo createInfo )const;
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
		*	Creates a pipeline layout.
		*\param[in] setLayouts
		*	The descriptor sets layouts.
		*\param[in] pushConstantRanges
		*	The push constants ranges.
		*\return
		*	The created layout.
		*/
		PipelineLayoutPtr createPipelineLayout( std::string const & debugName
			, DescriptorSetLayoutCRefArray const & setLayouts
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
		*	Creates a descriptor set layout.
		*\param[in] bindings
		*	The layout bindings.
		*\return
		*	The created layout.
		*/
		DescriptorSetLayoutPtr createDescriptorSetLayout( std::string const & debugName
			, VkDescriptorSetLayoutBindingArray bindings )const;
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
		DescriptorPoolPtr createDescriptorPool( std::string const & debugName
			, VkDescriptorPoolCreateFlags flags
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
		*	Allocates memory on the device.
		*\param[in] allocateInfo
		*	The memory allocation requirements.
		*\return
		*	The DeviceMemory object holding the allocated memory.
		*/
		DeviceMemoryPtr allocateMemory( std::string const & debugName
			, VkMemoryAllocateInfo allocateInfo )const;
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
		*\param[in] createInfo
		*	The creation informations.
		*/
		ImagePtr createImage( std::string const & debugName
			, ImageCreateInfo createInfo )const;
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
		*	Creates a sampler.
		*\param[in] createInfo
		*	The creation informations.
		*/
		SamplerPtr createSampler( std::string const & debugName
			, SamplerCreateInfo createInfo )const;
		/**
		*\brief
		*	Creates a sampler.
		*\param[in] createInfo
		*	The creation informations.
		*/
		SamplerPtr createSampler( VkSamplerCreateInfo createInfo )const;
		/**
		*\brief
		*	Creates a sampler.
		*\param[in] createInfo
		*	The creation informations.
		*/
		SamplerPtr createSampler( std::string const & debugName
			, VkSamplerCreateInfo createInfo )const;
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
		*	Creates a GPU buffer.
		*\param[in] size
		*	The buffer size.
		*\param[in] usage
		*	The buffer usage flags.
		*/
		BufferBasePtr createBuffer( std::string const & debugName
			, VkDeviceSize size
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
		BufferViewPtr createBufferView( std::string const & debugName
			, BufferBase const & buffer
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
		*	Creates a swap chain.
		*\param[in] createInfo
		*	The creation info.
		*/
		SwapChainPtr createSwapChain( std::string const & debugName
			, VkSwapchainCreateInfoKHR createInfo )const;
		/**
		*\brief
		*	Creates a semaphore.
		*/
		SemaphorePtr createSemaphore()const;
		/**
		*\brief
		*	Creates a semaphore.
		*/
		SemaphorePtr createSemaphore( std::string const & debugName )const;
		/**
		*\brief
		*	Creates a fence.
		*\param[in] flags
		*	The fence creation flags.
		*/
		FencePtr createFence( VkFenceCreateFlags flags = 0 )const;
		/**
		*\brief
		*	Creates a fence.
		*\param[in] flags
		*	The fence creation flags.
		*/
		FencePtr createFence( std::string const & debugName
			, VkFenceCreateFlags flags = 0 )const;
		/**
		*\brief
		*	Creates an event.
		*/
		EventPtr createEvent()const;
		/**
		*\brief
		*	Creates an event.
		*/
		EventPtr createEvent( std::string const & debugName )const;
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
		*	Creates a command buffer pool.
		*\param[in] queueFamilyIndex
		*	The family index of the queue to which the pool belongs.
		*\param[in] flags
		*	The command pool creation flags.
		*/
		CommandPoolPtr createCommandPool( std::string const & debugName
			, uint32_t queueFamilyIndex
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
		*	Creates a shader module.
		*\param[in] shader
		*	The module's SPIR-V code.
		*/
		ShaderModulePtr createShaderModule( std::string const & debugName
			, UInt32Array const & shader )const;
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
		QueryPoolPtr createQueryPool( std::string const & debugName
			, VkQueryType type
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
#if VK_EXT_debug_utils || VK_EXT_debug_marker

		/**
		*\brief
		*	Gives an object a tag.
		*\param[in] nameInfo
		*	The parameters of the tag to set on the object.
		*/
		inline void debugSetObjectTag( DebugObjectTagInfo const & tagInfo )const
		{
#	if VK_EXT_debug_utils
			setDebugUtilsObjectTag(
				{
					VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_TAG_INFO_EXT,
					nullptr,
					tagInfo.objectType,
					tagInfo.objectHandle,
					tagInfo.tagName,
					tagInfo.tagSize,
					tagInfo.pTag,
				} );
#	endif
#	if VK_EXT_debug_marker
			debugMarkerSetObjectTag(
				{
					VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_TAG_INFO_EXT,
					nullptr,
					VkDebugReportObjectTypeEXT( tagInfo.objectType ),
					tagInfo.objectHandle,
					tagInfo.tagName,
					tagInfo.tagSize,
					tagInfo.pTag,
				} );
#	endif
		}
		/**
		*\brief
		*	Gives an object a user-friendly name.
		*\param[in] nameInfo
		*	The parameters of the name to set on the object.
		*/
		inline void setDebugObjectName( DebugObjectNameInfo const & nameInfo )const
		{
#	if VK_EXT_debug_utils
			setDebugUtilsObjectName(
				{
					VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
					nullptr,
					nameInfo.objectType,
					nameInfo.object,
					nameInfo.objectName.c_str(),
				} );
#	endif
#	if VK_EXT_debug_marker
			debugMarkerSetObjectName(
				{
					VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT,
					nullptr,
					VkDebugReportObjectTypeEXT( nameInfo.objectType ),
					nameInfo.object,
					nameInfo.objectName.c_str(),
				} );
#	endif
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
		*\return
		*	The created layout.
		*/
		PipelineLayoutPtr createPipelineLayout( std::string const & debugName )const;
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
		*\param[in] layout
		*	The descriptor set layout.
		*\return
		*	The created layout.
		*/
		PipelineLayoutPtr createPipelineLayout( std::string const & debugName
			, DescriptorSetLayout const & layout )const;
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
		*\param[in] pushConstantRange
		*	The push constants range.
		*\return
		*	The created layout.
		*\return
		*	Le layout créé.
		*/
		PipelineLayoutPtr createPipelineLayout( std::string const & debugName
			, VkPushConstantRange const & pushConstantRange )const;
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
		*\param[in] layout
		*	The descriptor set layout.
		*\param[in] pushConstantRange
		*	The push constants range.
		*\return
		*	The created layout.
		*/
		PipelineLayoutPtr createPipelineLayout( std::string const & debugName
			, DescriptorSetLayout const & layout
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
		*\param[in] layouts
		*	The descriptor sets layouts.
		*\return
		*	The created layout.
		*/
		PipelineLayoutPtr createPipelineLayout( std::string const & debugName
			, DescriptorSetLayoutCRefArray const & layouts )const;
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
		*	Creates a pipeline layout.
		*\param[in] pushConstantRanges
		*	The push constants ranges.
		*\return
		*	The created layout.
		*/
		PipelineLayoutPtr createPipelineLayout( std::string const & debugName
			, VkPushConstantRangeArray const & pushConstantRanges )const;
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
		SamplerPtr createSampler( std::string const & debugName
			, VkSamplerAddressMode wrapS
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

		inline VkAllocationCallbacks const * getAllocationCallbacks()const
		{
			return m_callbacks;
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

		PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
#define VK_LIB_DEVICE_FUNCTION( ver, fun ) PFN_vk##fun vk##fun{ nullptr };
#	include <ashes/ashes_functions_list.hpp>

	private:
		Instance const & m_instance;
		VkAllocationCallbacks const * m_callbacks;
		PhysicalDevice const & m_physicalDevice;
		ashes::DeviceCreateInfo m_createInfos;
		VkDevice m_internal{ VK_NULL_HANDLE };
		float m_timestampPeriod;
		VkPhysicalDeviceMemoryProperties m_memoryProperties;
		VkPhysicalDeviceProperties m_properties;
		VkPhysicalDeviceFeatures m_features;
		VkQueueFamilyPropertiesArray m_queueFamilyProperties;

#if VK_EXT_debug_utils || VK_EXT_debug_marker
		struct ObjectAllocation
		{
			std::string type;
			std::string name;
			std::string callstack;
		};

		using CallstackCallback = std::function< std::string() >;
		CallstackCallback m_callstackCallback;
		mutable std::mutex m_allocationMutex;
		mutable std::unordered_map< size_t, ObjectAllocation > m_allocated;

	public:
		void setCallstackCallback( CallstackCallback callback )
		{
			m_callstackCallback = callback;
		}

		template< typename AshesType >
		static inline void stRegisterObject( Device const & device
			, std::string const & name
			, AshesType & object )
		{
			using VkType = typename AshesTypeTraits< AshesType >::VkType;
			using DebugTypeTraits = AshesDebugTypeTraits< AshesType >;
			device.doRegisterObject( uint64_t( VkType( object ) )
#if VK_EXT_debug_utils
				, uint32_t( DebugTypeTraits::UtilsValue )
#elif VK_EXT_debug_marker
				, uint32_t( DebugTypeTraits::ReportValue )
#endif
				, name
				, DebugTypeTraits::getName() );
		}

		template< typename AshesType >
		static inline void stUnregisterObject( Device const & device, AshesType & object )
		{
			using VkType = typename AshesTypeTraits< AshesType >::VkType;
			device.doUnregisterObject( uint64_t( VkType( object ) ) );
		}

	private:
		void doRegisterObject( uint64_t object
			, uint32_t objectType
			, std::string const & name
			, std::string const & typeName )const;
		void doUnregisterObject( uint64_t object )const;
		void doReportRegisteredObjects()const;

#	define registerObject( Dev, TypeName, Object )\
	Device::stRegisterObject( Dev, TypeName, Object )
#	define unregisterObject( Dev, Object )\
	Device::stUnregisterObject( Dev, Object )

#	ifndef NDEBUG
#		define reportRegisteredObjects()\
		doReportRegisteredObjects()
#	else
#		define reportRegisteredObjects()
#	endif
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

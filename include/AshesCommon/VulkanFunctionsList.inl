/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#if defined( CreateSemaphore )
#	undef CreateSemaphore
#endif

#if defined( CreateEvent )
#	undef CreateEvent
#endif

#ifndef VK_LIB_GLOBAL_FUNCTION
#	define VK_LIB_GLOBAL_FUNCTION( x )
#endif

#ifndef VK_LIB_GLOBAL_FUNCTION_EXT
#	define VK_LIB_GLOBAL_FUNCTION_EXT( x ) VK_LIB_GLOBAL_FUNCTION( x )
#endif

#ifdef VK_VERSION_1_0
	VK_LIB_GLOBAL_FUNCTION( CreateInstance )
	VK_LIB_GLOBAL_FUNCTION( EnumerateInstanceExtensionProperties )
	VK_LIB_GLOBAL_FUNCTION( EnumerateInstanceLayerProperties )
#endif

#ifdef VK_VERSION_1_1
	VK_LIB_GLOBAL_FUNCTION( EnumerateInstanceVersion )
#endif

#undef VK_LIB_GLOBAL_FUNCTION
#undef VK_LIB_GLOBAL_FUNCTION_EXT

#ifndef VK_LIB_INSTANCE_FUNCTION
#	define VK_LIB_INSTANCE_FUNCTION( x )
#endif

#ifndef VK_LIB_INSTANCE_FUNCTION_EXT
#	define VK_LIB_INSTANCE_FUNCTION_EXT( x ) VK_LIB_INSTANCE_FUNCTION( x )
#endif

#ifdef VK_VERSION_1_0
	VK_LIB_INSTANCE_FUNCTION( CreateDevice )
	VK_LIB_INSTANCE_FUNCTION( DestroyInstance )
	VK_LIB_INSTANCE_FUNCTION( EnumeratePhysicalDevices )
	VK_LIB_INSTANCE_FUNCTION( EnumerateDeviceExtensionProperties )
	VK_LIB_INSTANCE_FUNCTION( EnumerateDeviceLayerProperties )
	VK_LIB_INSTANCE_FUNCTION( GetDeviceProcAddr )
	VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceFeatures )
	VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceFormatProperties )
	VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceImageFormatProperties )
	VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceMemoryProperties )
	VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceProperties )
	VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceQueueFamilyProperties )
#endif

#ifdef VK_VERSION_1_1
	VK_LIB_INSTANCE_FUNCTION( EnumeratePhysicalDeviceGroups )
	VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceFeatures2 )
	VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceProperties2 )
	VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceFormatProperties2 )
	VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceImageFormatProperties2 )
	VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceQueueFamilyProperties2 )
	VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceMemoryProperties2 )
	VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceSparseImageFormatProperties2 )
	VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceExternalBufferProperties )
	VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceExternalFenceProperties )
	VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceExternalSemaphoreProperties )
#endif

#ifdef VK_KHR_surface
	VK_LIB_INSTANCE_FUNCTION( DestroySurfaceKHR )
	VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceSurfaceCapabilitiesKHR )
	VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceSurfaceFormatsKHR )
	VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceSurfaceSupportKHR )
	VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceSurfacePresentModesKHR )
#endif

#ifdef VK_KHR_swapchain
	VK_LIB_INSTANCE_FUNCTION( GetPhysicalDevicePresentRectanglesKHR )
#endif

#ifdef VK_KHR_display
	VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceDisplayPropertiesKHR )
	VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceDisplayPlanePropertiesKHR )
	VK_LIB_INSTANCE_FUNCTION( GetDisplayPlaneSupportedDisplaysKHR )
	VK_LIB_INSTANCE_FUNCTION( GetDisplayModePropertiesKHR )
	VK_LIB_INSTANCE_FUNCTION( CreateDisplayModeKHR )
	VK_LIB_INSTANCE_FUNCTION( GetDisplayPlaneCapabilitiesKHR )
	VK_LIB_INSTANCE_FUNCTION( CreateDisplayPlaneSurfaceKHR )
#endif

#ifdef VK_KHR_get_display_properties2
	VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceDisplayProperties2KHR )
	VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceDisplayPlaneProperties2KHR )
	VK_LIB_INSTANCE_FUNCTION( GetDisplayModeProperties2KHR )
	VK_LIB_INSTANCE_FUNCTION( GetDisplayPlaneCapabilities2KHR )
#endif

#ifdef VK_KHR_display_swapchain
	VK_LIB_INSTANCE_FUNCTION_EXT( GetPhysicalDeviceFeatures2KHR )
	VK_LIB_INSTANCE_FUNCTION_EXT( GetPhysicalDeviceProperties2KHR )
	VK_LIB_INSTANCE_FUNCTION_EXT( GetPhysicalDeviceFormatProperties2KHR )
	VK_LIB_INSTANCE_FUNCTION_EXT( GetPhysicalDeviceImageFormatProperties2KHR )
	VK_LIB_INSTANCE_FUNCTION_EXT( GetPhysicalDeviceQueueFamilyProperties2KHR )
	VK_LIB_INSTANCE_FUNCTION_EXT( GetPhysicalDeviceMemoryProperties2KHR )
	VK_LIB_INSTANCE_FUNCTION_EXT( GetPhysicalDeviceSparseImageFormatProperties2KHR )
#endif

#ifdef VK_KHR_device_group_creation
	VK_LIB_INSTANCE_FUNCTION_EXT( EnumeratePhysicalDeviceGroupsKHR )
#endif

#ifdef VK_KHR_external_memory_capabilities
	VK_LIB_INSTANCE_FUNCTION_EXT( GetPhysicalDeviceExternalBufferPropertiesKHR )
#endif

#ifdef VK_KHR_external_semaphore_capabilities
	VK_LIB_INSTANCE_FUNCTION_EXT( GetPhysicalDeviceExternalSemaphorePropertiesKHR )
#endif

#ifdef VK_KHR_external_fence_capabilities
	VK_LIB_INSTANCE_FUNCTION_EXT( GetPhysicalDeviceExternalFencePropertiesKHR )
#endif

#ifdef VK_KHR_get_surface_capabilities2
	VK_LIB_INSTANCE_FUNCTION_EXT( GetPhysicalDeviceSurfaceCapabilities2KHR )
	VK_LIB_INSTANCE_FUNCTION_EXT( GetPhysicalDeviceSurfaceFormats2KHR )
#endif

#ifdef VK_EXT_debug_report
	VK_LIB_INSTANCE_FUNCTION_EXT( CreateDebugReportCallbackEXT )
	VK_LIB_INSTANCE_FUNCTION_EXT( DestroyDebugReportCallbackEXT )
	VK_LIB_INSTANCE_FUNCTION_EXT( DebugReportMessageEXT )
#endif

#ifdef VK_NV_external_memory_capabilities
	VK_LIB_INSTANCE_FUNCTION_EXT( GetPhysicalDeviceExternalImageFormatPropertiesNV )
#endif

#ifdef VK_EXT_direct_mode_display
	VK_LIB_INSTANCE_FUNCTION_EXT( ReleaseDisplayEXT )
#endif

#ifdef VK_NVX_device_generated_commands
	VK_LIB_INSTANCE_FUNCTION_EXT( GetPhysicalDeviceGeneratedCommandsPropertiesNVX )
#endif

#ifdef VK_EXT_display_surface_counter
	VK_LIB_INSTANCE_FUNCTION_EXT( GetPhysicalDeviceSurfaceCapabilities2EXT )
#endif

#ifdef VK_EXT_debug_utils
	VK_LIB_INSTANCE_FUNCTION_EXT( CreateDebugUtilsMessengerEXT )
	VK_LIB_INSTANCE_FUNCTION_EXT( DestroyDebugUtilsMessengerEXT )
	VK_LIB_INSTANCE_FUNCTION_EXT( SubmitDebugUtilsMessageEXT )
#endif

#ifdef VK_EXT_sample_locations
	VK_LIB_INSTANCE_FUNCTION_EXT( GetPhysicalDeviceMultisamplePropertiesEXT )
#endif

#ifdef VK_EXT_calibrated_timestamps
	VK_LIB_INSTANCE_FUNCTION_EXT( GetPhysicalDeviceCalibrateableTimeDomainsEXT )
#endif

#ifdef VK_NV_cooperative_matrix
	VK_LIB_INSTANCE_FUNCTION_EXT( GetPhysicalDeviceCooperativeMatrixPropertiesNV )
#endif

#ifdef VK_USE_PLATFORM_ANDROID_KHR
#	ifdef VK_KHR_android_surface
	VK_LIB_INSTANCE_FUNCTION( CreateAndroidSurfaceKHR )
#	endif
#endif

#ifdef VK_USE_PLATFORM_MIR_KHR
#	ifdef VK_KHR_mir_surface
	VK_LIB_INSTANCE_FUNCTION( CreateMirSurfaceKHR )
	VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceMirPresentationSupportKHR )
#	endif
#endif

#ifdef VK_USE_PLATFORM_XCB_KHR
#	ifdef VK_KHR_xcb_surface
	VK_LIB_INSTANCE_FUNCTION( CreateXcbSurfaceKHR )
	VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceXcbPresentationSupportKHR )
#	endif
#endif

#ifdef VK_USE_PLATFORM_XLIB_KHR
#	ifdef VK_KHR_xlib_surface
	VK_LIB_INSTANCE_FUNCTION( CreateXlibSurfaceKHR )
	VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceXlibPresentationSupportKHR )
#	endif
#endif

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
#	ifdef VK_KHR_wayland_surface
	VK_LIB_INSTANCE_FUNCTION( CreateWaylandSurfaceKHR )
	VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceWaylandPresentationSupportKHR )
#	endif
#endif

#ifdef VK_USE_PLATFORM_WIN32_KHR
#	ifdef VK_KHR_win32_surface
	VK_LIB_INSTANCE_FUNCTION( CreateWin32SurfaceKHR )
	VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceWin32PresentationSupportKHR )
#	endif
#endif

#undef VK_LIB_INSTANCE_FUNCTION
#undef VK_LIB_INSTANCE_FUNCTION_EXT

#ifndef VK_LIB_DEVICE_FUNCTION
#	define VK_LIB_DEVICE_FUNCTION( x )
#endif

#ifndef VK_LIB_DEVICE_FUNCTION_EXT
#	define VK_LIB_DEVICE_FUNCTION_EXT( x ) VK_LIB_DEVICE_FUNCTION( x )
#endif

#ifdef VK_VERSION_1_0
	VK_LIB_DEVICE_FUNCTION( AllocateCommandBuffers )
	VK_LIB_DEVICE_FUNCTION( AllocateDescriptorSets )
	VK_LIB_DEVICE_FUNCTION( AllocateMemory )
	VK_LIB_DEVICE_FUNCTION( BeginCommandBuffer )
	VK_LIB_DEVICE_FUNCTION( BindBufferMemory )
	VK_LIB_DEVICE_FUNCTION( BindImageMemory )
	VK_LIB_DEVICE_FUNCTION( CmdBeginQuery )
	VK_LIB_DEVICE_FUNCTION( CmdBeginRenderPass )
	VK_LIB_DEVICE_FUNCTION( CmdBindDescriptorSets )
	VK_LIB_DEVICE_FUNCTION( CmdBindIndexBuffer )
	VK_LIB_DEVICE_FUNCTION( CmdBindPipeline )
	VK_LIB_DEVICE_FUNCTION( CmdBindVertexBuffers )
	VK_LIB_DEVICE_FUNCTION( CmdBlitImage )
	VK_LIB_DEVICE_FUNCTION( CmdClearAttachments )
	VK_LIB_DEVICE_FUNCTION( CmdClearColorImage )
	VK_LIB_DEVICE_FUNCTION( CmdClearDepthStencilImage )
	VK_LIB_DEVICE_FUNCTION( CmdCopyBuffer )
	VK_LIB_DEVICE_FUNCTION( CmdCopyBufferToImage )
	VK_LIB_DEVICE_FUNCTION( CmdCopyImage )
	VK_LIB_DEVICE_FUNCTION( CmdDispatch )
	VK_LIB_DEVICE_FUNCTION( CmdDispatchIndirect )
	VK_LIB_DEVICE_FUNCTION( CmdDraw )
	VK_LIB_DEVICE_FUNCTION( CmdDrawIndexed )
	VK_LIB_DEVICE_FUNCTION( CmdDrawIndirect )
	VK_LIB_DEVICE_FUNCTION( CmdDrawIndexedIndirect )
	VK_LIB_DEVICE_FUNCTION( CmdEndQuery )
	VK_LIB_DEVICE_FUNCTION( CmdEndRenderPass )
	VK_LIB_DEVICE_FUNCTION( CmdExecuteCommands )
	VK_LIB_DEVICE_FUNCTION( CmdCopyImageToBuffer )
	VK_LIB_DEVICE_FUNCTION( CmdNextSubpass )
	VK_LIB_DEVICE_FUNCTION( CmdPipelineBarrier )
	VK_LIB_DEVICE_FUNCTION( CmdPushConstants )
	VK_LIB_DEVICE_FUNCTION( CmdResetEvent )
	VK_LIB_DEVICE_FUNCTION( CmdResetQueryPool )
	VK_LIB_DEVICE_FUNCTION( CmdSetDepthBias )
	VK_LIB_DEVICE_FUNCTION( CmdSetEvent )
	VK_LIB_DEVICE_FUNCTION( CmdSetLineWidth )
	VK_LIB_DEVICE_FUNCTION( CmdSetScissor )
	VK_LIB_DEVICE_FUNCTION( CmdSetViewport )
	VK_LIB_DEVICE_FUNCTION( CmdWaitEvents )
	VK_LIB_DEVICE_FUNCTION( CmdWriteTimestamp )
	VK_LIB_DEVICE_FUNCTION( CreateBuffer )
	VK_LIB_DEVICE_FUNCTION( CreateBufferView )
	VK_LIB_DEVICE_FUNCTION( CreateCommandPool )
	VK_LIB_DEVICE_FUNCTION( CreateComputePipelines )
	VK_LIB_DEVICE_FUNCTION( CreateDescriptorPool )
	VK_LIB_DEVICE_FUNCTION( CreateDescriptorSetLayout )
	VK_LIB_DEVICE_FUNCTION( CreateEvent )
	VK_LIB_DEVICE_FUNCTION( CreateFence )
	VK_LIB_DEVICE_FUNCTION( CreateFramebuffer )
	VK_LIB_DEVICE_FUNCTION( CreateGraphicsPipelines )
	VK_LIB_DEVICE_FUNCTION( CreateImage )
	VK_LIB_DEVICE_FUNCTION( CreateImageView )
	VK_LIB_DEVICE_FUNCTION( CreatePipelineLayout )
	VK_LIB_DEVICE_FUNCTION( CreateRenderPass )
	VK_LIB_DEVICE_FUNCTION( CreateQueryPool )
	VK_LIB_DEVICE_FUNCTION( CreateSampler )
	VK_LIB_DEVICE_FUNCTION( CreateSemaphore )
	VK_LIB_DEVICE_FUNCTION( CreateShaderModule )
	VK_LIB_DEVICE_FUNCTION( DestroyBuffer )
	VK_LIB_DEVICE_FUNCTION( DestroyBufferView )
	VK_LIB_DEVICE_FUNCTION( DestroyCommandPool )
	VK_LIB_DEVICE_FUNCTION( DestroyDescriptorPool )
	VK_LIB_DEVICE_FUNCTION( DestroyDescriptorSetLayout )
	VK_LIB_DEVICE_FUNCTION( DestroyDevice )
	VK_LIB_DEVICE_FUNCTION( DestroyEvent )
	VK_LIB_DEVICE_FUNCTION( DestroyFence )
	VK_LIB_DEVICE_FUNCTION( DestroyFramebuffer )
	VK_LIB_DEVICE_FUNCTION( DestroyImage )
	VK_LIB_DEVICE_FUNCTION( DestroyImageView )
	VK_LIB_DEVICE_FUNCTION( DestroyPipeline )
	VK_LIB_DEVICE_FUNCTION( DestroyPipelineLayout )
	VK_LIB_DEVICE_FUNCTION( DestroyQueryPool )
	VK_LIB_DEVICE_FUNCTION( DestroyRenderPass )
	VK_LIB_DEVICE_FUNCTION( DestroySampler )
	VK_LIB_DEVICE_FUNCTION( DestroySemaphore )
	VK_LIB_DEVICE_FUNCTION( DestroyShaderModule )
	VK_LIB_DEVICE_FUNCTION( DeviceWaitIdle )
	VK_LIB_DEVICE_FUNCTION( EndCommandBuffer )
	VK_LIB_DEVICE_FUNCTION( FlushMappedMemoryRanges )
	VK_LIB_DEVICE_FUNCTION( FreeCommandBuffers )
	VK_LIB_DEVICE_FUNCTION( FreeDescriptorSets )
	VK_LIB_DEVICE_FUNCTION( FreeMemory )
	VK_LIB_DEVICE_FUNCTION( GetBufferMemoryRequirements )
	VK_LIB_DEVICE_FUNCTION( GetDeviceQueue )
	VK_LIB_DEVICE_FUNCTION( GetEventStatus )
	VK_LIB_DEVICE_FUNCTION( GetImageMemoryRequirements )
	VK_LIB_DEVICE_FUNCTION( GetImageSparseMemoryRequirements )
	VK_LIB_DEVICE_FUNCTION( GetPhysicalDeviceSparseImageFormatProperties )
	VK_LIB_DEVICE_FUNCTION( GetImageSubresourceLayout )
	VK_LIB_DEVICE_FUNCTION( GetQueryPoolResults )
	VK_LIB_DEVICE_FUNCTION( InvalidateMappedMemoryRanges )
	VK_LIB_DEVICE_FUNCTION( GetDeviceMemoryCommitment )
	VK_LIB_DEVICE_FUNCTION( MapMemory )
	VK_LIB_DEVICE_FUNCTION( QueueSubmit )
	VK_LIB_DEVICE_FUNCTION( QueueWaitIdle )
	VK_LIB_DEVICE_FUNCTION( ResetCommandBuffer )
	VK_LIB_DEVICE_FUNCTION( ResetEvent )
	VK_LIB_DEVICE_FUNCTION( ResetFences )
	VK_LIB_DEVICE_FUNCTION( SetEvent )
	VK_LIB_DEVICE_FUNCTION( UnmapMemory )
	VK_LIB_DEVICE_FUNCTION( UpdateDescriptorSets )
	VK_LIB_DEVICE_FUNCTION( WaitForFences )
	VK_LIB_DEVICE_FUNCTION( CreatePipelineCache )
	VK_LIB_DEVICE_FUNCTION( DestroyPipelineCache )
	VK_LIB_DEVICE_FUNCTION( GetPipelineCacheData )
	VK_LIB_DEVICE_FUNCTION( MergePipelineCaches )
	VK_LIB_DEVICE_FUNCTION( ResetDescriptorPool )
	VK_LIB_DEVICE_FUNCTION( GetRenderAreaGranularity )
	VK_LIB_DEVICE_FUNCTION( CmdSetBlendConstants )
	VK_LIB_DEVICE_FUNCTION( CmdSetDepthBounds )
	VK_LIB_DEVICE_FUNCTION( CmdSetStencilCompareMask )
	VK_LIB_DEVICE_FUNCTION( CmdSetStencilWriteMask )
	VK_LIB_DEVICE_FUNCTION( CmdSetStencilReference )
	VK_LIB_DEVICE_FUNCTION( CmdUpdateBuffer )
	VK_LIB_DEVICE_FUNCTION( CmdFillBuffer )
	VK_LIB_DEVICE_FUNCTION( CmdResolveImage )
	VK_LIB_DEVICE_FUNCTION( QueueBindSparse )
	VK_LIB_DEVICE_FUNCTION( GetFenceStatus )
	VK_LIB_DEVICE_FUNCTION( ResetCommandPool )
	VK_LIB_DEVICE_FUNCTION( CmdCopyQueryPoolResults )
#endif

#ifdef VK_VERSION_1_1
	VK_LIB_DEVICE_FUNCTION( BindBufferMemory2 )
	VK_LIB_DEVICE_FUNCTION( BindImageMemory2 )
	VK_LIB_DEVICE_FUNCTION( GetDeviceGroupPeerMemoryFeatures )
	VK_LIB_DEVICE_FUNCTION( CmdSetDeviceMask )
	VK_LIB_DEVICE_FUNCTION( CmdDispatchBase )
	VK_LIB_DEVICE_FUNCTION( GetImageMemoryRequirements2 )
	VK_LIB_DEVICE_FUNCTION( GetBufferMemoryRequirements2 )
	VK_LIB_DEVICE_FUNCTION( GetImageSparseMemoryRequirements2 )
	VK_LIB_DEVICE_FUNCTION( TrimCommandPool )
	VK_LIB_DEVICE_FUNCTION( GetDeviceQueue2 )
	VK_LIB_DEVICE_FUNCTION( CreateSamplerYcbcrConversion )
	VK_LIB_DEVICE_FUNCTION( DestroySamplerYcbcrConversion )
	VK_LIB_DEVICE_FUNCTION( CreateDescriptorUpdateTemplate )
	VK_LIB_DEVICE_FUNCTION( DestroyDescriptorUpdateTemplate )
	VK_LIB_DEVICE_FUNCTION( UpdateDescriptorSetWithTemplate )
	VK_LIB_DEVICE_FUNCTION( GetDescriptorSetLayoutSupport )
#endif

#ifdef VK_KHR_swapchain
	VK_LIB_DEVICE_FUNCTION( CreateSwapchainKHR )
	VK_LIB_DEVICE_FUNCTION( DestroySwapchainKHR )
	VK_LIB_DEVICE_FUNCTION( GetSwapchainImagesKHR )
	VK_LIB_DEVICE_FUNCTION( AcquireNextImageKHR )
	VK_LIB_DEVICE_FUNCTION( QueuePresentKHR )
	VK_LIB_DEVICE_FUNCTION( GetDeviceGroupPresentCapabilitiesKHR )
	VK_LIB_DEVICE_FUNCTION( GetDeviceGroupSurfacePresentModesKHR )
	VK_LIB_DEVICE_FUNCTION( AcquireNextImage2KHR )
#endif

#ifdef VK_KHR_device_group
	VK_LIB_DEVICE_FUNCTION_EXT( GetDeviceGroupPeerMemoryFeaturesKHR )
	VK_LIB_DEVICE_FUNCTION_EXT( CmdSetDeviceMaskKHR )
	VK_LIB_DEVICE_FUNCTION_EXT( CmdDispatchBaseKHR )
#endif

#ifdef VK_KHR_shader_draw_parameters
	VK_LIB_DEVICE_FUNCTION_EXT( TrimCommandPoolKHR )
#endif

#ifdef VK_KHR_external_memory_fd
	VK_LIB_DEVICE_FUNCTION_EXT( GetMemoryFdKHR )
	VK_LIB_DEVICE_FUNCTION_EXT( GetMemoryFdPropertiesKHR )
#endif

#ifdef VK_KHR_external_semaphore_fd
	VK_LIB_DEVICE_FUNCTION_EXT( ImportSemaphoreFdKHR )
	VK_LIB_DEVICE_FUNCTION_EXT( GetSemaphoreFdKHR )
#endif

#ifdef VK_KHR_push_descriptor
	VK_LIB_DEVICE_FUNCTION_EXT( CmdPushDescriptorSetKHR )
	VK_LIB_DEVICE_FUNCTION_EXT( CmdPushDescriptorSetWithTemplateKHR )
#endif

#ifdef VK_KHR_descriptor_update_template
	VK_LIB_DEVICE_FUNCTION_EXT( CreateDescriptorUpdateTemplateKHR )
	VK_LIB_DEVICE_FUNCTION_EXT( DestroyDescriptorUpdateTemplateKHR )
	VK_LIB_DEVICE_FUNCTION_EXT( UpdateDescriptorSetWithTemplateKHR )
#endif

#ifdef VK_KHR_create_renderpass2
	VK_LIB_DEVICE_FUNCTION_EXT( CreateRenderPass2KHR )
	VK_LIB_DEVICE_FUNCTION_EXT( CmdBeginRenderPass2KHR )
	VK_LIB_DEVICE_FUNCTION_EXT( CmdNextSubpass2KHR )
	VK_LIB_DEVICE_FUNCTION_EXT( CmdEndRenderPass2KHR )
#endif

#ifdef VK_KHR_shared_presentable_image
	VK_LIB_DEVICE_FUNCTION_EXT( GetSwapchainStatusKHR )
#endif

#ifdef VK_KHR_external_fence_fd
	VK_LIB_DEVICE_FUNCTION_EXT( ImportFenceFdKHR )
	VK_LIB_DEVICE_FUNCTION_EXT( GetFenceFdKHR )
#endif

#ifdef VK_KHR_get_memory_requirements2
	VK_LIB_DEVICE_FUNCTION_EXT( GetImageMemoryRequirements2KHR )
	VK_LIB_DEVICE_FUNCTION_EXT( GetBufferMemoryRequirements2KHR )
	VK_LIB_DEVICE_FUNCTION_EXT( GetImageSparseMemoryRequirements2KHR )
#endif

#ifdef VK_KHR_sampler_ycbcr_conversion
	VK_LIB_DEVICE_FUNCTION_EXT( CreateSamplerYcbcrConversionKHR )
	VK_LIB_DEVICE_FUNCTION_EXT( DestroySamplerYcbcrConversionKHR )
#endif

#ifdef VK_KHR_bind_memory2
	VK_LIB_DEVICE_FUNCTION_EXT( BindBufferMemory2KHR )
	VK_LIB_DEVICE_FUNCTION_EXT( BindImageMemory2KHR )
#endif

#ifdef VK_KHR_maintenance3
	VK_LIB_DEVICE_FUNCTION_EXT( GetDescriptorSetLayoutSupportKHR )
#endif

#ifdef VK_KHR_draw_indirect_count
	VK_LIB_DEVICE_FUNCTION_EXT( CmdDrawIndirectCountKHR )
	VK_LIB_DEVICE_FUNCTION_EXT( CmdDrawIndexedIndirectCountKHR )
#endif

#ifdef VK_EXT_debug_marker
	VK_LIB_DEVICE_FUNCTION_EXT( DebugMarkerSetObjectTagEXT )
	VK_LIB_DEVICE_FUNCTION_EXT( DebugMarkerSetObjectNameEXT )
	VK_LIB_DEVICE_FUNCTION_EXT( CmdDebugMarkerBeginEXT )
	VK_LIB_DEVICE_FUNCTION_EXT( CmdDebugMarkerEndEXT )
	VK_LIB_DEVICE_FUNCTION_EXT( CmdDebugMarkerInsertEXT )
#endif

#ifdef VK_EXT_transform_feedback
	VK_LIB_DEVICE_FUNCTION_EXT( CmdBindTransformFeedbackBuffersEXT )
	VK_LIB_DEVICE_FUNCTION_EXT( CmdBeginTransformFeedbackEXT )
	VK_LIB_DEVICE_FUNCTION_EXT( CmdEndTransformFeedbackEXT )
	VK_LIB_DEVICE_FUNCTION_EXT( CmdBeginQueryIndexedEXT )
	VK_LIB_DEVICE_FUNCTION_EXT( CmdEndQueryIndexedEXT )
	VK_LIB_DEVICE_FUNCTION_EXT( CmdDrawIndirectByteCountEXT )
#endif

#ifdef VK_AMD_draw_indirect_count
	VK_LIB_DEVICE_FUNCTION_EXT( CmdDrawIndirectCountAMD )
	VK_LIB_DEVICE_FUNCTION_EXT( CmdDrawIndexedIndirectCountAMD )
#endif

#ifdef VK_AMD_shader_info
	VK_LIB_DEVICE_FUNCTION_EXT( GetShaderInfoAMD )
#endif

#ifdef VK_EXT_conditional_rendering
	VK_LIB_DEVICE_FUNCTION_EXT( CmdBeginConditionalRenderingEXT )
	VK_LIB_DEVICE_FUNCTION_EXT( CmdEndConditionalRenderingEXT )
#endif

#ifdef VK_NVX_device_generated_commands
	VK_LIB_DEVICE_FUNCTION_EXT( CmdProcessCommandsNVX )
	VK_LIB_DEVICE_FUNCTION_EXT( CmdReserveSpaceForCommandsNVX )
	VK_LIB_DEVICE_FUNCTION_EXT( CreateIndirectCommandsLayoutNVX )
	VK_LIB_DEVICE_FUNCTION_EXT( DestroyIndirectCommandsLayoutNVX )
	VK_LIB_DEVICE_FUNCTION_EXT( CreateObjectTableNVX )
	VK_LIB_DEVICE_FUNCTION_EXT( DestroyObjectTableNVX )
	VK_LIB_DEVICE_FUNCTION_EXT( RegisterObjectsNVX )
	VK_LIB_DEVICE_FUNCTION_EXT( UnregisterObjectsNVX )
#endif

#ifdef VK_NV_clip_space_w_scaling
	VK_LIB_DEVICE_FUNCTION_EXT( CmdSetViewportWScalingNV )
#endif

#ifdef VK_EXT_display_control
	VK_LIB_DEVICE_FUNCTION_EXT( DisplayPowerControlEXT )
	VK_LIB_DEVICE_FUNCTION_EXT( RegisterDeviceEventEXT )
	VK_LIB_DEVICE_FUNCTION_EXT( RegisterDisplayEventEXT )
	VK_LIB_DEVICE_FUNCTION_EXT( GetSwapchainCounterEXT )
#endif

#ifdef VK_GOOGLE_display_timing
	VK_LIB_DEVICE_FUNCTION_EXT( GetRefreshCycleDurationGOOGLE )
	VK_LIB_DEVICE_FUNCTION_EXT( GetPastPresentationTimingGOOGLE )
#endif

#ifdef VK_EXT_discard_rectangles
	VK_LIB_DEVICE_FUNCTION_EXT( CmdSetDiscardRectangleEXT )
#endif

#ifdef VK_EXT_hdr_metadata
	VK_LIB_DEVICE_FUNCTION_EXT( SetHdrMetadataEXT )
#endif

#ifdef VK_EXT_debug_utils
	VK_LIB_DEVICE_FUNCTION_EXT( SetDebugUtilsObjectNameEXT )
	VK_LIB_DEVICE_FUNCTION_EXT( SetDebugUtilsObjectTagEXT )
	VK_LIB_DEVICE_FUNCTION_EXT( QueueBeginDebugUtilsLabelEXT )
	VK_LIB_DEVICE_FUNCTION_EXT( QueueEndDebugUtilsLabelEXT )
	VK_LIB_DEVICE_FUNCTION_EXT( QueueInsertDebugUtilsLabelEXT )
	VK_LIB_DEVICE_FUNCTION_EXT( CmdBeginDebugUtilsLabelEXT )
	VK_LIB_DEVICE_FUNCTION_EXT( CmdEndDebugUtilsLabelEXT )
	VK_LIB_DEVICE_FUNCTION_EXT( CmdInsertDebugUtilsLabelEXT )
#endif

#ifdef VK_EXT_sample_locations
	VK_LIB_DEVICE_FUNCTION_EXT( CmdSetSampleLocationsEXT )
#endif

#ifdef VK_EXT_image_drm_format_modifier
	VK_LIB_DEVICE_FUNCTION_EXT( GetImageDrmFormatModifierPropertiesEXT )
#endif

#ifdef VK_EXT_validation_cache
	VK_LIB_DEVICE_FUNCTION_EXT( CreateValidationCacheEXT )
	VK_LIB_DEVICE_FUNCTION_EXT( DestroyValidationCacheEXT )
	VK_LIB_DEVICE_FUNCTION_EXT( MergeValidationCachesEXT )
	VK_LIB_DEVICE_FUNCTION_EXT( GetValidationCacheDataEXT )
#endif

#ifdef VK_NV_shading_rate_image
	VK_LIB_DEVICE_FUNCTION_EXT( CmdBindShadingRateImageNV )
	VK_LIB_DEVICE_FUNCTION_EXT( CmdSetViewportShadingRatePaletteNV )
	VK_LIB_DEVICE_FUNCTION_EXT( CmdSetCoarseSampleOrderNV )
#endif

#ifdef VK_NV_ray_tracing
	VK_LIB_DEVICE_FUNCTION_EXT( CreateAccelerationStructureNV )
	VK_LIB_DEVICE_FUNCTION_EXT( DestroyAccelerationStructureNV )
	VK_LIB_DEVICE_FUNCTION_EXT( GetAccelerationStructureMemoryRequirementsNV )
	VK_LIB_DEVICE_FUNCTION_EXT( BindAccelerationStructureMemoryNV )
	VK_LIB_DEVICE_FUNCTION_EXT( CmdBuildAccelerationStructureNV )
	VK_LIB_DEVICE_FUNCTION_EXT( CmdCopyAccelerationStructureNV )
	VK_LIB_DEVICE_FUNCTION_EXT( CmdTraceRaysNV )
	VK_LIB_DEVICE_FUNCTION_EXT( CreateRayTracingPipelinesNV )
	VK_LIB_DEVICE_FUNCTION_EXT( GetRayTracingShaderGroupHandlesNV )
	VK_LIB_DEVICE_FUNCTION_EXT( GetAccelerationStructureHandleNV )
	VK_LIB_DEVICE_FUNCTION_EXT( CmdWriteAccelerationStructuresPropertiesNV )
	VK_LIB_DEVICE_FUNCTION_EXT( CompileDeferredNV )
#endif

#ifdef VK_EXT_external_memory_host
	VK_LIB_DEVICE_FUNCTION_EXT( GetMemoryHostPointerPropertiesEXT )
#endif

#ifdef VK_AMD_buffer_marker
	VK_LIB_DEVICE_FUNCTION_EXT( CmdWriteBufferMarkerAMD )
#endif

#ifdef VK_EXT_calibrated_timestamps
	VK_LIB_DEVICE_FUNCTION_EXT( GetCalibratedTimestampsEXT )
#endif

#ifdef VK_NV_mesh_shader
	VK_LIB_DEVICE_FUNCTION_EXT( CmdDrawMeshTasksNV )
	VK_LIB_DEVICE_FUNCTION_EXT( CmdDrawMeshTasksIndirectNV )
	VK_LIB_DEVICE_FUNCTION_EXT( CmdDrawMeshTasksIndirectCountNV )
#endif

#ifdef VK_NV_scissor_exclusive
	VK_LIB_DEVICE_FUNCTION_EXT( CmdSetExclusiveScissorNV )
#endif

#ifdef VK_NV_device_diagnostic_checkpoints
	VK_LIB_DEVICE_FUNCTION_EXT( CmdSetCheckpointNV )
	VK_LIB_DEVICE_FUNCTION_EXT( GetQueueCheckpointDataNV )
#endif

#ifdef VK_EXT_buffer_device_address
	VK_LIB_DEVICE_FUNCTION_EXT( GetBufferDeviceAddressEXT )
#endif

#undef VK_LIB_DEVICE_FUNCTION
#undef VK_LIB_DEVICE_FUNCTION_EXT

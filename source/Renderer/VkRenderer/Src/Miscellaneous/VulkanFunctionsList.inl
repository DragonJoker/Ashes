/*
This file belongs to VkLib.
See LICENSE file in root folder.
*/
#if defined( CreateSemaphore )
#	undef CreateSemaphore
#endif

#ifndef VK_LIB_GLOBAL_FUNCTION
#	define VK_LIB_GLOBAL_FUNCTION( x )
#endif

VK_LIB_GLOBAL_FUNCTION( CreateInstance )
VK_LIB_GLOBAL_FUNCTION( EnumerateInstanceExtensionProperties )
VK_LIB_GLOBAL_FUNCTION( EnumerateInstanceLayerProperties )

#undef VK_LIB_GLOBAL_FUNCTION

#ifndef VK_LIB_INSTANCE_FUNCTION
#	define VK_LIB_INSTANCE_FUNCTION( x )
#endif


// Vulkan core
VK_LIB_INSTANCE_FUNCTION( CreateDevice );
VK_LIB_INSTANCE_FUNCTION( DestroyInstance );
VK_LIB_INSTANCE_FUNCTION( EnumeratePhysicalDevices );
VK_LIB_INSTANCE_FUNCTION( EnumerateDeviceExtensionProperties )
VK_LIB_INSTANCE_FUNCTION( GetDeviceProcAddr );
VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceFeatures );
VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceFormatProperties );
VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceImageFormatProperties );
VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceMemoryProperties );
VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceProperties );
VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceQueueFamilyProperties );

// VK_KHR_display
//VK_LIB_INSTANCE_FUNCTION( CreateDisplayPlaneSurfaceKHR );
//VK_LIB_INSTANCE_FUNCTION( GetDisplayModePropertiesKHR );
//VK_LIB_INSTANCE_FUNCTION( GetDisplayPlaneCapabilitiesKHR );
//VK_LIB_INSTANCE_FUNCTION( GetDisplayPlaneSupportedDisplaysKHR );
//VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceDisplayPlanePropertiesKHR );
//VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceDisplayPropertiesKHR );

// VK_KHR_surface
VK_LIB_INSTANCE_FUNCTION( DestroySurfaceKHR );
VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceSurfaceCapabilitiesKHR );
VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceSurfaceFormatsKHR );
VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceSurfacePresentModesKHR );
VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceSurfaceSupportKHR );

// VK_EXT_debug_report
VK_LIB_INSTANCE_FUNCTION( CreateDebugReportCallbackEXT );
VK_LIB_INSTANCE_FUNCTION( DestroyDebugReportCallbackEXT );
VK_LIB_INSTANCE_FUNCTION( DebugReportMessageEXT );

#ifdef VK_USE_PLATFORM_ANDROID_KHR
// VK_KHR_android_surface
VK_LIB_INSTANCE_FUNCTION( CreateAndroidSurfaceKHR );
#endif

#ifdef VK_USE_PLATFORM_MIR_KHR
// VK_KHR_mir_surface
VK_LIB_INSTANCE_FUNCTION( CreateMirSurfaceKHR );
VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceMirPresentationSupportKHR );
#endif

#ifdef VK_USE_PLATFORM_XCB_KHR
// VK_KHR_xcb_surface
VK_LIB_INSTANCE_FUNCTION( CreateXcbSurfaceKHR );
VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceXcbPresentationSupportKHR );
#endif

#ifdef VK_USE_PLATFORM_XLIB_KHR
// VK_KHR_xlib_surface
VK_LIB_INSTANCE_FUNCTION( CreateXlibSurfaceKHR );
VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceXlibPresentationSupportKHR );
#endif

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
// VK_KHR_wayland_surface
VK_LIB_INSTANCE_FUNCTION( CreateWaylandSurfaceKHR );
VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceWaylandPresentationSupportKHR );
#endif

#ifdef VK_USE_PLATFORM_WIN32_KHR
// VK_KHR_win32_surface
VK_LIB_INSTANCE_FUNCTION( CreateWin32SurfaceKHR );
VK_LIB_INSTANCE_FUNCTION( GetPhysicalDeviceWin32PresentationSupportKHR );
#endif

#undef VK_LIB_INSTANCE_FUNCTION

#ifndef VK_LIB_DEVICE_FUNCTION
#	define VK_LIB_DEVICE_FUNCTION( x )
#endif

VK_LIB_DEVICE_FUNCTION( AcquireNextImageKHR )
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
VK_LIB_DEVICE_FUNCTION( CmdClearColorImage )
VK_LIB_DEVICE_FUNCTION( CmdCopyBuffer )
VK_LIB_DEVICE_FUNCTION( CmdCopyBufferToImage )
VK_LIB_DEVICE_FUNCTION( CmdCopyImage )
VK_LIB_DEVICE_FUNCTION( CmdDispatch )
VK_LIB_DEVICE_FUNCTION( CmdDraw )
VK_LIB_DEVICE_FUNCTION( CmdDrawIndexed )
VK_LIB_DEVICE_FUNCTION( CmdEndQuery )
VK_LIB_DEVICE_FUNCTION( CmdEndRenderPass )
VK_LIB_DEVICE_FUNCTION( CmdExecuteCommands )
VK_LIB_DEVICE_FUNCTION( CmdCopyImageToBuffer )
VK_LIB_DEVICE_FUNCTION( CmdNextSubpass )
VK_LIB_DEVICE_FUNCTION( CmdPipelineBarrier )
VK_LIB_DEVICE_FUNCTION( CmdPushConstants )
VK_LIB_DEVICE_FUNCTION( CmdResetQueryPool )
VK_LIB_DEVICE_FUNCTION( CmdSetScissor )
VK_LIB_DEVICE_FUNCTION( CmdSetViewport )
VK_LIB_DEVICE_FUNCTION( CmdWriteTimestamp )
VK_LIB_DEVICE_FUNCTION( CreateBuffer )
VK_LIB_DEVICE_FUNCTION( CreateBufferView )
VK_LIB_DEVICE_FUNCTION( CreateCommandPool )
VK_LIB_DEVICE_FUNCTION( CreateDescriptorPool )
VK_LIB_DEVICE_FUNCTION( CreateDescriptorSetLayout )
VK_LIB_DEVICE_FUNCTION( CreateFence )
VK_LIB_DEVICE_FUNCTION( CreateFramebuffer )
VK_LIB_DEVICE_FUNCTION( CreateGraphicsPipelines )
VK_LIB_DEVICE_FUNCTION( CreateImage )
VK_LIB_DEVICE_FUNCTION( CreateImageView )
VK_LIB_DEVICE_FUNCTION( CreateInstance )
VK_LIB_DEVICE_FUNCTION( CreatePipelineLayout )
VK_LIB_DEVICE_FUNCTION( CreateRenderPass )
VK_LIB_DEVICE_FUNCTION( CreateQueryPool )
VK_LIB_DEVICE_FUNCTION( CreateSampler )
VK_LIB_DEVICE_FUNCTION( CreateSemaphore )
VK_LIB_DEVICE_FUNCTION( CreateShaderModule )
VK_LIB_DEVICE_FUNCTION( CreateSwapchainKHR )
VK_LIB_DEVICE_FUNCTION( DestroyBuffer )
VK_LIB_DEVICE_FUNCTION( DestroyBufferView )
VK_LIB_DEVICE_FUNCTION( DestroyCommandPool )
VK_LIB_DEVICE_FUNCTION( DestroyDescriptorPool )
VK_LIB_DEVICE_FUNCTION( DestroyDescriptorSetLayout )
VK_LIB_DEVICE_FUNCTION( DestroyDevice )
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
VK_LIB_DEVICE_FUNCTION( DestroySwapchainKHR )
VK_LIB_DEVICE_FUNCTION( DeviceWaitIdle )
VK_LIB_DEVICE_FUNCTION( EndCommandBuffer )
VK_LIB_DEVICE_FUNCTION( FlushMappedMemoryRanges )
VK_LIB_DEVICE_FUNCTION( FreeCommandBuffers )
VK_LIB_DEVICE_FUNCTION( FreeDescriptorSets )
VK_LIB_DEVICE_FUNCTION( FreeMemory )
VK_LIB_DEVICE_FUNCTION( GetBufferMemoryRequirements )
VK_LIB_DEVICE_FUNCTION( GetDeviceQueue )
VK_LIB_DEVICE_FUNCTION( GetImageMemoryRequirements )
VK_LIB_DEVICE_FUNCTION( GetImageSubresourceLayout )
VK_LIB_DEVICE_FUNCTION( GetQueryPoolResults )
VK_LIB_DEVICE_FUNCTION( GetSwapchainImagesKHR )
VK_LIB_DEVICE_FUNCTION( MapMemory )
VK_LIB_DEVICE_FUNCTION( QueuePresentKHR )
VK_LIB_DEVICE_FUNCTION( QueueSubmit )
VK_LIB_DEVICE_FUNCTION( QueueWaitIdle )
VK_LIB_DEVICE_FUNCTION( ResetCommandBuffer )
VK_LIB_DEVICE_FUNCTION( ResetFences )
VK_LIB_DEVICE_FUNCTION( UnmapMemory )
VK_LIB_DEVICE_FUNCTION( UpdateDescriptorSets )
VK_LIB_DEVICE_FUNCTION( WaitForFences )

#undef VK_LIB_DEVICE_FUNCTION

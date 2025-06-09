# Install script for directory: C:/Users/Naalf/Documents/Ashes/source/ashespp

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/Naalf/Documents/Ashes/package/Ashes")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Ashes" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ashespp" TYPE FILE FILES
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/AshesPPPch.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/AshesPPPrerequisites.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/ashespp.hpp"
      )
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Ashes" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ashespp/AccelerationStructure" TYPE FILE FILES
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/AccelerationStructure/AccelerationStructure.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/AccelerationStructure/AccelerationStructureBuildGeometryInfo.hpp"
      )
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Ashes" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ashespp/Buffer" TYPE FILE FILES
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Buffer/Buffer.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Buffer/Buffer.inl"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Buffer/BufferView.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Buffer/PushConstantsBuffer.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Buffer/StagingBuffer.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Buffer/StagingBuffer.inl"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Buffer/UniformBuffer.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Buffer/UniformBuffer.inl"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Buffer/VertexBuffer.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Buffer/VertexBuffer.inl"
      )
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Ashes" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ashespp/Command" TYPE FILE FILES
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Command/CommandBuffer.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Command/CommandPool.hpp"
      )
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Ashes" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ashespp/Core" TYPE FILE FILES
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Core/ApplicationInfo.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Core/Device.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Core/DeviceCreateInfo.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Core/DeviceQueueCreateInfo.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Core/Instance.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Core/InstanceCreateInfo.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Core/PhysicalDevice.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Core/PlatformWindowHandle.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Core/RendererList.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Core/Surface.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Core/SwapChain.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Core/SwapChainCreateInfo.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Core/WindowHandle.hpp"
      )
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Ashes" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ashespp/Core/Platform" TYPE FILE FILES
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Core/Platform/AndroidWindowHandle.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Core/Platform/MacOsWindowHandle.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Core/Platform/MirWindowHandle.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Core/Platform/MswWindowHandle.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Core/Platform/WaylandWindowHandle.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Core/Platform/XWindowHandle.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Core/Platform/XcbWindowHandle.hpp"
      )
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Ashes" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ashespp/Descriptor" TYPE FILE FILES
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Descriptor/DescriptorPool.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Descriptor/DescriptorSet.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Descriptor/DescriptorSetLayout.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Descriptor/DescriptorSetPool.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Descriptor/WriteDescriptorSet.hpp"
      )
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Ashes" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ashespp/Enum" TYPE FILE FILES
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/AccelerationStructureBuildType.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/AccelerationStructureCompatibility.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/AccelerationStructureCreateFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/AccessFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/AttachmentLoadOp.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/AttachmentStoreOp.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/BlendFactor.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/BlendOp.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/BorderColor.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/BufferCreateFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/BufferUsageFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/BuildAccelerationStructureMode.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/ClipDirection.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/ColorComponentFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/ColorSpace.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/CommandBufferResetFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/CommandBufferUsageFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/CommandPoolCreateFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/CompareOp.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/ComponentSwizzle.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/CompositeAlphaFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/ConstantFormat.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/CullModeFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/DebugReportFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/DebugReportObjectType.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/DependencyFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/DescriptorPoolCreateFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/DescriptorSetLayoutCreateFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/DeviceCreateFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/DeviceQueueCreateFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/DynamicStateEnable.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/FenceCreateFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/Filter.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/Flags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/FormatFeatureFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/FramebufferCreateFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/FrontFace.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/ImageAspectFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/ImageCreateFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/ImageLayout.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/ImageTiling.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/ImageType.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/ImageUsageFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/ImageViewType.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/IndexType.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/InstanceCreateFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/LogicOp.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/MemoryAllocateFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/MemoryHeapFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/MemoryMapFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/MemoryPropertyFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/ObjectType.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/PhysicalDeviceType.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/PipelineBindPoint.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/PipelineCreateFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/PipelineDepthStencilStateCreateFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/PipelineDynamicStateCreateFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/PipelineMultisampleStateCreateFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/PipelineRasterizationStateCreateFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/PipelineStageFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/PipelineTessellationStateCreateFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/PipelineViewportStateCreateFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/PolygonMode.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/PresentMode.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/PrimitiveTopology.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/QueryControlFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/QueryPipelineStatisticFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/QueryResultFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/QueryType.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/QueueFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/RenderPassCreateFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/SampleCountFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/SamplerAddressMode.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/SamplerMipmapMode.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/ShaderGroupShader.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/ShaderStageFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/SharingMode.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/StencilOp.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/SubpassContents.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/SubpassDescriptionFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/SurfaceCounterFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/SurfaceTransformFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/SwapChainCreateFlags.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Enum/VertexInputRate.hpp"
      )
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Ashes" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ashespp/Image" TYPE FILE FILES
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Image/Image.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Image/ImageCreateInfo.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Image/ImageView.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Image/ImageViewCreateInfo.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Image/Sampler.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Image/SamplerCreateInfo.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Image/StagingTexture.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Image/StagingTexture.inl"
      )
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Ashes" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ashespp/Miscellaneous" TYPE FILE FILES
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Miscellaneous/Debug.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Miscellaneous/DeferredOperation.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Miscellaneous/DeviceMemory.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Miscellaneous/Error.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Miscellaneous/Log.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Miscellaneous/QueryPool.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Miscellaneous/QueueShare.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Miscellaneous/RendererFeatures.hpp"
      )
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Ashes" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ashespp/Pipeline" TYPE FILE FILES
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Pipeline/ComputePipeline.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Pipeline/ComputePipelineCreateInfo.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Pipeline/GraphicsPipeline.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Pipeline/GraphicsPipelineCreateInfo.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Pipeline/Pipeline.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Pipeline/PipelineColorBlendStateCreateInfo.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Pipeline/PipelineDepthStencilStateCreateInfo.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Pipeline/PipelineDynamicStateCreateInfo.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Pipeline/PipelineInputAssemblyStateCreateInfo.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Pipeline/PipelineLayout.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Pipeline/PipelineLibraryCreateInfo.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Pipeline/PipelineMultisampleStateCreateInfo.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Pipeline/PipelineRasterizationStateCreateInfo.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Pipeline/PipelineShaderStageCreateInfo.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Pipeline/PipelineTessellationStateCreateInfo.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Pipeline/PipelineVertexInputStateCreateInfo.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Pipeline/PipelineViewportStateCreateInfo.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Pipeline/RayTracingPipeline.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Pipeline/RayTracingPipelineCreateInfo.hpp"
      )
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Ashes" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ashespp/RenderPass" TYPE FILE FILES
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/RenderPass/FrameBuffer.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/RenderPass/RenderPass.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/RenderPass/RenderPassCreateInfo.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/RenderPass/SubpassDescription.hpp"
      )
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Ashes" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ashespp/Shader" TYPE FILE FILES "C:/Users/Naalf/Documents/Ashes/include/ashespp/Shader/ShaderModule.hpp")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Ashes" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ashespp/Sync" TYPE FILE FILES
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Sync/Event.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Sync/Fence.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Sync/Queue.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Sync/Semaphore.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashespp/Sync/SyncException.hpp"
      )
  endif()
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/Users/Naalf/Documents/Ashes/source/ashespp/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()

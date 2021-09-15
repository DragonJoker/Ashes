/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/TestPhysicalDevice.hpp"

#include "Core/TestInstance.hpp"
#include "Core/TestDevice.hpp"

#include "ashestest_api.hpp"

#include <cstring>

namespace ashes::test
{
	PhysicalDevice::PhysicalDevice( VkInstance instance )
		: m_instance{ instance }
	{
		doInitialise();
	}

	PhysicalDevice::~PhysicalDevice()
	{
	}

	VkBool32 PhysicalDevice::getPresentationSupport( uint32_t queueFamilyIndex )const
	{
		return VK_TRUE;
	}

	VkLayerPropertiesArray PhysicalDevice::enumerateLayerProperties()const
	{
		VkLayerPropertiesArray result;
		return result;
	}

	VkExtensionPropertiesArray PhysicalDevice::enumerateExtensionProperties( std::string const & layerName )const
	{
		static std::vector< VkExtensionProperties > const extensions
		{
#if VK_KHR_swapchain
			VkExtensionProperties{ VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_KHR_SWAPCHAIN_SPEC_VERSION },
#endif
#if VK_EXT_debug_report
			VkExtensionProperties{ VK_EXT_DEBUG_REPORT_EXTENSION_NAME, VK_EXT_DEBUG_REPORT_SPEC_VERSION },
#endif
#if VK_EXT_debug_marker
			VkExtensionProperties{ VK_EXT_DEBUG_MARKER_EXTENSION_NAME, VK_EXT_DEBUG_MARKER_SPEC_VERSION },
#endif
#if VK_EXT_debug_utils
			VkExtensionProperties{ VK_EXT_DEBUG_UTILS_EXTENSION_NAME, VK_EXT_DEBUG_UTILS_SPEC_VERSION },
#endif
#if VK_EXT_inline_uniform_block
			VkExtensionProperties{ VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME, VK_EXT_INLINE_UNIFORM_BLOCK_SPEC_VERSION },
#endif
#if VK_KHR_maintenance1
			VkExtensionProperties{ VK_KHR_MAINTENANCE1_EXTENSION_NAME, VK_KHR_MAINTENANCE1_SPEC_VERSION },
#endif
#if VK_KHR_get_physical_device_properties2
			VkExtensionProperties{ VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_SPEC_VERSION },
#endif
#if VK_KHR_portability_subset
			VkExtensionProperties{ VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME, VK_KHR_PORTABILITY_SUBSET_SPEC_VERSION },
#endif
		};
		return extensions;
	}

	VkPhysicalDeviceProperties const & PhysicalDevice::getProperties()const
	{
		return m_properties;
	}

	VkPhysicalDeviceMemoryProperties PhysicalDevice::getMemoryProperties()const
	{
		return Instance::getMemoryProperties();
	}

	VkPhysicalDeviceFeatures PhysicalDevice::getFeatures()const
	{
		return m_features;
	}

	VkQueueFamilyPropertiesArray PhysicalDevice::getQueueFamilyProperties()const
	{
		return m_queueProperties;
	}

	VkFormatProperties PhysicalDevice::getFormatProperties( VkFormat fmt )const
	{
		return m_formatProperties[fmt];
	}

	VkResult PhysicalDevice::getImageFormatProperties( VkFormat format
		, VkImageType type
		, VkImageTiling tiling
		, VkImageUsageFlags usage
		, VkImageCreateFlags flags
		, VkImageFormatProperties & imageProperties )const
	{
		return VK_ERROR_FORMAT_NOT_SUPPORTED;
	}

	VkResult PhysicalDevice::getSparseImageFormatProperties( VkFormat format
		, VkImageType type
		, VkSampleCountFlagBits samples
		, VkImageUsageFlags usage
		, VkImageTiling tiling
		, std::vector< VkSparseImageFormatProperties > & sparseImageFormatProperties )const
	{
		return VK_ERROR_FORMAT_NOT_SUPPORTED;
	}

#if VK_VERSION_1_1

	VkPhysicalDeviceFeatures2 const & PhysicalDevice::getFeatures2()const
	{
		return m_features2;
	}

	VkPhysicalDeviceProperties2 const & PhysicalDevice::getProperties2()const
	{
		return m_properties2;
	}

	VkFormatProperties2 const & PhysicalDevice::getFormatProperties2( VkFormat format )const
	{
		return m_formatProperties2[format];
	}

	VkResult PhysicalDevice::getImageFormatProperties2( VkPhysicalDeviceImageFormatInfo2 const & imageFormatInfo
		, VkImageFormatProperties2 & imageFormatProperties )const
	{
		imageFormatProperties.sType = VK_STRUCTURE_TYPE_IMAGE_FORMAT_PROPERTIES_2;
		imageFormatProperties.pNext = nullptr;
		return getImageFormatProperties( imageFormatInfo.format
			, imageFormatInfo.type
			, imageFormatInfo.tiling
			, imageFormatInfo.usage
			, imageFormatInfo.flags
			, imageFormatProperties.imageFormatProperties );
	}

	std::vector< VkQueueFamilyProperties2 > PhysicalDevice::getQueueFamilyProperties2()const
	{
		return m_queueProperties2;
	}

	VkPhysicalDeviceMemoryProperties2 const & PhysicalDevice::getMemoryProperties2()const
	{
		return Instance::getMemoryProperties2();
	}

	VkResult PhysicalDevice::getSparseImageFormatProperties2( VkPhysicalDeviceSparseImageFormatInfo2 const & formatInfo
		, std::vector< VkSparseImageFormatProperties2 > & sparseImageFormatProperties )const
	{
		std::vector< VkSparseImageFormatProperties > props;
		auto result = getSparseImageFormatProperties( formatInfo.format
			, formatInfo.type
			, formatInfo.samples
			, formatInfo.usage
			, formatInfo.tiling
			, props );

		if ( result != VK_ERROR_FORMAT_NOT_SUPPORTED )
		{
			for ( auto & prop : props )
			{
				sparseImageFormatProperties.push_back(
					{
						VK_STRUCTURE_TYPE_SPARSE_IMAGE_FORMAT_PROPERTIES_2,
						nullptr,
						prop,
					} );
			}
		}

		return result;
	}

#elif VK_KHR_get_physical_device_properties2

	VkPhysicalDeviceFeatures2KHR const & PhysicalDevice::getFeatures2()const
	{
		return m_features2;
	}

	VkPhysicalDeviceProperties2KHR const & PhysicalDevice::getProperties2()const
	{
		return m_properties2;
	}

	VkFormatProperties2KHR const & PhysicalDevice::getFormatProperties2( VkFormat format )const
	{
		return m_formatProperties2[format];
	}

	VkResult PhysicalDevice::getImageFormatProperties2( VkPhysicalDeviceImageFormatInfo2KHR const & imageFormatInfo
		, VkImageFormatProperties2KHR & imageProperties )const
	{
		imageFormatProperties.sType = VK_STRUCTURE_TYPE_IMAGE_FORMAT_PROPERTIES_2_KHR;
		imageFormatProperties.pNext = nullptr;
		return getImageFormatProperties( imageFormatInfo.format
			, imageFormatInfo.type
			, imageFormatInfo.tiling
			, imageFormatInfo.usage
			, imageFormatInfo.flags
			, imageFormatProperties.imageFormatProperties );
	}

	std::vector< VkQueueFamilyProperties2KHR > PhysicalDevice::getQueueFamilyProperties2()const
	{
		return m_queueProperties2;
	}

	VkPhysicalDeviceMemoryProperties2KHR const & PhysicalDevice::getMemoryProperties2()const
	{
		return Instance::getMemoryProperties2();
	}

	VkResult PhysicalDevice::getSparseImageFormatProperties2( VkPhysicalDeviceSparseImageFormatInfo2KHR const & formatInfo
		, std::vector< VkSparseImageFormatProperties2KHR > & sparseImageFormatProperties )const
	{
		std::vector< VkSparseImageFormatProperties > props;
		auto result = getSparseImageFormatProperties( formatInfo.format
			, formatInfo.type
			, formatInfo.samples
			, formatInfo.usage
			, formatInfo.tiling
			, props );

		if ( result != VK_ERROR_FORMAT_NOT_SUPPORTED )
		{
			for ( auto & prop : props )
			{
				sparseImageFormatProperties.push_back(
					{
						VK_STRUCTURE_TYPE_SPARSE_IMAGE_FORMAT_PROPERTIES_2,
						nullptr,
						prop,
					} );
			}
		}

		return result;
	}

#endif
#ifdef VK_KHR_display

	std::vector< VkDisplayPropertiesKHR > const & PhysicalDevice::getDisplayProperties()const
	{
		return m_displays;
	}

	std::vector< VkDisplayPlanePropertiesKHR > PhysicalDevice::getDisplayPlaneProperties()const
	{
		return m_displayPlanes;
	}

	std::vector< VkDisplayKHR > PhysicalDevice::getDisplayPlaneSupportedDisplays( uint32_t planeIndex )const
	{
		std::vector< VkDisplayKHR > result
		{
			m_displayPlanes[planeIndex].currentDisplay
		};
		return result;
	}

#endif

	void PhysicalDevice::doInitialise()
	{
		std::string_view name{ "Test" };
		strncpy( m_properties.deviceName
			, name.data()
			, std::min( name.size() + 1u, size_t( VK_MAX_PHYSICAL_DEVICE_NAME_SIZE - 1u ) ) );
		m_properties.deviceID = 0u;
		m_properties.vendorID = 0u;
		m_properties.driverVersion = ashes::makeVersion( 1, 0, 0 );
		m_properties.apiVersion = ashes::makeVersion( 1, 0, 0 );
		m_properties.deviceType = VK_PHYSICAL_DEVICE_TYPE_CPU;

#	if VK_KHR_portability_subset
		m_portabilityFeatures = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PORTABILITY_SUBSET_FEATURES_KHR
			, nullptr
			, VK_TRUE /* constantAlphaColorBlendFactors; */
			, VK_TRUE /* events; */
			, VK_TRUE /* imageViewFormatReinterpretation; */
			, VK_TRUE /* imageViewFormatSwizzle; */
			, VK_TRUE /* imageView2DOn3DImage; */
			, VK_TRUE /* multisampleArrayImage; */
			, VK_TRUE /* mutableComparisonSamplers; */
			, VK_TRUE /* pointPolygons; */
			, VK_TRUE /* samplerMipLodBias; */
			, VK_TRUE /* separateStencilMaskRef; */
			, m_features.sampleRateShading /* shaderSampleRateInterpolationFunctions; */
			, m_features.tessellationShader /* tessellationIsolines; */
			, m_features.tessellationShader /* tessellationPointMode; */
			, VK_TRUE /* triangleFans; */
			, VK_TRUE /* vertexAttributeAccessBeyondStride; */ };
#	endif

		m_features.robustBufferAccess = true;
		m_features.fullDrawIndexUint32 = true;
		m_features.imageCubeArray = true;
		m_features.independentBlend = true;
		m_features.geometryShader = true;
		m_features.tessellationShader = true;
		m_features.sampleRateShading = true;
		m_features.dualSrcBlend = true;
		m_features.logicOp = true;
		m_features.multiDrawIndirect = true;
		m_features.drawIndirectFirstInstance = true;
		m_features.depthClamp = true;
		m_features.depthBiasClamp = true;
		m_features.fillModeNonSolid = true;
		m_features.depthBounds = true;
		m_features.wideLines = true;
		m_features.largePoints = true;
		m_features.alphaToOne = true;
		m_features.multiViewport = true;
		m_features.samplerAnisotropy = true;
		m_features.textureCompressionETC2 = true;
		m_features.textureCompressionASTC_LDR = true;
		m_features.textureCompressionBC = true;
		m_features.occlusionQueryPrecise = true;
		m_features.pipelineStatisticsQuery = true;
		m_features.vertexPipelineStoresAndAtomics = true;
		m_features.fragmentStoresAndAtomics = true;
		m_features.shaderTessellationAndGeometryPointSize = true;
		m_features.shaderImageGatherExtended = true;
		m_features.shaderStorageImageExtendedFormats = true;
		m_features.shaderStorageImageMultisample = true;
		m_features.shaderStorageImageReadWithoutFormat = true;
		m_features.shaderStorageImageWriteWithoutFormat = true;
		m_features.shaderUniformBufferArrayDynamicIndexing = true;
		m_features.shaderSampledImageArrayDynamicIndexing = true;
		m_features.shaderStorageBufferArrayDynamicIndexing = true;
		m_features.shaderStorageImageArrayDynamicIndexing = true;
		m_features.shaderClipDistance = true;
		m_features.shaderCullDistance = true;
		m_features.shaderFloat64 = true;
		m_features.shaderInt64 = true;
		m_features.shaderInt16 = true;
		m_features.shaderResourceResidency = true;
		m_features.shaderResourceMinLod = true;
		m_features.sparseBinding = true;
		m_features.sparseResidencyBuffer = true;
		m_features.sparseResidencyImage2D = true;
		m_features.sparseResidencyImage3D = true;
		m_features.sparseResidency2Samples = true;
		m_features.sparseResidency4Samples = true;
		m_features.sparseResidency8Samples = true;
		m_features.sparseResidency16Samples = true;
		m_features.sparseResidencyAliased = true;
		m_features.variableMultisampleRate = true;
		m_features.inheritedQueries = true;

		m_properties.limits.maxImageDimension1D = 16384u;
		m_properties.limits.maxImageDimension2D = 16384u;
		m_properties.limits.maxImageDimension3D = 2048u;
		m_properties.limits.maxImageDimensionCube = 16384u;
		m_properties.limits.maxImageArrayLayers = 2048u;
		m_properties.limits.maxTexelBufferElements = 134217728u;
		m_properties.limits.maxUniformBufferRange = 65536u;
		m_properties.limits.maxStorageBufferRange = 4294967295u;
		m_properties.limits.maxPushConstantsSize = 256u;
		m_properties.limits.maxMemoryAllocationCount = 4096u;
		m_properties.limits.maxSamplerAllocationCount = 4000u;
		m_properties.limits.bufferImageGranularity = 1024u;
		m_properties.limits.sparseAddressSpaceSize = 18446744073709551615u;
		m_properties.limits.maxBoundDescriptorSets = 8u;
		m_properties.limits.maxPerStageDescriptorSamplers = 1048576u;
		m_properties.limits.maxPerStageDescriptorUniformBuffers = 15u;
		m_properties.limits.maxPerStageDescriptorStorageBuffers = 1048576u;
		m_properties.limits.maxPerStageDescriptorSampledImages = 1048576u;
		m_properties.limits.maxPerStageDescriptorStorageImages = 1048576u;
		m_properties.limits.maxPerStageDescriptorInputAttachments = 1048576u;
		m_properties.limits.maxPerStageResources = 4294967295u;
		m_properties.limits.maxDescriptorSetSamplers = 1048576u;
		m_properties.limits.maxDescriptorSetUniformBuffers = 90u;
		m_properties.limits.maxDescriptorSetUniformBuffersDynamic = 15u;
		m_properties.limits.maxDescriptorSetStorageBuffers = 1048576u;
		m_properties.limits.maxDescriptorSetStorageBuffersDynamic = 16u;
		m_properties.limits.maxDescriptorSetSampledImages = 1048576u;
		m_properties.limits.maxDescriptorSetStorageImages = 1048576u;
		m_properties.limits.maxDescriptorSetInputAttachments = 1048576u;
		m_properties.limits.maxVertexInputAttributes = 32u;
		m_properties.limits.maxVertexInputBindings = 32u;
		m_properties.limits.maxVertexInputAttributeOffset = 2047u;
		m_properties.limits.maxVertexInputBindingStride = 2048u;
		m_properties.limits.maxVertexOutputComponents = 128u;
		m_properties.limits.maxTessellationGenerationLevel = 64u;
		m_properties.limits.maxTessellationPatchSize = 32u;
		m_properties.limits.maxTessellationControlPerVertexInputComponents = 128u;
		m_properties.limits.maxTessellationControlPerVertexOutputComponents = 128u;
		m_properties.limits.maxTessellationControlPerPatchOutputComponents = 120u;
		m_properties.limits.maxTessellationControlTotalOutputComponents = 4216u;
		m_properties.limits.maxTessellationEvaluationInputComponents = 128u;
		m_properties.limits.maxTessellationEvaluationOutputComponents = 128u;
		m_properties.limits.maxGeometryShaderInvocations = 32u;
		m_properties.limits.maxGeometryInputComponents = 128u;
		m_properties.limits.maxGeometryOutputComponents = 128u;
		m_properties.limits.maxGeometryOutputVertices = 1024u;
		m_properties.limits.maxGeometryTotalOutputComponents = 1024u;
		m_properties.limits.maxFragmentInputComponents = 128u;
		m_properties.limits.maxFragmentOutputAttachments = 8u;
		m_properties.limits.maxFragmentDualSrcAttachments = 1u;
		m_properties.limits.maxFragmentCombinedOutputResources = 16u;
		m_properties.limits.maxComputeSharedMemorySize = 49152u;
		m_properties.limits.maxComputeWorkGroupCount[0] = 2147483647u;
		m_properties.limits.maxComputeWorkGroupCount[1] = 65535u;
		m_properties.limits.maxComputeWorkGroupCount[2] = 65535u;
		m_properties.limits.maxComputeWorkGroupInvocations = 1536u;
		m_properties.limits.maxComputeWorkGroupSize[0] = 1536u;
		m_properties.limits.maxComputeWorkGroupSize[1] = 1024u;
		m_properties.limits.maxComputeWorkGroupSize[2] = 64u;
		m_properties.limits.subPixelPrecisionBits = 8u;
		m_properties.limits.subTexelPrecisionBits = 8u;
		m_properties.limits.mipmapPrecisionBits = 8u;
		m_properties.limits.maxDrawIndexedIndexValue = 4294967295u;
		m_properties.limits.maxDrawIndirectCount = 4294967295u;
		m_properties.limits.maxSamplerLodBias = 15.0f;
		m_properties.limits.maxSamplerAnisotropy = 16.0f;
		m_properties.limits.maxViewports = 16u;
		m_properties.limits.maxViewportDimensions[0] = 16384u;
		m_properties.limits.maxViewportDimensions[1] = 16384u;
		m_properties.limits.viewportBoundsRange[0] = -32768.0f;
		m_properties.limits.viewportBoundsRange[1] = 32768.0f;
		m_properties.limits.viewportSubPixelBits = 8u;
		m_properties.limits.minMemoryMapAlignment = 64u;
		m_properties.limits.minTexelBufferOffsetAlignment = 16u;
		m_properties.limits.minUniformBufferOffsetAlignment = 256u;
		m_properties.limits.minStorageBufferOffsetAlignment = 32u;
		m_properties.limits.minTexelOffset = -8;
		m_properties.limits.maxTexelOffset = 7u;
		m_properties.limits.minTexelGatherOffset = -32;
		m_properties.limits.maxTexelGatherOffset = 31u;
		m_properties.limits.minInterpolationOffset = -0.5f;
		m_properties.limits.maxInterpolationOffset = 0.4375f;
		m_properties.limits.subPixelInterpolationOffsetBits = 4u;
		m_properties.limits.maxFramebufferWidth = 16384u;
		m_properties.limits.maxFramebufferHeight = 16384u;
		m_properties.limits.maxFramebufferLayers = 2048u;
		m_properties.limits.framebufferColorSampleCounts = 15u;
		m_properties.limits.framebufferDepthSampleCounts = 15u;
		m_properties.limits.framebufferStencilSampleCounts = 31u;
		m_properties.limits.framebufferNoAttachmentsSampleCounts = 31u;
		m_properties.limits.maxColorAttachments = 8u;
		m_properties.limits.sampledImageColorSampleCounts = 15u;
		m_properties.limits.sampledImageIntegerSampleCounts = 15u;
		m_properties.limits.sampledImageDepthSampleCounts = 15u;
		m_properties.limits.sampledImageStencilSampleCounts = 31u;
		m_properties.limits.storageImageSampleCounts = 15u;
		m_properties.limits.maxSampleMaskWords = 1u;
		m_properties.limits.timestampComputeAndGraphics = true;
		m_properties.limits.timestampPeriod = 1.0f;
		m_properties.limits.maxClipDistances = 8u;
		m_properties.limits.maxCullDistances = 8u;
		m_properties.limits.maxCombinedClipAndCullDistances = 8u;
		m_properties.limits.discreteQueuePriorities = 2u;
		m_properties.limits.pointSizeRange[0] = 1.0f;
		m_properties.limits.pointSizeRange[1] = 189.875f;
		m_properties.limits.lineWidthRange[0] = 0.5f;
		m_properties.limits.lineWidthRange[1] = 10.0f;
		m_properties.limits.pointSizeGranularity = 0.125f;
		m_properties.limits.lineWidthGranularity = 0.125f;
		m_properties.limits.strictLines = true;
		m_properties.limits.standardSampleLocations = true;
		m_properties.limits.optimalBufferCopyOffsetAlignment = 1u;
		m_properties.limits.optimalBufferCopyRowPitchAlignment = 1u;
		m_properties.limits.nonCoherentAtomSize = 64ull;

		m_properties.sparseProperties.residencyAlignedMipSize = true;
		m_properties.sparseProperties.residencyNonResidentStrict = true;
		m_properties.sparseProperties.residencyStandard2DBlockShape = true;
		m_properties.sparseProperties.residencyStandard2DMultisampleBlockShape = true;
		m_properties.sparseProperties.residencyStandard3DBlockShape = true;

		// Et enfin les propriétés des familles de files du GPU.
		m_queueProperties.reserve( 1u );
		m_queueProperties.push_back(
			{
				0xFFFFFFFF,
				1u,
				1u,
				{
					1u,
					1u,
					1u,
				}
			} );

		for ( VkFormat fmt = beginFmt(); fmt != endFmt(); fmt = VkFormat( fmt + 1 ) )
		{
			VkFormatProperties props{};
			props.bufferFeatures |= VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT;
			props.bufferFeatures |= VK_FORMAT_FEATURE_BLIT_SRC_BIT;
			props.bufferFeatures |= VK_FORMAT_FEATURE_BLIT_DST_BIT;
			props.optimalTilingFeatures |= VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;
			props.optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_SRC_BIT;
			props.optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_DST_BIT;
			props.optimalTilingFeatures |= VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT;
			props.optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_SRC_BIT;
			props.optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_DST_BIT;
			props.optimalTilingFeatures |= VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT;
			props.optimalTilingFeatures |= VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT;
			props.optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_SRC_BIT;
			props.optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_DST_BIT;
			props.optimalTilingFeatures |= VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT;
			props.optimalTilingFeatures |= VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT;
			props.optimalTilingFeatures |= VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_MINMAX_BIT_EXT;
			props.optimalTilingFeatures |= VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_CUBIC_BIT_IMG;
			props.optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_SRC_BIT;
			props.optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_DST_BIT;
			props.optimalTilingFeatures |= VK_FORMAT_FEATURE_TRANSFER_SRC_BIT;
			props.optimalTilingFeatures |= VK_FORMAT_FEATURE_TRANSFER_DST_BIT;
			props.optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_SRC_BIT;
			props.optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_DST_BIT;
			props.linearTilingFeatures = props.optimalTilingFeatures;
			m_formatProperties[fmt] = props;
		}

#if VK_VERSION_1_1

#	if VK_KHR_portability_subset
		m_features2.pNext = &m_portabilityFeatures;
#	else
		m_features2.pNext = nullptr;
#	endif
		m_features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
		m_features2.features = m_features;

		m_properties2.pNext = nullptr;
		m_properties2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
		m_properties2.properties = m_properties;

		for ( auto & queueProperty : m_queueProperties )
		{
			m_queueProperties2.push_back(
				{
					VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2,
					nullptr,
					queueProperty,
				} );
		}

		for ( auto & formatProperty : m_formatProperties )
		{
			m_formatProperties2.emplace( formatProperty.first
				, VkFormatProperties2
				{
					VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2,
					nullptr,
					formatProperty.second,
				} );
		}

#elif VK_KHR_get_physical_device_properties2

#	if VK_KHR_portability_subset
		m_features2.pNext = &m_portabilityFeatures;
#	else
		m_features2.pNext = nullptr;
#	endif
		m_features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR;
		m_features2.features = m_features;

		m_properties2.pNext = nullptr;
		m_properties2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2_KHR;
		m_properties2.properties = m_properties;

		for ( auto & queueProperty : m_queueProperties )
		{
			m_queueProperties2.push_back(
				{
					VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2_KHR,
					nullptr,
					queueProperty,
				} );
		}

		for ( auto & formatProperty : m_formatProperties )
		{
			m_formatProperties2.emplace( formatProperty.first
				, VkFormatProperties2
				{
					VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR,
					nullptr,
					formatProperty.second,
				} );
		}

#endif
	}
}

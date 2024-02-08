/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Core/PhysicalDevice.hpp"

#include "ashespp/Core/Instance.hpp"

#include <algorithm>

namespace ashes
{
	PhysicalDevice::PhysicalDevice( Instance const & instance
		, VkPhysicalDevice gpu )
		: m_instance{ instance }
		, m_gpu{ gpu }
	{
	}

	VkLayerPropertiesArray PhysicalDevice::enumerateLayerProperties()const
	{
		VkLayerPropertiesArray result;
		VkResult res;

		do
		{
			uint32_t layersCount = 0u;
			res = m_instance.vkEnumerateDeviceLayerProperties( m_gpu
				, &layersCount
				, nullptr );

			if ( layersCount )
			{
				result.resize( layersCount );
				res = m_instance.vkEnumerateDeviceLayerProperties( m_gpu
					, &layersCount
					, result.data() );
			}
		}
		while ( res == VK_INCOMPLETE );

		checkError( res, "Device layers retrieval" );
		return result;
	}

	VkExtensionPropertiesArray PhysicalDevice::enumerateExtensionProperties( std::string const & layerName )const
	{
		VkExtensionPropertiesArray result;
		VkResult res;

		do
		{
			uint32_t extensionsCount;
			res = m_instance.vkEnumerateDeviceExtensionProperties( m_gpu
				, layerName.empty() ? nullptr : layerName.c_str()
				, &extensionsCount
				, nullptr );

			if ( extensionsCount )
			{
				result.resize( extensionsCount );
				res = m_instance.vkEnumerateDeviceExtensionProperties( m_gpu
					, layerName.empty() ? nullptr : layerName.c_str()
					, &extensionsCount
					, result.data() );
			}
		}
		while ( res == VK_INCOMPLETE );

		checkError( res, "Device layer extensions retrieval" );
		return result;
	}

	VkPhysicalDeviceProperties PhysicalDevice::getProperties()const
	{
		VkPhysicalDeviceProperties properties;
		m_instance.vkGetPhysicalDeviceProperties( m_gpu, &properties );
		return properties;
	}

	void PhysicalDevice::getProperties( VkPhysicalDeviceProperties2 & properties )const
	{
		m_instance.vkGetPhysicalDeviceProperties2( m_gpu, &properties );
	}

	VkPhysicalDeviceMemoryProperties PhysicalDevice::getMemoryProperties()const
	{
		VkPhysicalDeviceMemoryProperties memoryProperties;
		m_instance.vkGetPhysicalDeviceMemoryProperties( m_gpu, &memoryProperties );
		return memoryProperties;
	}

	void PhysicalDevice::getMemoryProperties( VkPhysicalDeviceMemoryProperties2 & properties )const
	{
		m_instance.vkGetPhysicalDeviceMemoryProperties2( m_gpu, &properties );
	}

	VkPhysicalDeviceFeatures PhysicalDevice::getFeatures()const
	{
		VkPhysicalDeviceFeatures features;
		m_instance.vkGetPhysicalDeviceFeatures( m_gpu, &features );
		return features;
	}

	void PhysicalDevice::getFeatures( VkPhysicalDeviceFeatures2 & features )const
	{
		m_instance.vkGetPhysicalDeviceFeatures2( m_gpu, &features );
	}

	VkQueueFamilyPropertiesArray PhysicalDevice::getQueueFamilyProperties()const
	{
		uint32_t queueCount{ 0 };
		m_instance.vkGetPhysicalDeviceQueueFamilyProperties( m_gpu, &queueCount, nullptr );
		assert( queueCount >= 1 );

		std::vector< VkQueueFamilyProperties > queueProperties( queueCount );
		m_instance.vkGetPhysicalDeviceQueueFamilyProperties( m_gpu, &queueCount, queueProperties.data() );
		assert( queueCount >= 1 );

		return queueProperties;
	}

	VkFormatProperties PhysicalDevice::getFormatProperties( VkFormat fmt )const
	{
		VkFormatProperties props;
		m_instance.vkGetPhysicalDeviceFormatProperties( m_gpu, fmt, &props );
		return props;
	}

#if VK_VERSION_1_3 || VK_KHR_format_feature_flags2
	VkFormatProperties3KHR PhysicalDevice::getFormatProperties3( VkFormat fmt )const
	{
		VkFormatProperties3KHR result{ VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_3_KHR };
		VkFormatProperties2KHR props{ VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR, &result, {} };
		m_instance.vkGetPhysicalDeviceFormatProperties2KHR( m_gpu
			, fmt
			, &props );
		return result;
	}
#endif

	VkResult PhysicalDevice::getImageFormatProperties( VkFormat format
		, VkImageType type
		, VkImageTiling tiling
		, VkImageUsageFlags usage
		, VkImageCreateFlags flags
		, VkImageFormatProperties & imageProperties )const
	{
		return m_instance.vkGetPhysicalDeviceImageFormatProperties( m_gpu
			, format
			, type
			, tiling
			, usage
			, flags
			, &imageProperties );
	}

	std::string PhysicalDevice::dumpProperties()const
	{
		auto features = getFeatures();
		auto properties = getProperties();
		std::stringstream result;
		result.imbue( std::locale{ "C" } );
		result << "features.robustBufferAccess = " << features.robustBufferAccess << ";" << std::endl;
		result << "features.fullDrawIndexUint32 = " << features.fullDrawIndexUint32 << ";" << std::endl;
		result << "features.imageCubeArray = " << features.imageCubeArray << ";" << std::endl;
		result << "features.independentBlend = " << features.independentBlend << ";" << std::endl;
		result << "features.geometryShader = " << features.geometryShader << ";" << std::endl;
		result << "features.tessellationShader = " << features.tessellationShader << ";" << std::endl;
		result << "features.sampleRateShading = " << features.sampleRateShading << ";" << std::endl;
		result << "features.dualSrcBlend = " << features.dualSrcBlend << ";" << std::endl;
		result << "features.logicOp = " << features.logicOp << ";" << std::endl;
		result << "features.multiDrawIndirect = " << features.multiDrawIndirect << ";" << std::endl;
		result << "features.drawIndirectFirstInstance = " << features.drawIndirectFirstInstance << ";" << std::endl;
		result << "features.depthClamp = " << features.depthClamp << ";" << std::endl;
		result << "features.depthBiasClamp = " << features.depthBiasClamp << ";" << std::endl;
		result << "features.fillModeNonSolid = " << features.fillModeNonSolid << ";" << std::endl;
		result << "features.depthBounds = " << features.depthBounds << ";" << std::endl;
		result << "features.wideLines = " << features.wideLines << ";" << std::endl;
		result << "features.largePoints = " << features.largePoints << ";" << std::endl;
		result << "features.alphaToOne = " << features.alphaToOne << ";" << std::endl;
		result << "features.multiViewport = " << features.multiViewport << ";" << std::endl;
		result << "features.samplerAnisotropy = " << features.samplerAnisotropy << ";" << std::endl;
		result << "features.textureCompressionETC2 = " << features.textureCompressionETC2 << ";" << std::endl;
		result << "features.textureCompressionASTC_LDR = " << features.textureCompressionASTC_LDR << ";" << std::endl;
		result << "features.textureCompressionBC = " << features.textureCompressionBC << ";" << std::endl;
		result << "features.occlusionQueryPrecise = " << features.occlusionQueryPrecise << ";" << std::endl;
		result << "features.pipelineStatisticsQuery = " << features.pipelineStatisticsQuery << ";" << std::endl;
		result << "features.vertexPipelineStoresAndAtomics = " << features.vertexPipelineStoresAndAtomics << ";" << std::endl;
		result << "features.fragmentStoresAndAtomics = " << features.fragmentStoresAndAtomics << ";" << std::endl;
		result << "features.shaderTessellationAndGeometryPointSize = " << features.shaderTessellationAndGeometryPointSize << ";" << std::endl;
		result << "features.shaderImageGatherExtended = " << features.shaderImageGatherExtended << ";" << std::endl;
		result << "features.shaderStorageImageExtendedFormats = " << features.shaderStorageImageExtendedFormats << ";" << std::endl;
		result << "features.shaderStorageImageMultisample = " << features.shaderStorageImageMultisample << ";" << std::endl;
		result << "features.shaderStorageImageReadWithoutFormat = " << features.shaderStorageImageReadWithoutFormat << ";" << std::endl;
		result << "features.shaderStorageImageWriteWithoutFormat = " << features.shaderStorageImageWriteWithoutFormat << ";" << std::endl;
		result << "features.shaderUniformBufferArrayDynamicIndexing = " << features.shaderUniformBufferArrayDynamicIndexing << ";" << std::endl;
		result << "features.shaderSampledImageArrayDynamicIndexing = " << features.shaderSampledImageArrayDynamicIndexing << ";" << std::endl;
		result << "features.shaderStorageBufferArrayDynamicIndexing = " << features.shaderStorageBufferArrayDynamicIndexing << ";" << std::endl;
		result << "features.shaderStorageImageArrayDynamicIndexing = " << features.shaderStorageImageArrayDynamicIndexing << ";" << std::endl;
		result << "features.shaderClipDistance = " << features.shaderClipDistance << ";" << std::endl;
		result << "features.shaderCullDistance = " << features.shaderCullDistance << ";" << std::endl;
		result << "features.shaderFloat64 = " << features.shaderFloat64 << ";" << std::endl;
		result << "features.shaderInt64 = " << features.shaderInt64 << ";" << std::endl;
		result << "features.shaderInt16 = " << features.shaderInt16 << ";" << std::endl;
		result << "features.shaderResourceResidency = " << features.shaderResourceResidency << ";" << std::endl;
		result << "features.shaderResourceMinLod = " << features.shaderResourceMinLod << ";" << std::endl;
		result << "features.sparseBinding = " << features.sparseBinding << ";" << std::endl;
		result << "features.sparseResidencyBuffer = " << features.sparseResidencyBuffer << ";" << std::endl;
		result << "features.sparseResidencyImage2D = " << features.sparseResidencyImage2D << ";" << std::endl;
		result << "features.sparseResidencyImage3D = " << features.sparseResidencyImage3D << ";" << std::endl;
		result << "features.sparseResidency2Samples = " << features.sparseResidency2Samples << ";" << std::endl;
		result << "features.sparseResidency4Samples = " << features.sparseResidency4Samples << ";" << std::endl;
		result << "features.sparseResidency8Samples = " << features.sparseResidency8Samples << ";" << std::endl;
		result << "features.sparseResidency16Samples = " << features.sparseResidency16Samples << ";" << std::endl;
		result << "features.sparseResidencyAliased = " << features.sparseResidencyAliased << ";" << std::endl;
		result << "features.variableMultisampleRate = " << features.variableMultisampleRate << ";" << std::endl;
		result << "features.inheritedQueries = " << features.inheritedQueries << ";" << std::endl;
		result << std::endl;
		result << "properties.apiVersion = " << properties.apiVersion << ";" << std::endl;
		result << "properties.deviceID = " << properties.deviceID << ";" << std::endl;
		result << "properties.deviceName = " << properties.deviceName << ";" << std::endl;
		result << "properties.vendorID = " << properties.vendorID << ";" << std::endl;
		result << "properties.deviceType = " << getName( properties.deviceType ) << ";" << std::endl;
		result << "properties.driverVersion = " << properties.driverVersion << ";" << std::endl;
		result << std::endl;
		result << "properties.limits.maxImageDimension1D = " << properties.limits.maxImageDimension1D << ";" << std::endl;
		result << "properties.limits.maxImageDimension2D = " << properties.limits.maxImageDimension2D << ";" << std::endl;
		result << "properties.limits.maxImageDimension3D = " << properties.limits.maxImageDimension3D << ";" << std::endl;
		result << "properties.limits.maxImageDimensionCube = " << properties.limits.maxImageDimensionCube << ";" << std::endl;
		result << "properties.limits.maxImageArrayLayers = " << properties.limits.maxImageArrayLayers << ";" << std::endl;
		result << "properties.limits.maxTexelBufferElements = " << properties.limits.maxTexelBufferElements << ";" << std::endl;
		result << "properties.limits.maxUniformBufferRange = " << properties.limits.maxUniformBufferRange << ";" << std::endl;
		result << "properties.limits.maxStorageBufferRange = " << properties.limits.maxStorageBufferRange << ";" << std::endl;
		result << "properties.limits.maxPushConstantsSize = " << properties.limits.maxPushConstantsSize << ";" << std::endl;
		result << "properties.limits.maxMemoryAllocationCount = " << properties.limits.maxMemoryAllocationCount << ";" << std::endl;
		result << "properties.limits.maxSamplerAllocationCount = " << properties.limits.maxSamplerAllocationCount << ";" << std::endl;
		result << "properties.limits.bufferImageGranularity = " << properties.limits.bufferImageGranularity << ";" << std::endl;
		result << "properties.limits.sparseAddressSpaceSize = " << properties.limits.sparseAddressSpaceSize << ";" << std::endl;
		result << "properties.limits.maxBoundDescriptorSets = " << properties.limits.maxBoundDescriptorSets << ";" << std::endl;
		result << "properties.limits.maxPerStageDescriptorSamplers = " << properties.limits.maxPerStageDescriptorSamplers << ";" << std::endl;
		result << "properties.limits.maxPerStageDescriptorUniformBuffers = " << properties.limits.maxPerStageDescriptorUniformBuffers << ";" << std::endl;
		result << "properties.limits.maxPerStageDescriptorStorageBuffers = " << properties.limits.maxPerStageDescriptorStorageBuffers << ";" << std::endl;
		result << "properties.limits.maxPerStageDescriptorSampledImages = " << properties.limits.maxPerStageDescriptorSampledImages << ";" << std::endl;
		result << "properties.limits.maxPerStageDescriptorStorageImages = " << properties.limits.maxPerStageDescriptorStorageImages << ";" << std::endl;
		result << "properties.limits.maxPerStageDescriptorInputAttachments = " << properties.limits.maxPerStageDescriptorInputAttachments << ";" << std::endl;
		result << "properties.limits.maxPerStageResources = " << properties.limits.maxPerStageResources << ";" << std::endl;
		result << "properties.limits.maxDescriptorSetSamplers = " << properties.limits.maxDescriptorSetSamplers << ";" << std::endl;
		result << "properties.limits.maxDescriptorSetUniformBuffers = " << properties.limits.maxDescriptorSetUniformBuffers << ";" << std::endl;
		result << "properties.limits.maxDescriptorSetUniformBuffersDynamic = " << properties.limits.maxDescriptorSetUniformBuffersDynamic << ";" << std::endl;
		result << "properties.limits.maxDescriptorSetStorageBuffers = " << properties.limits.maxDescriptorSetStorageBuffers << ";" << std::endl;
		result << "properties.limits.maxDescriptorSetStorageBuffersDynamic = " << properties.limits.maxDescriptorSetStorageBuffersDynamic << ";" << std::endl;
		result << "properties.limits.maxDescriptorSetSampledImages = " << properties.limits.maxDescriptorSetSampledImages << ";" << std::endl;
		result << "properties.limits.maxDescriptorSetStorageImages = " << properties.limits.maxDescriptorSetStorageImages << ";" << std::endl;
		result << "properties.limits.maxDescriptorSetInputAttachments = " << properties.limits.maxDescriptorSetInputAttachments << ";" << std::endl;
		result << "properties.limits.maxVertexInputAttributes = " << properties.limits.maxVertexInputAttributes << ";" << std::endl;
		result << "properties.limits.maxVertexInputBindings = " << properties.limits.maxVertexInputBindings << ";" << std::endl;
		result << "properties.limits.maxVertexInputAttributeOffset = " << properties.limits.maxVertexInputAttributeOffset << ";" << std::endl;
		result << "properties.limits.maxVertexInputBindingStride = " << properties.limits.maxVertexInputBindingStride << ";" << std::endl;
		result << "properties.limits.maxVertexOutputComponents = " << properties.limits.maxVertexOutputComponents << ";" << std::endl;
		result << "properties.limits.maxTessellationGenerationLevel = " << properties.limits.maxTessellationGenerationLevel << ";" << std::endl;
		result << "properties.limits.maxTessellationPatchSize = " << properties.limits.maxTessellationPatchSize << ";" << std::endl;
		result << "properties.limits.maxTessellationControlPerVertexInputComponents = " << properties.limits.maxTessellationControlPerVertexInputComponents << ";" << std::endl;
		result << "properties.limits.maxTessellationControlPerVertexOutputComponents = " << properties.limits.maxTessellationControlPerVertexOutputComponents << ";" << std::endl;
		result << "properties.limits.maxTessellationControlPerPatchOutputComponents = " << properties.limits.maxTessellationControlPerPatchOutputComponents << ";" << std::endl;
		result << "properties.limits.maxTessellationControlTotalOutputComponents = " << properties.limits.maxTessellationControlTotalOutputComponents << ";" << std::endl;
		result << "properties.limits.maxTessellationEvaluationInputComponents = " << properties.limits.maxTessellationEvaluationInputComponents << ";" << std::endl;
		result << "properties.limits.maxTessellationEvaluationOutputComponents = " << properties.limits.maxTessellationEvaluationOutputComponents << ";" << std::endl;
		result << "properties.limits.maxGeometryShaderInvocations = " << properties.limits.maxGeometryShaderInvocations << ";" << std::endl;
		result << "properties.limits.maxGeometryInputComponents = " << properties.limits.maxGeometryInputComponents << ";" << std::endl;
		result << "properties.limits.maxGeometryOutputComponents = " << properties.limits.maxGeometryOutputComponents << ";" << std::endl;
		result << "properties.limits.maxGeometryOutputVertices = " << properties.limits.maxGeometryOutputVertices << ";" << std::endl;
		result << "properties.limits.maxGeometryTotalOutputComponents = " << properties.limits.maxGeometryTotalOutputComponents << ";" << std::endl;
		result << "properties.limits.maxFragmentInputComponents = " << properties.limits.maxFragmentInputComponents << ";" << std::endl;
		result << "properties.limits.maxFragmentOutputAttachments = " << properties.limits.maxFragmentOutputAttachments << ";" << std::endl;
		result << "properties.limits.maxFragmentDualSrcAttachments = " << properties.limits.maxFragmentDualSrcAttachments << ";" << std::endl;
		result << "properties.limits.maxFragmentCombinedOutputResources = " << properties.limits.maxFragmentCombinedOutputResources << ";" << std::endl;
		result << "properties.limits.maxComputeSharedMemorySize = " << properties.limits.maxComputeSharedMemorySize << ";" << std::endl;
		result << "properties.limits.maxComputeWorkGroupCount[0] = " << properties.limits.maxComputeWorkGroupCount[0] << ";" << std::endl;
		result << "properties.limits.maxComputeWorkGroupCount[1] = " << properties.limits.maxComputeWorkGroupCount[1] << ";" << std::endl;
		result << "properties.limits.maxComputeWorkGroupCount[2] = " << properties.limits.maxComputeWorkGroupCount[2] << ";" << std::endl;
		result << "properties.limits.maxComputeWorkGroupInvocations = " << properties.limits.maxComputeWorkGroupInvocations << ";" << std::endl;
		result << "properties.limits.maxComputeWorkGroupSize[0] = " << properties.limits.maxComputeWorkGroupSize[0] << ";" << std::endl;
		result << "properties.limits.maxComputeWorkGroupSize[1] = " << properties.limits.maxComputeWorkGroupSize[1] << ";" << std::endl;
		result << "properties.limits.maxComputeWorkGroupSize[2] = " << properties.limits.maxComputeWorkGroupSize[2] << ";" << std::endl;
		result << "properties.limits.subPixelPrecisionBits = " << properties.limits.subPixelPrecisionBits << ";" << std::endl;
		result << "properties.limits.subTexelPrecisionBits = " << properties.limits.subTexelPrecisionBits << ";" << std::endl;
		result << "properties.limits.mipmapPrecisionBits = " << properties.limits.mipmapPrecisionBits << ";" << std::endl;
		result << "properties.limits.maxDrawIndexedIndexValue = " << properties.limits.maxDrawIndexedIndexValue << ";" << std::endl;
		result << "properties.limits.maxDrawIndirectCount = " << properties.limits.maxDrawIndirectCount << ";" << std::endl;
		result << "properties.limits.maxSamplerLodBias = " << properties.limits.maxSamplerLodBias << ";" << std::endl;
		result << "properties.limits.maxSamplerAnisotropy = " << properties.limits.maxSamplerAnisotropy << ";" << std::endl;
		result << "properties.limits.maxViewports = " << properties.limits.maxViewports << ";" << std::endl;
		result << "properties.limits.maxViewportDimensions[0] = " << properties.limits.maxViewportDimensions[0] << ";" << std::endl;
		result << "properties.limits.maxViewportDimensions[1] = " << properties.limits.maxViewportDimensions[1] << ";" << std::endl;
		result << "properties.limits.viewportBoundsRange[0] = " << properties.limits.viewportBoundsRange[0] << ";" << std::endl;
		result << "properties.limits.viewportBoundsRange[1] = " << properties.limits.viewportBoundsRange[1] << ";" << std::endl;
		result << "properties.limits.viewportSubPixelBits = " << properties.limits.viewportSubPixelBits << ";" << std::endl;
		result << "properties.limits.minMemoryMapAlignment = " << properties.limits.minMemoryMapAlignment << ";" << std::endl;
		result << "properties.limits.minTexelBufferOffsetAlignment = " << properties.limits.minTexelBufferOffsetAlignment << ";" << std::endl;
		result << "properties.limits.minUniformBufferOffsetAlignment = " << properties.limits.minUniformBufferOffsetAlignment << ";" << std::endl;
		result << "properties.limits.minStorageBufferOffsetAlignment = " << properties.limits.minStorageBufferOffsetAlignment << ";" << std::endl;
		result << "properties.limits.minTexelOffset = " << properties.limits.minTexelOffset << ";" << std::endl;
		result << "properties.limits.maxTexelOffset = " << properties.limits.maxTexelOffset << ";" << std::endl;
		result << "properties.limits.minTexelGatherOffset = " << properties.limits.minTexelGatherOffset << ";" << std::endl;
		result << "properties.limits.maxTexelGatherOffset = " << properties.limits.maxTexelGatherOffset << ";" << std::endl;
		result << "properties.limits.minInterpolationOffset = " << properties.limits.minInterpolationOffset << ";" << std::endl;
		result << "properties.limits.maxInterpolationOffset = " << properties.limits.maxInterpolationOffset << ";" << std::endl;
		result << "properties.limits.subPixelInterpolationOffsetBits = " << properties.limits.subPixelInterpolationOffsetBits << ";" << std::endl;
		result << "properties.limits.maxFramebufferWidth = " << properties.limits.maxFramebufferWidth << ";" << std::endl;
		result << "properties.limits.maxFramebufferHeight = " << properties.limits.maxFramebufferHeight << ";" << std::endl;
		result << "properties.limits.maxFramebufferLayers = " << properties.limits.maxFramebufferLayers << ";" << std::endl;
		result << "properties.limits.framebufferColorSampleCounts = " << properties.limits.framebufferColorSampleCounts << ";" << std::endl;
		result << "properties.limits.framebufferDepthSampleCounts = " << properties.limits.framebufferDepthSampleCounts << ";" << std::endl;
		result << "properties.limits.framebufferStencilSampleCounts = " << properties.limits.framebufferStencilSampleCounts << ";" << std::endl;
		result << "properties.limits.framebufferNoAttachmentsSampleCounts = " << properties.limits.framebufferNoAttachmentsSampleCounts << ";" << std::endl;
		result << "properties.limits.maxColorAttachments = " << properties.limits.maxColorAttachments << ";" << std::endl;
		result << "properties.limits.sampledImageColorSampleCounts = " << properties.limits.sampledImageColorSampleCounts << ";" << std::endl;
		result << "properties.limits.sampledImageIntegerSampleCounts = " << properties.limits.sampledImageIntegerSampleCounts << ";" << std::endl;
		result << "properties.limits.sampledImageDepthSampleCounts = " << properties.limits.sampledImageDepthSampleCounts << ";" << std::endl;
		result << "properties.limits.sampledImageStencilSampleCounts = " << properties.limits.sampledImageStencilSampleCounts << ";" << std::endl;
		result << "properties.limits.storageImageSampleCounts = " << properties.limits.storageImageSampleCounts << ";" << std::endl;
		result << "properties.limits.maxSampleMaskWords = " << properties.limits.maxSampleMaskWords << ";" << std::endl;
		result << "properties.limits.timestampComputeAndGraphics = " << properties.limits.timestampComputeAndGraphics << ";" << std::endl;
		result << "properties.limits.timestampPeriod = " << properties.limits.timestampPeriod << ";" << std::endl;
		result << "properties.limits.maxClipDistances = " << properties.limits.maxClipDistances << ";" << std::endl;
		result << "properties.limits.maxCullDistances = " << properties.limits.maxCullDistances << ";" << std::endl;
		result << "properties.limits.maxCombinedClipAndCullDistances = " << properties.limits.maxCombinedClipAndCullDistances << ";" << std::endl;
		result << "properties.limits.discreteQueuePriorities = " << properties.limits.discreteQueuePriorities << ";" << std::endl;
		result << "properties.limits.pointSizeRange[0] = " << properties.limits.pointSizeRange[0] << ";" << std::endl;
		result << "properties.limits.pointSizeRange[1] = " << properties.limits.pointSizeRange[1] << ";" << std::endl;
		result << "properties.limits.lineWidthRange[0] = " << properties.limits.lineWidthRange[0] << ";" << std::endl;
		result << "properties.limits.lineWidthRange[1] = " << properties.limits.lineWidthRange[1] << ";" << std::endl;
		result << "properties.limits.pointSizeGranularity = " << properties.limits.pointSizeGranularity << ";" << std::endl;
		result << "properties.limits.lineWidthGranularity = " << properties.limits.lineWidthGranularity << ";" << std::endl;
		result << "properties.limits.strictLines = " << properties.limits.strictLines << ";" << std::endl;
		result << "properties.limits.standardSampleLocations = " << properties.limits.standardSampleLocations << ";" << std::endl;
		result << "properties.limits.optimalBufferCopyOffsetAlignment = " << properties.limits.optimalBufferCopyOffsetAlignment << ";" << std::endl;
		result << "properties.limits.optimalBufferCopyRowPitchAlignment = " << properties.limits.optimalBufferCopyRowPitchAlignment << ";" << std::endl;
		result << "properties.limits.nonCoherentAtomSize = " << properties.limits.nonCoherentAtomSize << ";" << std::endl;
		result << std::endl;
		result << "properties.sparseProperties.residencyAlignedMipSize = " << properties.sparseProperties.residencyAlignedMipSize << ";" << std::endl;
		result << "properties.sparseProperties.residencyNonResidentStrict = " << properties.sparseProperties.residencyNonResidentStrict << ";" << std::endl;
		result << "properties.sparseProperties.residencyStandard2DBlockShape = " << properties.sparseProperties.residencyStandard2DBlockShape << ";" << std::endl;
		result << "properties.sparseProperties.residencyStandard2DMultisampleBlockShape = " << properties.sparseProperties.residencyStandard2DMultisampleBlockShape << ";" << std::endl;
		result << "properties.sparseProperties.residencyStandard3DBlockShape = " << properties.sparseProperties.residencyStandard3DBlockShape << ";" << std::endl;

		return result.str();
	}
}

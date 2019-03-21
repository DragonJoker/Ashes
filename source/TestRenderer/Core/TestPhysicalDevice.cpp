/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/TestPhysicalDevice.hpp"

#include "Core/TestInstance.hpp"
#include "Core/TestDevice.hpp"

namespace test_renderer
{
	PhysicalDevice::PhysicalDevice( Instance const & instance )
		: ashes::PhysicalDevice{ instance }
		, m_instance{ instance }
	{
		m_shaderVersion = 450u;
		doInitialise();
	}

	ashes::LayerPropertiesArray PhysicalDevice::enumerateLayerProperties()const
	{
		ashes::LayerPropertiesArray result;
		return result;
	}

	ashes::ExtensionPropertiesArray PhysicalDevice::enumerateExtensionProperties( std::string const & layerName )const
	{
		ashes::ExtensionPropertiesArray result;
		return result;
	}

	ashes::PhysicalDeviceProperties PhysicalDevice::getProperties()const
	{
		return m_properties;
	}

	ashes::PhysicalDeviceMemoryProperties PhysicalDevice::getMemoryProperties()const
	{
		return Instance::getMemoryProperties();
	}

	ashes::PhysicalDeviceFeatures PhysicalDevice::getFeatures()const
	{
		return m_features;
	}

	ashes::QueueFamilyPropertiesArray PhysicalDevice::getQueueFamilyProperties()const
	{
		return m_queueProperties;
	}

	ashes::FormatProperties PhysicalDevice::getFormatProperties( ashes::Format fmt )const
	{
		return m_formatProperties[size_t( fmt )];
	}

	void PhysicalDevice::doInitialise()
	{
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

		m_properties.apiVersion = 4198470u;
		m_properties.deviceID = 1u;
		m_properties.deviceName = "Dummy";
		m_properties.vendorID = 1u;
		m_properties.deviceType = ashes::PhysicalDeviceType::eCpu;
		m_properties.driverVersion = 1u;

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
	}
}

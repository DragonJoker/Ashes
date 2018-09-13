/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "PhysicalDevice.hpp"

#include "Renderer.hpp"
#include "Device.hpp"

#include <algorithm>

namespace ashes
{
	PhysicalDevice::PhysicalDevice( Renderer & renderer )
		: m_renderer{ renderer }
	{
	}

	bool PhysicalDevice::deduceMemoryType( uint32_t typeBits
		, MemoryPropertyFlags requirements
		, uint32_t & typeIndex )const
	{
		bool result{ false };

		// Recherche parmi les types de mémoire de la première ayant les propriétés voulues.
		uint32_t i{ 0 };

		while ( i < m_memoryProperties.memoryTypes.size() && !result )
		{
			if ( ( typeBits & 1 ) == 1 )
			{
				// Le type de mémoire est disponible, a-t-il les propriétés demandées?
				if ( ( m_memoryProperties.memoryTypes[i].propertyFlags & requirements ) == requirements )
				{
					typeIndex = i;
					result = true;
				}
			}

			typeBits >>= 1;
			++i;
		}

		return result;
	}

	std::string PhysicalDevice::dumpProperties()const
	{
		std::stringstream result;
		result.imbue( std::locale{ "C" } );
		result << "m_features.robustBufferAccess = " << m_features.robustBufferAccess << ";" << std::endl;
		result << "m_features.fullDrawIndexUint32 = " << m_features.fullDrawIndexUint32 << ";" << std::endl;
		result << "m_features.imageCubeArray = " << m_features.imageCubeArray << ";" << std::endl;
		result << "m_features.independentBlend = " << m_features.independentBlend << ";" << std::endl;
		result << "m_features.geometryShader = " << m_features.geometryShader << ";" << std::endl;
		result << "m_features.tessellationShader = " << m_features.tessellationShader << ";" << std::endl;
		result << "m_features.sampleRateShading = " << m_features.sampleRateShading << ";" << std::endl;
		result << "m_features.dualSrcBlend = " << m_features.dualSrcBlend << ";" << std::endl;
		result << "m_features.logicOp = " << m_features.logicOp << ";" << std::endl;
		result << "m_features.multiDrawIndirect = " << m_features.multiDrawIndirect << ";" << std::endl;
		result << "m_features.drawIndirectFirstInstance = " << m_features.drawIndirectFirstInstance << ";" << std::endl;
		result << "m_features.depthClamp = " << m_features.depthClamp << ";" << std::endl;
		result << "m_features.depthBiasClamp = " << m_features.depthBiasClamp << ";" << std::endl;
		result << "m_features.fillModeNonSolid = " << m_features.fillModeNonSolid << ";" << std::endl;
		result << "m_features.depthBounds = " << m_features.depthBounds << ";" << std::endl;
		result << "m_features.wideLines = " << m_features.wideLines << ";" << std::endl;
		result << "m_features.largePoints = " << m_features.largePoints << ";" << std::endl;
		result << "m_features.alphaToOne = " << m_features.alphaToOne << ";" << std::endl;
		result << "m_features.multiViewport = " << m_features.multiViewport << ";" << std::endl;
		result << "m_features.samplerAnisotropy = " << m_features.samplerAnisotropy << ";" << std::endl;
		result << "m_features.textureCompressionETC2 = " << m_features.textureCompressionETC2 << ";" << std::endl;
		result << "m_features.textureCompressionASTC_LDR = " << m_features.textureCompressionASTC_LDR << ";" << std::endl;
		result << "m_features.textureCompressionBC = " << m_features.textureCompressionBC << ";" << std::endl;
		result << "m_features.occlusionQueryPrecise = " << m_features.occlusionQueryPrecise << ";" << std::endl;
		result << "m_features.pipelineStatisticsQuery = " << m_features.pipelineStatisticsQuery << ";" << std::endl;
		result << "m_features.vertexPipelineStoresAndAtomics = " << m_features.vertexPipelineStoresAndAtomics << ";" << std::endl;
		result << "m_features.fragmentStoresAndAtomics = " << m_features.fragmentStoresAndAtomics << ";" << std::endl;
		result << "m_features.shaderTessellationAndGeometryPointSize = " << m_features.shaderTessellationAndGeometryPointSize << ";" << std::endl;
		result << "m_features.shaderImageGatherExtended = " << m_features.shaderImageGatherExtended << ";" << std::endl;
		result << "m_features.shaderStorageImageExtendedFormats = " << m_features.shaderStorageImageExtendedFormats << ";" << std::endl;
		result << "m_features.shaderStorageImageMultisample = " << m_features.shaderStorageImageMultisample << ";" << std::endl;
		result << "m_features.shaderStorageImageReadWithoutFormat = " << m_features.shaderStorageImageReadWithoutFormat << ";" << std::endl;
		result << "m_features.shaderStorageImageWriteWithoutFormat = " << m_features.shaderStorageImageWriteWithoutFormat << ";" << std::endl;
		result << "m_features.shaderUniformBufferArrayDynamicIndexing = " << m_features.shaderUniformBufferArrayDynamicIndexing << ";" << std::endl;
		result << "m_features.shaderSampledImageArrayDynamicIndexing = " << m_features.shaderSampledImageArrayDynamicIndexing << ";" << std::endl;
		result << "m_features.shaderStorageBufferArrayDynamicIndexing = " << m_features.shaderStorageBufferArrayDynamicIndexing << ";" << std::endl;
		result << "m_features.shaderStorageImageArrayDynamicIndexing = " << m_features.shaderStorageImageArrayDynamicIndexing << ";" << std::endl;
		result << "m_features.shaderClipDistance = " << m_features.shaderClipDistance << ";" << std::endl;
		result << "m_features.shaderCullDistance = " << m_features.shaderCullDistance << ";" << std::endl;
		result << "m_features.shaderFloat64 = " << m_features.shaderFloat64 << ";" << std::endl;
		result << "m_features.shaderInt64 = " << m_features.shaderInt64 << ";" << std::endl;
		result << "m_features.shaderInt16 = " << m_features.shaderInt16 << ";" << std::endl;
		result << "m_features.shaderResourceResidency = " << m_features.shaderResourceResidency << ";" << std::endl;
		result << "m_features.shaderResourceMinLod = " << m_features.shaderResourceMinLod << ";" << std::endl;
		result << "m_features.sparseBinding = " << m_features.sparseBinding << ";" << std::endl;
		result << "m_features.sparseResidencyBuffer = " << m_features.sparseResidencyBuffer << ";" << std::endl;
		result << "m_features.sparseResidencyImage2D = " << m_features.sparseResidencyImage2D << ";" << std::endl;
		result << "m_features.sparseResidencyImage3D = " << m_features.sparseResidencyImage3D << ";" << std::endl;
		result << "m_features.sparseResidency2Samples = " << m_features.sparseResidency2Samples << ";" << std::endl;
		result << "m_features.sparseResidency4Samples = " << m_features.sparseResidency4Samples << ";" << std::endl;
		result << "m_features.sparseResidency8Samples = " << m_features.sparseResidency8Samples << ";" << std::endl;
		result << "m_features.sparseResidency16Samples = " << m_features.sparseResidency16Samples << ";" << std::endl;
		result << "m_features.sparseResidencyAliased = " << m_features.sparseResidencyAliased << ";" << std::endl;
		result << "m_features.variableMultisampleRate = " << m_features.variableMultisampleRate << ";" << std::endl;
		result << "m_features.inheritedQueries = " << m_features.inheritedQueries << ";" << std::endl;
		result << std::endl;
		result << "m_properties.apiVersion = " << m_properties.apiVersion << ";" << std::endl;
		result << "m_properties.deviceID = " << m_properties.deviceID << ";" << std::endl;
		result << "m_properties.deviceName = " << m_properties.deviceName << ";" << std::endl;
		result << "m_properties.vendorID = " << m_properties.vendorID << ";" << std::endl;
		result << "m_properties.deviceType = " << getName( m_properties.deviceType ) << ";" << std::endl;
		result << "m_properties.driverVersion = " << m_properties.driverVersion << ";" << std::endl;
		result << std::endl;
		result << "m_properties.limits.maxImageDimension1D = " << m_properties.limits.maxImageDimension1D << ";" << std::endl;
		result << "m_properties.limits.maxImageDimension2D = " << m_properties.limits.maxImageDimension2D << ";" << std::endl;
		result << "m_properties.limits.maxImageDimension3D = " << m_properties.limits.maxImageDimension3D << ";" << std::endl;
		result << "m_properties.limits.maxImageDimensionCube = " << m_properties.limits.maxImageDimensionCube << ";" << std::endl;
		result << "m_properties.limits.maxImageArrayLayers = " << m_properties.limits.maxImageArrayLayers << ";" << std::endl;
		result << "m_properties.limits.maxTexelBufferElements = " << m_properties.limits.maxTexelBufferElements << ";" << std::endl;
		result << "m_properties.limits.maxUniformBufferRange = " << m_properties.limits.maxUniformBufferRange << ";" << std::endl;
		result << "m_properties.limits.maxStorageBufferRange = " << m_properties.limits.maxStorageBufferRange << ";" << std::endl;
		result << "m_properties.limits.maxPushConstantsSize = " << m_properties.limits.maxPushConstantsSize << ";" << std::endl;
		result << "m_properties.limits.maxMemoryAllocationCount = " << m_properties.limits.maxMemoryAllocationCount << ";" << std::endl;
		result << "m_properties.limits.maxSamplerAllocationCount = " << m_properties.limits.maxSamplerAllocationCount << ";" << std::endl;
		result << "m_properties.limits.bufferImageGranularity = " << m_properties.limits.bufferImageGranularity << ";" << std::endl;
		result << "m_properties.limits.sparseAddressSpaceSize = " << m_properties.limits.sparseAddressSpaceSize << ";" << std::endl;
		result << "m_properties.limits.maxBoundDescriptorSets = " << m_properties.limits.maxBoundDescriptorSets << ";" << std::endl;
		result << "m_properties.limits.maxPerStageDescriptorSamplers = " << m_properties.limits.maxPerStageDescriptorSamplers << ";" << std::endl;
		result << "m_properties.limits.maxPerStageDescriptorUniformBuffers = " << m_properties.limits.maxPerStageDescriptorUniformBuffers << ";" << std::endl;
		result << "m_properties.limits.maxPerStageDescriptorStorageBuffers = " << m_properties.limits.maxPerStageDescriptorStorageBuffers << ";" << std::endl;
		result << "m_properties.limits.maxPerStageDescriptorSampledImages = " << m_properties.limits.maxPerStageDescriptorSampledImages << ";" << std::endl;
		result << "m_properties.limits.maxPerStageDescriptorStorageImages = " << m_properties.limits.maxPerStageDescriptorStorageImages << ";" << std::endl;
		result << "m_properties.limits.maxPerStageDescriptorInputAttachments = " << m_properties.limits.maxPerStageDescriptorInputAttachments << ";" << std::endl;
		result << "m_properties.limits.maxPerStageResources = " << m_properties.limits.maxPerStageResources << ";" << std::endl;
		result << "m_properties.limits.maxDescriptorSetSamplers = " << m_properties.limits.maxDescriptorSetSamplers << ";" << std::endl;
		result << "m_properties.limits.maxDescriptorSetUniformBuffers = " << m_properties.limits.maxDescriptorSetUniformBuffers << ";" << std::endl;
		result << "m_properties.limits.maxDescriptorSetUniformBuffersDynamic = " << m_properties.limits.maxDescriptorSetUniformBuffersDynamic << ";" << std::endl;
		result << "m_properties.limits.maxDescriptorSetStorageBuffers = " << m_properties.limits.maxDescriptorSetStorageBuffers << ";" << std::endl;
		result << "m_properties.limits.maxDescriptorSetStorageBuffersDynamic = " << m_properties.limits.maxDescriptorSetStorageBuffersDynamic << ";" << std::endl;
		result << "m_properties.limits.maxDescriptorSetSampledImages = " << m_properties.limits.maxDescriptorSetSampledImages << ";" << std::endl;
		result << "m_properties.limits.maxDescriptorSetStorageImages = " << m_properties.limits.maxDescriptorSetStorageImages << ";" << std::endl;
		result << "m_properties.limits.maxDescriptorSetInputAttachments = " << m_properties.limits.maxDescriptorSetInputAttachments << ";" << std::endl;
		result << "m_properties.limits.maxVertexInputAttributes = " << m_properties.limits.maxVertexInputAttributes << ";" << std::endl;
		result << "m_properties.limits.maxVertexInputBindings = " << m_properties.limits.maxVertexInputBindings << ";" << std::endl;
		result << "m_properties.limits.maxVertexInputAttributeOffset = " << m_properties.limits.maxVertexInputAttributeOffset << ";" << std::endl;
		result << "m_properties.limits.maxVertexInputBindingStride = " << m_properties.limits.maxVertexInputBindingStride << ";" << std::endl;
		result << "m_properties.limits.maxVertexOutputComponents = " << m_properties.limits.maxVertexOutputComponents << ";" << std::endl;
		result << "m_properties.limits.maxTessellationGenerationLevel = " << m_properties.limits.maxTessellationGenerationLevel << ";" << std::endl;
		result << "m_properties.limits.maxTessellationPatchSize = " << m_properties.limits.maxTessellationPatchSize << ";" << std::endl;
		result << "m_properties.limits.maxTessellationControlPerVertexInputComponents = " << m_properties.limits.maxTessellationControlPerVertexInputComponents << ";" << std::endl;
		result << "m_properties.limits.maxTessellationControlPerVertexOutputComponents = " << m_properties.limits.maxTessellationControlPerVertexOutputComponents << ";" << std::endl;
		result << "m_properties.limits.maxTessellationControlPerPatchOutputComponents = " << m_properties.limits.maxTessellationControlPerPatchOutputComponents << ";" << std::endl;
		result << "m_properties.limits.maxTessellationControlTotalOutputComponents = " << m_properties.limits.maxTessellationControlTotalOutputComponents << ";" << std::endl;
		result << "m_properties.limits.maxTessellationEvaluationInputComponents = " << m_properties.limits.maxTessellationEvaluationInputComponents << ";" << std::endl;
		result << "m_properties.limits.maxTessellationEvaluationOutputComponents = " << m_properties.limits.maxTessellationEvaluationOutputComponents << ";" << std::endl;
		result << "m_properties.limits.maxGeometryShaderInvocations = " << m_properties.limits.maxGeometryShaderInvocations << ";" << std::endl;
		result << "m_properties.limits.maxGeometryInputComponents = " << m_properties.limits.maxGeometryInputComponents << ";" << std::endl;
		result << "m_properties.limits.maxGeometryOutputComponents = " << m_properties.limits.maxGeometryOutputComponents << ";" << std::endl;
		result << "m_properties.limits.maxGeometryOutputVertices = " << m_properties.limits.maxGeometryOutputVertices << ";" << std::endl;
		result << "m_properties.limits.maxGeometryTotalOutputComponents = " << m_properties.limits.maxGeometryTotalOutputComponents << ";" << std::endl;
		result << "m_properties.limits.maxFragmentInputComponents = " << m_properties.limits.maxFragmentInputComponents << ";" << std::endl;
		result << "m_properties.limits.maxFragmentOutputAttachments = " << m_properties.limits.maxFragmentOutputAttachments << ";" << std::endl;
		result << "m_properties.limits.maxFragmentDualSrcAttachments = " << m_properties.limits.maxFragmentDualSrcAttachments << ";" << std::endl;
		result << "m_properties.limits.maxFragmentCombinedOutputResources = " << m_properties.limits.maxFragmentCombinedOutputResources << ";" << std::endl;
		result << "m_properties.limits.maxComputeSharedMemorySize = " << m_properties.limits.maxComputeSharedMemorySize << ";" << std::endl;
		result << "m_properties.limits.maxComputeWorkGroupCount[0] = " << m_properties.limits.maxComputeWorkGroupCount[0] << ";" << std::endl;
		result << "m_properties.limits.maxComputeWorkGroupCount[1] = " << m_properties.limits.maxComputeWorkGroupCount[1] << ";" << std::endl;
		result << "m_properties.limits.maxComputeWorkGroupCount[2] = " << m_properties.limits.maxComputeWorkGroupCount[2] << ";" << std::endl;
		result << "m_properties.limits.maxComputeWorkGroupInvocations = " << m_properties.limits.maxComputeWorkGroupInvocations << ";" << std::endl;
		result << "m_properties.limits.maxComputeWorkGroupSize[0] = " << m_properties.limits.maxComputeWorkGroupSize[0] << ";" << std::endl;
		result << "m_properties.limits.maxComputeWorkGroupSize[1] = " << m_properties.limits.maxComputeWorkGroupSize[1] << ";" << std::endl;
		result << "m_properties.limits.maxComputeWorkGroupSize[2] = " << m_properties.limits.maxComputeWorkGroupSize[2] << ";" << std::endl;
		result << "m_properties.limits.subPixelPrecisionBits = " << m_properties.limits.subPixelPrecisionBits << ";" << std::endl;
		result << "m_properties.limits.subTexelPrecisionBits = " << m_properties.limits.subTexelPrecisionBits << ";" << std::endl;
		result << "m_properties.limits.mipmapPrecisionBits = " << m_properties.limits.mipmapPrecisionBits << ";" << std::endl;
		result << "m_properties.limits.maxDrawIndexedIndexValue = " << m_properties.limits.maxDrawIndexedIndexValue << ";" << std::endl;
		result << "m_properties.limits.maxDrawIndirectCount = " << m_properties.limits.maxDrawIndirectCount << ";" << std::endl;
		result << "m_properties.limits.maxSamplerLodBias = " << m_properties.limits.maxSamplerLodBias << ";" << std::endl;
		result << "m_properties.limits.maxSamplerAnisotropy = " << m_properties.limits.maxSamplerAnisotropy << ";" << std::endl;
		result << "m_properties.limits.maxViewports = " << m_properties.limits.maxViewports << ";" << std::endl;
		result << "m_properties.limits.maxViewportDimensions[0] = " << m_properties.limits.maxViewportDimensions[0] << ";" << std::endl;
		result << "m_properties.limits.maxViewportDimensions[1] = " << m_properties.limits.maxViewportDimensions[1] << ";" << std::endl;
		result << "m_properties.limits.viewportBoundsRange[0] = " << m_properties.limits.viewportBoundsRange[0] << ";" << std::endl;
		result << "m_properties.limits.viewportBoundsRange[1] = " << m_properties.limits.viewportBoundsRange[1] << ";" << std::endl;
		result << "m_properties.limits.viewportSubPixelBits = " << m_properties.limits.viewportSubPixelBits << ";" << std::endl;
		result << "m_properties.limits.minMemoryMapAlignment = " << m_properties.limits.minMemoryMapAlignment << ";" << std::endl;
		result << "m_properties.limits.minTexelBufferOffsetAlignment = " << m_properties.limits.minTexelBufferOffsetAlignment << ";" << std::endl;
		result << "m_properties.limits.minUniformBufferOffsetAlignment = " << m_properties.limits.minUniformBufferOffsetAlignment << ";" << std::endl;
		result << "m_properties.limits.minStorageBufferOffsetAlignment = " << m_properties.limits.minStorageBufferOffsetAlignment << ";" << std::endl;
		result << "m_properties.limits.minTexelOffset = " << m_properties.limits.minTexelOffset << ";" << std::endl;
		result << "m_properties.limits.maxTexelOffset = " << m_properties.limits.maxTexelOffset << ";" << std::endl;
		result << "m_properties.limits.minTexelGatherOffset = " << m_properties.limits.minTexelGatherOffset << ";" << std::endl;
		result << "m_properties.limits.maxTexelGatherOffset = " << m_properties.limits.maxTexelGatherOffset << ";" << std::endl;
		result << "m_properties.limits.minInterpolationOffset = " << m_properties.limits.minInterpolationOffset << ";" << std::endl;
		result << "m_properties.limits.maxInterpolationOffset = " << m_properties.limits.maxInterpolationOffset << ";" << std::endl;
		result << "m_properties.limits.subPixelInterpolationOffsetBits = " << m_properties.limits.subPixelInterpolationOffsetBits << ";" << std::endl;
		result << "m_properties.limits.maxFramebufferWidth = " << m_properties.limits.maxFramebufferWidth << ";" << std::endl;
		result << "m_properties.limits.maxFramebufferHeight = " << m_properties.limits.maxFramebufferHeight << ";" << std::endl;
		result << "m_properties.limits.maxFramebufferLayers = " << m_properties.limits.maxFramebufferLayers << ";" << std::endl;
		result << "m_properties.limits.framebufferColorSampleCounts = " << m_properties.limits.framebufferColorSampleCounts << ";" << std::endl;
		result << "m_properties.limits.framebufferDepthSampleCounts = " << m_properties.limits.framebufferDepthSampleCounts << ";" << std::endl;
		result << "m_properties.limits.framebufferStencilSampleCounts = " << m_properties.limits.framebufferStencilSampleCounts << ";" << std::endl;
		result << "m_properties.limits.framebufferNoAttachmentsSampleCounts = " << m_properties.limits.framebufferNoAttachmentsSampleCounts << ";" << std::endl;
		result << "m_properties.limits.maxColorAttachments = " << m_properties.limits.maxColorAttachments << ";" << std::endl;
		result << "m_properties.limits.sampledImageColorSampleCounts = " << m_properties.limits.sampledImageColorSampleCounts << ";" << std::endl;
		result << "m_properties.limits.sampledImageIntegerSampleCounts = " << m_properties.limits.sampledImageIntegerSampleCounts << ";" << std::endl;
		result << "m_properties.limits.sampledImageDepthSampleCounts = " << m_properties.limits.sampledImageDepthSampleCounts << ";" << std::endl;
		result << "m_properties.limits.sampledImageStencilSampleCounts = " << m_properties.limits.sampledImageStencilSampleCounts << ";" << std::endl;
		result << "m_properties.limits.storageImageSampleCounts = " << m_properties.limits.storageImageSampleCounts << ";" << std::endl;
		result << "m_properties.limits.maxSampleMaskWords = " << m_properties.limits.maxSampleMaskWords << ";" << std::endl;
		result << "m_properties.limits.timestampComputeAndGraphics = " << m_properties.limits.timestampComputeAndGraphics << ";" << std::endl;
		result << "m_properties.limits.timestampPeriod = " << m_properties.limits.timestampPeriod << ";" << std::endl;
		result << "m_properties.limits.maxClipDistances = " << m_properties.limits.maxClipDistances << ";" << std::endl;
		result << "m_properties.limits.maxCullDistances = " << m_properties.limits.maxCullDistances << ";" << std::endl;
		result << "m_properties.limits.maxCombinedClipAndCullDistances = " << m_properties.limits.maxCombinedClipAndCullDistances << ";" << std::endl;
		result << "m_properties.limits.discreteQueuePriorities = " << m_properties.limits.discreteQueuePriorities << ";" << std::endl;
		result << "m_properties.limits.pointSizeRange[0] = " << m_properties.limits.pointSizeRange[0] << ";" << std::endl;
		result << "m_properties.limits.pointSizeRange[1] = " << m_properties.limits.pointSizeRange[1] << ";" << std::endl;
		result << "m_properties.limits.lineWidthRange[0] = " << m_properties.limits.lineWidthRange[0] << ";" << std::endl;
		result << "m_properties.limits.lineWidthRange[1] = " << m_properties.limits.lineWidthRange[1] << ";" << std::endl;
		result << "m_properties.limits.pointSizeGranularity = " << m_properties.limits.pointSizeGranularity << ";" << std::endl;
		result << "m_properties.limits.lineWidthGranularity = " << m_properties.limits.lineWidthGranularity << ";" << std::endl;
		result << "m_properties.limits.strictLines = " << m_properties.limits.strictLines << ";" << std::endl;
		result << "m_properties.limits.standardSampleLocations = " << m_properties.limits.standardSampleLocations << ";" << std::endl;
		result << "m_properties.limits.optimalBufferCopyOffsetAlignment = " << m_properties.limits.optimalBufferCopyOffsetAlignment << ";" << std::endl;
		result << "m_properties.limits.optimalBufferCopyRowPitchAlignment = " << m_properties.limits.optimalBufferCopyRowPitchAlignment << ";" << std::endl;
		result << "m_properties.limits.nonCoherentAtomSize = " << m_properties.limits.nonCoherentAtomSize << ";" << std::endl;
		result << std::endl;
		result << "m_properties.sparseProperties.residencyAlignedMipSize = " << m_properties.sparseProperties.residencyAlignedMipSize << ";" << std::endl;
		result << "m_properties.sparseProperties.residencyNonResidentStrict = " << m_properties.sparseProperties.residencyNonResidentStrict << ";" << std::endl;
		result << "m_properties.sparseProperties.residencyStandard2DBlockShape = " << m_properties.sparseProperties.residencyStandard2DBlockShape << ";" << std::endl;
		result << "m_properties.sparseProperties.residencyStandard2DMultisampleBlockShape = " << m_properties.sparseProperties.residencyStandard2DMultisampleBlockShape << ";" << std::endl;
		result << "m_properties.sparseProperties.residencyStandard3DBlockShape = " << m_properties.sparseProperties.residencyStandard3DBlockShape << ";" << std::endl;

		return result.str();
	}

	std::vector< ExtensionProperties > const & PhysicalDevice::getExtensionProperties( std::string const & layerName )const
	{
		auto it = std::find_if( m_layerExtensions.begin()
			, m_layerExtensions.end()
			, [&layerName]( ashes::LayerProperties const & lookup )
			{
				return lookup.layerName == layerName;
			} );

		if ( it != m_layerExtensions.end() )
		{
			return it->extensions;
		}

		static std::vector< ExtensionProperties > const dummy;
		return dummy;
	}
}

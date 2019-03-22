#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkBool32 convert( bool value )
	{
		return value ? 1u : 0u;
	}

	VkPhysicalDeviceFeatures convert( ashes::PhysicalDeviceFeatures const & value )
	{
		return VkPhysicalDeviceFeatures
		{
			convert( value.robustBufferAccess ),
			convert( value.fullDrawIndexUint32 ),
			convert( value.imageCubeArray ),
			convert( value.independentBlend ),
			convert( value.geometryShader ),
			convert( value.tessellationShader ),
			convert( value.sampleRateShading ),
			convert( value.dualSrcBlend ),
			convert( value.logicOp ),
			convert( value.multiDrawIndirect ),
			convert( value.drawIndirectFirstInstance ),
			convert( value.depthClamp ),
			convert( value.depthBiasClamp ),
			convert( value.fillModeNonSolid ),
			convert( value.depthBounds ),
			convert( value.wideLines ),
			convert( value.largePoints ),
			convert( value.alphaToOne ),
			convert( value.multiViewport ),
			convert( value.samplerAnisotropy ),
			convert( value.textureCompressionETC2 ),
			convert( value.textureCompressionASTC_LDR ),
			convert( value.textureCompressionBC ),
			convert( value.occlusionQueryPrecise ),
			convert( value.pipelineStatisticsQuery ),
			convert( value.vertexPipelineStoresAndAtomics ),
			convert( value.fragmentStoresAndAtomics ),
			convert( value.shaderTessellationAndGeometryPointSize ),
			convert( value.shaderImageGatherExtended ),
			convert( value.shaderStorageImageExtendedFormats ),
			convert( value.shaderStorageImageMultisample ),
			convert( value.shaderStorageImageReadWithoutFormat ),
			convert( value.shaderStorageImageWriteWithoutFormat ),
			convert( value.shaderUniformBufferArrayDynamicIndexing ),
			convert( value.shaderSampledImageArrayDynamicIndexing ),
			convert( value.shaderStorageBufferArrayDynamicIndexing ),
			convert( value.shaderStorageImageArrayDynamicIndexing ),
			convert( value.shaderClipDistance ),
			convert( value.shaderCullDistance ),
			convert( value.shaderFloat64 ),
			convert( value.shaderInt64 ),
			convert( value.shaderInt16 ),
			convert( value.shaderResourceResidency ),
			convert( value.shaderResourceMinLod ),
			convert( value.sparseBinding ),
			convert( value.sparseResidencyBuffer ),
			convert( value.sparseResidencyImage2D ),
			convert( value.sparseResidencyImage3D ),
			convert( value.sparseResidency2Samples ),
			convert( value.sparseResidency4Samples ),
			convert( value.sparseResidency8Samples ),
			convert( value.sparseResidency16Samples ),
			convert( value.sparseResidencyAliased ),
			convert( value.variableMultisampleRate ),
			convert( value.inheritedQueries ),
		};
	}

	bool convert( VkBool32 value )
	{
		return value ? true : false;
	}

	ashes::PhysicalDeviceFeatures convert( VkPhysicalDeviceFeatures const & value )
	{
		return ashes::PhysicalDeviceFeatures
		{
			convert( value.robustBufferAccess ),
			convert( value.fullDrawIndexUint32 ),
			convert( value.imageCubeArray ),
			convert( value.independentBlend ),
			convert( value.geometryShader ),
			convert( value.tessellationShader ),
			convert( value.sampleRateShading ),
			convert( value.dualSrcBlend ),
			convert( value.logicOp ),
			convert( value.multiDrawIndirect ),
			convert( value.drawIndirectFirstInstance ),
			convert( value.depthClamp ),
			convert( value.depthBiasClamp ),
			convert( value.fillModeNonSolid ),
			convert( value.depthBounds ),
			convert( value.wideLines ),
			convert( value.largePoints ),
			convert( value.alphaToOne ),
			convert( value.multiViewport ),
			convert( value.samplerAnisotropy ),
			convert( value.textureCompressionETC2 ),
			convert( value.textureCompressionASTC_LDR ),
			convert( value.textureCompressionBC ),
			convert( value.occlusionQueryPrecise ),
			convert( value.pipelineStatisticsQuery ),
			convert( value.vertexPipelineStoresAndAtomics ),
			convert( value.fragmentStoresAndAtomics ),
			convert( value.shaderTessellationAndGeometryPointSize ),
			convert( value.shaderImageGatherExtended ),
			convert( value.shaderStorageImageExtendedFormats ),
			convert( value.shaderStorageImageMultisample ),
			convert( value.shaderStorageImageReadWithoutFormat ),
			convert( value.shaderStorageImageWriteWithoutFormat ),
			convert( value.shaderUniformBufferArrayDynamicIndexing ),
			convert( value.shaderSampledImageArrayDynamicIndexing ),
			convert( value.shaderStorageBufferArrayDynamicIndexing ),
			convert( value.shaderStorageImageArrayDynamicIndexing ),
			convert( value.shaderClipDistance ),
			convert( value.shaderCullDistance ),
			convert( value.shaderFloat64 ),
			convert( value.shaderInt64 ),
			convert( value.shaderInt16 ),
			convert( value.shaderResourceResidency ),
			convert( value.shaderResourceMinLod ),
			convert( value.sparseBinding ),
			convert( value.sparseResidencyBuffer ),
			convert( value.sparseResidencyImage2D ),
			convert( value.sparseResidencyImage3D ),
			convert( value.sparseResidency2Samples ),
			convert( value.sparseResidency4Samples ),
			convert( value.sparseResidency8Samples ),
			convert( value.sparseResidency16Samples ),
			convert( value.sparseResidencyAliased ),
			convert( value.variableMultisampleRate ),
			convert( value.inheritedQueries ),
		};
	}
}

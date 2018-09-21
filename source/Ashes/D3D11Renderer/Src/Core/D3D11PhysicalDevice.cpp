/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/D3D11PhysicalDevice.hpp"

#include "Core/D3D11Renderer.hpp"
#include "Core/D3D11Device.hpp"

namespace d3d11_renderer
{
	PhysicalDevice::PhysicalDevice( Renderer & renderer
		, IDXGIAdapter * adapter
		, IDXGIAdapter1 * adapter1
		, IDXGIAdapter2 * adapter2 )
		: ashes::PhysicalDevice{ renderer }
		, m_renderer{ renderer }
		, m_adapter{ adapter }
		, m_adapter1{ adapter1 }
		, m_adapter2{ adapter2 }
	{
		m_shaderVersion = 450u;
		initialise();
	}

	PhysicalDevice::~PhysicalDevice()
	{
		safeRelease( m_output );
		safeRelease( m_adapter2 );
		safeRelease( m_adapter1 );
		safeRelease( m_adapter );
	}

	void PhysicalDevice::initialise()
	{
		DXGI_ADAPTER_DESC2 adapterDesc;
		m_properties.apiVersion = 4198470u;

		if ( m_adapter2
			&& SUCCEEDED( m_adapter2->GetDesc2( &adapterDesc ) ) )
		{
			m_properties.deviceName = toString( adapterDesc.Description );
			m_properties.deviceID = adapterDesc.DeviceId;
			m_properties.vendorID = adapterDesc.VendorId;
			m_properties.driverVersion = adapterDesc.Revision;
			// Store the dedicated video card memory in megabytes.
			auto videoCardMemory = int( adapterDesc.DedicatedVideoMemory / 1024 / 1024 );
		}

		if ( SUCCEEDED( m_adapter->EnumOutputs( 0, &m_output ) ) )
		{
			DXGI_OUTPUT_DESC desc{};
			m_output->GetDesc( &desc );
		}

		m_properties.deviceType = ashes::PhysicalDeviceType::eDiscreteGpu;
		m_features.robustBufferAccess = true;
		m_features.fullDrawIndexUint32 = true;
		m_features.imageCubeArray = true;
		m_features.independentBlend = true;
		m_features.geometryShader = true;
		m_features.tessellationShader = true;
		m_features.sampleRateShading = true;
		m_features.dualSrcBlend = true;
		m_features.logicOp = false;
		m_features.multiDrawIndirect = false;
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
		m_features.textureCompressionETC2 = false;
		m_features.textureCompressionASTC_LDR = false;
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

		m_properties.limits.maxImageDimension1D = D3D11_REQ_TEXTURE1D_U_DIMENSION;
		m_properties.limits.maxImageDimension2D = D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION;
		m_properties.limits.maxImageDimension3D = D3D11_REQ_TEXTURE3D_U_V_OR_W_DIMENSION;
		m_properties.limits.maxImageDimensionCube = D3D11_REQ_TEXTURECUBE_DIMENSION;
		m_properties.limits.maxImageArrayLayers = std::min( D3D11_REQ_TEXTURE1D_ARRAY_AXIS_DIMENSION, D3D11_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION );
		m_properties.limits.maxTexelBufferElements = 1 << D3D11_REQ_BUFFER_RESOURCE_TEXEL_COUNT_2_TO_EXP;
		m_properties.limits.maxUniformBufferRange = 65536u;
		m_properties.limits.maxStorageBufferRange = 4294967295u;
		m_properties.limits.maxPushConstantsSize = 256u;
		m_properties.limits.maxMemoryAllocationCount = 4096u;
		m_properties.limits.maxSamplerAllocationCount = 4000u;
		m_properties.limits.bufferImageGranularity = 1024u;
		m_properties.limits.sparseAddressSpaceSize = 18446744073709551615u;
		m_properties.limits.maxBoundDescriptorSets = 8u;
		m_properties.limits.maxPerStageDescriptorSamplers = D3D11_REQ_SAMPLER_OBJECT_COUNT_PER_DEVICE;
		m_properties.limits.maxPerStageDescriptorUniformBuffers = D3D11_COMMONSHADER_CONSTANT_BUFFER_HW_SLOT_COUNT;
		m_properties.limits.maxPerStageDescriptorStorageBuffers = 1048576u;
		m_properties.limits.maxPerStageDescriptorSampledImages = D3D11_REQ_SAMPLER_OBJECT_COUNT_PER_DEVICE;
		m_properties.limits.maxPerStageDescriptorStorageImages = 1048576u;
		m_properties.limits.maxPerStageDescriptorInputAttachments = 1048576u;
		m_properties.limits.maxPerStageResources = 4294967295u;
		m_properties.limits.maxDescriptorSetSamplers = D3D11_REQ_SAMPLER_OBJECT_COUNT_PER_DEVICE;
		m_properties.limits.maxDescriptorSetUniformBuffers = D3D11_COMMONSHADER_CONSTANT_BUFFER_HW_SLOT_COUNT;
		m_properties.limits.maxDescriptorSetUniformBuffersDynamic = D3D11_COMMONSHADER_CONSTANT_BUFFER_HW_SLOT_COUNT;
		m_properties.limits.maxDescriptorSetStorageBuffers = 1048576u;
		m_properties.limits.maxDescriptorSetStorageBuffersDynamic = 16u;
		m_properties.limits.maxDescriptorSetSampledImages = D3D11_REQ_SAMPLER_OBJECT_COUNT_PER_DEVICE;
		m_properties.limits.maxDescriptorSetStorageImages = 1048576u;
		m_properties.limits.maxDescriptorSetInputAttachments = 1048576u;
		m_properties.limits.maxVertexInputAttributes = D3D11_VS_INPUT_REGISTER_COUNT;
		m_properties.limits.maxVertexInputBindings = D3D11_VS_INPUT_REGISTER_COUNT;
		m_properties.limits.maxVertexInputAttributeOffset = 2047u;
		m_properties.limits.maxVertexInputBindingStride = 2048u;
		m_properties.limits.maxVertexOutputComponents = D3D11_VS_OUTPUT_REGISTER_COUNT;
		m_properties.limits.maxTessellationGenerationLevel = 64u;
		m_properties.limits.maxTessellationPatchSize = D3D11_HS_OUTPUT_PATCH_CONSTANT_REGISTER_COUNT;
		m_properties.limits.maxTessellationControlPerVertexInputComponents = D3D11_HS_CONTROL_POINT_REGISTER_COMPONENT_BIT_COUNT;
		m_properties.limits.maxTessellationControlPerVertexOutputComponents = D3D11_HS_CONTROL_POINT_PHASE_OUTPUT_REGISTER_COUNT;
		m_properties.limits.maxTessellationControlPerPatchOutputComponents = D3D11_HS_OUTPUT_PATCH_CONSTANT_REGISTER_COUNT;
		m_properties.limits.maxTessellationControlTotalOutputComponents = 4216u;
		m_properties.limits.maxTessellationEvaluationInputComponents = D3D11_DS_INPUT_CONTROL_POINT_REGISTER_COUNT;
		m_properties.limits.maxTessellationEvaluationOutputComponents = D3D11_DS_OUTPUT_REGISTER_COUNT;
		m_properties.limits.maxGeometryShaderInvocations = 32u;
		m_properties.limits.maxGeometryInputComponents = D3D11_GS_INPUT_REGISTER_COUNT;
		m_properties.limits.maxGeometryOutputComponents = D3D11_GS_OUTPUT_REGISTER_COUNT;
		m_properties.limits.maxGeometryOutputVertices = 1024u;
		m_properties.limits.maxGeometryTotalOutputComponents = 1024u;
		m_properties.limits.maxFragmentInputComponents = D3D11_PS_INPUT_REGISTER_COUNT;
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
		m_properties.limits.maxDrawIndexedIndexValue = ( 1ull << D3D11_REQ_DRAWINDEXED_INDEX_COUNT_2_TO_EXP ) - 1u;
		m_properties.limits.maxDrawIndirectCount = 4294967295u;
		m_properties.limits.maxSamplerLodBias = 15.0f;
		m_properties.limits.maxSamplerAnisotropy = D3D11_REQ_MAXANISOTROPY - 0.01f;
		m_properties.limits.maxViewports = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
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
		m_properties.limits.sampledImageColorSampleCounts = D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT;
		m_properties.limits.sampledImageIntegerSampleCounts = D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT;
		m_properties.limits.sampledImageDepthSampleCounts = D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT;
		m_properties.limits.sampledImageStencilSampleCounts = D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT;
		m_properties.limits.storageImageSampleCounts = 15u;
		m_properties.limits.maxSampleMaskWords = 1u;
		m_properties.limits.timestampComputeAndGraphics = true;
		m_properties.limits.timestampPeriod = 1.0f;
		m_properties.limits.maxClipDistances = D3D11_CLIP_OR_CULL_DISTANCE_COUNT;
		m_properties.limits.maxCullDistances = D3D11_CLIP_OR_CULL_DISTANCE_COUNT;
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
		m_properties.limits.nonCoherentAtomSize = 64u;

		m_properties.sparseProperties.residencyAlignedMipSize = false;
		m_properties.sparseProperties.residencyNonResidentStrict = false;
		m_properties.sparseProperties.residencyStandard2DBlockShape = false;
		m_properties.sparseProperties.residencyStandard2DMultisampleBlockShape = false;
		m_properties.sparseProperties.residencyStandard3DBlockShape = false;


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

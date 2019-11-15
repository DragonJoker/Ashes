/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/D3D11PhysicalDevice.hpp"

#include "Core/D3D11Instance.hpp"
#include "Core/D3D11Device.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
{
	PhysicalDevice::PhysicalDevice( VkInstance instance
		, AdapterInfo adapterInfo )
		: m_instance{ instance }
		, m_adapterInfo{ std::move( adapterInfo ) }
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
		VkExtensionPropertiesArray result;
		return result;
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

	void PhysicalDevice::doInitialise()
	{
		DXGI_ADAPTER_DESC2 adapterDesc;

		if ( m_adapterInfo.adapter2
			&& SUCCEEDED( m_adapterInfo.adapter2->GetDesc2( &adapterDesc ) ) )
		{
			strncpy( m_properties.deviceName
				, toString( adapterDesc.Description ).c_str()
				, sizeof( m_properties.deviceName ) - 1u );
			m_properties.deviceID = adapterDesc.DeviceId;
			m_properties.vendorID = adapterDesc.VendorId;
			m_properties.driverVersion = adapterDesc.Revision;
			// Store the dedicated video card memory in megabytes.
			auto videoCardMemory = int( adapterDesc.DedicatedVideoMemory / 1024 / 1024 );
		}

		uint32_t major = m_adapterInfo.featureLevel >> 12;
		uint32_t minor = ( m_adapterInfo.featureLevel >> 8 ) & 0x01;
		m_properties.apiVersion = ( major << 22 ) | ( minor << 12 );
		m_properties.deviceType = VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;

		m_features.robustBufferAccess = true;
		m_features.fullDrawIndexUint32 = true;
		m_features.imageCubeArray = true;
		m_features.independentBlend = true;
		m_features.geometryShader = m_adapterInfo.featureLevel >= D3D_FEATURE_LEVEL_10_0;
		m_features.tessellationShader = m_adapterInfo.featureLevel >= D3D_FEATURE_LEVEL_11_0;
		m_features.sampleRateShading = true;
		m_features.dualSrcBlend = true;
		m_features.logicOp = true;
		m_features.multiDrawIndirect = false;
		m_features.drawIndirectFirstInstance = true;
		m_features.depthClamp = true;
		m_features.depthBiasClamp = true;
		m_features.fillModeNonSolid = true;
		m_features.depthBounds = true;
		m_features.wideLines = false;
		m_features.largePoints = false;
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
		m_features.shaderTessellationAndGeometryPointSize = m_adapterInfo.featureLevel >= D3D_FEATURE_LEVEL_11_0;
		m_features.shaderImageGatherExtended = m_adapterInfo.featureLevel >= D3D_FEATURE_LEVEL_11_0;
		m_features.shaderStorageImageExtendedFormats = m_adapterInfo.featureLevel >= D3D_FEATURE_LEVEL_11_0;
		m_features.shaderStorageImageMultisample = m_adapterInfo.featureLevel >= D3D_FEATURE_LEVEL_11_0;
		m_features.shaderStorageImageReadWithoutFormat = m_adapterInfo.featureLevel >= D3D_FEATURE_LEVEL_11_0;
		m_features.shaderStorageImageWriteWithoutFormat = m_adapterInfo.featureLevel >= D3D_FEATURE_LEVEL_11_0;
		m_features.shaderUniformBufferArrayDynamicIndexing = true;
		m_features.shaderSampledImageArrayDynamicIndexing = true;
		m_features.shaderStorageBufferArrayDynamicIndexing = true;
		m_features.shaderStorageImageArrayDynamicIndexing = true;
		m_features.shaderClipDistance = true;
		m_features.shaderCullDistance = true;
		m_features.shaderFloat64 = false;
		m_features.shaderInt64 = false;
		m_features.shaderInt16 = false;
		m_features.shaderResourceResidency = false;
		m_features.shaderResourceMinLod = true;
		m_features.sparseBinding = false;
		m_features.sparseResidencyBuffer = false;
		m_features.sparseResidencyImage2D = false;
		m_features.sparseResidencyImage3D = false;
		m_features.sparseResidency2Samples = false;
		m_features.sparseResidency4Samples = false;
		m_features.sparseResidency8Samples = false;
		m_features.sparseResidency16Samples = false;
		m_features.sparseResidencyAliased = false;
		m_features.variableMultisampleRate = false;
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
		m_properties.limits.maxPerStageDescriptorSamplers = D3D11_COMMONSHADER_SAMPLER_REGISTER_COUNT;
		m_properties.limits.maxPerStageDescriptorUniformBuffers = D3D11_COMMONSHADER_CONSTANT_BUFFER_HW_SLOT_COUNT;
		m_properties.limits.maxPerStageDescriptorStorageBuffers = D3D11_1_UAV_SLOT_COUNT;
		m_properties.limits.maxPerStageDescriptorSampledImages = D3D11_COMMONSHADER_SAMPLER_REGISTER_COUNT;
		m_properties.limits.maxPerStageDescriptorStorageImages = 1048576u;
		m_properties.limits.maxPerStageDescriptorInputAttachments = 1048576u;
		m_properties.limits.maxPerStageResources = 4294967295u;
		m_properties.limits.maxDescriptorSetSamplers = D3D11_COMMONSHADER_SAMPLER_REGISTER_COUNT;
		m_properties.limits.maxDescriptorSetUniformBuffers = D3D11_COMMONSHADER_CONSTANT_BUFFER_HW_SLOT_COUNT;
		m_properties.limits.maxDescriptorSetUniformBuffersDynamic = D3D11_COMMONSHADER_CONSTANT_BUFFER_HW_SLOT_COUNT;
		m_properties.limits.maxDescriptorSetStorageBuffers = D3D11_1_UAV_SLOT_COUNT;
		m_properties.limits.maxDescriptorSetStorageBuffersDynamic = D3D11_1_UAV_SLOT_COUNT;
		m_properties.limits.maxDescriptorSetSampledImages = D3D11_COMMONSHADER_SAMPLER_REGISTER_COUNT;
		m_properties.limits.maxDescriptorSetStorageImages = 1048576u;
		m_properties.limits.maxDescriptorSetInputAttachments = 1048576u;
		m_properties.limits.maxVertexInputAttributes = D3D11_VS_INPUT_REGISTER_COUNT;
		m_properties.limits.maxVertexInputBindings = D3D11_VS_INPUT_REGISTER_COUNT;
		m_properties.limits.maxVertexInputAttributeOffset = 2047u;
		m_properties.limits.maxVertexInputBindingStride = 2048u;
		m_properties.limits.maxVertexOutputComponents = D3D11_VS_OUTPUT_REGISTER_COUNT;
		m_properties.limits.maxTessellationGenerationLevel = D3D11_TESSELLATOR_MAX_TESSELLATION_FACTOR;
		m_properties.limits.maxTessellationPatchSize = D3D11_HS_OUTPUT_PATCH_CONSTANT_REGISTER_COUNT;
		m_properties.limits.maxTessellationControlPerVertexInputComponents = D3D11_HS_CONTROL_POINT_REGISTER_COMPONENT_BIT_COUNT;
		m_properties.limits.maxTessellationControlPerVertexOutputComponents = D3D11_HS_CONTROL_POINT_PHASE_OUTPUT_REGISTER_COUNT;
		m_properties.limits.maxTessellationControlPerPatchOutputComponents = D3D11_HS_OUTPUT_PATCH_CONSTANT_REGISTER_COUNT;
		m_properties.limits.maxTessellationControlTotalOutputComponents = D3D11_HS_OUTPUT_CONTROL_POINTS_MAX_TOTAL_SCALARS;
		m_properties.limits.maxTessellationEvaluationInputComponents = D3D11_DS_INPUT_CONTROL_POINT_REGISTER_COUNT;
		m_properties.limits.maxTessellationEvaluationOutputComponents = D3D11_DS_OUTPUT_REGISTER_COUNT;
		m_properties.limits.maxGeometryShaderInvocations = D3D11_GS_MAX_INSTANCE_COUNT;
		m_properties.limits.maxGeometryInputComponents = D3D11_GS_INPUT_REGISTER_COUNT;
		m_properties.limits.maxGeometryOutputComponents = D3D11_GS_OUTPUT_REGISTER_COUNT;
		m_properties.limits.maxGeometryOutputVertices = D3D11_GS_MAX_OUTPUT_VERTEX_COUNT_ACROSS_INSTANCES;
		m_properties.limits.maxGeometryTotalOutputComponents = D3D11_GS_MAX_OUTPUT_VERTEX_COUNT_ACROSS_INSTANCES;
		m_properties.limits.maxFragmentInputComponents = D3D11_PS_INPUT_REGISTER_COUNT;
		m_properties.limits.maxFragmentOutputAttachments = D3D11_PS_OUTPUT_REGISTER_COUNT;
		m_properties.limits.maxFragmentDualSrcAttachments = 1u;
		m_properties.limits.maxFragmentCombinedOutputResources = 16u;
		m_properties.limits.maxComputeSharedMemorySize = 49152u;
		m_properties.limits.maxComputeWorkGroupCount[0] = D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION;
		m_properties.limits.maxComputeWorkGroupCount[1] = D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION;
		m_properties.limits.maxComputeWorkGroupCount[2] = D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION;
		m_properties.limits.maxComputeWorkGroupInvocations = 1536u;
		m_properties.limits.maxComputeWorkGroupSize[0] = D3D11_CS_THREAD_GROUP_MAX_X;
		m_properties.limits.maxComputeWorkGroupSize[1] = D3D11_CS_THREAD_GROUP_MAX_Y;
		m_properties.limits.maxComputeWorkGroupSize[2] = D3D11_CS_THREAD_GROUP_MAX_Z;
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
		m_properties.limits.viewportBoundsRange[0] = D3D11_VIEWPORT_BOUNDS_MIN;
		m_properties.limits.viewportBoundsRange[1] = D3D11_VIEWPORT_BOUNDS_MAX;
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

		std::vector< D3D_FEATURE_LEVEL > requestedFeatureLevels
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1,
		};
		HRESULT hr;
		UINT flags = 0;

#if !defined( NDEBUG )
		flags = D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_FEATURE_LEVEL supportedFeatureLevel = getFeatureLevel();
		D3D_FEATURE_LEVEL featureLevel;
		ID3D11Device * d3dDevice;
		hr = D3D11CreateDevice( nullptr
			, D3D_DRIVER_TYPE_HARDWARE
			, nullptr
			, flags
			, &supportedFeatureLevel
			, 1
			, D3D11_SDK_VERSION
			, &d3dDevice
			, &featureLevel
			, nullptr );

		if ( d3dDevice )
		{
			auto fillProps = [&d3dDevice]( VkFormat fmt
				, VkFormatProperties & props
				, DXGI_FORMAT ( *convertFmt )( VkFormat const & ) )
			{
				auto dxgi = convertFmt( fmt );

				if ( dxgi != DXGI_FORMAT_UNKNOWN )
				{
					UINT support;
					d3dDevice->CheckFormatSupport( dxgi, &support );

					if ( checkFlag( support, D3D11_FORMAT_SUPPORT_IA_VERTEX_BUFFER ) )
					{
						props.bufferFeatures |= VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT;
						props.bufferFeatures |= VK_FORMAT_FEATURE_BLIT_SRC_BIT;
						props.bufferFeatures |= VK_FORMAT_FEATURE_BLIT_DST_BIT;
					}

					if ( checkFlag( support, D3D11_FORMAT_SUPPORT_DEPTH_STENCIL ) )
					{
						props.optimalTilingFeatures |= VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;
						props.optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_SRC_BIT;
						props.optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_DST_BIT;
					}

					if ( checkFlag( support, D3D11_FORMAT_SUPPORT_RENDER_TARGET ) )
					{
						props.optimalTilingFeatures |= VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT;
						props.optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_SRC_BIT;
						props.optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_DST_BIT;
					}

					if ( checkFlag( support, D3D11_FORMAT_SUPPORT_SHADER_LOAD ) )
					{
						props.optimalTilingFeatures |= VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT;
						props.optimalTilingFeatures |= VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT;
						props.optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_SRC_BIT;
						props.optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_DST_BIT;
					}

					if ( checkFlag( support, D3D11_FORMAT_SUPPORT_SHADER_SAMPLE ) )
					{
						props.optimalTilingFeatures |= VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT;
						props.optimalTilingFeatures |= VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT;
						props.optimalTilingFeatures |= VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_MINMAX_BIT_EXT;
						props.optimalTilingFeatures |= VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_CUBIC_BIT_EXT;
						props.optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_SRC_BIT;
						props.optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_DST_BIT;
					}

					if ( checkFlag( support, D3D11_FORMAT_SUPPORT_CPU_LOCKABLE ) )
					{
						props.optimalTilingFeatures |= VK_FORMAT_FEATURE_TRANSFER_SRC_BIT;
						props.optimalTilingFeatures |= VK_FORMAT_FEATURE_TRANSFER_DST_BIT;
						props.optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_SRC_BIT;
						props.optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_DST_BIT;
					}
				}
			};

			for ( VkFormat fmt = VkFormat( VK_FORMAT_BEGIN_RANGE + 1 ); fmt != VK_FORMAT_END_RANGE; fmt = VkFormat( fmt + 1 ) )
			{
				VkFormatProperties props{};
				fillProps( fmt, props, getDxgiFormat );
				fillProps( fmt, props, getTextureFormat );
				fillProps( fmt, props, getSRVFormat );
				fillProps( fmt, props, getRTVFormat );
				fillProps( fmt, props, getUAVFormat );
				fillProps( fmt, props, getBufferFormat );
				props.linearTilingFeatures = props.optimalTilingFeatures;
				m_formatProperties[fmt] = props;
			}

			safeRelease( d3dDevice );

#if VK_VERSION_1_1

			m_features2.pNext = nullptr;
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

			m_features2.pNext = nullptr;
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
}

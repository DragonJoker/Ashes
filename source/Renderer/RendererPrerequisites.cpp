#include "RendererPrerequisites.hpp"

namespace renderer
{
	VkFormat convert( PixelFormat format )
	{
		switch ( format )
		{
		case PixelFormat::eL8:
			return VK_FORMAT_R8_UINT;

		case PixelFormat::eL8A8:
			return VK_FORMAT_R8G8_UINT;

		case PixelFormat::eR8G8B8:
			return VK_FORMAT_R8G8B8_UINT;

		case PixelFormat::eRGB565:
			return VK_FORMAT_R5G6B5_UNORM_PACK16;

		case PixelFormat::eR8G8B8A8:
			return VK_FORMAT_R8G8B8A8_UNORM;

		case PixelFormat::eRGBA5551:
			return VK_FORMAT_R5G5B5A1_UNORM_PACK16;

		case PixelFormat::eRGBA4444:
			return VK_FORMAT_R4G4B4A4_UNORM_PACK16;

		case PixelFormat::eD16:
			return VK_FORMAT_D16_UNORM;

		case PixelFormat::eS8:
			return VK_FORMAT_S8_UINT;
		}

		assert( false && "Unsupported pixel format." );
		return VK_FORMAT_UNDEFINED;
	}

	VkFilter convert( Filter filter )
	{
		switch ( filter )
		{
		case Filter::eNearest:
			return VK_FILTER_NEAREST;

		case Filter::eLinear:
			return VK_FILTER_LINEAR;

		default:
			assert( false );
			return VK_FILTER_NEAREST;
		}
	}

	VkSamplerMipmapMode convert( MipmapMode mode )
	{
		switch ( mode )
		{
		case MipmapMode::eNearest:
			return VK_SAMPLER_MIPMAP_MODE_NEAREST;

		case MipmapMode::eLinear:
			return VK_SAMPLER_MIPMAP_MODE_LINEAR;

		default:
			assert( false );
			return VK_SAMPLER_MIPMAP_MODE_NEAREST;
		}
	}

	VkSamplerAddressMode convert( WrapMode mode )
	{
		switch ( mode )
		{
		case WrapMode::eRepeat:
			return VK_SAMPLER_ADDRESS_MODE_REPEAT;

		case WrapMode::eMirroredRepeat:
			return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;

		case WrapMode::eClampToEdge:
			return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;

		case WrapMode::eClampToBorder:
			return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;

		case WrapMode::eMirrorClampToEdge:
			return VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;

		default:
			assert( false );
			return VK_SAMPLER_ADDRESS_MODE_REPEAT;
		}
	}

	VkMemoryPropertyFlags convert( MemoryPropertyFlags flags )
	{
		VkMemoryPropertyFlags result{ 0 };

		if ( checkFlag( flags, MemoryPropertyFlag::eDeviceLocal ) )
		{
			result |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		}

		if ( checkFlag( flags, MemoryPropertyFlag::eHostCached ) )
		{
			result |= VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
		}

		if ( checkFlag( flags, MemoryPropertyFlag::eHostCoherent ) )
		{
			result |= VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		}

		if ( checkFlag( flags, MemoryPropertyFlag::eHostVisible ) )
		{
			result |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
		}

		if ( checkFlag( flags, MemoryPropertyFlag::eLazilyAllocated ) )
		{
			result |= VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT;
		}

		return result;
	}

	VkBufferUsageFlags convert( BufferTargets targets )
	{
		VkBufferUsageFlags result{ 0 };

		if ( checkFlag( targets, BufferTarget::eIndexBuffer ) )
		{
			result |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		}

		if ( checkFlag( targets, BufferTarget::eIndirectBuffer ) )
		{
			result |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
		}

		if ( checkFlag( targets, BufferTarget::eStorageBuffer ) )
		{
			result |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
		}

		if ( checkFlag( targets, BufferTarget::eStorageTexelBuffer ) )
		{
			result |= VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT;
		}

		if ( checkFlag( targets, BufferTarget::eTransferDst ) )
		{
			result |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		}

		if ( checkFlag( targets, BufferTarget::eTransferSrc ) )
		{
			result |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		}

		if ( checkFlag( targets, BufferTarget::eUniformBuffer ) )
		{
			result |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		}

		if ( checkFlag( targets, BufferTarget::eUniformTexelBuffer ) )
		{
			result |= VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;
		}

		if ( checkFlag( targets, BufferTarget::eVertexBuffer ) )
		{
			result |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		}

		return result;
	}

	VkMemoryMapFlags convert( MemoryMapFlags flags )
	{
		return VkMemoryMapFlags{ 0 };
	}

	VkPrimitiveTopology convert( PrimitiveTopology topology )
	{
		switch ( topology )
		{
		case PrimitiveTopology::ePointList:
			return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;

		case PrimitiveTopology::eLineList:
			return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;

		case PrimitiveTopology::eLineStrip:
			return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;

		case PrimitiveTopology::eTriangleList:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

		case PrimitiveTopology::eTriangleStrip:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;

		case PrimitiveTopology::eTriangleFan:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;

		case PrimitiveTopology::eLineListWithAdjacency:
			return VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY;

		case PrimitiveTopology::eLineStripWithAdjacency:
			return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY;

		case PrimitiveTopology::eTriangleListWithAdjacency:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY;

		case PrimitiveTopology::eTriangleStripWithAdjacency:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY;

		case PrimitiveTopology::ePatchList:
			return VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;

		default:
			assert( false );
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		}
	}

	VkDescriptorType convert( DescriptorType type )
	{
		switch ( type )
		{
		case DescriptorType::eSampler:
			return VK_DESCRIPTOR_TYPE_SAMPLER;

		case DescriptorType::eCombinedImageSampler:
			return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

		case DescriptorType::eSampledImage:
			return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;

		case DescriptorType::eStorageImage:
			return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;

		case DescriptorType::eUniformTexelBuffer:
			return VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;

		case DescriptorType::eStorageTexelBuffer:
			return VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;

		case DescriptorType::eUniformBuffer:
			return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

		case DescriptorType::eStorageBuffer:
			return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;

		case DescriptorType::eUniformBufferDynamic:
			return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;

		case DescriptorType::eStorageBufferDynamic:
			return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;

		case DescriptorType::eInputAttachment:
			return VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;

		default:
			assert( false );
			return VK_DESCRIPTOR_TYPE_SAMPLER;
		}
	}

	VkShaderStageFlags convert( ShaderStageFlags flags )
	{
		VkShaderStageFlags result{ 0 };

		if ( checkFlag( flags, ShaderStageFlag::eVertex ) )
		{
			result |= VK_SHADER_STAGE_VERTEX_BIT;
		}

		if ( checkFlag( flags, ShaderStageFlag::eTessellationControl ) )
		{
			result |= VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		}

		if ( checkFlag( flags, ShaderStageFlag::eTessellationEvaluation ) )
		{
			result |= VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
		}

		if ( checkFlag( flags, ShaderStageFlag::eGeometry ) )
		{
			result |= VK_SHADER_STAGE_GEOMETRY_BIT;
		}

		if ( checkFlag( flags, ShaderStageFlag::eFragment ) )
		{
			result |= VK_SHADER_STAGE_FRAGMENT_BIT;
		}

		if ( checkFlag( flags, ShaderStageFlag::eCompute ) )
		{
			result |= VK_SHADER_STAGE_COMPUTE_BIT;
		}

		return result;
	}

	VkShaderStageFlagBits convert( ShaderStageFlag flag )
	{
		VkShaderStageFlags result{ 0 };

		switch ( flag )
		{
		case ShaderStageFlag::eVertex:
			return VK_SHADER_STAGE_VERTEX_BIT;

		case ShaderStageFlag::eTessellationControl:
			return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;

		case ShaderStageFlag::eTessellationEvaluation:
			return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;

		case ShaderStageFlag::eGeometry:
			return VK_SHADER_STAGE_GEOMETRY_BIT;

		case ShaderStageFlag::eFragment:
			return VK_SHADER_STAGE_FRAGMENT_BIT;

		case ShaderStageFlag::eCompute:
			return VK_SHADER_STAGE_COMPUTE_BIT;

		default:
			assert( false );
			return VK_SHADER_STAGE_ALL;
		}
	}

	VkCommandBufferUsageFlags convert( CommandBufferUsageFlags flags )
	{
		VkCommandBufferUsageFlags result{ 0 };

		if ( checkFlag( flags, CommandBufferUsageFlag::eOneTimeSubmit ) )
		{
			result |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		}

		if ( checkFlag( flags, CommandBufferUsageFlag::eRenderPassContinue ) )
		{
			result |= VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
		}

		if ( checkFlag( flags, CommandBufferUsageFlag::eSimultaneousUse ) )
		{
			result |= VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		}

		return result;
	}

	uint32_t pixelSize( PixelFormat format )noexcept
	{
		switch ( format )
		{
		case PixelFormat::eL8:
		case PixelFormat::eS8:
			return 1;
			break;

		case PixelFormat::eL8A8:
		case PixelFormat::eRGB565:
		case PixelFormat::eRGBA5551:
		case PixelFormat::eRGBA4444:
		case PixelFormat::eD16:
			return 2;
			break;

		case PixelFormat::eR8G8B8:
			return 3;
			break;

		case PixelFormat::eR8G8B8A8:
			return 4;
			break;

		default:
			assert( false && "Unsupported pixel format" );
			return 0;
		}
	}

	VkCommandBufferResetFlags convert( CommandBufferResetFlags flags )
	{
		VkCommandBufferResetFlags result{ 0 };

		if ( checkFlag( flags, CommandBufferResetFlag::eReleaseResources ) )
		{
			result |= VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT;
		}

		return result;
	}

	VkPipelineStageFlags convert( PipelineStageFlags flags )
	{
		VkPipelineStageFlags result{ 0 };

		if ( checkFlag( flags, PipelineStageFlag::eTopOfPipe ) )
		{
			result |= VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		}

		if ( checkFlag( flags, PipelineStageFlag::eDrawIndirect ) )
		{
			result |= VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;
		}

		if ( checkFlag( flags, PipelineStageFlag::eVertexInput ) )
		{
			result |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
		}

		if ( checkFlag( flags, PipelineStageFlag::eVertexShader ) )
		{
			result |= VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
		}

		if ( checkFlag( flags, PipelineStageFlag::eTessellationControlShader ) )
		{
			result |= VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT;
		}

		if ( checkFlag( flags, PipelineStageFlag::eTessellationEvaluationShader ) )
		{
			result |= VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT;
		}

		if ( checkFlag( flags, PipelineStageFlag::eGeometryShader ) )
		{
			result |= VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT;
		}

		if ( checkFlag( flags, PipelineStageFlag::eFragmentShader ) )
		{
			result |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}

		if ( checkFlag( flags, PipelineStageFlag::eEarlyFragmentTests ) )
		{
			result |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}

		if ( checkFlag( flags, PipelineStageFlag::eLateFragmentTests ) )
		{
			result |= VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		}

		if ( checkFlag( flags, PipelineStageFlag::eColourAttachmentOutput ) )
		{
			result |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		}

		if ( checkFlag( flags, PipelineStageFlag::eComputeShader ) )
		{
			result |= VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
		}

		if ( checkFlag( flags, PipelineStageFlag::eTransfer ) )
		{
			result |= VK_PIPELINE_STAGE_TRANSFER_BIT;
		}

		if ( checkFlag( flags, PipelineStageFlag::eBottomOfPipe ) )
		{
			result |= VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		}

		if ( checkFlag( flags, PipelineStageFlag::eHost ) )
		{
			result |= VK_PIPELINE_STAGE_HOST_BIT;
		}

		if ( checkFlag( flags, PipelineStageFlag::eAllGraphics ) )
		{
			result |= VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
		}

		if ( checkFlag( flags, PipelineStageFlag::eAllCommands ) )
		{
			result |= VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
		}

		return result;
	}

	VkPipelineBindPoint convert( PipelineBindPoint point )
	{
		switch ( point )
		{
		case PipelineBindPoint::eGraphics:
			return VK_PIPELINE_BIND_POINT_GRAPHICS;

		case PipelineBindPoint::eCompute:
			return VK_PIPELINE_BIND_POINT_COMPUTE;

		default:
			assert( false && "Unsupported bind point" );
			return VkPipelineBindPoint( 0 );
		}
	}

	VkAccessFlags convert( AccessFlags flags )
	{
		VkAccessFlags result{ 0 };

		if ( checkFlag( flags, AccessFlag::eIndirectCommandRead ) )
		{
			result |= VK_ACCESS_INDIRECT_COMMAND_READ_BIT;
		}

		if ( checkFlag( flags, AccessFlag::eIndexRead ) )
		{
			result |= VK_ACCESS_INDEX_READ_BIT;
		}

		if ( checkFlag( flags, AccessFlag::eVertexAttributeRead ) )
		{
			result |= VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
		}

		if ( checkFlag( flags, AccessFlag::eUniformRead ) )
		{
			result |= VK_ACCESS_UNIFORM_READ_BIT;
		}

		if ( checkFlag( flags, AccessFlag::eInputAttachmentRead ) )
		{
			result |= VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
		}

		if ( checkFlag( flags, AccessFlag::eShaderRead ) )
		{
			result |= VK_ACCESS_SHADER_READ_BIT;
		}

		if ( checkFlag( flags, AccessFlag::eShaderWrite ) )
		{
			result |= VK_ACCESS_SHADER_WRITE_BIT;
		}

		if ( checkFlag( flags, AccessFlag::eColourAttachmentRead ) )
		{
			result |= VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
		}

		if ( checkFlag( flags, AccessFlag::eColourAttachmentWrite ) )
		{
			result |= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		}

		if ( checkFlag( flags, AccessFlag::eDepthStencilAttachmentRead ) )
		{
			result |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
		}

		if ( checkFlag( flags, AccessFlag::eDepthStencilAttachmentWrite ) )
		{
			result |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		}

		if ( checkFlag( flags, AccessFlag::eTransferRead ) )
		{
			result |= VK_ACCESS_TRANSFER_READ_BIT;
		}

		if ( checkFlag( flags, AccessFlag::eTransferWrite ) )
		{
			result |= VK_ACCESS_TRANSFER_WRITE_BIT;
		}

		if ( checkFlag( flags, AccessFlag::eHostRead ) )
		{
			result |= VK_ACCESS_HOST_READ_BIT;
		}

		if ( checkFlag( flags, AccessFlag::eHostWrite ) )
		{
			result |= VK_ACCESS_HOST_WRITE_BIT;
		}

		if ( checkFlag( flags, AccessFlag::eMemoryRead ) )
		{
			result |= VK_ACCESS_MEMORY_READ_BIT;
		}

		if ( checkFlag( flags, AccessFlag::eMemoryWrite ) )
		{
			result |= VK_ACCESS_MEMORY_WRITE_BIT;
		}

		return result;
	}

	VkImageLayout convert( ImageLayout layout )
	{
		switch ( layout )
		{
		case ImageLayout::eUndefined:
			return VK_IMAGE_LAYOUT_UNDEFINED;

		case ImageLayout::eGeneral:
			return VK_IMAGE_LAYOUT_GENERAL;

		case ImageLayout::eColourAttachmentOptimal:
			return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		case ImageLayout::eDepthStencilAttachmentOptimal:
			return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		case ImageLayout::eDepthStencilReadOnlyOptimal:
			return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

		case ImageLayout::eShaderReadOnlyOptimal:
			return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		case ImageLayout::eTransferSrcOptimal:
			return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;

		case ImageLayout::eTransferDstOptimal:
			return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

		case ImageLayout::ePreinitialised:
			return VK_IMAGE_LAYOUT_PREINITIALIZED;

		case ImageLayout::ePresentSrc:
			return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		default:
			assert( false && "Unsupported image layout" );
			return VK_IMAGE_LAYOUT_UNDEFINED;
		}
	}

	VkImageAspectFlags convert( ImageAspectFlags flags )
	{
		VkImageAspectFlags result{ 0 };

		if ( checkFlag( flags, ImageAspectFlag::eColour ) )
		{
			result |= VK_IMAGE_ASPECT_COLOR_BIT;
		}

		if ( checkFlag( flags, ImageAspectFlag::eDepth ) )
		{
			result |= VK_IMAGE_ASPECT_DEPTH_BIT;
		}

		if ( checkFlag( flags, ImageAspectFlag::eStencil ) )
		{
			result |= VK_IMAGE_ASPECT_STENCIL_BIT;
		}

		if ( checkFlag( flags, ImageAspectFlag::eMetaData ) )
		{
			result |= VK_IMAGE_ASPECT_METADATA_BIT;
		}

		return result;
	}
}

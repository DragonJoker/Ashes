/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11BlitImageCommand.hpp"
#include "Command/Commands/D3D11BindDescriptorSetCommand.hpp"

#include "Command/D3D11CommandBuffer.hpp"
#include "Core/D3D11Device.hpp"
#include "Descriptor/D3D11DescriptorSetLayout.hpp"
#include "Image/D3D11Image.hpp"
#include "Image/D3D11ImageView.hpp"
#include "Pipeline/D3D11Pipeline.hpp"
#include "Pipeline/D3D11PipelineLayout.hpp"
#include "RenderPass/D3D11FrameBuffer.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
{
	//*********************************************************************************************
	
	namespace
	{
		std::string getImgFmtName( VkFormat fmt )
		{
			switch ( fmt )
			{
			case VK_FORMAT_R8_UNORM:
			case VK_FORMAT_R8_SNORM:
			case VK_FORMAT_R8_USCALED:
			case VK_FORMAT_R8_SSCALED:
			case VK_FORMAT_R8_UINT:
			case VK_FORMAT_R8_SINT:
			case VK_FORMAT_R8_SRGB:
			case VK_FORMAT_R16_UNORM:
			case VK_FORMAT_R16_SNORM:
			case VK_FORMAT_R16_USCALED:
			case VK_FORMAT_R16_SSCALED:
			case VK_FORMAT_R16_UINT:
			case VK_FORMAT_R16_SINT:
			case VK_FORMAT_R16_SFLOAT:
			case VK_FORMAT_R32_UINT:
			case VK_FORMAT_R32_SINT:
			case VK_FORMAT_R32_SFLOAT:
			case VK_FORMAT_R64_UINT:
			case VK_FORMAT_R64_SINT:
			case VK_FORMAT_R64_SFLOAT:
			case VK_FORMAT_D16_UNORM:
			case VK_FORMAT_D32_SFLOAT:
			case VK_FORMAT_S8_UINT:
			case VK_FORMAT_D16_UNORM_S8_UINT:
			case VK_FORMAT_R10X6_UNORM_PACK16:
			case VK_FORMAT_R12X4_UNORM_PACK16:
				return "float";

			case VK_FORMAT_R4G4_UNORM_PACK8:
			case VK_FORMAT_R8G8_UNORM:
			case VK_FORMAT_R8G8_SNORM:
			case VK_FORMAT_R8G8_USCALED:
			case VK_FORMAT_R8G8_SSCALED:
			case VK_FORMAT_R8G8_UINT:
			case VK_FORMAT_R8G8_SINT:
			case VK_FORMAT_R8G8_SRGB:
			case VK_FORMAT_R16G16_UNORM:
			case VK_FORMAT_R16G16_SNORM:
			case VK_FORMAT_R16G16_USCALED:
			case VK_FORMAT_R16G16_SSCALED:
			case VK_FORMAT_R16G16_UINT:
			case VK_FORMAT_R16G16_SINT:
			case VK_FORMAT_R16G16_SFLOAT:
			case VK_FORMAT_R32G32_UINT:
			case VK_FORMAT_R32G32_SINT:
			case VK_FORMAT_R32G32_SFLOAT:
			case VK_FORMAT_R64G64_UINT:
			case VK_FORMAT_R64G64_SINT:
			case VK_FORMAT_R64G64_SFLOAT:
			case VK_FORMAT_EAC_R11_UNORM_BLOCK:
			case VK_FORMAT_EAC_R11_SNORM_BLOCK:
			case VK_FORMAT_EAC_R11G11_UNORM_BLOCK:
			case VK_FORMAT_EAC_R11G11_SNORM_BLOCK:
				return "float2";

			case VK_FORMAT_R5G6B5_UNORM_PACK16:
			case VK_FORMAT_B5G6R5_UNORM_PACK16:
			case VK_FORMAT_R8G8B8_UNORM:
			case VK_FORMAT_R8G8B8_SNORM:
			case VK_FORMAT_R8G8B8_USCALED:
			case VK_FORMAT_R8G8B8_SSCALED:
			case VK_FORMAT_R8G8B8_UINT:
			case VK_FORMAT_R8G8B8_SINT:
			case VK_FORMAT_R8G8B8_SRGB:
			case VK_FORMAT_B8G8R8_UNORM:
			case VK_FORMAT_B8G8R8_SNORM:
			case VK_FORMAT_B8G8R8_USCALED:
			case VK_FORMAT_B8G8R8_SSCALED:
			case VK_FORMAT_B8G8R8_UINT:
			case VK_FORMAT_B8G8R8_SINT:
			case VK_FORMAT_B8G8R8_SRGB:
			case VK_FORMAT_R16G16B16_UNORM:
			case VK_FORMAT_R16G16B16_SNORM:
			case VK_FORMAT_R16G16B16_USCALED:
			case VK_FORMAT_R16G16B16_SSCALED:
			case VK_FORMAT_R16G16B16_UINT:
			case VK_FORMAT_R16G16B16_SINT:
			case VK_FORMAT_R16G16B16_SFLOAT:
			case VK_FORMAT_R32G32B32_UINT:
			case VK_FORMAT_R32G32B32_SINT:
			case VK_FORMAT_R32G32B32_SFLOAT:
			case VK_FORMAT_R64G64B64_UINT:
			case VK_FORMAT_R64G64B64_SINT:
			case VK_FORMAT_R64G64B64_SFLOAT:
			case VK_FORMAT_X8_D24_UNORM_PACK32:
			case VK_FORMAT_B10G11R11_UFLOAT_PACK32:
			case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32:
			case VK_FORMAT_BC1_RGB_UNORM_BLOCK:
			case VK_FORMAT_BC1_RGB_SRGB_BLOCK:
			case VK_FORMAT_BC2_UNORM_BLOCK:
			case VK_FORMAT_BC2_SRGB_BLOCK:
			case VK_FORMAT_BC3_UNORM_BLOCK:
			case VK_FORMAT_BC3_SRGB_BLOCK:
			case VK_FORMAT_BC4_UNORM_BLOCK:
			case VK_FORMAT_BC4_SNORM_BLOCK:
			case VK_FORMAT_BC5_UNORM_BLOCK:
			case VK_FORMAT_BC5_SNORM_BLOCK:
			case VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK:
			case VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK:
			case VK_FORMAT_G8B8G8R8_422_UNORM:
			case VK_FORMAT_B8G8R8G8_422_UNORM:
			case VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM:
			case VK_FORMAT_G8_B8R8_2PLANE_420_UNORM:
			case VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM:
			case VK_FORMAT_G8_B8R8_2PLANE_422_UNORM:
			case VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM:
			case VK_FORMAT_R10X6G10X6_UNORM_2PACK16:
			case VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16:
			case VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16:
			case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16:
			case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16:
			case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16:
			case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16:
			case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16:
			case VK_FORMAT_R12X4G12X4_UNORM_2PACK16:
			case VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16:
			case VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16:
			case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16:
			case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16:
			case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16:
			case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16:
			case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16:
			case VK_FORMAT_G16B16G16R16_422_UNORM:
			case VK_FORMAT_B16G16R16G16_422_UNORM:
			case VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM:
			case VK_FORMAT_G16_B16R16_2PLANE_420_UNORM:
			case VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM:
			case VK_FORMAT_G16_B16R16_2PLANE_422_UNORM:
			case VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM:
				return "float3";

			default:
				return "float4";
			}
		}

		ashes::UInt32Array getBlitShaderCode( VkFormat src
			, VkFormat dst )
		{
			auto srcName = getImgFmtName( src );
			auto dstName = getImgFmtName( dst );
			std::string const hlsl =
				R"(Texture2D<)" + srcName + R"(> srcTexture : register(t0);
RWTexture2D<)" + dstName + R"(> dstTexture : register(u0);
SamplerState bilinearClamp : register(s0);

cbuffer CB : register(b0)
{
	float4 srcBox;   // 1.0 / source float4( offset, dimension )
	float4 dstBox;   // destination float4( offset, dimension )
}

[numthreads( 8, 8, 1 )]
void main( uint3 threadID : SV_DispatchThreadID )
{
	//threadID is the thread ID * the values from numthreads above and in this case correspond to the pixels location in number of pixels.
	//As a result texcoords (in 0-1 range) will point at the center between the 4 pixels used for the mipmap.
	int2 dstTextcoords = int2( mad( threadID.xy, dstBox.zw, dstBox.xy ) );
	float2 srcTextcoords = mad( threadID.xy, srcBox.zw, srcBox.xy );

	//The samplers interpolation will mix the four pixel values to the new pixels color
	)" + srcName + R"( color =  srcTexture.SampleLevel( bilinearClamp, srcTextcoords, 0 );

	//Write the final color into the destination texture.
	dstTexture[dstTextcoords] = )" + dstName + R"(( color );
})";
			auto size = hlsl.size() + 1u;
			UInt32Array result;
			result.resize( size_t( std::ceil( float( size ) / sizeof( uint32_t ) ) ) );
			std::memcpy( result.data(), hlsl.data(), hlsl.size() );
			return result;
		}

		VkDescriptorSetLayout doCreateBlitDescriptorLayout( Device const & device )
		{
			VkDescriptorSetLayoutBindingArray bindings
			{
				{ 0u, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1u, VK_SHADER_STAGE_COMPUTE_BIT, nullptr },
				{ 1u, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1u, VK_SHADER_STAGE_COMPUTE_BIT, nullptr },
				{ 2u, VK_DESCRIPTOR_TYPE_SAMPLER, 1u, VK_SHADER_STAGE_COMPUTE_BIT, nullptr },
				{ 3u, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1u, VK_SHADER_STAGE_COMPUTE_BIT, nullptr },
			};
			VkDescriptorSetLayout result;
			allocate( result
				, device.getAllocationCallbacks()
				, get( &device )
				, VkDescriptorSetLayoutCreateInfo
				{
					VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
					nullptr,
					0u,
					uint32_t( bindings.size() ),
					bindings.data()
				} );
			return result;
		}

		VkPipelineLayout doCreateBlitPipelineLayout( Device const & device
			, VkDescriptorSetLayout descriptorLayout )
		{
			VkPipelineLayout result;
			allocate( result
				, device.getAllocationCallbacks()
				, get( &device )
				, VkPipelineLayoutCreateInfo
				{
					VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
					nullptr,
					0u,
					1u,
					&descriptorLayout,
					0u,
					nullptr
				} );
			return result;
		}

		VkShaderModule doCreateShaderModule( Device const & device
			, VkFormat src
			, VkFormat dst
			, UInt32Array & spirv )
		{
			spirv = getBlitShaderCode( src, dst );
			VkShaderModule result;
			allocate( result
				, device.getAllocationCallbacks()
				, get( &device )
				, VkShaderModuleCreateInfo
				{
					VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
					nullptr,
					0u,
					spirv.size() * sizeof( uint32_t ),
					spirv.data()
				} );
			return result;
		}

		VkPipeline doCreateBlitPipeline( Device const & device
			, VkPipelineLayout pipelineLayout
			, VkShaderModule module )
		{
			VkPipeline result;
			allocate( result
				, device.getAllocationCallbacks()
				, get( &device )
				, VkComputePipelineCreateInfo
				{
					VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
					nullptr,
					0u,
					VkPipelineShaderStageCreateInfo
					{
						VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
						nullptr,
						0u,
						VK_SHADER_STAGE_COMPUTE_BIT,
						module,
						"main",
						nullptr,
					},
					pipelineLayout,
					VK_NULL_HANDLE,
					0u,
				} );
			return result;
		}

		VkSampler doCreateSampler( VkDevice device
			, VkFilter filter
			, uint32_t mipLevels )
		{
			VkSampler result{ VK_NULL_HANDLE };
			allocate( result
				, get( device )->getAllocationCallbacks()
				, device
				, VkSamplerCreateInfo
				{
					VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
					nullptr,
					0u,
					filter,
					filter,
					VK_SAMPLER_MIPMAP_MODE_LINEAR,
					VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
					VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
					VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
					0.0f,
					VK_FALSE,
					0.0f,
					VK_FALSE,
					VK_COMPARE_OP_ALWAYS,
					0.0f,
					float( mipLevels ),
					VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK,
					VK_FALSE
				} );
			return result;
		}

		VkBuffer doCreateUbo( VkDevice device
			, uint32_t count
			, VkDeviceMemory & memory
			, VkDeviceSize & range )
		{
			VkBuffer result{ VK_NULL_HANDLE };
			allocate( result
				, get( device )->getAllocationCallbacks()
				, device
				, VkBufferCreateInfo
				{
					VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
					nullptr,
					0u,
					count * 256u,
					VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
					VK_SHARING_MODE_EXCLUSIVE,
					0u,
					nullptr,
				} );

			if ( result != VK_NULL_HANDLE )
			{
				auto requirements = get( result )->getMemoryRequirements();
				range = requirements.alignment;
				uint32_t deduced = deduceMemoryType( requirements.memoryTypeBits
					, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT );

				allocate( memory
					, get( device )->getAllocationCallbacks()
					, device
					, VkMemoryAllocateInfo
					{
						VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
						nullptr,
						count * range,
						deduced,
					} );
				get( result )->bindMemory( memory, 0u );
			}

			return result;
		}

		VkDescriptorSet doCreateDescriptorSet( VkDevice device
			, VkDescriptorPool pool
			, VkDescriptorSetLayout descriptorLayout
			, VkBuffer ubo
			, VkDeviceSize offset
			, VkDeviceSize range
			, VkSampler sampler
			, VkImageView srcView
			, VkImageView dstView )
		{
			VkDescriptorSet result{ VK_NULL_HANDLE };
			allocateNA( result
				, device
				, pool
				, descriptorLayout );

			if ( result )
			{
				VkDescriptorBufferInfo bufferInfo
				{
					ubo,
					offset,
					range,
				};
				get( result )->update( VkWriteDescriptorSet
					{
						VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
						nullptr,
						result,
						3u,
						0u,
						1u,
						VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
						nullptr,
						&bufferInfo,
						nullptr
					} );
				VkDescriptorImageInfo srcSampler
				{
					sampler,
					VK_NULL_HANDLE,
					VK_IMAGE_LAYOUT_UNDEFINED,
				};
				get( result )->update( VkWriteDescriptorSet
					{
						VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
						nullptr,
						result,
						2u,
						0u,
						1u,
						VK_DESCRIPTOR_TYPE_SAMPLER,
						&srcSampler,
						nullptr,
						nullptr
					} );
				VkDescriptorImageInfo srcImageInfo
				{
					VK_NULL_HANDLE,
					srcView,
					VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
				};
				get( result )->update( VkWriteDescriptorSet
					{
						VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
						nullptr,
						result,
						0u,
						0u,
						1u,
						VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
						&srcImageInfo,
						nullptr,
						nullptr
					} );
				VkDescriptorImageInfo dstImageInfo
				{
					VK_NULL_HANDLE,
					dstView,
					VK_IMAGE_LAYOUT_GENERAL,
				};
				get( result )->update( VkWriteDescriptorSet
					{
						VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
						nullptr,
						result,
						1u,
						0u,
						1u,
						VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
						&dstImageInfo,
						nullptr,
						nullptr
					} );
			}

			return result;
		}

		VkImage doGetImage( VkDevice device
			, VkImage image
			, VkDeviceMemory & memory
			, VkImageUsageFlags requiredUsage )
		{
			VkImage result = image;
			auto createInfo = get( image )->getCreateInfo();

			if ( !checkFlag( createInfo.usage, requiredUsage ) )
			{
				createInfo.usage = requiredUsage;
				allocate( result
					, get( device )->getAllocationCallbacks()
					, device
					, std::move( createInfo ) );
				auto requirements = get( result )->getMemoryRequirements();
				auto index = deduceMemoryType( requirements.memoryTypeBits
					, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
				allocate( memory
					, get( device )->getAllocationCallbacks()
					, device
					, VkMemoryAllocateInfo
					{
						VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
						nullptr,
						requirements.size,
						index,
					} );
				get( result )->bindMemory( memory, 0u );
			}

			return result;
		}

		VkImage doGetSamplable( VkDevice device
			, VkImage image
			, VkDeviceMemory & memory )
		{
			return doGetImage( device
				, image
				, memory
				, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT );
		}

		VkImage doGetStorable( VkDevice device
			, VkImage image
			, VkDeviceMemory & memory )
		{
			return doGetImage( device
				, image
				, memory
				, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_STORAGE_BIT );
		}

		VkImageCopyArray doGetCopyInfos( VkImage source
			, uint32_t baseMipLevel
			, uint32_t levelCount )
		{
			VkImageCopyArray result;

			for ( uint32_t level = baseMipLevel; level < baseMipLevel + levelCount; ++level )
			{
				result.push_back( VkImageCopy
					{
						VkImageSubresourceLayers{ getAspectMask( get( source )->getFormat() ), level, 0u, 1u },
						VkOffset3D{ 0, 0, 0 },
						VkImageSubresourceLayers{ getAspectMask( get( source )->getFormat() ), level, 0u, 1u },
						VkOffset3D{ 0, 0, 0 },
						getSubresourceDimensions( get( source )->getDimensions(), level ),
					} );
			}

			return result;
		}

		bool areInSameGroup( DXGI_FORMAT src, DXGI_FORMAT dst )
		{
			return getDxgiFormatGroup( src ) == getDxgiFormatGroup( dst );
		}

		bool areBlitCompatible( VkFormat src, VkFormat dst )
		{
			if ( src == dst
				|| areInSameGroup( getDxgiFormat( src ), getDxgiFormat( dst ) ) )
			{
				return true;
			}

			switch ( src )
			{
			case VK_FORMAT_R32_UINT:
			case VK_FORMAT_R32_SINT:
				return dst == VK_FORMAT_E5B9G9R9_UFLOAT_PACK32;

			case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32:
				return dst == VK_FORMAT_R32_UINT
					|| dst == VK_FORMAT_R32_SINT;

			case VK_FORMAT_R16G16B16A16_UINT:
			case VK_FORMAT_R16G16B16A16_SINT:
			case VK_FORMAT_R32G32_UINT:
			case VK_FORMAT_R32G32_SINT:
				return dst == VK_FORMAT_BC1_RGB_SRGB_BLOCK
					|| dst == VK_FORMAT_BC1_RGB_UNORM_BLOCK
					|| dst == VK_FORMAT_BC4_UNORM_BLOCK
					|| dst == VK_FORMAT_BC4_SNORM_BLOCK;

			case VK_FORMAT_BC1_RGB_SRGB_BLOCK:
			case VK_FORMAT_BC1_RGB_UNORM_BLOCK:
			case VK_FORMAT_BC4_UNORM_BLOCK:
			case VK_FORMAT_BC4_SNORM_BLOCK:
				return dst == VK_FORMAT_R16G16B16A16_UINT
					|| dst == VK_FORMAT_R16G16B16A16_SINT
					|| dst == VK_FORMAT_R32G32_UINT
					|| dst == VK_FORMAT_R32G32_SINT;

			case VK_FORMAT_R32G32B32A32_UINT:
			case VK_FORMAT_R32G32B32A32_SINT:
				return dst == VK_FORMAT_BC2_UNORM_BLOCK
					|| dst == VK_FORMAT_BC2_SRGB_BLOCK
					|| dst == VK_FORMAT_BC3_UNORM_BLOCK
					|| dst == VK_FORMAT_BC3_SRGB_BLOCK
					|| dst == VK_FORMAT_BC5_UNORM_BLOCK
					|| dst == VK_FORMAT_BC5_SNORM_BLOCK;

			case VK_FORMAT_BC2_SRGB_BLOCK:
			case VK_FORMAT_BC2_UNORM_BLOCK:
			case VK_FORMAT_BC3_SRGB_BLOCK:
			case VK_FORMAT_BC3_UNORM_BLOCK:
			case VK_FORMAT_BC5_UNORM_BLOCK:
			case VK_FORMAT_BC5_SNORM_BLOCK:
				return dst == VK_FORMAT_R32G32B32A32_UINT
					|| dst == VK_FORMAT_R32G32B32A32_SINT;

			default:
				return false;
			}
		}

		VkImageCopy makeImageCopy( VkImageBlit const & region
			, int direction )
		{
			return VkImageCopy
			{
				( direction == 2u ? region.dstSubresource : region.srcSubresource ),
				region.srcOffsets[0],
				( direction == 1u ? region.srcSubresource : region.dstSubresource ),
				region.dstOffsets[0],
				VkExtent3D
				{
					uint32_t( direction == 1u ? region.srcOffsets[1].x : region.dstOffsets[1].x ),
					uint32_t( direction == 1u ? region.srcOffsets[1].y : region.dstOffsets[1].y ),
					uint32_t( direction == 1u ? region.srcOffsets[1].z : region.dstOffsets[1].z ),
				},
			};
		}
	}
	
	//*********************************************************************************************

	BlitPipeline::BlitPipeline( Device const & device
		, VkFormat src
		, VkFormat dst )
		: device{ get( &device ) }
		, descriptorLayout{ doCreateBlitDescriptorLayout( device ) }
		, pipelineLayout{ doCreateBlitPipelineLayout( device, descriptorLayout ) }
		, shader{ doCreateShaderModule( device, src, dst, spirv ) }
		, pipeline{ doCreateBlitPipeline( device, pipelineLayout, shader ) }
	{
	}

	BlitPipeline::~BlitPipeline()
	{
		deallocate( pipeline, get( device )->getAllocationCallbacks() );
		deallocate( shader, get( device )->getAllocationCallbacks() );
		deallocate( pipelineLayout, get( device )->getAllocationCallbacks() );
		deallocate( descriptorLayout, get( device )->getAllocationCallbacks() );
	}

	//*********************************************************************************************

	BlitImageCommand::Attachment::Attachment( VkDevice device
		, VkImageSubresourceLayers & subresource
		, VkImage image
		, uint32_t layer
		, bool dest )
		: device{ device }
		, image{ get( image )->getResource() }
		, subResourceIndex{ D3D11CalcSubresource( subresource.mipLevel
			, layer
			, 1u ) }
	{
		allocate( view
			, get( device )->getAllocationCallbacks()
			, device
			, VkImageViewCreateInfo
			{
				VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				nullptr,
				0u,
				image,
				VkImageViewType( get( image )->getType() ),
				get( image )->getFormat(),
				VkComponentMapping{},
				VkImageSubresourceRange{ getAspectMask( get( image )->getFormat() ), subresource.mipLevel, 1u, layer, 1u }
			} );
	}

	BlitImageCommand::Attachment::~Attachment()
	{
		deallocate( view, get( device )->getAllocationCallbacks() );
	}

	//*********************************************************************************************

	BlitImageCommand::LayerCopy::LayerCopy( VkDevice device
		, VkDescriptorPool pool
		, VkDescriptorSetLayout descriptorLayout
		, VkBuffer ubo
		, VkDeviceSize offset
		, VkDeviceSize range
		, VkSampler sampler
		, VkImageBlit blitRegion
		, VkImage srcImage
		, VkImage dstImage
		, uint32_t layer )
		: dstOffset{ blitRegion.dstOffsets[0] }
		, srcBox
		{
			UINT( blitRegion.srcOffsets[0].x ),
			UINT( blitRegion.srcOffsets[0].y ),
			UINT( blitRegion.srcOffsets[0].z ),
			UINT( blitRegion.srcOffsets[0].x + blitRegion.srcOffsets[1].x ),
			UINT( blitRegion.srcOffsets[0].y + blitRegion.srcOffsets[1].y ),
			UINT( blitRegion.srcOffsets[0].z + blitRegion.srcOffsets[1].z )
		}
		, dstBox
		{
			UINT( blitRegion.dstOffsets[0].x ),
			UINT( blitRegion.dstOffsets[0].y ),
			UINT( blitRegion.dstOffsets[0].z ),
			UINT( blitRegion.dstOffsets[0].x + blitRegion.dstOffsets[1].x ),
			UINT( blitRegion.dstOffsets[0].y + blitRegion.dstOffsets[1].y ),
			UINT( blitRegion.dstOffsets[0].z + blitRegion.dstOffsets[1].z )
		}
		, src{ device, blitRegion.srcSubresource, srcImage, layer, false }
		, dst{ device, blitRegion.dstSubresource, dstImage, layer, true }
		, set{ doCreateDescriptorSet( device
			, pool
			, descriptorLayout
			, ubo
			, offset
			, range
			, sampler
			, src.view
			, dst.view ) }
		, blitBox
		{
			{
				float( blitRegion.srcOffsets[0].x ) / get( srcImage )->getDimensions().width,
				float( blitRegion.srcOffsets[0].y ) / get( srcImage )->getDimensions().height,
				1.0f / blitRegion.srcOffsets[1].x,
				1.0f / blitRegion.srcOffsets[1].y,
			},
			{
				float( blitRegion.dstOffsets[0].x ),
				float( blitRegion.dstOffsets[0].y ),
				float( blitRegion.dstOffsets[1].x ) / blitRegion.srcOffsets[1].x,
				float( blitRegion.dstOffsets[1].y ) / blitRegion.srcOffsets[1].y,
			},
		}
	{
	}

	//*********************************************************************************************

	BlitImageCommand::BlitImageCommand( VkCommandPool pool
		, VkCommandBuffer cb
		, VkDevice device
		, BlitPipeline const & pipeline
		, VkImage srcImage
		, VkImage dstImage
		, ArrayView< VkImageBlit const > const & regions
		, VkFilter filter )
		: CommandBase{ device }
		, m_srcTexture{ srcImage }
		, m_dstTexture{ dstImage }
		, m_sampler{ doCreateSampler( device, filter, get( m_srcTexture )->getMipmapLevels() ) }
	{
		assert( get( m_srcTexture )->getLayerCount() == get( m_dstTexture )->getLayerCount() );

		uint32_t count = 0u;
		uint32_t srcMinLevel = std::numeric_limits< uint32_t >::max();
		uint32_t srcMaxLevel = std::numeric_limits< uint32_t >::lowest();
		uint32_t dstMinLevel = std::numeric_limits< uint32_t >::max();
		uint32_t dstMaxLevel = std::numeric_limits< uint32_t >::lowest();

		for ( auto & region : regions )
		{
			if ( region.dstOffsets[1] == region.srcOffsets[1]
				&& areBlitCompatible( get( srcImage )->getFormat(), get( dstImage )->getFormat() ) )
			{
				m_layerBlits.push_back( makeImageCopy( region, 0u ) );
			}
			else
			{
				m_stretchRegions.push_back( region );
				count += get( m_srcTexture )->getLayerCount();
				srcMinLevel = std::min( srcMinLevel, region.srcSubresource.mipLevel );
				srcMaxLevel = std::max( srcMaxLevel, region.srcSubresource.mipLevel );
				dstMinLevel = std::min( dstMinLevel, region.dstSubresource.mipLevel );
				dstMaxLevel = std::max( dstMaxLevel, region.dstSubresource.mipLevel );
			}

			m_layerBlitsToTmp.push_back( makeImageCopy( region, 1u ) );
			m_layerBlitsFromTmp.push_back( makeImageCopy( region, 2u ) );
		}

		doInitialiseStretches( pipeline.descriptorLayout, count );

		doCreateCommandBuffer( pool
			, pipeline.pipeline
			, pipeline.pipelineLayout
			, srcMinLevel
			, srcMaxLevel
			, dstMinLevel
			, dstMaxLevel );
		get( cb )->executeCommands( makeArrayView( const_cast< VkCommandBuffer const * >( &m_commandBuffer ), 1u ) );
	}

	void BlitImageCommand::doInitialiseStretchUbo( VkDescriptorSetLayout descriptorLayout
		, uint32_t count )
	{
		VkDeviceSize range = 0u;
		m_ubo = doCreateUbo( m_device, count, m_uboMemory, range );

		ashes::VkDescriptorPoolSizeArray poolSizes
		{
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, count },
			{ VK_DESCRIPTOR_TYPE_SAMPLER, count * 3u },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, count },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, count },
		};

		allocate( m_pool
			, get( m_device )->getAllocationCallbacks()
			, m_device
			, VkDescriptorPoolCreateInfo
			{
				VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
				nullptr,
				VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
				count,
				uint32_t( poolSizes.size() ),
				poolSizes.data()
			} );

		VkDeviceSize offset = 0u;
		ashes::ByteArray data;
		data.resize( range * count );
		auto buffer = data.data();

		for ( auto & region : m_stretchRegions )
		{
			for ( uint32_t layer = 0u; layer < get( m_tmpSrcTexture )->getLayerCount(); ++layer )
			{
				m_layerStretches.emplace_back( std::make_shared< BlitImageCommand::LayerCopy >( m_device
					, m_pool
					, descriptorLayout
					, m_ubo
					, offset
					, range
					, m_sampler
					, region
					, m_tmpSrcTexture
					, m_tmpDstTexture
					, layer ) );

				*reinterpret_cast< LayerCopy::BlitBox * >( buffer ) = m_layerStretches.back()->blitBox;
				offset += range;
				buffer += range;
			}
		}

		assert( offset == data.size() );
		uint8_t * gpu = nullptr;
		get( m_uboMemory )->lock( 0u, offset, 0u, reinterpret_cast< void ** >( &gpu ) );

		if ( gpu )
		{
			std::memcpy( gpu, data.data(), offset );
			get( m_uboMemory )->flush( 0u, offset );
			get( m_uboMemory )->unlock();
		}
	}

	void BlitImageCommand::doInitialiseStretches( VkDescriptorSetLayout descriptorLayout
		, uint32_t count )
	{
		if ( count )
		{
			m_tmpSrcTexture = doGetSamplable( m_device, m_srcTexture, m_srcMemory );
			m_tmpDstTexture = doGetStorable( m_device, m_dstTexture, m_dstMemory );
			doInitialiseStretchUbo( descriptorLayout, count );
		}
	}

	VkImage BlitImageCommand::doUpdateTmpSrc()
	{
		auto srcTexture = m_srcTexture;

		if ( m_tmpSrcTexture != VK_NULL_HANDLE
			&& m_tmpSrcTexture != m_srcTexture
			&& !m_layerBlitsToTmp.empty() )
		{
			// Copy source to staging
			get( m_commandBuffer )->copyImage( m_srcTexture
				, VK_IMAGE_LAYOUT_UNDEFINED // don't care
				, m_tmpSrcTexture
				, VK_IMAGE_LAYOUT_UNDEFINED // don't care
				, makeArrayView( const_cast< VkImageCopy const * >( m_layerBlitsToTmp.data() )
					, m_layerBlitsToTmp.size() ) );
			srcTexture = m_tmpSrcTexture;
		}

		return srcTexture;
	}

	void BlitImageCommand::doUpdateTmpDst()
	{
		if ( m_tmpDstTexture != VK_NULL_HANDLE
			&& m_tmpDstTexture != m_dstTexture
			&& !m_layerBlitsFromTmp.empty() )
		{
			// Copy staging to destination
			get( m_commandBuffer )->copyImage( m_tmpDstTexture
				, VK_IMAGE_LAYOUT_UNDEFINED // don't care
				, m_dstTexture
				, VK_IMAGE_LAYOUT_UNDEFINED // don't care
				, makeArrayView( const_cast< VkImageCopy const * >( m_layerBlitsFromTmp.data() )
					, m_layerBlitsFromTmp.size() ) );
		}
	}

	void BlitImageCommand::doAddBlits( VkImage srcTexture )
	{
		if ( !m_layerBlits.empty() )
		{
			get( m_commandBuffer )->copyImage( srcTexture
				, VK_IMAGE_LAYOUT_UNDEFINED
				, m_dstTexture
				, VK_IMAGE_LAYOUT_UNDEFINED
				, makeArrayView( const_cast< VkImageCopy const * >( m_layerBlits.data() )
					, m_layerBlits.size() ) );
		}
	}

	void BlitImageCommand::doAddStretches( VkPipeline pipeline
		, VkPipelineLayout pipelineLayout
		, uint32_t srcMinLevel
		, uint32_t srcMaxLevel
		, uint32_t dstMinLevel
		, uint32_t dstMaxLevel )
	{
		if ( !m_layerStretches.empty() )
		{
			get( m_commandBuffer )->bindPipeline( pipeline, VK_PIPELINE_BIND_POINT_COMPUTE );

			for ( auto & copy : m_layerStretches )
			{
				uint32_t srcWidth = copy->srcBox.right - copy->srcBox.left;
				uint32_t srcHeight = std::abs( int( copy->srcBox.bottom ) - int( copy->srcBox.top ) );
				get( m_commandBuffer )->bindDescriptorSets( VK_PIPELINE_BIND_POINT_COMPUTE
					, pipelineLayout
					, 0u
					, makeArrayView( const_cast< VkDescriptorSet const * >( &copy->set ), 1u )
					, {} );
				get( m_commandBuffer )->dispatch( std::max( srcWidth / 8u, 1u ), std::max( srcHeight / 8u, 1u ), 1u );
			}
		}
	}

	void BlitImageCommand::doCreateCommandBuffer( VkCommandPool pool
		, VkPipeline pipeline
		, VkPipelineLayout pipelineLayout
		, uint32_t srcMinLevel
		, uint32_t srcMaxLevel
		, uint32_t dstMinLevel
		, uint32_t dstMaxLevel )
	{
		allocateNA( m_commandBuffer
			, m_device
			, pool
			, true );
		get( m_commandBuffer )->begin( VkCommandBufferBeginInfo
			{
				VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
				nullptr,
				VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
				nullptr
			} );
		VkImage image = doUpdateTmpSrc();
		doAddBlits( image );
		doAddStretches( pipeline
			, pipelineLayout
			, srcMinLevel
			, srcMaxLevel
			, dstMinLevel
			, dstMaxLevel );
		doUpdateTmpDst();
		get( m_commandBuffer )->end();
	}

	BlitImageCommand::~BlitImageCommand()
	{
		deallocate( m_pool, get( m_device )->getAllocationCallbacks() );
		deallocate( m_uboMemory, get( m_device )->getAllocationCallbacks() );
		deallocate( m_ubo, get( m_device )->getAllocationCallbacks() );
		deallocate( m_sampler, get( m_device )->getAllocationCallbacks() );

		if ( m_tmpSrcTexture != m_srcTexture )
		{
			deallocate( m_dstMemory, get( m_device )->getAllocationCallbacks() );
			deallocate( m_tmpDstTexture, get( m_device )->getAllocationCallbacks() );
		}

		if ( m_tmpDstTexture != m_dstTexture )
		{
			deallocate( m_srcMemory, get( m_device )->getAllocationCallbacks() );
			deallocate( m_tmpDstTexture, get( m_device )->getAllocationCallbacks() );
		}
	}

	void BlitImageCommand::apply( Context const & context )const
	{
	}

	CommandPtr BlitImageCommand::clone()const
	{
		return std::make_unique< BlitImageCommand >( *this );
	}

	//*********************************************************************************************
}

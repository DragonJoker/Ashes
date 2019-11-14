/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/GlPhysicalDevice.hpp"

#include "Core/GlInstance.hpp"
#include "Core/GlDevice.hpp"

#include "ashesgl3_api.hpp"

#if ASHES_XLIB
#	include <X11/Xlib.h>
#	include <GL/glx.h>
#endif

#include <GL/gl.h>

#include <algorithm>
#include <cstring>
#include <iterator>
#include <stdexcept>

#ifdef max
#	undef max
#endif

namespace ashes::gl3
{
	namespace
	{
#define GL_MAX_3D_TEXTURE_SIZE 0x8073
#define GL_MAX_ARRAY_TEXTURE_LAYERS 0x88FF
#define GL_MAX_CLIP_DISTANCES 0x0D32
#define GL_MAX_COLOR_ATTACHMENTS 0x8CDF
#define GL_MAX_COLOR_TEXTURE_SAMPLES 0x910E
#define GL_MAX_COMBINED_ATOMIC_COUNTERS 0x92D7
#define GL_MAX_COMBINED_CLIP_AND_CULL_DISTANCES 0x82FA
#define GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS 0x8266
#define GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS 0x8A33
#define GL_MAX_COMBINED_IMAGE_UNIFORMS 0x90CF
#define GL_MAX_COMBINED_SHADER_OUTPUT_RESOURCES 0x8F39
#define GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS 0x90DC
#define GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS 0x8E1E
#define GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS 0x8E1F
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 0x8B4D
#define GL_MAX_COMBINED_UNIFORM_BLOCKS 0x8A2E
#define GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS 0x8A31
#define GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS 0x8264
#define GL_MAX_COMPUTE_ATOMIC_COUNTERS 0x8265
#define GL_MAX_COMPUTE_FIXED_GROUP_INVOCATIONS 0x90EB
#define GL_MAX_COMPUTE_FIXED_GROUP_SIZE 0x91BF
#define GL_MAX_COMPUTE_IMAGE_UNIFORMS 0x91BD
#define GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS 0x90DB
#define GL_MAX_COMPUTE_SHARED_MEMORY_SIZE 0x8262
#define GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS 0x91BC
#define GL_MAX_COMPUTE_UNIFORM_BLOCKS 0x91BB
#define GL_MAX_COMPUTE_UNIFORM_COMPONENTS 0x8263
#define GL_MAX_COMPUTE_VARIABLE_GROUP_INVOCATIONS 0x9344
#define GL_MAX_COMPUTE_VARIABLE_GROUP_SIZE 0x9345
#define GL_MAX_COMPUTE_WORK_GROUP_COUNT 0x91BE
#define GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS 0x90EB
#define GL_MAX_COMPUTE_WORK_GROUP_SIZE 0x91BF
#define GL_MAX_CONVOLUTION_HEIGHT 0x801B
#define GL_MAX_CONVOLUTION_WIDTH 0x801A
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE 0x851C
#define GL_MAX_CULL_DISTANCES 0x82F9
#define GL_MAX_DEPTH_TEXTURE_SAMPLES 0x910F
#define GL_MAX_DRAW_BUFFERS 0x8824
#define GL_MAX_DUAL_SOURCE_DRAW_BUFFERS 0x88FC
#define GL_MAX_ELEMENTS_INDICES 0x80E9
#define GL_MAX_ELEMENTS_VERTICES 0x80E8
#define GL_MAX_FRAGMENT_ATOMIC_COUNTERS 0x92D6
#define GL_MAX_FRAGMENT_IMAGE_UNIFORMS 0x90CE
#define GL_MAX_FRAGMENT_INPUT_COMPONENTS 0x9125
#define GL_MAX_FRAGMENT_INTERPOLATION_OFFSET 0x8E5C
#define GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS 0x90DA
#define GL_MAX_FRAGMENT_UNIFORM_BLOCKS 0x8A2D
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS 0x8B49
#define GL_MAX_FRAMEBUFFER_HEIGHT 0x9316
#define GL_MAX_FRAMEBUFFER_LAYERS 0x9317
#define GL_MAX_FRAMEBUFFER_SAMPLES 0x9318
#define GL_MAX_FRAMEBUFFER_WIDTH 0x9315
#define GL_MAX_GEOMETRY_ATOMIC_COUNTERS 0x92D5
#define GL_MAX_GEOMETRY_IMAGE_UNIFORMS 0x90CD
#define GL_MAX_GEOMETRY_INPUT_COMPONENTS 0x9123
#define GL_MAX_GEOMETRY_OUTPUT_COMPONENTS 0x9124
#define GL_MAX_GEOMETRY_OUTPUT_VERTICES 0x8DE0
#define GL_MAX_GEOMETRY_SHADER_INVOCATIONS 0x8E5A
#define GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS 0x90D7
#define GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS 0x8C29
#define GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS 0x8DE1
#define GL_MAX_GEOMETRY_UNIFORM_BLOCKS 0x8A2C
#define GL_MAX_GEOMETRY_UNIFORM_COMPONENTS 0x8DDF
#define GL_MAX_GEOMETRY_VARYING_COMPONENTS 0x8DDD
#define GL_MAX_IMAGE_SAMPLES 0x906D
#define GL_MAX_IMAGE_UNITS 0x8F38
#define GL_MAX_INTEGER_SAMPLES 0x9110
#define GL_MAX_PATCH_VERTICES 0x8E7D
#define GL_MAX_PROGRAM_ALU_INSTRUCTIONS 0x880B
#define GL_MAX_PROGRAM_NATIVE_ALU_INSTRUCTIONS 0x880E
#define GL_MAX_PROGRAM_NATIVE_TEX_INDIRECTIONS 0x8810
#define GL_MAX_PROGRAM_NATIVE_TEX_INSTRUCTIONS 0x880F
#define GL_MAX_PROGRAM_TEX_INDIRECTIONS 0x880D
#define GL_MAX_PROGRAM_TEX_INSTRUCTIONS 0x880C
#define GL_MAX_PROGRAM_TEXEL_OFFSET 0x8905
#define GL_MAX_PROGRAM_TEXTURE_GATHER_COMPONENTS 0x8F9F
#define GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET 0x8E5F
#define GL_MAX_RECTANGLE_TEXTURE_SIZE 0x84F8
#define GL_MAX_SAMPLE_MASK_WORDS 0x8E59
#define GL_MAX_SAMPLES 0x8D57
#define GL_MAX_SERVER_WAIT_TIMEOUT 0x9111
#define GL_MAX_SHADER_STORAGE_BLOCK_SIZE 0x90DE
#define GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS 0x90DD
#define GL_MAX_SPARSE_3D_TEXTURE_SIZE 0x9199
#define GL_MAX_SPARSE_ARRAY_TEXTURE_LAYERS 0x919A
#define GL_MAX_SPARSE_TEXTURE_SIZE 0x9198
#define GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS 0x8DE8
#define GL_MAX_SUBROUTINES 0x8DE7
#define GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS 0x92D3
#define GL_MAX_TESS_CONTROL_IMAGE_UNIFORMS 0x90CB
#define GL_MAX_TESS_CONTROL_INPUT_COMPONENTS 0x886C
#define GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS 0x8E83
#define GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS 0x90D8
#define GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS 0x8E81
#define GL_MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS 0x8E85
#define GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS 0x8E89
#define GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS 0x8E7F
#define GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS 0x92D4
#define GL_MAX_TESS_EVALUATION_IMAGE_UNIFORMS 0x90CC
#define GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS 0x886D
#define GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS 0x8E86
#define GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS 0x90D9
#define GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS 0x8E82
#define GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS 0x8E8A
#define GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS 0x8E80
#define GL_MAX_TESS_GEN_LEVEL 0x8E7E
#define GL_MAX_TESS_PATCH_COMPONENTS 0x8E84
#define GL_MAX_TEXTURE_BUFFER_SIZE 0x8C2B
#define GL_MAX_TEXTURE_COORDS 0x8871
#define GL_MAX_TEXTURE_IMAGE_UNITS 0x8872
#define GL_MAX_TEXTURE_LOD_BIAS 0x84FD
#define GL_MAX_TEXTURE_MAX_ANISOTROPY 0x84FF
#define GL_MAX_UNIFORM_BLOCK_SIZE 0x8A30
#define GL_MAX_UNIFORM_BUFFER_BINDINGS 0x8A2F
#define GL_UNIFORM_BUFFER_SIZE 0x8A2A
#define GL_MAX_UNIFORM_LOCATIONS 0x826E
#define GL_MAX_VARYING_FLOATS 0x8B4B
#define GL_MAX_VERTEX_ATOMIC_COUNTERS 0x92D2
#define GL_MAX_VERTEX_ATTRIB_BINDINGS 0x82DA
#define GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET 0x82D9
#define GL_MAX_VERTEX_ATTRIB_STRIDE 0x82E5
#define GL_MAX_VERTEX_ATTRIBS 0x8869
#define GL_MAX_VERTEX_IMAGE_UNIFORMS 0x90CA
#define GL_MAX_VERTEX_OUTPUT_COMPONENTS 0x9122
#define GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS 0x90D6
#define GL_MAX_VERTEX_STREAMS 0x8E71
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS 0x8B4C
#define GL_MAX_VERTEX_UNIFORM_BLOCKS 0x8A2B
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS 0x8B4A
#define GL_MAX_VERTEX_UNITS 0x86A4
#define GL_MAX_VERTEX_VARYING_COMPONENTS 0x8DDE
#define GL_MAX_VIEWPORTS 0x825B
#define GL_MIN_FRAGMENT_INTERPOLATION_OFFSET 0x8E5B
#define GL_MIN_LOD_WARNING 0x919C// AMD
#define GL_MIN_PROGRAM_TEXEL_OFFSET 0x8904
#define GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET 0x8E5E
#define GL_MIN_SAMPLE_SHADING_VALUE 0x8C37
#define GL_MIN_SPARSE_LEVEL 0x919B// AMD
#define GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT 0x8A34

#define GL_SAMPLES                                         0x80A9
#define GL_NUM_SAMPLE_COUNTS                               0x9380
#define GL_INTERNALFORMAT_SUPPORTED                        0x826F
#define GL_INTERNALFORMAT_PREFERRED                        0x8270
#define GL_INTERNALFORMAT_RED_SIZE                         0x8271
#define GL_INTERNALFORMAT_GREEN_SIZE                       0x8272
#define GL_INTERNALFORMAT_BLUE_SIZE                        0x8273
#define GL_INTERNALFORMAT_ALPHA_SIZE                       0x8274
#define GL_INTERNALFORMAT_DEPTH_SIZE                       0x8275
#define GL_INTERNALFORMAT_STENCIL_SIZE                     0x8276
#define GL_INTERNALFORMAT_SHARED_SIZE                      0x8277
#define GL_INTERNALFORMAT_RED_TYPE                         0x8278
#define GL_INTERNALFORMAT_GREEN_TYPE                       0x8279
#define GL_INTERNALFORMAT_BLUE_TYPE                        0x827A
#define GL_INTERNALFORMAT_ALPHA_TYPE                       0x827B
#define GL_INTERNALFORMAT_DEPTH_TYPE                       0x827C
#define GL_INTERNALFORMAT_STENCIL_TYPE                     0x827D
#define GL_MAX_WIDTH                                       0x827E
#define GL_MAX_HEIGHT                                      0x827F
#define GL_MAX_DEPTH                                       0x8280
#define GL_MAX_LAYERS                                      0x8281
#define GL_MAX_COMBINED_DIMENSIONS                         0x8282
#define GL_COLOR_COMPONENTS                                0x8283
#define GL_DEPTH_COMPONENTS                                0x8284
#define GL_STENCIL_COMPONENTS                              0x8285
#define GL_COLOR_RENDERABLE                                0x8286
#define GL_DEPTH_RENDERABLE                                0x8287
#define GL_STENCIL_RENDERABLE                              0x8288
#define GL_FRAMEBUFFER_RENDERABLE                          0x8289
#define GL_FRAMEBUFFER_RENDERABLE_LAYERED                  0x828A
#define GL_FRAMEBUFFER_BLEND                               0x828B
#define GL_READ_PIXELS                                     0x828C
#define GL_READ_PIXELS_FORMAT                              0x828D
#define GL_READ_PIXELS_TYPE                                0x828E
#define GL_TEXTURE_IMAGE_FORMAT                            0x828F
#define GL_TEXTURE_IMAGE_TYPE                              0x8290
#define GL_GET_TEXTURE_IMAGE_FORMAT                        0x8291
#define GL_GET_TEXTURE_IMAGE_TYPE                          0x8292
#define GL_MIPMAP                                          0x8293
#define GL_MANUAL_GENERATE_MIPMAP                          0x8294
#define GL_AUTO_GENERATE_MIPMAP                            0x8295
#define GL_COLOR_ENCODING                                  0x8296
#define GL_SRGB_READ                                       0x8297
#define GL_SRGB_WRITE                                      0x8298
#define GL_SRGB_DECODE_ARB                                 0x8299
#define GL_FILTER                                          0x829A
#define GL_VERTEX_TEXTURE                                  0x829B
#define GL_TESS_CONTROL_TEXTURE                            0x829C
#define GL_TESS_EVALUATION_TEXTURE                         0x829D
#define GL_GEOMETRY_TEXTURE                                0x829E
#define GL_FRAGMENT_TEXTURE                                0x829F
#define GL_COMPUTE_TEXTURE                                 0x82A0
#define GL_TEXTURE_SHADOW                                  0x82A1
#define GL_TEXTURE_GATHER                                  0x82A2
#define GL_TEXTURE_GATHER_SHADOW                           0x82A3
#define GL_SHADER_IMAGE_LOAD                               0x82A4
#define GL_SHADER_IMAGE_STORE                              0x82A5
#define GL_SHADER_IMAGE_ATOMIC                             0x82A6
#define GL_IMAGE_TEXEL_SIZE                                0x82A7
#define GL_IMAGE_COMPATIBILITY_CLASS                       0x82A8
#define GL_IMAGE_PIXEL_FORMAT                              0x82A9
#define GL_IMAGE_PIXEL_TYPE                                0x82AA
#define GL_IMAGE_FORMAT_COMPATIBILITY_TYPE                 0x90C7
#define GL_SIMULTANEOUS_TEXTURE_AND_DEPTH_TEST             0x82AC
#define GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_TEST           0x82AD
#define GL_SIMULTANEOUS_TEXTURE_AND_DEPTH_WRITE            0x82AE
#define GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_WRITE          0x82AF
#define GL_TEXTURE_COMPRESSED                              0x86A1
#define GL_TEXTURE_COMPRESSED_BLOCK_WIDTH                  0x82B1
#define GL_TEXTURE_COMPRESSED_BLOCK_HEIGHT                 0x82B2
#define GL_TEXTURE_COMPRESSED_BLOCK_SIZE                   0x82B3
#define GL_CLEAR_BUFFER                                    0x82B4
#define GL_TEXTURE_VIEW                                    0x82B5
#define GL_VIEW_COMPATIBILITY_CLASS                        0x82B6

#define GL_FULL_SUPPORT                                    0x82B7
#define GL_CAVEAT_SUPPORT                                  0x82B8
#define GL_IMAGE_CLASS_4_X_32                              0x82B9
#define GL_IMAGE_CLASS_2_X_32                              0x82BA
#define GL_IMAGE_CLASS_1_X_32                              0x82BB
#define GL_IMAGE_CLASS_4_X_16                              0x82BC
#define GL_IMAGE_CLASS_2_X_16                              0x82BD
#define GL_IMAGE_CLASS_1_X_16                              0x82BE
#define GL_IMAGE_CLASS_4_X_8                               0x82BF
#define GL_IMAGE_CLASS_2_X_8                               0x82C0
#define GL_IMAGE_CLASS_1_X_8                               0x82C1
#define GL_IMAGE_CLASS_11_11_10                            0x82C2
#define GL_IMAGE_CLASS_10_10_10_2                          0x82C3
#define GL_VIEW_CLASS_128_BITS                             0x82C4
#define GL_VIEW_CLASS_96_BITS                              0x82C5
#define GL_VIEW_CLASS_64_BITS                              0x82C6
#define GL_VIEW_CLASS_48_BITS                              0x82C7
#define GL_VIEW_CLASS_32_BITS                              0x82C8
#define GL_VIEW_CLASS_24_BITS                              0x82C9
#define GL_VIEW_CLASS_16_BITS                              0x82CA
#define GL_VIEW_CLASS_8_BITS                               0x82CB
#define GL_VIEW_CLASS_S3TC_DXT1_RGB                        0x82CC
#define GL_VIEW_CLASS_S3TC_DXT1_RGBA                       0x82CD
#define GL_VIEW_CLASS_S3TC_DXT3_RGBA                       0x82CE
#define GL_VIEW_CLASS_S3TC_DXT5_RGBA                       0x82CF
#define GL_VIEW_CLASS_RGTC1_RED                            0x82D0
#define GL_VIEW_CLASS_RGTC2_RG                             0x82D1
#define GL_VIEW_CLASS_BPTC_UNORM                           0x82D2
#define GL_VIEW_CLASS_BPTC_FLOAT                           0x82D3

		using PFN_glGetInteger64v = void( GLAPIENTRY * )( GLenum pname, GLint64 * data );
		PFN_glGetInteger64v glGetInteger64v;
		using PFN_glGetFloati_v = void( GLAPIENTRY * )( GLenum target, GLuint index, GLfloat * data );
		PFN_glGetFloati_v glGetFloati_v;
		using PFN_glGetIntegeri_v = void( GLAPIENTRY * )( GLenum target, GLuint index, GLint * data );
		PFN_glGetIntegeri_v glGetIntegeri_v;
		using PFN_glGetInteger64i_v = void( GLAPIENTRY * )( GLenum target, GLuint index, GLint64 * data );
		PFN_glGetInteger64i_v glGetInteger64i_v;
		using PFN_glGetStringi = const GLubyte *( GLAPIENTRY * )( GLenum name, GLuint index );
		PFN_glGetStringi glGetStringi;
		PFN_glGetInternalformativ glGetInternalformativ;

		uint32_t doGetVendorID( std::string vendorName )
		{
			uint32_t result = 0u;
			std::transform( vendorName.begin()
				, vendorName.end()
				, vendorName.begin()
				, ::tolower );

			if ( vendorName.find( "nvidia" ) != std::string::npos )
			{
				result = 0x10DE;
			}
			else if ( vendorName.find( "intel" ) != std::string::npos )
			{
				result = 0x8086;
			}
			else if ( vendorName.find( "amd" ) != std::string::npos )
			{
				result = 0x1002;
			}
			else if ( vendorName.find( "arm" ) != std::string::npos )
			{
				result = 0x13B5;
			}

			return result;
		}
	}

	PhysicalDevice::PhysicalDevice( VkInstance instance )
		: m_instance{ instance }
	{
		doInitialise();
	}

	std::vector< VkLayerProperties > PhysicalDevice::enumerateLayerProperties()const
	{
		return {};
	}

	std::vector < VkExtensionProperties > PhysicalDevice::enumerateExtensionProperties( std::string const & layerName )const
	{
		return {};
	}

	VkPhysicalDeviceProperties const & PhysicalDevice::getProperties()const
	{
		return m_properties;
	}

	VkPhysicalDeviceMemoryProperties const & PhysicalDevice::getMemoryProperties()const
	{
		return Instance::getMemoryProperties();
	}

	VkPhysicalDeviceFeatures const & PhysicalDevice::getFeatures()const
	{
		return m_features;
	}

	std::vector< VkQueueFamilyProperties > const & PhysicalDevice::getQueueFamilyProperties()const
	{
		return m_queueProperties;
	}

	VkFormatProperties const & PhysicalDevice::getFormatProperties( VkFormat fmt )const
	{
		return m_formatProperties[fmt];
	}

#if VK_KHR_get_physical_device_properties2

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
		, VkImageFormatProperties2KHR & imageFormatProperties )const
	{
		imageFormatProperties = VkImageFormatProperties2KHR{};
		return VK_SUCCESS;
	}

	std::vector< VkQueueFamilyProperties2KHR > PhysicalDevice::getQueueFamilyProperties2()const
	{
		return m_queueProperties2;
	}

	VkPhysicalDeviceMemoryProperties2KHR const & PhysicalDevice::getMemoryProperties2()const
	{
		return Instance::getMemoryProperties2();
	}

	std::vector< VkSparseImageFormatProperties2KHR > PhysicalDevice::getSparseImageFormatProperties2( VkPhysicalDeviceSparseImageFormatInfo2KHR const & formatInfo )const
	{
		return m_sparseImageFormatProperties2;
	}

#endif

	bool PhysicalDevice::find( std::string const & name )const
	{
		return get( m_instance )->getExtensions().find( name );
	}

	bool PhysicalDevice::findAny( StringArray const & names )const
	{
		return get( m_instance )->getExtensions().findAny( names );
	}

	bool PhysicalDevice::findAll( StringArray const & names )const
	{
		return get( m_instance )->getExtensions().findAll( names );
	}

	int PhysicalDevice::getMajor()const
	{
		return get( m_instance )->getExtensions().getMajor();
	}

	int PhysicalDevice::getMinor()const
	{
		return get( m_instance )->getExtensions().getMinor();
	}

	void PhysicalDevice::doInitialise()
	{
		// On récupère les extensions supportées par le GPU.
		getFunction( "glGetInteger64v", glGetInteger64v );
		getFunction( "glGetFloati_v", glGetFloati_v );
		getFunction( "glGetIntegeri_v", glGetIntegeri_v );
		getFunction( "glGetInteger64i_v", glGetInteger64i_v );
		getFunction( "glGetStringi", glGetStringi );
		getFunction( "glGetInternalformativ", glGetInternalformativ );
		auto & extensions = get( m_instance )->getExtensions();

		m_properties.apiVersion = ( extensions.getMajor() << 22 ) | ( extensions.getMinor() << 12 );
		m_properties.deviceID = 0u;
		strncpy( m_properties.deviceName, ( char const * )glGetString( GL_RENDERER ), VK_MAX_PHYSICAL_DEVICE_NAME_SIZE );
		std::memset( m_properties.pipelineCacheUUID, 0u, sizeof( m_properties.pipelineCacheUUID ) );
		m_properties.vendorID = doGetVendorID( ( char const * )glGetString( GL_VENDOR ) );
		m_properties.deviceType = VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
		m_properties.driverVersion = 0;

		doGetValue( GL_MAX_TEXTURE_SIZE, m_properties.limits.maxImageDimension1D );
		doGetValue( GL_MAX_TEXTURE_SIZE, m_properties.limits.maxImageDimension2D );
		doGetValue( GL_MAX_3D_TEXTURE_SIZE, m_properties.limits.maxImageDimension3D );
		doGetValue( GL_MAX_CUBE_MAP_TEXTURE_SIZE, m_properties.limits.maxImageDimensionCube );
		doGetValue( GL_MAX_ARRAY_TEXTURE_LAYERS, m_properties.limits.maxImageArrayLayers );
		doGetValue( GL_MAX_TEXTURE_BUFFER_SIZE, m_properties.limits.maxTexelBufferElements );
		doGetValue( GL_MAX_UNIFORM_BLOCK_SIZE, m_properties.limits.maxUniformBufferRange );
		m_properties.limits.maxStorageBufferRange = NonAvailable< uint32_t >;
		doGetValue( GL_MAX_UNIFORM_BLOCK_SIZE, m_properties.limits.maxPushConstantsSize );
		m_properties.limits.maxMemoryAllocationCount = NonAvailable< uint32_t >;
		m_properties.limits.maxSamplerAllocationCount = NonAvailable< uint32_t >;
		m_properties.limits.bufferImageGranularity = NonAvailable< uint64_t >;
		m_properties.limits.sparseAddressSpaceSize = NonAvailable< uint64_t >;
		m_properties.limits.maxBoundDescriptorSets = NonAvailable< uint32_t >;
		doGetValue( GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, m_properties.limits.maxPerStageDescriptorSamplers );
		doGetValue( GL_MAX_COMBINED_UNIFORM_BLOCKS, m_properties.limits.maxPerStageDescriptorUniformBuffers );
		m_properties.limits.maxPerStageDescriptorStorageBuffers = NonAvailable< uint32_t >;
		doGetValue( GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, m_properties.limits.maxPerStageDescriptorSampledImages );
		doGetValue( GL_MAX_COMBINED_IMAGE_UNIFORMS, m_properties.limits.maxPerStageDescriptorStorageImages );
		m_properties.limits.maxPerStageDescriptorInputAttachments = NonAvailable< uint32_t >;
		m_properties.limits.maxPerStageResources = NonAvailable< uint32_t >;
		doGetValue( GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, m_properties.limits.maxDescriptorSetSamplers );
		doGetValue( GL_MAX_COMBINED_UNIFORM_BLOCKS, m_properties.limits.maxDescriptorSetUniformBuffers );
		doGetValue( GL_MAX_COMBINED_UNIFORM_BLOCKS, m_properties.limits.maxDescriptorSetUniformBuffersDynamic );
		m_properties.limits.maxDescriptorSetStorageBuffers = NonAvailable< uint32_t >;
		m_properties.limits.maxDescriptorSetStorageBuffersDynamic = NonAvailable< uint32_t >;
		doGetValue( GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, m_properties.limits.maxDescriptorSetSampledImages );
		doGetValue( GL_MAX_COMBINED_IMAGE_UNIFORMS, m_properties.limits.maxDescriptorSetStorageImages );
		m_properties.limits.maxDescriptorSetInputAttachments = NonAvailable< uint32_t >;
		doGetValue( GL_MAX_VERTEX_ATTRIBS, m_properties.limits.maxVertexInputAttributes );
		doGetValue( GL_MAX_VERTEX_ATTRIB_BINDINGS, m_properties.limits.maxVertexInputBindings );
		doGetValue( GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET, m_properties.limits.maxVertexInputAttributeOffset );
		doGetValue( GL_MAX_VERTEX_ATTRIB_STRIDE, m_properties.limits.maxVertexInputBindingStride );
		doGetValue( GL_MAX_VERTEX_OUTPUT_COMPONENTS, m_properties.limits.maxVertexOutputComponents );
		m_properties.limits.maxTessellationGenerationLevel = NonAvailable< uint32_t >;
		m_properties.limits.maxTessellationPatchSize = NonAvailable< uint32_t >;
		m_properties.limits.maxTessellationControlPerVertexInputComponents = NonAvailable< uint32_t >;
		m_properties.limits.maxTessellationControlPerVertexOutputComponents = NonAvailable< uint32_t >;
		m_properties.limits.maxTessellationControlPerPatchOutputComponents = NonAvailable< uint32_t >;
		m_properties.limits.maxTessellationControlTotalOutputComponents = NonAvailable< uint32_t >;
		m_properties.limits.maxTessellationEvaluationInputComponents = NonAvailable< uint32_t >;
		m_properties.limits.maxTessellationEvaluationOutputComponents = NonAvailable< uint32_t >;
		doGetValue( GL_MAX_GEOMETRY_SHADER_INVOCATIONS, m_properties.limits.maxGeometryShaderInvocations );
		doGetValue( GL_MAX_GEOMETRY_INPUT_COMPONENTS, m_properties.limits.maxGeometryInputComponents );
		doGetValue( GL_MAX_GEOMETRY_OUTPUT_COMPONENTS, m_properties.limits.maxGeometryOutputComponents );
		doGetValue( GL_MAX_GEOMETRY_OUTPUT_VERTICES, m_properties.limits.maxGeometryOutputVertices );
		doGetValue( GL_MAX_GEOMETRY_OUTPUT_COMPONENTS, m_properties.limits.maxGeometryTotalOutputComponents );
		doGetValue( GL_MAX_FRAGMENT_INPUT_COMPONENTS, m_properties.limits.maxFragmentInputComponents );
		doGetValue( GL_MAX_DRAW_BUFFERS, m_properties.limits.maxFragmentOutputAttachments );
		doGetValue( GL_MAX_DUAL_SOURCE_DRAW_BUFFERS, m_properties.limits.maxFragmentDualSrcAttachments );
		doGetValue( GL_MAX_COLOR_ATTACHMENTS, m_properties.limits.maxFragmentCombinedOutputResources );
		m_properties.limits.maxComputeSharedMemorySize = NonAvailable< uint32_t >;
		m_properties.limits.maxComputeWorkGroupCount[0] = NonAvailable< uint32_t >;
		m_properties.limits.maxComputeWorkGroupCount[1] = NonAvailable< uint32_t >;
		m_properties.limits.maxComputeWorkGroupCount[2] = NonAvailable< uint32_t >;
		m_properties.limits.maxComputeWorkGroupInvocations = NonAvailable< uint32_t >;
		m_properties.limits.maxComputeWorkGroupSize[0] = NonAvailable< uint32_t >;
		m_properties.limits.maxComputeWorkGroupSize[1] = NonAvailable< uint32_t >;
		m_properties.limits.maxComputeWorkGroupSize[2] = NonAvailable< uint32_t >;
		m_properties.limits.subPixelPrecisionBits = NonAvailable< uint32_t >;
		m_properties.limits.subTexelPrecisionBits = NonAvailable< uint32_t >;
		m_properties.limits.mipmapPrecisionBits = NonAvailable< uint32_t >;
		doGetValue( GL_MAX_ELEMENTS_INDICES, m_properties.limits.maxDrawIndexedIndexValue );
		m_properties.limits.maxDrawIndirectCount = NonAvailable< uint32_t >;
		doGetValue( GL_MAX_TEXTURE_LOD_BIAS, m_properties.limits.maxSamplerLodBias );
		doGetValue( GL_MAX_TEXTURE_MAX_ANISOTROPY, m_properties.limits.maxSamplerAnisotropy );
		doGetValue( GL_MAX_VIEWPORTS, m_properties.limits.maxViewports );
		doGetValues( GL_MAX_VIEWPORT_DIMS, m_properties.limits.maxViewportDimensions );
		doGetValues( GL_MAX_VIEWPORT_DIMS, m_properties.limits.viewportBoundsRange );
		m_properties.limits.viewportSubPixelBits = NonAvailable< uint32_t >;
		m_properties.limits.minMemoryMapAlignment = NonAvailable< size_t >;
		m_properties.limits.minTexelBufferOffsetAlignment = NonAvailable< uint64_t >;
		doGetValue( GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, m_properties.limits.minUniformBufferOffsetAlignment );
		m_properties.limits.minStorageBufferOffsetAlignment = NonAvailable< uint64_t >;
		doGetValue( GL_MIN_PROGRAM_TEXEL_OFFSET, m_properties.limits.minTexelOffset );
		doGetValue( GL_MAX_PROGRAM_TEXEL_OFFSET, m_properties.limits.maxTexelOffset );
		doGetValue( GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET, m_properties.limits.minTexelGatherOffset );
		doGetValue( GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET, m_properties.limits.maxTexelGatherOffset );
		m_properties.limits.minInterpolationOffset = NonAvailable< float >;
		m_properties.limits.maxInterpolationOffset = NonAvailable< float >;
		m_properties.limits.subPixelInterpolationOffsetBits = NonAvailable< uint32_t >;
		doGetValue( GL_MAX_FRAMEBUFFER_WIDTH, m_properties.limits.maxFramebufferWidth );
		doGetValue( GL_MAX_FRAMEBUFFER_HEIGHT, m_properties.limits.maxFramebufferHeight );
		doGetValue( GL_MAX_FRAMEBUFFER_LAYERS, m_properties.limits.maxFramebufferLayers );
		doGetValue( GL_MAX_FRAMEBUFFER_SAMPLES, *reinterpret_cast< uint32_t * >( &m_properties.limits.framebufferColorSampleCounts ) );
		doGetValue( GL_MAX_FRAMEBUFFER_SAMPLES, *reinterpret_cast< uint32_t * >( &m_properties.limits.framebufferDepthSampleCounts ) );
		doGetValue( GL_MAX_FRAMEBUFFER_SAMPLES, *reinterpret_cast< uint32_t * >( &m_properties.limits.framebufferStencilSampleCounts ) );
		doGetValue( GL_MAX_FRAMEBUFFER_SAMPLES, *reinterpret_cast< uint32_t * >( &m_properties.limits.framebufferNoAttachmentsSampleCounts ) );
		doGetValue( GL_MAX_COLOR_ATTACHMENTS, m_properties.limits.maxColorAttachments );
		doGetValue( GL_MAX_COLOR_TEXTURE_SAMPLES, *reinterpret_cast< uint32_t * >( &m_properties.limits.sampledImageColorSampleCounts ) );
		doGetValue( GL_MAX_COLOR_TEXTURE_SAMPLES, *reinterpret_cast< uint32_t * >( &m_properties.limits.sampledImageIntegerSampleCounts ) );
		doGetValue( GL_MAX_COLOR_TEXTURE_SAMPLES, *reinterpret_cast< uint32_t * >( &m_properties.limits.sampledImageDepthSampleCounts ) );
		doGetValue( GL_MAX_COLOR_TEXTURE_SAMPLES, *reinterpret_cast< uint32_t * >( &m_properties.limits.sampledImageStencilSampleCounts ) );
		doGetValue( GL_MAX_COLOR_TEXTURE_SAMPLES, *reinterpret_cast< uint32_t * >( &m_properties.limits.storageImageSampleCounts ) );
		doGetValue( GL_MAX_SAMPLE_MASK_WORDS, m_properties.limits.maxSampleMaskWords );
		m_properties.limits.timestampComputeAndGraphics = true;
		m_properties.limits.timestampPeriod = 1;
		doGetValue( GL_MAX_CLIP_DISTANCES, m_properties.limits.maxClipDistances );
		doGetValue( GL_MAX_CULL_DISTANCES, m_properties.limits.maxCullDistances );
		doGetValue( GL_MAX_COMBINED_CLIP_AND_CULL_DISTANCES, m_properties.limits.maxCombinedClipAndCullDistances );
		m_properties.limits.discreteQueuePriorities = 2u;
		doGetValues( GL_POINT_SIZE_RANGE, m_properties.limits.pointSizeRange );
		doGetValues( GL_ALIASED_LINE_WIDTH_RANGE, m_properties.limits.lineWidthRange );
		doGetValue( GL_POINT_SIZE_GRANULARITY, m_properties.limits.pointSizeGranularity );
		doGetValue( GL_LINE_WIDTH_GRANULARITY, m_properties.limits.lineWidthGranularity );
		m_properties.limits.strictLines = true;
		m_properties.limits.standardSampleLocations = false;
		m_properties.limits.optimalBufferCopyOffsetAlignment = NonAvailable< uint64_t >;
		m_properties.limits.optimalBufferCopyRowPitchAlignment = NonAvailable< uint64_t >;
		m_properties.limits.nonCoherentAtomSize = 64ull;

		m_properties.sparseProperties.residencyAlignedMipSize = false;
		m_properties.sparseProperties.residencyNonResidentStrict = false;
		m_properties.sparseProperties.residencyStandard2DBlockShape = false;
		m_properties.sparseProperties.residencyStandard2DMultisampleBlockShape = false;
		m_properties.sparseProperties.residencyStandard3DBlockShape = false;

		m_features.robustBufferAccess = false;
		m_features.fullDrawIndexUint32 = false;
		m_features.imageCubeArray = find( "GL_ARB_texture_cube_map_array" );
		m_features.independentBlend = findAny( { "GL_ARB_draw_buffers_blend", "GL_EXT_draw_buffers2" } );
		m_features.geometryShader = find( "GL_ARB_geometry_shader4" );
		m_features.tessellationShader = false;
		m_features.sampleRateShading = find( "GL_ARB_sample_shading" );
		m_features.dualSrcBlend = find( "GL_ARB_blend_func_extended" );
		m_features.logicOp = true;
		m_features.multiDrawIndirect = findAll( { "GL_ARB_multi_draw_indirect", "GL_ARB_draw_indirect" } );
		m_features.drawIndirectFirstInstance = findAll( { "GL_ARB_base_instance", "GL_ARB_draw_instanced" } );
		m_features.depthClamp = find( "GL_ARB_depth_clamp" );
		m_features.depthBiasClamp = find( "GL_ARB_polygon_offset_clamp" );
		m_features.fillModeNonSolid = true;
		m_features.depthBounds = true;
		GLint range[2];
		glGetIntegerv( GL_ALIASED_LINE_WIDTH_RANGE, range );
		m_features.wideLines = ( range[1] > 1 );
		glGetIntegerv( GL_SMOOTH_LINE_WIDTH_RANGE, range );
		m_features.wideLines &= ( range[1] > 1 );
		m_features.largePoints = true;
		m_features.alphaToOne = findAny( { "GL_ARB_multisample", "GLX_ARB_multisample", "WGL_ARB_multisample" } );
		m_features.multiViewport = find( "GL_ARB_viewport_array" );
		m_features.samplerAnisotropy = findAny( { "GL_ARB_texture_filter_anisotropic", "GL_ARB_texture_filter_anisotropic" } );
		m_features.textureCompressionETC2 = findAll( { "GL_ARB_ES3_compatibility", "GL_ARB_ES2_compatibility", "GL_ARB_invalidate_subdata", "GL_ARB_texture_storage" } );
		m_features.textureCompressionASTC_LDR = find( "GL_KHR_texture_compression_astc_ldr" );
		m_features.textureCompressionBC = findAll( { "GL_EXT_texture_compression_s3tc", "GL_EXT_texture_sRGB" } );
		m_features.occlusionQueryPrecise = true;
		m_features.pipelineStatisticsQuery = false;
		m_features.vertexPipelineStoresAndAtomics = find( "GL_ARB_shader_atomic_counters" );
		m_features.fragmentStoresAndAtomics = m_features.vertexPipelineStoresAndAtomics;
		m_features.shaderTessellationAndGeometryPointSize = m_features.tessellationShader && m_features.geometryShader;
		m_features.shaderImageGatherExtended = findAll( { "GL_ARB_texture_gather", "GL_ARB_gpu_shader5" } );
		m_features.shaderStorageImageExtendedFormats = false;
		m_features.shaderStorageImageMultisample = false;
		m_features.shaderStorageImageReadWithoutFormat = false;
		m_features.shaderStorageImageWriteWithoutFormat = false;
		m_features.shaderUniformBufferArrayDynamicIndexing = false;
		m_features.shaderSampledImageArrayDynamicIndexing = false;
		m_features.shaderStorageBufferArrayDynamicIndexing = false;
		m_features.shaderStorageImageArrayDynamicIndexing = false;
		m_features.shaderClipDistance = true;
		m_features.shaderCullDistance = find( "GL_ARB_cull_distance" );
		m_features.shaderFloat64 = find( "GL_ARB_gpu_shader_fp64" );
		m_features.shaderInt64 = find( "GL_ARB_gpu_shader_int64" );
		m_features.shaderInt16 = false;
		m_features.shaderResourceResidency = false;
		m_features.shaderResourceMinLod = false;
		m_features.sparseBinding = false;
		m_features.sparseResidencyBuffer = false;
		m_features.sparseResidencyImage2D = false;
		m_features.sparseResidencyImage3D = false;
		m_features.sparseResidency2Samples = false;
		m_features.sparseResidency4Samples = false;
		m_features.sparseResidency8Samples = false;
		m_features.sparseResidency16Samples = false;
		m_features.sparseResidencyAliased = false;
		m_features.variableMultisampleRate = true;
		m_features.inheritedQueries = true;

		// Et enfin les propriétés des familles de files du GPU.
		m_queueProperties.push_back(
		{
			0xFF,
			1u,
			64u,
			{
				1u,
				1u,
				1u,
			}
		} );

		if ( glGetInternalformativ )
		{
			for ( VkFormat fmt = VK_FORMAT_BEGIN_RANGE; fmt < VK_FORMAT_END_RANGE; fmt = VkFormat( fmt + 1 ) )
			{
				if ( isSupportedInternal( fmt ) )
				{
					auto internal = getInternalFormat( fmt );
					GLint value;
					glGetInternalformativ( GL_TEXTURE_2D, internal, GL_INTERNALFORMAT_SUPPORTED, 1, &value );

					if ( value == GL_TRUE )
					{
#if defined( VK_KHR_maintenance ) || defined( VK_API_VERSION_1_1 )
						m_formatProperties[fmt].optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_SRC_BIT;
						m_formatProperties[fmt].optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_DST_BIT;
#endif
						glGetInternalformativ( GL_TEXTURE_2D, internal, GL_FRAMEBUFFER_RENDERABLE, 1, &value );

						if ( value == GL_FULL_SUPPORT )
						{
							if ( isDepthOrStencilFormat( fmt ) )
							{
								m_formatProperties[fmt].optimalTilingFeatures |= VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;
							}
							else
							{
								m_formatProperties[fmt].optimalTilingFeatures |= VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT;
							}
						}

						glGetInternalformativ( GL_TEXTURE_2D, internal, GL_FRAMEBUFFER_BLEND, 1, &value );

						if ( value == GL_FULL_SUPPORT )
						{
							m_formatProperties[fmt].optimalTilingFeatures |= VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT;
							m_formatProperties[fmt].optimalTilingFeatures |= VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT;
						}

						glGetInternalformativ( GL_TEXTURE_2D, internal, GL_FRAGMENT_TEXTURE, 1, &value );

						if ( value == GL_FULL_SUPPORT )
						{
							m_formatProperties[fmt].optimalTilingFeatures |= VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT;
						}

						glGetInternalformativ( GL_TEXTURE_2D, internal, GL_FILTER, 1, &value );

						if ( value == GL_FULL_SUPPORT )
						{
							m_formatProperties[fmt].optimalTilingFeatures |= VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT;
						}

						value = 0;
						glGetInternalformativ( GL_TEXTURE_2D, internal, GL_SHADER_IMAGE_LOAD, 1, &value );

						if ( value == GL_FULL_SUPPORT )
						{
							m_formatProperties[fmt].optimalTilingFeatures |= VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT;
						}

						value = 0;
						glGetInternalformativ( GL_TEXTURE_2D, internal, GL_SHADER_IMAGE_ATOMIC, 1, &value );

						if ( value == GL_FULL_SUPPORT )
						{
							m_formatProperties[fmt].optimalTilingFeatures |= VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT;
						}

#if defined( VK_KHR_maintenance ) || defined( VK_API_VERSION_1_1 )
						glGetInternalformativ( GL_TEXTURE_2D, internal, GL_READ_PIXELS, 1, &value );

						if ( value == GL_FULL_SUPPORT )
						{
							m_formatProperties[fmt].optimalTilingFeatures |= VK_FORMAT_FEATURE_TRANSFER_SRC_BIT;
							m_formatProperties[fmt].optimalTilingFeatures |= VK_FORMAT_FEATURE_TRANSFER_DST_BIT;
						}
#endif
					}
				}

				m_formatProperties[fmt].linearTilingFeatures = m_formatProperties[fmt].optimalTilingFeatures;
			}
		}
	}

	void PhysicalDevice::doGetValue( GLenum name, int32_t & value )const
	{
		glGetIntegerv( name, &value );
	}

	void PhysicalDevice::doGetValue( GLenum name, uint32_t & value )const
	{
		int v;
		glGetIntegerv( name, &v );
		value = uint32_t( v );
	}

	void PhysicalDevice::doGetValues( GLenum name, int32_t( &value )[2] )const
	{
		glGetIntegerv( name, value );
	}

	void PhysicalDevice::doGetValues( GLenum name, uint32_t( &value )[2] )const
	{
		int v[2];
		glGetIntegerv( name, v );
		value[0] = v[0];
		value[1] = v[1];
	}

	void PhysicalDevice::doGetValues( GLenum name, int32_t( &value )[3] )const
	{
		glGetIntegerv( name, value );
	}

	void PhysicalDevice::doGetValues( GLenum name, uint32_t( &value )[3] )const
	{
		int v[3];
		glGetIntegerv( name, v );
		value[0] = v[0];
		value[1] = v[1];
		value[2] = v[2];
	}

	void PhysicalDevice::doGetValuesI( GLenum name, int32_t( &value )[2] )const
	{
		glGetIntegeri_v( name, 0, &value[0] );
		glGetIntegeri_v( name, 1, &value[1] );
	}

	void PhysicalDevice::doGetValuesI( GLenum name, uint32_t( &value )[2] )const
	{
		int v[2];
		glGetIntegeri_v( name, 0, &v[0] );
		glGetIntegeri_v( name, 1, &v[1] );
		value[0] = v[0];
		value[1] = v[1];
	}

	void PhysicalDevice::doGetValuesI( GLenum name, int32_t( &value )[3] )const
	{
		glGetIntegeri_v( name, 0, &value[0] );
		glGetIntegeri_v( name, 1, &value[1] );
		glGetIntegeri_v( name, 2, &value[2] );
	}

	void PhysicalDevice::doGetValuesI( GLenum name, uint32_t( &value )[3] )const
	{
		int v[3];
		glGetIntegeri_v( name, 0, &v[0] );
		glGetIntegeri_v( name, 1, &v[1] );
		glGetIntegeri_v( name, 2, &v[2] );
		value[0] = v[0];
		value[1] = v[1];
		value[2] = v[2];
	}

	void PhysicalDevice::doGetValue( GLenum name, int64_t & value )const
	{
		glGetInteger64v( name, &value );
	}

	void PhysicalDevice::doGetValue( GLenum name, uint64_t & value )const
	{
		int64_t v;
		glGetInteger64v( name, &v );
		value = uint64_t( v );
	}

	void PhysicalDevice::doGetValues( GLenum name, int64_t( &value )[2] )const
	{
		glGetInteger64v( name, value );
	}

	void PhysicalDevice::doGetValues( GLenum name, uint64_t( &value )[2] )const
	{
		int64_t v[2];
		glGetInteger64v( name, v );
		value[0] = v[0];
		value[1] = v[1];
	}

	void PhysicalDevice::doGetValues( GLenum name, int64_t( &value )[3] )const
	{
		glGetInteger64v( name, value );
	}

	void PhysicalDevice::doGetValues( GLenum name, uint64_t( &value )[3] )const
	{
		int64_t v[3];
		glGetInteger64v( name, v );
		value[0] = v[0];
		value[1] = v[1];
		value[2] = v[2];
	}

	void PhysicalDevice::doGetValuesI( GLenum name, int64_t( &value )[2] )const
	{
		glGetInteger64i_v( name, 0, &value[0] );
		glGetInteger64i_v( name, 1, &value[1] );
	}

	void PhysicalDevice::doGetValuesI( GLenum name, uint64_t( &value )[2] )const
	{
		int64_t v[2];
		glGetInteger64i_v( name, 0, &v[0] );
		glGetInteger64i_v( name, 1, &v[1] );
		value[0] = v[0];
		value[1] = v[1];
	}

	void PhysicalDevice::doGetValuesI( GLenum name, int64_t( &value )[3] )const
	{
		glGetInteger64i_v( name, 0, &value[0] );
		glGetInteger64i_v( name, 1, &value[1] );
		glGetInteger64i_v( name, 2, &value[2] );
	}

	void PhysicalDevice::doGetValuesI( GLenum name, uint64_t( &value )[3] )const
	{
		int64_t v[3];
		glGetInteger64i_v( name, 0, &v[0] );
		glGetInteger64i_v( name, 1, &v[1] );
		glGetInteger64i_v( name, 2, &v[2] );
		value[0] = v[0];
		value[1] = v[1];
		value[2] = v[2];
	}

	void PhysicalDevice::doGetValue( GLenum name, float & value )const
	{
		glGetFloatv( name, &value );
	}

	void PhysicalDevice::doGetValues( GLenum name, float( &value )[2] )const
	{
		glGetFloatv( name, value );
	}

	void PhysicalDevice::doGetValues( GLenum name, float( &value )[3] )const
	{
		glGetFloatv( name, value );
	}

	void PhysicalDevice::doGetValuesI( GLenum name, float( &value )[2] )const
	{
		glGetFloati_v( name, 0, &value[0] );
		glGetFloati_v( name, 1, &value[1] );
	}

	void PhysicalDevice::doGetValuesI( GLenum name, float( &value )[3] )const
	{
		glGetFloati_v( name, 0, &value[0] );
		glGetFloati_v( name, 1, &value[1] );
		glGetFloati_v( name, 2, &value[2] );
	}
}

/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/GlPhysicalDevice.hpp"

#include "Core/GlInstance.hpp"
#include "Core/GlDevice.hpp"

#include "ashesgl4_api.hpp"

#include <renderer/GlRendererCommon/GlScreenHelpers.hpp>
#include <ashes/common/Hash.hpp>

#if defined( __linux__ )
#	include <X11/Xlib.h>
#elif defined( _WIN32 )
#	include <gl/GL.h>
#endif

#include <algorithm>
#include <cstring>
#include <iterator>
#include <stdexcept>

#ifdef max
#	undef max
#endif

namespace ashes::gl4
{
	namespace
	{
		static GLenum constexpr GL_MAX_3D_TEXTURE_SIZE = 0x8073;
		static GLenum constexpr GL_MAX_ARRAY_TEXTURE_LAYERS = 0x88FF;
		static GLenum constexpr GL_MAX_CLIP_DISTANCES = 0x0D32;
		static GLenum constexpr GL_MAX_COLOR_ATTACHMENTS = 0x8CDF;
		static GLenum constexpr GL_MAX_COLOR_TEXTURE_SAMPLES = 0x910E;
		static GLenum constexpr GL_MAX_COMBINED_ATOMIC_COUNTERS = 0x92D7;
		static GLenum constexpr GL_MAX_COMBINED_CLIP_AND_CULL_DISTANCES = 0x82FA;
		static GLenum constexpr GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS = 0x8266;
		static GLenum constexpr GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS = 0x8A33;
		static GLenum constexpr GL_MAX_COMBINED_IMAGE_UNIFORMS = 0x90CF;
		static GLenum constexpr GL_MAX_COMBINED_SHADER_OUTPUT_RESOURCES = 0x8F39;
		static GLenum constexpr GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS = 0x90DC;
		static GLenum constexpr GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS = 0x8E1E;
		static GLenum constexpr GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS = 0x8E1F;
		static GLenum constexpr GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS = 0x8B4D;
		static GLenum constexpr GL_MAX_COMBINED_UNIFORM_BLOCKS = 0x8A2E;
		static GLenum constexpr GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS = 0x8A31;
		static GLenum constexpr GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS = 0x8264;
		static GLenum constexpr GL_MAX_COMPUTE_ATOMIC_COUNTERS = 0x8265;
		static GLenum constexpr GL_MAX_COMPUTE_FIXED_GROUP_INVOCATIONS = 0x90EB;
		static GLenum constexpr GL_MAX_COMPUTE_FIXED_GROUP_SIZE = 0x91BF;
		static GLenum constexpr GL_MAX_COMPUTE_IMAGE_UNIFORMS = 0x91BD;
		static GLenum constexpr GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS = 0x90DB;
		static GLenum constexpr GL_MAX_COMPUTE_SHARED_MEMORY_SIZE = 0x8262;
		static GLenum constexpr GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS = 0x91BC;
		static GLenum constexpr GL_MAX_COMPUTE_UNIFORM_BLOCKS = 0x91BB;
		static GLenum constexpr GL_MAX_COMPUTE_UNIFORM_COMPONENTS = 0x8263;
		static GLenum constexpr GL_MAX_COMPUTE_VARIABLE_GROUP_INVOCATIONS = 0x9344;
		static GLenum constexpr GL_MAX_COMPUTE_VARIABLE_GROUP_SIZE = 0x9345;
		static GLenum constexpr GL_MAX_COMPUTE_WORK_GROUP_COUNT = 0x91BE;
		static GLenum constexpr GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS = 0x90EB;
		static GLenum constexpr GL_MAX_COMPUTE_WORK_GROUP_SIZE = 0x91BF;
		static GLenum constexpr GL_MAX_CONVOLUTION_HEIGHT = 0x801B;
		static GLenum constexpr GL_MAX_CONVOLUTION_WIDTH = 0x801A;
		static GLenum constexpr GL_MAX_CUBE_MAP_TEXTURE_SIZE = 0x851C;
		static GLenum constexpr GL_MAX_CULL_DISTANCES = 0x82F9;
		static GLenum constexpr GL_MAX_DEPTH_TEXTURE_SAMPLES = 0x910F;
		static GLenum constexpr GL_MAX_DRAW_BUFFERS = 0x8824;
		static GLenum constexpr GL_MAX_DUAL_SOURCE_DRAW_BUFFERS = 0x88FC;
		static GLenum constexpr GL_MAX_ELEMENTS_INDICES = 0x80E9;
		static GLenum constexpr GL_MAX_ELEMENTS_VERTICES = 0x80E8;
		static GLenum constexpr GL_MAX_FRAGMENT_ATOMIC_COUNTERS = 0x92D6;
		static GLenum constexpr GL_MAX_FRAGMENT_IMAGE_UNIFORMS = 0x90CE;
		static GLenum constexpr GL_MAX_FRAGMENT_INPUT_COMPONENTS = 0x9125;
		static GLenum constexpr GL_MAX_FRAGMENT_INTERPOLATION_OFFSET = 0x8E5C;
		static GLenum constexpr GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS = 0x90DA;
		static GLenum constexpr GL_MAX_FRAGMENT_UNIFORM_BLOCKS = 0x8A2D;
		static GLenum constexpr GL_MAX_FRAGMENT_UNIFORM_COMPONENTS = 0x8B49;
		static GLenum constexpr GL_MAX_FRAMEBUFFER_HEIGHT = 0x9316;
		static GLenum constexpr GL_MAX_FRAMEBUFFER_LAYERS = 0x9317;
		static GLenum constexpr GL_MAX_FRAMEBUFFER_SAMPLES = 0x9318;
		static GLenum constexpr GL_MAX_FRAMEBUFFER_WIDTH = 0x9315;
		static GLenum constexpr GL_MAX_GEOMETRY_ATOMIC_COUNTERS = 0x92D5;
		static GLenum constexpr GL_MAX_GEOMETRY_IMAGE_UNIFORMS = 0x90CD;
		static GLenum constexpr GL_MAX_GEOMETRY_INPUT_COMPONENTS = 0x9123;
		static GLenum constexpr GL_MAX_GEOMETRY_OUTPUT_COMPONENTS = 0x9124;
		static GLenum constexpr GL_MAX_GEOMETRY_OUTPUT_VERTICES = 0x8DE0;
		static GLenum constexpr GL_MAX_GEOMETRY_SHADER_INVOCATIONS = 0x8E5A;
		static GLenum constexpr GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS = 0x90D7;
		static GLenum constexpr GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS = 0x8C29;
		static GLenum constexpr GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS = 0x8DE1;
		static GLenum constexpr GL_MAX_GEOMETRY_UNIFORM_BLOCKS = 0x8A2C;
		static GLenum constexpr GL_MAX_GEOMETRY_UNIFORM_COMPONENTS = 0x8DDF;
		static GLenum constexpr GL_MAX_GEOMETRY_VARYING_COMPONENTS = 0x8DDD;
		static GLenum constexpr GL_MAX_IMAGE_SAMPLES = 0x906D;
		static GLenum constexpr GL_MAX_IMAGE_UNITS = 0x8F38;
		static GLenum constexpr GL_MAX_INTEGER_SAMPLES = 0x9110;
		static GLenum constexpr GL_MAX_PATCH_VERTICES = 0x8E7D;
		static GLenum constexpr GL_MAX_PROGRAM_ALU_INSTRUCTIONS = 0x880B;
		static GLenum constexpr GL_MAX_PROGRAM_NATIVE_ALU_INSTRUCTIONS = 0x880E;
		static GLenum constexpr GL_MAX_PROGRAM_NATIVE_TEX_INDIRECTIONS = 0x8810;
		static GLenum constexpr GL_MAX_PROGRAM_NATIVE_TEX_INSTRUCTIONS = 0x880F;
		static GLenum constexpr GL_MAX_PROGRAM_TEX_INDIRECTIONS = 0x880D;
		static GLenum constexpr GL_MAX_PROGRAM_TEX_INSTRUCTIONS = 0x880C;
		static GLenum constexpr GL_MAX_PROGRAM_TEXEL_OFFSET = 0x8905;
		static GLenum constexpr GL_MAX_PROGRAM_TEXTURE_GATHER_COMPONENTS = 0x8F9F;
		static GLenum constexpr GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET = 0x8E5F;
		static GLenum constexpr GL_MAX_RECTANGLE_TEXTURE_SIZE = 0x84F8;
		static GLenum constexpr GL_MAX_SAMPLE_MASK_WORDS = 0x8E59;
		static GLenum constexpr GL_MAX_SAMPLES = 0x8D57;
		static GLenum constexpr GL_MAX_SERVER_WAIT_TIMEOUT = 0x9111;
		static GLenum constexpr GL_MAX_SHADER_STORAGE_BLOCK_SIZE = 0x90DE;
		static GLenum constexpr GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS = 0x90DD;
		static GLenum constexpr GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS = 0x8DE8;
		static GLenum constexpr GL_MAX_SUBROUTINES = 0x8DE7;
		static GLenum constexpr GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS = 0x92D3;
		static GLenum constexpr GL_MAX_TESS_CONTROL_IMAGE_UNIFORMS = 0x90CB;
		static GLenum constexpr GL_MAX_TESS_CONTROL_INPUT_COMPONENTS = 0x886C;
		static GLenum constexpr GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS = 0x8E83;
		static GLenum constexpr GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS = 0x90D8;
		static GLenum constexpr GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS = 0x8E81;
		static GLenum constexpr GL_MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS = 0x8E85;
		static GLenum constexpr GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS = 0x8E89;
		static GLenum constexpr GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS = 0x8E7F;
		static GLenum constexpr GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS = 0x92D4;
		static GLenum constexpr GL_MAX_TESS_EVALUATION_IMAGE_UNIFORMS = 0x90CC;
		static GLenum constexpr GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS = 0x886D;
		static GLenum constexpr GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS = 0x8E86;
		static GLenum constexpr GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS = 0x90D9;
		static GLenum constexpr GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS = 0x8E82;
		static GLenum constexpr GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS = 0x8E8A;
		static GLenum constexpr GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS = 0x8E80;
		static GLenum constexpr GL_MAX_TESS_GEN_LEVEL = 0x8E7E;
		static GLenum constexpr GL_MAX_TESS_PATCH_COMPONENTS = 0x8E84;
		static GLenum constexpr GL_MAX_TEXTURE_BUFFER_SIZE = 0x8C2B;
		static GLenum constexpr GL_MAX_TEXTURE_COORDS = 0x8871;
		static GLenum constexpr GL_MAX_TEXTURE_IMAGE_UNITS = 0x8872;
		static GLenum constexpr GL_MAX_TEXTURE_LOD_BIAS = 0x84FD;
		static GLenum constexpr GL_MAX_TEXTURE_MAX_ANISOTROPY = 0x84FF;
		static GLenum constexpr GL_MAX_UNIFORM_BLOCK_SIZE = 0x8A30;
		static GLenum constexpr GL_MAX_UNIFORM_BUFFER_BINDINGS = 0x8A2F;
		static GLenum constexpr GL_UNIFORM_BUFFER_SIZE = 0x8A2A;
		static GLenum constexpr GL_MAX_UNIFORM_LOCATIONS = 0x826E;
		static GLenum constexpr GL_MAX_VARYING_FLOATS = 0x8B4B;
		static GLenum constexpr GL_MAX_VERTEX_ATOMIC_COUNTERS = 0x92D2;
		static GLenum constexpr GL_MAX_VERTEX_ATTRIB_BINDINGS = 0x82DA;
		static GLenum constexpr GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET = 0x82D9;
		static GLenum constexpr GL_MAX_VERTEX_ATTRIB_STRIDE = 0x82E5;
		static GLenum constexpr GL_MAX_VERTEX_ATTRIBS = 0x8869;
		static GLenum constexpr GL_MAX_VERTEX_IMAGE_UNIFORMS = 0x90CA;
		static GLenum constexpr GL_MAX_VERTEX_OUTPUT_COMPONENTS = 0x9122;
		static GLenum constexpr GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS = 0x90D6;
		static GLenum constexpr GL_MAX_VERTEX_STREAMS = 0x8E71;
		static GLenum constexpr GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS = 0x8B4C;
		static GLenum constexpr GL_MAX_VERTEX_UNIFORM_BLOCKS = 0x8A2B;
		static GLenum constexpr GL_MAX_VERTEX_UNIFORM_COMPONENTS = 0x8B4A;
		static GLenum constexpr GL_MAX_VERTEX_UNITS = 0x86A4;
		static GLenum constexpr GL_MAX_VERTEX_VARYING_COMPONENTS = 0x8DDE;
		static GLenum constexpr GL_MAX_VIEWPORTS = 0x825B;
		static GLenum constexpr GL_MIN_FRAGMENT_INTERPOLATION_OFFSET = 0x8E5B;
		static GLenum constexpr GL_MIN_LOD_WARNING = 0x919C;// AMD
		static GLenum constexpr GL_MIN_PROGRAM_TEXEL_OFFSET = 0x8904;
		static GLenum constexpr GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET = 0x8E5E;
		static GLenum constexpr GL_MIN_SAMPLE_SHADING_VALUE = 0x8C37;
		static GLenum constexpr GL_MIN_SPARSE_LEVEL = 0x919B;// AMD

		static GLenum constexpr GL_SAMPLES = 0x80A9;
		static GLenum constexpr GL_NUM_SAMPLE_COUNTS = 0x9380;
		static GLenum constexpr GL_INTERNALFORMAT_SUPPORTED = 0x826F;
		static GLenum constexpr GL_INTERNALFORMAT_PREFERRED = 0x8270;
		static GLenum constexpr GL_INTERNALFORMAT_RED_SIZE = 0x8271;
		static GLenum constexpr GL_INTERNALFORMAT_GREEN_SIZE = 0x8272;
		static GLenum constexpr GL_INTERNALFORMAT_BLUE_SIZE = 0x8273;
		static GLenum constexpr GL_INTERNALFORMAT_ALPHA_SIZE = 0x8274;
		static GLenum constexpr GL_INTERNALFORMAT_DEPTH_SIZE = 0x8275;
		static GLenum constexpr GL_INTERNALFORMAT_STENCIL_SIZE = 0x8276;
		static GLenum constexpr GL_INTERNALFORMAT_SHARED_SIZE = 0x8277;
		static GLenum constexpr GL_INTERNALFORMAT_RED_TYPE = 0x8278;
		static GLenum constexpr GL_INTERNALFORMAT_GREEN_TYPE = 0x8279;
		static GLenum constexpr GL_INTERNALFORMAT_BLUE_TYPE = 0x827A;
		static GLenum constexpr GL_INTERNALFORMAT_ALPHA_TYPE = 0x827B;
		static GLenum constexpr GL_INTERNALFORMAT_DEPTH_TYPE = 0x827C;
		static GLenum constexpr GL_INTERNALFORMAT_STENCIL_TYPE = 0x827D;
		static GLenum constexpr GL_MAX_WIDTH = 0x827E;
		static GLenum constexpr GL_MAX_HEIGHT = 0x827F;
		static GLenum constexpr GL_MAX_DEPTH = 0x8280;
		static GLenum constexpr GL_MAX_LAYERS = 0x8281;
		static GLenum constexpr GL_MAX_COMBINED_DIMENSIONS = 0x8282;
		static GLenum constexpr GL_COLOR_COMPONENTS = 0x8283;
		static GLenum constexpr GL_DEPTH_COMPONENTS = 0x8284;
		static GLenum constexpr GL_STENCIL_COMPONENTS = 0x8285;
		static GLenum constexpr GL_COLOR_RENDERABLE = 0x8286;
		static GLenum constexpr GL_DEPTH_RENDERABLE = 0x8287;
		static GLenum constexpr GL_STENCIL_RENDERABLE = 0x8288;
		static GLenum constexpr GL_FRAMEBUFFER_RENDERABLE = 0x8289;
		static GLenum constexpr GL_FRAMEBUFFER_RENDERABLE_LAYERED = 0x828A;
		static GLenum constexpr GL_FRAMEBUFFER_BLEND = 0x828B;
		static GLenum constexpr GL_READ_PIXELS = 0x828C;
		static GLenum constexpr GL_READ_PIXELS_FORMAT = 0x828D;
		static GLenum constexpr GL_READ_PIXELS_TYPE = 0x828E;
		static GLenum constexpr GL_TEXTURE_IMAGE_FORMAT = 0x828F;
		static GLenum constexpr GL_TEXTURE_IMAGE_TYPE = 0x8290;
		static GLenum constexpr GL_GET_TEXTURE_IMAGE_FORMAT = 0x8291;
		static GLenum constexpr GL_GET_TEXTURE_IMAGE_TYPE = 0x8292;
		static GLenum constexpr GL_MIPMAP = 0x8293;
		static GLenum constexpr GL_MANUAL_GENERATE_MIPMAP = 0x8294;
		static GLenum constexpr GL_AUTO_GENERATE_MIPMAP = 0x8295;
		static GLenum constexpr GL_COLOR_ENCODING = 0x8296;
		static GLenum constexpr GL_SRGB_READ = 0x8297;
		static GLenum constexpr GL_SRGB_WRITE = 0x8298;
		static GLenum constexpr GL_SRGB_DECODE_ARB = 0x8299;
		static GLenum constexpr GL_FILTER = 0x829A;
		static GLenum constexpr GL_VERTEX_TEXTURE = 0x829B;
		static GLenum constexpr GL_TESS_CONTROL_TEXTURE = 0x829C;
		static GLenum constexpr GL_TESS_EVALUATION_TEXTURE = 0x829D;
		static GLenum constexpr GL_GEOMETRY_TEXTURE = 0x829E;
		static GLenum constexpr GL_FRAGMENT_TEXTURE = 0x829F;
		static GLenum constexpr GL_COMPUTE_TEXTURE = 0x82A0;
		static GLenum constexpr GL_TEXTURE_SHADOW = 0x82A1;
		static GLenum constexpr GL_TEXTURE_GATHER = 0x82A2;
		static GLenum constexpr GL_TEXTURE_GATHER_SHADOW = 0x82A3;
		static GLenum constexpr GL_SHADER_IMAGE_LOAD = 0x82A4;
		static GLenum constexpr GL_SHADER_IMAGE_STORE = 0x82A5;
		static GLenum constexpr GL_SHADER_IMAGE_ATOMIC = 0x82A6;
		static GLenum constexpr GL_IMAGE_TEXEL_SIZE = 0x82A7;
		static GLenum constexpr GL_IMAGE_COMPATIBILITY_CLASS = 0x82A8;
		static GLenum constexpr GL_IMAGE_PIXEL_FORMAT = 0x82A9;
		static GLenum constexpr GL_IMAGE_PIXEL_TYPE = 0x82AA;
		static GLenum constexpr GL_IMAGE_FORMAT_COMPATIBILITY_TYPE = 0x90C7;
		static GLenum constexpr GL_SIMULTANEOUS_TEXTURE_AND_DEPTH_TEST = 0x82AC;
		static GLenum constexpr GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_TEST = 0x82AD;
		static GLenum constexpr GL_SIMULTANEOUS_TEXTURE_AND_DEPTH_WRITE = 0x82AE;
		static GLenum constexpr GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_WRITE = 0x82AF;
		static GLenum constexpr GL_TEXTURE_COMPRESSED = 0x86A1;
		static GLenum constexpr GL_TEXTURE_COMPRESSED_BLOCK_WIDTH = 0x82B1;
		static GLenum constexpr GL_TEXTURE_COMPRESSED_BLOCK_HEIGHT = 0x82B2;
		static GLenum constexpr GL_TEXTURE_COMPRESSED_BLOCK_SIZE = 0x82B3;
		static GLenum constexpr GL_CLEAR_BUFFER = 0x82B4;
		static GLenum constexpr GL_TEXTURE_VIEW = 0x82B5;
		static GLenum constexpr GL_VIEW_COMPATIBILITY_CLASS = 0x82B6;

		static GLenum constexpr GL_FULL_SUPPORT = 0x82B7;
		static GLenum constexpr GL_CAVEAT_SUPPORT = 0x82B8;
		static GLenum constexpr GL_IMAGE_CLASS_4_X_32 = 0x82B9;
		static GLenum constexpr GL_IMAGE_CLASS_2_X_32 = 0x82BA;
		static GLenum constexpr GL_IMAGE_CLASS_1_X_32 = 0x82BB;
		static GLenum constexpr GL_IMAGE_CLASS_4_X_16 = 0x82BC;
		static GLenum constexpr GL_IMAGE_CLASS_2_X_16 = 0x82BD;
		static GLenum constexpr GL_IMAGE_CLASS_1_X_16 = 0x82BE;
		static GLenum constexpr GL_IMAGE_CLASS_4_X_8 = 0x82BF;
		static GLenum constexpr GL_IMAGE_CLASS_2_X_8 = 0x82C0;
		static GLenum constexpr GL_IMAGE_CLASS_1_X_8 = 0x82C1;
		static GLenum constexpr GL_IMAGE_CLASS_11_11_10 = 0x82C2;
		static GLenum constexpr GL_IMAGE_CLASS_10_10_10_2 = 0x82C3;
		static GLenum constexpr GL_VIEW_CLASS_128_BITS = 0x82C4;
		static GLenum constexpr GL_VIEW_CLASS_96_BITS = 0x82C5;
		static GLenum constexpr GL_VIEW_CLASS_64_BITS = 0x82C6;
		static GLenum constexpr GL_VIEW_CLASS_48_BITS = 0x82C7;
		static GLenum constexpr GL_VIEW_CLASS_32_BITS = 0x82C8;
		static GLenum constexpr GL_VIEW_CLASS_24_BITS = 0x82C9;
		static GLenum constexpr GL_VIEW_CLASS_16_BITS = 0x82CA;
		static GLenum constexpr GL_VIEW_CLASS_8_BITS = 0x82CB;
		static GLenum constexpr GL_VIEW_CLASS_S3TC_DXT1_RGB = 0x82CC;
		static GLenum constexpr GL_VIEW_CLASS_S3TC_DXT1_RGBA = 0x82CD;
		static GLenum constexpr GL_VIEW_CLASS_S3TC_DXT3_RGBA = 0x82CE;
		static GLenum constexpr GL_VIEW_CLASS_S3TC_DXT5_RGBA = 0x82CF;
		static GLenum constexpr GL_VIEW_CLASS_RGTC1_RED = 0x82D0;
		static GLenum constexpr GL_VIEW_CLASS_RGTC2_RG = 0x82D1;
		static GLenum constexpr GL_VIEW_CLASS_BPTC_UNORM = 0x82D2;
		static GLenum constexpr GL_VIEW_CLASS_BPTC_FLOAT = 0x82D3;

#if !defined( _WIN32 )
		static GLenum constexpr GL_MAX_TEXTURE_SIZE = 0x0D33;
		static GLenum constexpr GL_MAX_VIEWPORT_DIMS = 0x0D3A;
		static GLenum constexpr GL_POINT_SIZE_RANGE = 0x0B12;
		static GLenum constexpr GL_POINT_SIZE_GRANULARITY = 0x0B13;
		static GLenum constexpr GL_LINE_WIDTH_GRANULARITY = 0x0B23;
#endif

		size_t makeKey( VkFormat format
			, VkImageType type
			, VkImageTiling tiling
			, VkImageUsageFlags usage
			, VkImageCreateFlags flags )
		{
			auto hash = std::hash< VkFormat >{}( format );
			hashCombine( hash, type );
			hashCombine( hash, tiling );
			hashCombine( hash, usage );
			hashCombine( hash, flags );
			return hash;
		}

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

	VkResult PhysicalDevice::getImageFormatProperties( VkFormat format
		, VkImageType type
		, VkImageTiling tiling
		, VkImageUsageFlags usage
		, VkImageCreateFlags flags
		, VkImageFormatProperties & imageProperties )const
	{
		auto pair = m_imageFormatProperties.insert( { makeKey( format
				, type
				, VkImageTiling{}
				, VkImageUsageFlags{}
				, VkImageCreateFlags{} )
			, {} } );

		if ( pair.second )
		{
			ContextLock context{ get( m_instance )->getCurrentContext() };
			auto internal = getInternalFormat( format );
			auto gltype = convert( type, 1u );
			GLint value;
			glLogCall( context, glGetInternalformativ, gltype, internal, GL_INTERNALFORMAT_SUPPORTED, 1, &value );

			if ( value == GL_FALSE )
			{
				return VK_ERROR_FORMAT_NOT_SUPPORTED;
			}

			glLogCall( context, glGetInternalformativ, gltype, internal, GL_MAX_WIDTH, 1, &value );
			pair.first->second.maxExtent.width = uint32_t( value );
			pair.first->second.maxExtent.height = 1u;
			pair.first->second.maxExtent.depth = 1u;

			if ( type == VK_IMAGE_TYPE_2D
				|| type == VK_IMAGE_TYPE_3D )
			{
				glLogCall( context, glGetInternalformativ, GL_TEXTURE_2D, internal, GL_MAX_HEIGHT, 1, &value );
				pair.first->second.maxExtent.height = uint32_t( value );

				if ( type == VK_IMAGE_TYPE_3D )
				{
					glLogCall( context, glGetInternalformativ, GL_TEXTURE_3D, internal, GL_MAX_DEPTH, 1, &value );
					pair.first->second.maxExtent.depth = uint32_t( value );
				}
			}

			glLogCall( context, glGetInternalformativ, gltype, internal, GL_SAMPLES, 1, &value );
			pair.first->second.sampleCounts = VkSampleCountFlagBits( value );

			glLogCall( context, glGetInternalformativ, gltype, internal, GL_IMAGE_TEXEL_SIZE, 1, &value );
			VkDeviceSize texelSize = VkDeviceSize( value );

			gltype = convert( type, 2u );
			glLogCall( context, glGetInternalformativ, gltype, internal, GL_MAX_LAYERS, 1, &value );
			pair.first->second.maxArrayLayers = uint32_t( value );
		}

		imageProperties = pair.first->second;
		return imageProperties.maxExtent.width == 0u
			? VK_ERROR_FORMAT_NOT_SUPPORTED
			: VK_SUCCESS;
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
		ContextLock context{ get( m_instance )->getCurrentContext() };
		doInitialiseProperties( context );
		doInitialiseFeatures( context );
		doInitialiseQueueProperties( context );
		doInitialiseFormatProperties( context );
		doInitialiseDisplayProperties( context );
		doInitialiseProperties2( context );
	}

	void PhysicalDevice::doInitialiseFeatures( ContextLock & context )
	{
		m_features.robustBufferAccess = find( "GL_KHR_robustness" );
		m_features.fullDrawIndexUint32 = false;
		m_features.imageCubeArray = find( "GL_ARB_texture_cube_map_array" );
		m_features.independentBlend = findAny( { "GL_ARB_draw_buffers_blend", "GL_EXT_draw_buffers2" } );
		m_features.geometryShader = find( "GL_ARB_geometry_shader4" );
		m_features.tessellationShader = find( "GL_ARB_tessellation_shader" );
		m_features.sampleRateShading = find( "GL_ARB_sample_shading" );
		m_features.dualSrcBlend = find( "GL_ARB_blend_func_extended" );
		m_features.logicOp = true;
		m_features.multiDrawIndirect = findAll( { "GL_ARB_multi_draw_indirect", "GL_ARB_draw_indirect" } );
		m_features.drawIndirectFirstInstance = find( "GL_ARB_base_instance" );
		m_features.depthClamp = find( "GL_ARB_depth_clamp" );
		m_features.depthBiasClamp = find( "GL_ARB_polygon_offset_clamp" );
		m_features.fillModeNonSolid = true;
		m_features.depthBounds = true;
		GLint range[2];
		glLogCall( context, glGetIntegerv
			, GL_ALIASED_LINE_WIDTH_RANGE
			, range );
		m_features.wideLines = ( range[1] > 1 );
		glLogCall( context, glGetIntegerv
			, GL_SMOOTH_LINE_WIDTH_RANGE
			, range );
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
		m_features.shaderStorageImageExtendedFormats = find( "GL_ARB_shader_image_load_store" );
		m_features.shaderStorageImageMultisample = find( "GL_ARB_shader_image_load_store" );
		m_features.shaderStorageImageReadWithoutFormat = find( "GL_EXT_shader_image_load_formatted" );
		m_features.shaderStorageImageWriteWithoutFormat = find( "GL_ARB_shader_image_load_store" );
		m_features.shaderUniformBufferArrayDynamicIndexing = find( "GL_ARB_gpu_shader5" );
		m_features.shaderSampledImageArrayDynamicIndexing = find( "GL_ARB_gpu_shader5" );
		m_features.shaderStorageBufferArrayDynamicIndexing = find( "GL_ARB_shader_storage_buffer_object" );
		m_features.shaderStorageImageArrayDynamicIndexing = find( "GL_ARB_shader_image_load_store" );
		m_features.shaderClipDistance = true;
		m_features.shaderCullDistance = find( "GL_ARB_cull_distance" );
		m_features.shaderFloat64 = find( "GL_ARB_gpu_shader_fp64" );
		m_features.shaderInt64 = find( "GL_ARB_gpu_shader_int64" );
		m_features.shaderInt16 = false;
		m_features.shaderResourceResidency = false;// find( "GL_ARB_sparse_texture2" );
		m_features.shaderResourceMinLod = false;// find( "GL_ARB_sparse_texture_clamp" );
		m_features.sparseBinding = false;// findAll( { "GL_ARB_sparse_buffer", "GL_ARB_sparse_texture2" } );
		m_features.sparseResidencyBuffer = false;// find( "GL_ARB_sparse_buffer" );
		m_features.sparseResidencyImage2D = false;// find( "GL_ARB_sparse_texture2" );
		m_features.sparseResidencyImage3D = false;// find( "GL_ARB_sparse_texture2" );
		m_features.sparseResidency2Samples = false;// find( "GL_ARB_sparse_texture2" );
		m_features.sparseResidency4Samples = false;// find( "GL_ARB_sparse_texture2" );
		m_features.sparseResidency8Samples = false;// find( "GL_ARB_sparse_texture2" );
		m_features.sparseResidency16Samples = false;// find( "GL_ARB_sparse_texture2" );
		m_features.sparseResidencyAliased = false;
		m_features.variableMultisampleRate = true;
		m_features.inheritedQueries = true;
	}

	void PhysicalDevice::doInitialiseProperties( ContextLock & context )
	{
		auto & extensions = get( m_instance )->getExtensions();
		m_properties.apiVersion = ( extensions.getMajor() << 22 ) | ( extensions.getMinor() << 12 );
		m_properties.deviceID = 0u;
		strncpy( m_properties.deviceName, ( char const * )context->glGetString( GL_RENDERER ), VK_MAX_PHYSICAL_DEVICE_NAME_SIZE );
		std::memset( m_properties.pipelineCacheUUID, 0u, sizeof( m_properties.pipelineCacheUUID ) );
		m_properties.vendorID = doGetVendorID( ( char const * )context->glGetString( GL_VENDOR ) );
		m_properties.deviceType = VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
		m_properties.driverVersion = 0;

		doGetValue( context, GL_MAX_TEXTURE_SIZE, m_properties.limits.maxImageDimension1D );
		doGetValue( context, GL_MAX_TEXTURE_SIZE, m_properties.limits.maxImageDimension2D );
		doGetValue( context, GL_MAX_3D_TEXTURE_SIZE, m_properties.limits.maxImageDimension3D );
		doGetValue( context, GL_MAX_CUBE_MAP_TEXTURE_SIZE, m_properties.limits.maxImageDimensionCube );
		doGetValue( context, GL_MAX_ARRAY_TEXTURE_LAYERS, m_properties.limits.maxImageArrayLayers );
		doGetValue( context, GL_MAX_TEXTURE_BUFFER_SIZE, m_properties.limits.maxTexelBufferElements );
		doGetValue( context, GL_MAX_UNIFORM_BLOCK_SIZE, m_properties.limits.maxUniformBufferRange );
		m_properties.limits.maxStorageBufferRange = NonAvailable< uint32_t >;
		doGetValue( context, GL_MAX_UNIFORM_BLOCK_SIZE, m_properties.limits.maxPushConstantsSize );
		m_properties.limits.maxMemoryAllocationCount = NonAvailable< uint32_t >;
		m_properties.limits.maxSamplerAllocationCount = NonAvailable< uint32_t >;
		m_properties.limits.bufferImageGranularity = NonAvailable< uint64_t >;
		m_properties.limits.sparseAddressSpaceSize = NonAvailable< uint64_t >;
		m_properties.limits.maxBoundDescriptorSets = NonAvailable< uint32_t >;
		doGetValue( context, GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, m_properties.limits.maxPerStageDescriptorSamplers );
		doGetValue( context, GL_MAX_COMBINED_UNIFORM_BLOCKS, m_properties.limits.maxPerStageDescriptorUniformBuffers );
		doGetValue( context, GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS, m_properties.limits.maxPerStageDescriptorStorageBuffers );
		doGetValue( context, GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, m_properties.limits.maxPerStageDescriptorSampledImages );
		doGetValue( context, GL_MAX_COMBINED_IMAGE_UNIFORMS, m_properties.limits.maxPerStageDescriptorStorageImages );
		m_properties.limits.maxPerStageDescriptorInputAttachments = NonAvailable< uint32_t >;
		m_properties.limits.maxPerStageResources = NonAvailable< uint32_t >;
		doGetValue( context, GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, m_properties.limits.maxDescriptorSetSamplers );
		doGetValue( context, GL_MAX_COMBINED_UNIFORM_BLOCKS, m_properties.limits.maxDescriptorSetUniformBuffers );
		doGetValue( context, GL_MAX_COMBINED_UNIFORM_BLOCKS, m_properties.limits.maxDescriptorSetUniformBuffersDynamic );
		doGetValue( context, GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS, m_properties.limits.maxDescriptorSetStorageBuffers );
		doGetValue( context, GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS, m_properties.limits.maxDescriptorSetStorageBuffersDynamic );
		doGetValue( context, GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, m_properties.limits.maxDescriptorSetSampledImages );
		doGetValue( context, GL_MAX_COMBINED_IMAGE_UNIFORMS, m_properties.limits.maxDescriptorSetStorageImages );
		m_properties.limits.maxDescriptorSetInputAttachments = NonAvailable< uint32_t >;
		doGetValue( context, GL_MAX_VERTEX_ATTRIBS, m_properties.limits.maxVertexInputAttributes );
		doGetValue( context, GL_MAX_VERTEX_ATTRIB_BINDINGS, m_properties.limits.maxVertexInputBindings );
		doGetValue( context, GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET, m_properties.limits.maxVertexInputAttributeOffset );
		doGetValue( context, GL_MAX_VERTEX_ATTRIB_STRIDE, m_properties.limits.maxVertexInputBindingStride );
		doGetValue( context, GL_MAX_VERTEX_OUTPUT_COMPONENTS, m_properties.limits.maxVertexOutputComponents );
		doGetValue( context, GL_MAX_TESS_GEN_LEVEL, m_properties.limits.maxTessellationGenerationLevel );
		doGetValue( context, GL_MAX_PATCH_VERTICES, m_properties.limits.maxTessellationPatchSize );
		doGetValue( context, GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS, m_properties.limits.maxTessellationControlPerVertexInputComponents );
		doGetValue( context, GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS, m_properties.limits.maxTessellationControlPerVertexOutputComponents );
		doGetValue( context, GL_MAX_TESS_PATCH_COMPONENTS, m_properties.limits.maxTessellationControlPerPatchOutputComponents );
		doGetValue( context, GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS, m_properties.limits.maxTessellationControlTotalOutputComponents );
		doGetValue( context, GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS, m_properties.limits.maxTessellationEvaluationInputComponents );
		doGetValue( context, GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS, m_properties.limits.maxTessellationEvaluationOutputComponents );
		doGetValue( context, GL_MAX_GEOMETRY_SHADER_INVOCATIONS, m_properties.limits.maxGeometryShaderInvocations );
		doGetValue( context, GL_MAX_GEOMETRY_INPUT_COMPONENTS, m_properties.limits.maxGeometryInputComponents );
		doGetValue( context, GL_MAX_GEOMETRY_OUTPUT_COMPONENTS, m_properties.limits.maxGeometryOutputComponents );
		doGetValue( context, GL_MAX_GEOMETRY_OUTPUT_VERTICES, m_properties.limits.maxGeometryOutputVertices );
		doGetValue( context, GL_MAX_GEOMETRY_OUTPUT_COMPONENTS, m_properties.limits.maxGeometryTotalOutputComponents );
		doGetValue( context, GL_MAX_FRAGMENT_INPUT_COMPONENTS, m_properties.limits.maxFragmentInputComponents );
		doGetValue( context, GL_MAX_DRAW_BUFFERS, m_properties.limits.maxFragmentOutputAttachments );
		doGetValue( context, GL_MAX_DUAL_SOURCE_DRAW_BUFFERS, m_properties.limits.maxFragmentDualSrcAttachments );
		doGetValue( context, GL_MAX_COLOR_ATTACHMENTS, m_properties.limits.maxFragmentCombinedOutputResources );
		doGetValue( context, GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, m_properties.limits.maxComputeSharedMemorySize );
		doGetValuesI( context, GL_MAX_COMPUTE_WORK_GROUP_COUNT, m_properties.limits.maxComputeWorkGroupCount );
		doGetValue( context, GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, m_properties.limits.maxComputeWorkGroupInvocations );
		doGetValuesI( context, GL_MAX_COMPUTE_WORK_GROUP_SIZE, m_properties.limits.maxComputeWorkGroupSize );
		m_properties.limits.subPixelPrecisionBits = NonAvailable< uint32_t >;
		m_properties.limits.subTexelPrecisionBits = NonAvailable< uint32_t >;
		m_properties.limits.mipmapPrecisionBits = NonAvailable< uint32_t >;
		doGetValue( context, GL_MAX_ELEMENTS_INDICES, m_properties.limits.maxDrawIndexedIndexValue );
		m_properties.limits.maxDrawIndirectCount = NonAvailable< uint32_t >;
		doGetValue( context, GL_MAX_TEXTURE_LOD_BIAS, m_properties.limits.maxSamplerLodBias );
		doGetValue( context, GL_MAX_TEXTURE_MAX_ANISOTROPY, m_properties.limits.maxSamplerAnisotropy );
		doGetValue( context, GL_MAX_VIEWPORTS, m_properties.limits.maxViewports );
		doGetValues( context, GL_MAX_VIEWPORT_DIMS, m_properties.limits.maxViewportDimensions );
		doGetValues( context, GL_MAX_VIEWPORT_DIMS, m_properties.limits.viewportBoundsRange );
		m_properties.limits.viewportSubPixelBits = NonAvailable< uint32_t >;
		m_properties.limits.minMemoryMapAlignment = NonAvailable< size_t >;
		m_properties.limits.minTexelBufferOffsetAlignment = NonAvailable< uint64_t >;
		doGetValue( context, GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, m_properties.limits.minUniformBufferOffsetAlignment );
		m_properties.limits.minStorageBufferOffsetAlignment = NonAvailable< uint64_t >;
		doGetValue( context, GL_MIN_PROGRAM_TEXEL_OFFSET, m_properties.limits.minTexelOffset );
		doGetValue( context, GL_MAX_PROGRAM_TEXEL_OFFSET, m_properties.limits.maxTexelOffset );
		doGetValue( context, GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET, m_properties.limits.minTexelGatherOffset );
		doGetValue( context, GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET, m_properties.limits.maxTexelGatherOffset );
		m_properties.limits.minInterpolationOffset = NonAvailable< float >;
		m_properties.limits.maxInterpolationOffset = NonAvailable< float >;
		m_properties.limits.subPixelInterpolationOffsetBits = NonAvailable< uint32_t >;
		doGetValue( context, GL_MAX_FRAMEBUFFER_WIDTH, m_properties.limits.maxFramebufferWidth );
		doGetValue( context, GL_MAX_FRAMEBUFFER_HEIGHT, m_properties.limits.maxFramebufferHeight );
		doGetValue( context, GL_MAX_FRAMEBUFFER_LAYERS, m_properties.limits.maxFramebufferLayers );
		doGetValue( context, GL_MAX_FRAMEBUFFER_SAMPLES, *reinterpret_cast< uint32_t * >( &m_properties.limits.framebufferColorSampleCounts ) );
		doGetValue( context, GL_MAX_FRAMEBUFFER_SAMPLES, *reinterpret_cast< uint32_t * >( &m_properties.limits.framebufferDepthSampleCounts ) );
		doGetValue( context, GL_MAX_FRAMEBUFFER_SAMPLES, *reinterpret_cast< uint32_t * >( &m_properties.limits.framebufferStencilSampleCounts ) );
		doGetValue( context, GL_MAX_FRAMEBUFFER_SAMPLES, *reinterpret_cast< uint32_t * >( &m_properties.limits.framebufferNoAttachmentsSampleCounts ) );
		doGetValue( context, GL_MAX_COLOR_ATTACHMENTS, m_properties.limits.maxColorAttachments );
		doGetValue( context, GL_MAX_COLOR_TEXTURE_SAMPLES, *reinterpret_cast< uint32_t * >( &m_properties.limits.sampledImageColorSampleCounts ) );
		doGetValue( context, GL_MAX_COLOR_TEXTURE_SAMPLES, *reinterpret_cast< uint32_t * >( &m_properties.limits.sampledImageIntegerSampleCounts ) );
		doGetValue( context, GL_MAX_COLOR_TEXTURE_SAMPLES, *reinterpret_cast< uint32_t * >( &m_properties.limits.sampledImageDepthSampleCounts ) );
		doGetValue( context, GL_MAX_COLOR_TEXTURE_SAMPLES, *reinterpret_cast< uint32_t * >( &m_properties.limits.sampledImageStencilSampleCounts ) );
		doGetValue( context, GL_MAX_COLOR_TEXTURE_SAMPLES, *reinterpret_cast< uint32_t * >( &m_properties.limits.storageImageSampleCounts ) );
		doGetValue( context, GL_MAX_SAMPLE_MASK_WORDS, m_properties.limits.maxSampleMaskWords );
		m_properties.limits.timestampComputeAndGraphics = true;
		m_properties.limits.timestampPeriod = 1;
		doGetValue( context, GL_MAX_CLIP_DISTANCES, m_properties.limits.maxClipDistances );
		doGetValue( context, GL_MAX_CULL_DISTANCES, m_properties.limits.maxCullDistances );
		doGetValue( context, GL_MAX_COMBINED_CLIP_AND_CULL_DISTANCES, m_properties.limits.maxCombinedClipAndCullDistances );
		m_properties.limits.discreteQueuePriorities = 2u;
		doGetValues( context, GL_POINT_SIZE_RANGE, m_properties.limits.pointSizeRange );
		doGetValues( context, GL_ALIASED_LINE_WIDTH_RANGE, m_properties.limits.lineWidthRange );
		doGetValue( context, GL_POINT_SIZE_GRANULARITY, m_properties.limits.pointSizeGranularity );
		doGetValue( context, GL_LINE_WIDTH_GRANULARITY, m_properties.limits.lineWidthGranularity );
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
	}

	void PhysicalDevice::doInitialiseQueueProperties( ContextLock & context )
	{
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
	}

	void PhysicalDevice::doInitialiseFormatProperties( ContextLock & context )
	{
		if ( context->m_glGetInternalformativ )
		{
			for ( VkFormat fmt = VK_FORMAT_BEGIN_RANGE; fmt < VK_FORMAT_END_RANGE; fmt = VkFormat( fmt + 1 ) )
			{
				if ( isSupportedInternal( fmt ) )
				{
					auto internal = getInternalFormat( fmt );
					GLint value;
					glLogCall( context, glGetInternalformativ
						, GL_TEXTURE_2D
						, internal
						, GL_INTERNALFORMAT_SUPPORTED
						, 1
						, &value );

					if ( value == GL_TRUE )
					{
#if defined( VK_KHR_maintenance ) || defined( VK_API_VERSION_1_1 )
						m_formatProperties[fmt].optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_SRC_BIT;
						m_formatProperties[fmt].optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_DST_BIT;
#endif
						glLogCall( context, glGetInternalformativ
							, GL_TEXTURE_2D
							, internal
							, GL_FRAMEBUFFER_RENDERABLE
							, 1
							, &value );

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

						glLogCall( context, glGetInternalformativ
							, GL_TEXTURE_2D
							, internal
							, GL_FRAMEBUFFER_BLEND
							, 1
							, &value );

						if ( value == GL_FULL_SUPPORT )
						{
							m_formatProperties[fmt].optimalTilingFeatures |= VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT;
							m_formatProperties[fmt].optimalTilingFeatures |= VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT;
						}

						glLogCall( context, glGetInternalformativ
							, GL_TEXTURE_2D
							, internal
							, GL_FRAGMENT_TEXTURE
							, 1
							, &value );

						if ( value == GL_FULL_SUPPORT )
						{
							m_formatProperties[fmt].optimalTilingFeatures |= VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT;
						}

						glLogCall( context, glGetInternalformativ
							, GL_TEXTURE_2D
							, internal
							, GL_FILTER
							, 1
							, &value );

						if ( value == GL_FULL_SUPPORT )
						{
							m_formatProperties[fmt].optimalTilingFeatures |= VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT;
						}

						glLogCall( context, glGetInternalformativ
							, GL_TEXTURE_2D
							, internal
							, GL_SHADER_IMAGE_LOAD
							, 1
							, &value );

						if ( value == GL_FULL_SUPPORT )
						{
							m_formatProperties[fmt].optimalTilingFeatures |= VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT;
						}

						glLogCall( context, glGetInternalformativ
							, GL_TEXTURE_2D
							, internal
							, GL_SHADER_IMAGE_ATOMIC
							, 1
							, &value );

						if ( value == GL_FULL_SUPPORT )
						{
							m_formatProperties[fmt].optimalTilingFeatures |= VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT;
						}

#if defined( VK_KHR_maintenance ) || defined( VK_API_VERSION_1_1 )
						glLogCall( context, glGetInternalformativ
							, GL_TEXTURE_2D
							, internal
							, GL_READ_PIXELS
							, 1
							, &value );

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

	void PhysicalDevice::doInitialiseDisplayProperties( ContextLock & context )
	{
#ifdef VK_KHR_display

		auto count = gl::getScreenCount();

		for ( uint32_t i = 0u; i < count; ++i )
		{
			std::string name;
			VkDisplayPropertiesKHR displayProps{};
			std::vector< VkDisplayModeParametersKHR > displayModesParams;
			gl::getScreenDesc( i, name, displayProps, displayModesParams );
			m_displayNames.emplace_back( std::move( name ) );
			displayProps.displayName = m_displayNames.back().c_str();
			allocate( displayProps.display
				, nullptr
				, displayProps
				, VK_FORMAT_R8G8B8A8_UNORM
				, i
				, displayModesParams );
			m_displays.push_back( displayProps );
		}

		for ( auto & display : m_displays )
		{
			m_displayPlanes.push_back( { display.display, 0u } );
		}

#endif
	}

	void PhysicalDevice::doInitialiseProperties2( ContextLock & context )
	{
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

	void PhysicalDevice::doGetValue( ContextLock & context, GLenum name, int32_t & value )const
	{
		glLogCall( context, glGetIntegerv
			, name
			, &value );
	}

	void PhysicalDevice::doGetValue( ContextLock & context, GLenum name, uint32_t & value )const
	{
		int v;
		glLogCall( context, glGetIntegerv
			, name
			, &v );
		value = uint32_t( v );
	}

	void PhysicalDevice::doGetValues( ContextLock & context, GLenum name, int32_t( &value )[2] )const
	{
		glLogCall( context, glGetIntegerv
			, name
			, value );
	}

	void PhysicalDevice::doGetValues( ContextLock & context, GLenum name, uint32_t( &value )[2] )const
	{
		int v[2];
		glLogCall( context, glGetIntegerv
			, name
			, v );
		value[0] = v[0];
		value[1] = v[1];
	}

	void PhysicalDevice::doGetValues( ContextLock & context, GLenum name, int32_t( &value )[3] )const
	{
		glLogCall( context, glGetIntegerv
			, name
			, value );
	}

	void PhysicalDevice::doGetValues( ContextLock & context, GLenum name, uint32_t( &value )[3] )const
	{
		int v[3];
		glLogCall( context, glGetIntegerv
			, name
			, v );
		value[0] = v[0];
		value[1] = v[1];
		value[2] = v[2];
	}

	void PhysicalDevice::doGetValuesI( ContextLock & context, GLenum name, int32_t( &value )[2] )const
	{
		glLogCall( context, glGetIntegeri_v
			, name
			, 0
			, &value[0] );
		glLogCall( context, glGetIntegeri_v
			, name
			, 1
			, &value[1] );
	}

	void PhysicalDevice::doGetValuesI( ContextLock & context, GLenum name, uint32_t( &value )[2] )const
	{
		int v[2];
		glLogCall( context, glGetIntegeri_v
			, name
			, 0
			, &v[0] );
		glLogCall( context, glGetIntegeri_v
			, name
			, 1
			, &v[1] );
		value[0] = v[0];
		value[1] = v[1];
	}

	void PhysicalDevice::doGetValuesI( ContextLock & context, GLenum name, int32_t( &value )[3] )const
	{
		glLogCall( context, glGetIntegeri_v
			, name
			, 0
			, &value[0] );
		glLogCall( context, glGetIntegeri_v
			, name
			, 1
			, &value[1] );
		glLogCall( context, glGetIntegeri_v
			, name
			, 2
			, &value[2] );
	}

	void PhysicalDevice::doGetValuesI( ContextLock & context, GLenum name, uint32_t( &value )[3] )const
	{
		int v[3];
		glLogCall( context, glGetIntegeri_v
			, name
			, 0
			, &v[0] );
		glLogCall( context, glGetIntegeri_v
			, name
			, 1
			, &v[1] );
		glLogCall( context, glGetIntegeri_v
			, name
			, 2
			, &v[2] );
		value[0] = v[0];
		value[1] = v[1];
		value[2] = v[2];
	}

	void PhysicalDevice::doGetValue( ContextLock & context, GLenum name, int64_t & value )const
	{
		glLogCall( context, glGetInteger64v
			, name
			, &value );
	}

	void PhysicalDevice::doGetValue( ContextLock & context, GLenum name, uint64_t & value )const
	{
		int64_t v;
		glLogCall( context, glGetInteger64v
			, name
			, &v );
		value = uint64_t( v );
	}

	void PhysicalDevice::doGetValues( ContextLock & context, GLenum name, int64_t( &value )[2] )const
	{
		glLogCall( context, glGetInteger64v
			, name
			, value );
	}

	void PhysicalDevice::doGetValues( ContextLock & context, GLenum name, uint64_t( &value )[2] )const
	{
		int64_t v[2];
		glLogCall( context, glGetInteger64v
			, name
			, v );
		value[0] = v[0];
		value[1] = v[1];
	}

	void PhysicalDevice::doGetValues( ContextLock & context, GLenum name, int64_t( &value )[3] )const
	{
		glLogCall( context, glGetInteger64v
			, name
			, value );
	}

	void PhysicalDevice::doGetValues( ContextLock & context, GLenum name, uint64_t( &value )[3] )const
	{
		int64_t v[3];
		glLogCall( context, glGetInteger64v
			, name
			, v );
		value[0] = v[0];
		value[1] = v[1];
		value[2] = v[2];
	}

	void PhysicalDevice::doGetValuesI( ContextLock & context, GLenum name, int64_t( &value )[2] )const
	{
		glLogCall( context, glGetInteger64i_v
			, name
			, 0
			, &value[0] );
		glLogCall( context, glGetInteger64i_v
			, name
			, 1
			, &value[1] );
	}

	void PhysicalDevice::doGetValuesI( ContextLock & context, GLenum name, uint64_t( &value )[2] )const
	{
		int64_t v[2];
		glLogCall( context, glGetInteger64i_v
			, name
			, 0
			, &v[0] );
		glLogCall( context, glGetInteger64i_v
			, name
			, 1
			, &v[1] );
		value[0] = v[0];
		value[1] = v[1];
	}

	void PhysicalDevice::doGetValuesI( ContextLock & context, GLenum name, int64_t( &value )[3] )const
	{
		glLogCall( context, glGetInteger64i_v
			, name
			, 0
			, &value[0] );
		glLogCall( context, glGetInteger64i_v
			, name
			, 1
			, &value[1] );
		glLogCall( context, glGetInteger64i_v
			, name
			, 2
			, &value[2] );
	}

	void PhysicalDevice::doGetValuesI( ContextLock & context, GLenum name, uint64_t( &value )[3] )const
	{
		int64_t v[3];
		glLogCall( context, glGetInteger64i_v
			, name
			, 0
			, &v[0] );
		glLogCall( context, glGetInteger64i_v
			, name
			, 1
			, &v[1] );
		glLogCall( context, glGetInteger64i_v
			, name
			, 2
			, &v[2] );
		value[0] = v[0];
		value[1] = v[1];
		value[2] = v[2];
	}

	void PhysicalDevice::doGetValue( ContextLock & context, GLenum name, float & value )const
	{
		glLogCall( context, glGetFloatv
			, name
			, &value );
	}

	void PhysicalDevice::doGetValues( ContextLock & context, GLenum name, float( &value )[2] )const
	{
		glLogCall( context, glGetFloatv
			, name
			, value );
	}

	void PhysicalDevice::doGetValues( ContextLock & context, GLenum name, float( &value )[3] )const
	{
		glLogCall( context, glGetFloatv
			, name
			, value );
	}

	void PhysicalDevice::doGetValuesI( ContextLock & context, GLenum name, float( &value )[2] )const
	{
		glLogCall( context, glGetFloati_v
			, name
			, 0
			, &value[0] );
		glLogCall( context, glGetFloati_v
			, name
			, 1
			, &value[1] );
	}

	void PhysicalDevice::doGetValuesI( ContextLock & context, GLenum name, float( &value )[3] )const
	{
		glLogCall( context, glGetFloati_v
			, name
			, 0
			, &value[0] );
		glLogCall( context, glGetFloati_v
			, name
			, 1
			, &value[1] );
		glLogCall( context, glGetFloati_v
			, name
			, 2
			, &value[2] );
	}
}

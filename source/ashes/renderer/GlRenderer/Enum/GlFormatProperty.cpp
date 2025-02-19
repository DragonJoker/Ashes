#include "GlRendererPrerequisites.hpp"

namespace ashes::gl
{
	std::string getName( GlFormatProperty value )
	{
		switch ( value )
		{
		case GL_FORMAT_PROPERTY_SAMPLES:
			return "GL_SAMPLES";
		case GL_FORMAT_PROPERTY_IS_SUPPORTED:
			return "GL_INTERNALFORMAT_SUPPORTED";
		case GL_FORMAT_PROPERTY_MAX_WIDTH:
			return "GL_MAX_WIDTH";
		case GL_FORMAT_PROPERTY_MAX_HEIGHT:
			return "GL_MAX_HEIGHT";
		case GL_FORMAT_PROPERTY_MAX_DEPTH:
			return "GL_MAX_DEPTH";
		case GL_FORMAT_PROPERTY_MAX_LAYERS:
			return "GL_MAX_LAYERS";
		case GL_FORMAT_PROPERTY_TEXEL_SIZE:
			return "GL_IMAGE_TEXEL_SIZE";
		case GL_FORMAT_PROPERTY_COLOR_RENDERABLE:
			return "GL_FRAMEBUFFER_COLOR_RENDERABLE";
		case GL_FORMAT_PROPERTY_DEPTH_RENDERABLE:
			return "GL_FRAMEBUFFER_DEPTH_RENDERABLE";
		case GL_FORMAT_PROPERTY_STENCIL_RENDERABLE:
			return "GL_FRAMEBUFFER_STENCIL_RENDERABLE";
		case GL_FORMAT_PROPERTY_RENDERABLE:
			return "GL_FRAMEBUFFER_RENDERABLE";
		case GL_FORMAT_PROPERTY_RENDERABLE_LAYERED:
			return "GL_FRAMEBUFFER_RENDERABLE_LAYERED";
		case GL_FORMAT_PROPERTY_BLEND:
			return "GL_FRAMEBUFFER_BLEND";
		case GL_FORMAT_PROPERTY_NUM_SAMPLE_COUNTS:
			return "GL_NUM_SAMPLE_COUNTS";
		case GL_FORMAT_PROPERTY_PREFERRED:
			return "GL_PREFERRED";
		case GL_FORMAT_PROPERTY_RED_SIZE:
			return "GL_RED_SIZE";
		case GL_FORMAT_PROPERTY_GREEN_SIZE:
			return "GL_GREEN_SIZE";
		case GL_FORMAT_PROPERTY_BLUE_SIZE:
			return "GL_BLUE_SIZE";
		case GL_FORMAT_PROPERTY_ALPHA_SIZE:
			return "GL_ALPHA_SIZE";
		case GL_FORMAT_PROPERTY_DEPTH_SIZE:
			return "GL_DEPTH_SIZE";
		case GL_FORMAT_PROPERTY_STENCIL_SIZE:
			return "GL_STENCIL_SIZE";
		case GL_FORMAT_PROPERTY_SHARED_SIZE:
			return "GL_SHARED_SIZE";
		case GL_FORMAT_PROPERTY_RED_TYPE:
			return "GL_RED_TYPE";
		case GL_FORMAT_PROPERTY_GREEN_TYPE:
			return "GL_GREEN_TYPE";
		case GL_FORMAT_PROPERTY_BLUE_TYPE:
			return "GL_BLUE_TYPE";
		case GL_FORMAT_PROPERTY_ALPHA_TYPE:
			return "GL_ALPHA_TYPE";
		case GL_FORMAT_PROPERTY_DEPTH_TYPE:
			return "GL_DEPTH_TYPE";
		case GL_FORMAT_PROPERTY_STENCIL_TYPE:
			return "GL_STENCIL_TYPE";
		case GL_FORMAT_PROPERTY_MAX_COMBINED_DIMENSIONS:
			return "GL_MAX_COMBINED_DIMENSIONS";
		case GL_FORMAT_PROPERTY_COLOR_COMPONENTS:
			return "GL_COLOR_COMPONENTS";
		case GL_FORMAT_PROPERTY_DEPTH_COMPONENTS:
			return "GL_DEPTH_COMPONENTS";
		case GL_FORMAT_PROPERTY_STENCIL_COMPONENTS:
			return "GL_STENCIL_COMPONENTS";
		case GL_FORMAT_PROPERTY_READ_PIXELS:
			return "GL_READ_PIXELS";
		case GL_FORMAT_PROPERTY_READ_PIXELS_FORMAT:
			return "GL_READ_PIXELS_FORMAT";
		case GL_FORMAT_PROPERTY_READ_PIXELS_TYPE:
			return "GL_READ_PIXELS_TYPE";
		case GL_FORMAT_PROPERTY_TEXTURE_IMAGE_FORMAT:
			return "GL_TEXTURE_IMAGE_FORMAT";
		case GL_FORMAT_PROPERTY_TEXTURE_IMAGE_TYPE:
			return "GL_TEXTURE_IMAGE_TYPE";
		case GL_FORMAT_PROPERTY_GET_TEXTURE_IMAGE_FORMAT:
			return "GL_GET_TEXTURE_IMAGE_FORMAT";
		case GL_FORMAT_PROPERTY_GET_TEXTURE_IMAGE_TYPE:
			return "GL_GET_TEXTURE_IMAGE_TYPE";
		case GL_FORMAT_PROPERTY_MIPMAP:
			return "GL_MIPMAP";
		case GL_FORMAT_PROPERTY_MANUAL_GENERATE_MIPMAP:
			return "GL_MANUAL_GENERATE_MIPMAP";
		case GL_FORMAT_PROPERTY_AUTO_GENERATE_MIPMAP:
			return "GL_AUTO_GENERATE_MIPMAP";
		case GL_FORMAT_PROPERTY_COLOR_ENCODING:
			return "GL_COLOR_ENCODING";
		case GL_FORMAT_PROPERTY_SRGB_READ:
			return "GL_SRGB_READ";
		case GL_FORMAT_PROPERTY_SRGB_WRITE:
			return "GL_SRGB_WRITE";
		case GL_FORMAT_PROPERTY_SRGB_DECODE_ARB:
			return "GL_SRGB_DECODE_ARB";
		case GL_FORMAT_PROPERTY_FILTER:
			return "GL_FILTER";
		case GL_FORMAT_PROPERTY_VERTEX_TEXTURE:
			return "GL_VERTEX_TEXTURE";
		case GL_FORMAT_PROPERTY_TESS_CONTROL_TEXTURE:
			return "GL_TESS_CONTROL_TEXTURE";
		case GL_FORMAT_PROPERTY_TESS_EVALUATION_TEXTURE:
			return "GL_TESS_EVALUATION_TEXTURE";
		case GL_FORMAT_PROPERTY_GEOMETRY_TEXTURE:
			return "GL_GEOMETRY_TEXTURE";
		case GL_FORMAT_PROPERTY_FRAGMENT_TEXTURE:
			return "GL_FRAGMENT_TEXTURE";
		case GL_FORMAT_PROPERTY_COMPUTE_TEXTURE:
			return "GL_COMPUTE_TEXTURE";
		case GL_FORMAT_PROPERTY_TEXTURE_SHADOW:
			return "GL_TEXTURE_SHADOW";
		case GL_FORMAT_PROPERTY_TEXTURE_GATHER:
			return "GL_TEXTURE_GATHER";
		case GL_FORMAT_PROPERTY_TEXTURE_GATHER_SHADOW:
			return "GL_TEXTURE_GATHER_SHADOW";
		case GL_FORMAT_PROPERTY_SHADER_IMAGE_LOAD:
			return "GL_SHADER_IMAGE_LOAD";
		case GL_FORMAT_PROPERTY_SHADER_IMAGE_STORE:
			return "GL_SHADER_IMAGE_STORE";
		case GL_FORMAT_PROPERTY_SHADER_IMAGE_ATOMIC:
			return "GL_SHADER_IMAGE_ATOMIC";
		case GL_FORMAT_PROPERTY_IMAGE_COMPATIBILITY_CLASS:
			return "GL_IMAGE_COMPATIBILITY_CLASS";
		case GL_FORMAT_PROPERTY_IMAGE_PIXEL_FORMAT:
			return "GL_IMAGE_PIXEL_FORMAT";
		case GL_FORMAT_PROPERTY_IMAGE_PIXEL_TYPE:
			return "GL_IMAGE_PIXEL_TYPE";
		case GL_FORMAT_PROPERTY_IMAGE_FORMAT_COMPATIBILITY_TYPE:
			return "GL_IMAGE_FORMAT_COMPATIBILITY_TYPE";
		case GL_FORMAT_PROPERTY_SIMULTANEOUS_TEXTURE_AND_DEPTH_TEST:
			return "GL_SIMULTANEOUS_TEXTURE_AND_DEPTH_TEST";
		case GL_FORMAT_PROPERTY_SIMULTANEOUS_TEXTURE_AND_STENCIL_TEST:
			return "GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_TEST";
		case GL_FORMAT_PROPERTY_SIMULTANEOUS_TEXTURE_AND_DEPTH_WRITE:
			return "GL_SIMULTANEOUS_TEXTURE_AND_DEPTH_WRITE";
		case GL_FORMAT_PROPERTY_SIMULTANEOUS_TEXTURE_AND_STENCIL_WRITE:
			return "GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_WRITE";
		case GL_FORMAT_PROPERTY_TEXTURE_COMPRESSED:
			return "GL_TEXTURE_COMPRESSED";
		case GL_FORMAT_PROPERTY_TEXTURE_COMPRESSED_BLOCK_WIDTH:
			return "GL_TEXTURE_COMPRESSED_BLOCK_WIDTH";
		case GL_FORMAT_PROPERTY_TEXTURE_COMPRESSED_BLOCK_HEIGHT:
			return "GL_TEXTURE_COMPRESSED_BLOCK_HEIGHT";
		case GL_FORMAT_PROPERTY_TEXTURE_COMPRESSED_BLOCK_SIZE:
			return "GL_TEXTURE_COMPRESSED_BLOCK_SIZE";
		case GL_FORMAT_PROPERTY_CLEAR_BUFFER:
			return "GL_CLEAR_BUFFER";
		case GL_FORMAT_PROPERTY_TEXTURE_VIEW:
			return "GL_TEXTURE_VIEW";
		case GL_FORMAT_PROPERTY_VIEW_COMPATIBILITY_CLASS:
			return "GL_VIEW_COMPATIBILITY_CLASS";
		default:
			assert( false && "Unsupported GlFormatProperty" );
			return "GlFormatProperty_UNKNOWN";
		}
	}

	std::string getName( GlFormatPropertyResult value )
	{
		switch ( value )
		{
		case GL_FORMAT_PROPERTY_UNSUPPORTED:
			return "GL_NONE";
		case GL_FORMAT_PROPERTY_SUPPORTED:
			return "GL_TRUE";
		case GL_FORMAT_PROPERTY_FULL_SUPPORT:
			return "GL_FULL_SUPPORT";
		case GL_FORMAT_PROPERTY_CAVEAT_SUPPORT:
			return "GL_CAVEAT_SUPPORT";
		default:
			assert( false && "Unsupported GlFormatPropertyResult" );
			return "GlFormatPropertyResult_UNKNOWN";
		}
	}
}

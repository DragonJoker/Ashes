/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

namespace ashes::gl
{
	enum GlFormatProperty
		: GLenum
	{
		GL_FORMAT_PROPERTY_SAMPLES = 0x80A9, // GL_SAMPLES
		GL_FORMAT_PROPERTY_IS_SUPPORTED = 0x826F, // GL_INTERNALFORMAT_SUPPORTED
		GL_FORMAT_PROPERTY_MAX_WIDTH = 0x827E, // GL_MAX_WIDTH
		GL_FORMAT_PROPERTY_MAX_HEIGHT = 0x827F, // GL_MAX_HEIGHT
		GL_FORMAT_PROPERTY_MAX_DEPTH = 0x8280, // GL_MAX_DEPTH
		GL_FORMAT_PROPERTY_MAX_LAYERS = 0x8281, // GL_MAX_LAYERS
		GL_FORMAT_PROPERTY_TEXEL_SIZE = 0x82A7, // GL_IMAGE_TEXEL_SIZE
		GL_FORMAT_PROPERTY_COLOR_RENDERABLE = 0x8286,
		GL_FORMAT_PROPERTY_DEPTH_RENDERABLE = 0x8287,
		GL_FORMAT_PROPERTY_STENCIL_RENDERABLE = 0x8288,
		GL_FORMAT_PROPERTY_RENDERABLE = 0x8289, // GL_FRAMEBUFFER_RENDERABLE
		GL_FORMAT_PROPERTY_RENDERABLE_LAYERED = 0x828A, // GL_FRAMEBUFFER_RENDERABLE_LAYERED
		GL_FORMAT_PROPERTY_BLEND = 0x828B,
		GL_FORMAT_PROPERTY_NUM_SAMPLE_COUNTS = 0x9380,
		GL_FORMAT_PROPERTY_PREFERRED = 0x8270,
		GL_FORMAT_PROPERTY_RED_SIZE = 0x8271,
		GL_FORMAT_PROPERTY_GREEN_SIZE = 0x8272,
		GL_FORMAT_PROPERTY_BLUE_SIZE = 0x8273,
		GL_FORMAT_PROPERTY_ALPHA_SIZE = 0x8274,
		GL_FORMAT_PROPERTY_DEPTH_SIZE = 0x8275,
		GL_FORMAT_PROPERTY_STENCIL_SIZE = 0x8276,
		GL_FORMAT_PROPERTY_SHARED_SIZE = 0x8277,
		GL_FORMAT_PROPERTY_RED_TYPE = 0x8278,
		GL_FORMAT_PROPERTY_GREEN_TYPE = 0x8279,
		GL_FORMAT_PROPERTY_BLUE_TYPE = 0x827A,
		GL_FORMAT_PROPERTY_ALPHA_TYPE = 0x827B,
		GL_FORMAT_PROPERTY_DEPTH_TYPE = 0x827C,
		GL_FORMAT_PROPERTY_STENCIL_TYPE = 0x827D,
		GL_FORMAT_PROPERTY_MAX_COMBINED_DIMENSIONS = 0x8282,
		GL_FORMAT_PROPERTY_COLOR_COMPONENTS = 0x8283,
		GL_FORMAT_PROPERTY_DEPTH_COMPONENTS = 0x8284,
		GL_FORMAT_PROPERTY_STENCIL_COMPONENTS = 0x8285,
		GL_FORMAT_PROPERTY_READ_PIXELS = 0x828C,
		GL_FORMAT_PROPERTY_READ_PIXELS_FORMAT = 0x828D,
		GL_FORMAT_PROPERTY_READ_PIXELS_TYPE = 0x828E,
		GL_FORMAT_PROPERTY_TEXTURE_IMAGE_FORMAT = 0x828F,
		GL_FORMAT_PROPERTY_TEXTURE_IMAGE_TYPE = 0x8290,
		GL_FORMAT_PROPERTY_GET_TEXTURE_IMAGE_FORMAT = 0x8291,
		GL_FORMAT_PROPERTY_GET_TEXTURE_IMAGE_TYPE = 0x8292,
		GL_FORMAT_PROPERTY_MIPMAP = 0x8293,
		GL_FORMAT_PROPERTY_MANUAL_GENERATE_MIPMAP = 0x8294,
		GL_FORMAT_PROPERTY_AUTO_GENERATE_MIPMAP = 0x8295,
		GL_FORMAT_PROPERTY_COLOR_ENCODING = 0x8296,
		GL_FORMAT_PROPERTY_SRGB_READ = 0x8297,
		GL_FORMAT_PROPERTY_SRGB_WRITE = 0x8298,
		GL_FORMAT_PROPERTY_SRGB_DECODE_ARB = 0x8299,
		GL_FORMAT_PROPERTY_FILTER = 0x829A,
		GL_FORMAT_PROPERTY_VERTEX_TEXTURE = 0x829B,
		GL_FORMAT_PROPERTY_TESS_CONTROL_TEXTURE = 0x829C,
		GL_FORMAT_PROPERTY_TESS_EVALUATION_TEXTURE = 0x829D,
		GL_FORMAT_PROPERTY_GEOMETRY_TEXTURE = 0x829E,
		GL_FORMAT_PROPERTY_FRAGMENT_TEXTURE = 0x829F,
		GL_FORMAT_PROPERTY_COMPUTE_TEXTURE = 0x82A0,
		GL_FORMAT_PROPERTY_TEXTURE_SHADOW = 0x82A1,
		GL_FORMAT_PROPERTY_TEXTURE_GATHER = 0x82A2,
		GL_FORMAT_PROPERTY_TEXTURE_GATHER_SHADOW = 0x82A3,
		GL_FORMAT_PROPERTY_SHADER_IMAGE_LOAD = 0x82A4,
		GL_FORMAT_PROPERTY_SHADER_IMAGE_STORE = 0x82A5,
		GL_FORMAT_PROPERTY_SHADER_IMAGE_ATOMIC = 0x82A6,
		GL_FORMAT_PROPERTY_IMAGE_COMPATIBILITY_CLASS = 0x82A8,
		GL_FORMAT_PROPERTY_IMAGE_PIXEL_FORMAT = 0x82A9,
		GL_FORMAT_PROPERTY_IMAGE_PIXEL_TYPE = 0x82AA,
		GL_FORMAT_PROPERTY_IMAGE_FORMAT_COMPATIBILITY_TYPE = 0x90C7,
		GL_FORMAT_PROPERTY_SIMULTANEOUS_TEXTURE_AND_DEPTH_TEST = 0x82AC,
		GL_FORMAT_PROPERTY_SIMULTANEOUS_TEXTURE_AND_STENCIL_TEST = 0x82AD,
		GL_FORMAT_PROPERTY_SIMULTANEOUS_TEXTURE_AND_DEPTH_WRITE = 0x82AE,
		GL_FORMAT_PROPERTY_SIMULTANEOUS_TEXTURE_AND_STENCIL_WRITE = 0x82AF,
		GL_FORMAT_PROPERTY_TEXTURE_COMPRESSED = 0x86A1,
		GL_FORMAT_PROPERTY_TEXTURE_COMPRESSED_BLOCK_WIDTH = 0x82B1,
		GL_FORMAT_PROPERTY_TEXTURE_COMPRESSED_BLOCK_HEIGHT = 0x82B2,
		GL_FORMAT_PROPERTY_TEXTURE_COMPRESSED_BLOCK_SIZE = 0x82B3,
		GL_FORMAT_PROPERTY_CLEAR_BUFFER = 0x82B4,
		GL_FORMAT_PROPERTY_TEXTURE_VIEW = 0x82B5,
		GL_FORMAT_PROPERTY_VIEW_COMPATIBILITY_CLASS = 0x82B6,
	};

	std::string getName( GlFormatProperty value );
	inline std::string toString( GlFormatProperty value ) { return getName( value ); }

	enum GlFormatPropertyResult
		: GLint
	{
		GL_FORMAT_PROPERTY_UNSUPPORTED = 0,
		GL_FORMAT_PROPERTY_SUPPORTED = 1,
		GL_FORMAT_PROPERTY_FULL_SUPPORT = 0x82B7,
		GL_FORMAT_PROPERTY_CAVEAT_SUPPORT = 0x82B8,
	};

	std::string getName( GlFormatPropertyResult value );
	inline std::string toString( GlFormatPropertyResult value ){ return getName( value ); }
}

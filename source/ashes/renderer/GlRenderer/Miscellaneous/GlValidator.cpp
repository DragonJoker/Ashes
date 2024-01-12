#include "Miscellaneous/GlValidator.hpp"

#include "Miscellaneous/GlValidatorInterfaceQuery.hpp"
#include "Miscellaneous/GlValidatorOldStyle.hpp"

#include "Core/GlDevice.hpp"
#include "Pipeline/GlPipelineLayout.hpp"
#include "RenderPass/GlRenderPass.hpp"

#include "ashesgl_api.hpp"

#include <algorithm>
#include <sstream>

namespace ashes::gl
{
	namespace
	{
		std::string getName( VkVertexInputRate value )
		{
			switch ( value )
			{
			case VK_VERTEX_INPUT_RATE_VERTEX:
				return "vertex";
			case VK_VERTEX_INPUT_RATE_INSTANCE:
				return "instance";
			default:
				assert( false && "Unsupported VkVertexInputRate" );
				return "Unknown";
			}
		}

		void validateInputs( ContextLock const & context
			, GLuint program
			, VkPipelineVertexInputStateCreateInfo const & vertexInputState )
		{
			auto inputs = getInputs( context
				, VK_SHADER_STAGE_VERTEX_BIT
				, program );
			auto attribsEnd = vertexInputState.pVertexAttributeDescriptions + vertexInputState.vertexAttributeDescriptionCount;

			for ( auto const & input : inputs.vertexAttributeDescriptions )
			{
				auto it = std::find( vertexInputState.pVertexAttributeDescriptions
					, attribsEnd
					, input );

				if ( it == attribsEnd )
				{
					std::stringstream stream;
					stream << "Attribute"
						<< " of type: " << ashes::getName( input.format )
						<< ", at binding: " << input.binding
						<< ", at location: " << input.location
						<< ", bin. offset: " << input.offset
						<< " is used in the shader program, but is not listed in the vertex layouts" << std::endl;
					context->reportMessage( VK_DEBUG_REPORT_ERROR_BIT_EXT
						, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_EXT
						, 0ULL
						, 0u
						, VK_ERROR_VALIDATION_FAILED_EXT
						, "OpenGL"
						, stream.str().c_str() );
				}
			}

			auto bindingsEnd = vertexInputState.pVertexBindingDescriptions + vertexInputState.vertexBindingDescriptionCount;

			for ( auto const & input : inputs.vertexBindingDescriptions )
			{
				auto it = std::find( vertexInputState.pVertexBindingDescriptions
					, bindingsEnd
					, input );

				if ( it == bindingsEnd )
				{
					std::stringstream stream;
					stream << "Attribute"
						<< " at binding: " << input.binding
						<< ", with input rate: " << getName( input.inputRate )
						<< ", bin. stride: " << input.stride
						<< " is used in the shader program, but is not listed in the vertex layouts" << std::endl;
					context->reportMessage( VK_DEBUG_REPORT_ERROR_BIT_EXT
						, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_EXT
						, 0ULL
						, 0u
						, VK_ERROR_VALIDATION_FAILED_EXT
						, "OpenGL"
						, stream.str().c_str() );
				}
			}
		}

		void validateOutputs( ContextLock const & context
			, GLuint program
			, VkRenderPass renderPass )
		{
			if ( hasProgramInterfaceQuery( context.getDevice() ) )
			{
				gl4::validateOutputs( context, program, renderPass );
			}
		}
	}

	std::string getName( GlslAttributeType type )noexcept
	{
		switch ( type )
		{
		case GLSL_ATTRIBUTE_INT:										return "GL_INT";
		case GLSL_ATTRIBUTE_UNSIGNED_INT:								return "GL_UNSIGNED_INT";
		case GLSL_ATTRIBUTE_FLOAT:										return "GL_FLOAT";
		case GLSL_ATTRIBUTE_DOUBLE:										return "GL_DOUBLE";
		case GLSL_ATTRIBUTE_HALF_FLOAT:									return "GL_HALF_FLOAT";
		case GLSL_ATTRIBUTE_FLOAT_VEC2:									return "GL_FLOAT_VEC2";
		case GLSL_ATTRIBUTE_FLOAT_VEC3:									return "GL_FLOAT_VEC3";
		case GLSL_ATTRIBUTE_FLOAT_VEC4:									return "GL_FLOAT_VEC4";
		case GLSL_ATTRIBUTE_INT_VEC2:									return "GL_INT_VEC2";
		case GLSL_ATTRIBUTE_INT_VEC3:									return "GL_INT_VEC3";
		case GLSL_ATTRIBUTE_INT_VEC4:									return "GL_INT_VEC4";
		case GLSL_ATTRIBUTE_BOOL:										return "GL_BOOL";
		case GLSL_ATTRIBUTE_BOOL_VEC2:									return "GL_BOOL_VEC2";
		case GLSL_ATTRIBUTE_BOOL_VEC3:									return "GL_BOOL_VEC3";
		case GLSL_ATTRIBUTE_BOOL_VEC4:									return "GL_BOOL_VEC4";
		case GLSL_ATTRIBUTE_FLOAT_MAT2:									return "GL_FLOAT_MAT2";
		case GLSL_ATTRIBUTE_FLOAT_MAT3:									return "GL_FLOAT_MAT3";
		case GLSL_ATTRIBUTE_FLOAT_MAT4:									return "GL_FLOAT_MAT4";
		case GLSL_ATTRIBUTE_SAMPLER_1D:									return "GL_SAMPLER_1D";
		case GLSL_ATTRIBUTE_SAMPLER_2D:									return "GL_SAMPLER_2D";
		case GLSL_ATTRIBUTE_SAMPLER_3D:									return "GL_SAMPLER_3D";
		case GLSL_ATTRIBUTE_SAMPLER_CUBE:								return "GL_SAMPLER_CUBE";
		case GLSL_ATTRIBUTE_SAMPLER_1D_SHADOW:							return "GL_SAMPLER_1D_SHADOW";
		case GLSL_ATTRIBUTE_SAMPLER_2D_SHADOW:							return "GL_SAMPLER_2D_SHADOW";
		case GLSL_ATTRIBUTE_SAMPLER_2D_RECT:							return "GL_SAMPLER_2D_RECT";
		case GLSL_ATTRIBUTE_SAMPLER_2D_RECT_SHADOW:						return "GL_SAMPLER_2D_RECT_SHADOW";
		case GLSL_ATTRIBUTE_FLOAT_MAT2x3:								return "GL_FLOAT_MAT2x3";
		case GLSL_ATTRIBUTE_FLOAT_MAT2x4:								return "GL_FLOAT_MAT2x4";
		case GLSL_ATTRIBUTE_FLOAT_MAT3x2:								return "GL_FLOAT_MAT3x2";
		case GLSL_ATTRIBUTE_FLOAT_MAT3x4:								return "GL_FLOAT_MAT3x4";
		case GLSL_ATTRIBUTE_FLOAT_MAT4x2:								return "GL_FLOAT_MAT4x2";
		case GLSL_ATTRIBUTE_FLOAT_MAT4x3:								return "GL_FLOAT_MAT4x3";
		case GLSL_ATTRIBUTE_SAMPLER_1D_ARRAY:							return "GL_SAMPLER_1D_ARRAY";
		case GLSL_ATTRIBUTE_SAMPLER_2D_ARRAY:							return "GL_SAMPLER_2D_ARRAY";
		case GLSL_ATTRIBUTE_SAMPLER_BUFFER:								return "GL_SAMPLER_BUFFER";
		case GLSL_ATTRIBUTE_SAMPLER_1D_ARRAY_SHADOW:					return "GL_SAMPLER_1D_ARRAY_SHADOW";
		case GLSL_ATTRIBUTE_SAMPLER_2D_ARRAY_SHADOW:					return "GL_SAMPLER_2D_ARRAY_SHADOW";
		case GLSL_ATTRIBUTE_SAMPLER_CUBE_SHADOW:						return "GL_SAMPLER_CUBE_SHADOW";
		case GLSL_ATTRIBUTE_UNSIGNED_INT_VEC2:							return "GL_UNSIGNED_INT_VEC2";
		case GLSL_ATTRIBUTE_UNSIGNED_INT_VEC3:							return "GL_UNSIGNED_INT_VEC3";
		case GLSL_ATTRIBUTE_UNSIGNED_INT_VEC4:							return "GL_UNSIGNED_INT_VEC4";
		case GLSL_ATTRIBUTE_INT_SAMPLER_1D:								return "GL_INT_SAMPLER_1D";
		case GLSL_ATTRIBUTE_INT_SAMPLER_2D:								return "GL_INT_SAMPLER_2D";
		case GLSL_ATTRIBUTE_INT_SAMPLER_3D:								return "GL_INT_SAMPLER_3D";
		case GLSL_ATTRIBUTE_INT_SAMPLER_CUBE:							return "GL_INT_SAMPLER_CUBE";
		case GLSL_ATTRIBUTE_INT_SAMPLER_2D_RECT:						return "GL_INT_SAMPLER_2D_RECT";
		case GLSL_ATTRIBUTE_INT_SAMPLER_1D_ARRAY:						return "GL_INT_SAMPLER_1D_ARRAY";
		case GLSL_ATTRIBUTE_INT_SAMPLER_2D_ARRAY:						return "GL_INT_SAMPLER_2D_ARRAY";
		case GLSL_ATTRIBUTE_INT_SAMPLER_BUFFER:							return "GL_INT_SAMPLER_BUFFER";
		case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_1D:					return "GL_UNSIGNED_INT_SAMPLER_1D";
		case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_2D:					return "GL_UNSIGNED_INT_SAMPLER_2D";
		case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_3D:					return "GL_UNSIGNED_INT_SAMPLER_3D";
		case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_CUBE:					return "GL_UNSIGNED_INT_SAMPLER_CUBE";
		case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_2D_RECT:				return "GL_UNSIGNED_INT_SAMPLER_2D_RECT";
		case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_1D_ARRAY:				return "GL_UNSIGNED_INT_SAMPLER_1D_ARRAY";
		case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_2D_ARRAY:				return "GL_UNSIGNED_INT_SAMPLER_2D_ARRAY";
		case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_BUFFER:				return "GL_UNSIGNED_INT_SAMPLER_BUFFER";
		case GLSL_ATTRIBUTE_DOUBLE_MAT2:								return "GL_DOUBLE_MAT2";
		case GLSL_ATTRIBUTE_DOUBLE_MAT3:								return "GL_DOUBLE_MAT3";
		case GLSL_ATTRIBUTE_DOUBLE_MAT4:								return "GL_DOUBLE_MAT4";
		case GLSL_ATTRIBUTE_DOUBLE_MAT2x3:								return "GL_DOUBLE_MAT2x3 ";
		case GLSL_ATTRIBUTE_DOUBLE_MAT2x4:								return "GL_DOUBLE_MAT2x4 ";
		case GLSL_ATTRIBUTE_DOUBLE_MAT3x2:								return "GL_DOUBLE_MAT3x2 ";
		case GLSL_ATTRIBUTE_DOUBLE_MAT3x4:								return "GL_DOUBLE_MAT3x4 ";
		case GLSL_ATTRIBUTE_DOUBLE_MAT4x2:								return "GL_DOUBLE_MAT4x2 ";
		case GLSL_ATTRIBUTE_DOUBLE_MAT4x3:								return "GL_DOUBLE_MAT4x3 ";
		case GLSL_ATTRIBUTE_DOUBLE_VEC2:								return "GL_DOUBLE_VEC2";
		case GLSL_ATTRIBUTE_DOUBLE_VEC3:								return "GL_DOUBLE_VEC3";
		case GLSL_ATTRIBUTE_DOUBLE_VEC4:								return "GL_DOUBLE_VEC4";
		case GLSL_ATTRIBUTE_SAMPLER_CUBE_ARRAY:							return "GL_SAMPLER_CUBE_MAP_ARRAY";
		case GLSL_ATTRIBUTE_SAMPLER_CUBE_ARRAY_SHADOW:					return "GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW";
		case GLSL_ATTRIBUTE_INT_SAMPLER_CUBE_ARRAY:						return "GL_INT_SAMPLER_CUBE_MAP_ARRAY";
		case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_CUBE_ARRAY:			return "GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY";
		case GLSL_ATTRIBUTE_IMAGE_1D:									return "GL_IMAGE_1D";
		case GLSL_ATTRIBUTE_IMAGE_2D:									return "GL_IMAGE_2D";
		case GLSL_ATTRIBUTE_IMAGE_3D:									return "GL_IMAGE_3D";
		case GLSL_ATTRIBUTE_IMAGE_2D_RECT:								return "GL_IMAGE_2D_RECT";
		case GLSL_ATTRIBUTE_IMAGE_CUBE:									return "GL_IMAGE_CUBE";
		case GLSL_ATTRIBUTE_IMAGE_BUFFER:								return "GL_IMAGE_BUFFER";
		case GLSL_ATTRIBUTE_IMAGE_1D_ARRAY:								return "GL_IMAGE_1D_ARRAY";
		case GLSL_ATTRIBUTE_IMAGE_2D_ARRAY:								return "GL_IMAGE_2D_ARRAY";
		case GLSL_ATTRIBUTE_IMAGE_CUBE_MAP_ARRAY:						return "GL_IMAGE_CUBE_MAP_ARRAY";
		case GLSL_ATTRIBUTE_IMAGE_2D_MULTISAMPLE:						return "GL_IMAGE_2D_MULTISAMPLE";
		case GLSL_ATTRIBUTE_IMAGE_2D_MULTISAMPLE_ARRAY:					return "GL_IMAGE_2D_MULTISAMPLE_ARRAY";
		case GLSL_ATTRIBUTE_INT_IMAGE_1D:								return "GL_INT_IMAGE_1D";
		case GLSL_ATTRIBUTE_INT_IMAGE_2D:								return "GL_INT_IMAGE_2D";
		case GLSL_ATTRIBUTE_INT_IMAGE_3D:								return "GL_INT_IMAGE_3D";
		case GLSL_ATTRIBUTE_INT_IMAGE_2D_RECT:							return "GL_INT_IMAGE_2D_RECT";
		case GLSL_ATTRIBUTE_INT_IMAGE_CUBE:								return "GL_INT_IMAGE_CUBE";
		case GLSL_ATTRIBUTE_INT_IMAGE_BUFFER:							return "GL_INT_IMAGE_BUFFER";
		case GLSL_ATTRIBUTE_INT_IMAGE_1D_ARRAY:							return "GL_INT_IMAGE_1D_ARRAY";
		case GLSL_ATTRIBUTE_INT_IMAGE_2D_ARRAY:							return "GL_INT_IMAGE_2D_ARRAY";
		case GLSL_ATTRIBUTE_INT_IMAGE_CUBE_MAP_ARRAY:					return "GL_INT_IMAGE_CUBE_MAP_ARRAY";
		case GLSL_ATTRIBUTE_INT_IMAGE_2D_MULTISAMPLE:					return "GL_INT_IMAGE_2D_MULTISAMPLE";
		case GLSL_ATTRIBUTE_INT_IMAGE_2D_MULTISAMPLE_ARRAY:				return "GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY";
		case GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_1D:						return "GL_UNSIGNED_INT_IMAGE_1D";
		case GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_2D:						return "GL_UNSIGNED_INT_IMAGE_2D";
		case GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_3D:						return "GL_UNSIGNED_INT_IMAGE_3D";
		case GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_2D_RECT:					return "GL_UNSIGNED_INT_IMAGE_2D_RECT";
		case GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_CUBE:					return "GL_UNSIGNED_INT_IMAGE_CUBE";
		case GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_BUFFER:					return "GL_UNSIGNED_INT_IMAGE_BUFFER";
		case GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_1D_ARRAY:				return "GL_UNSIGNED_INT_IMAGE_1D_ARRAY";
		case GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_2D_ARRAY:				return "GL_UNSIGNED_INT_IMAGE_2D_ARRAY";
		case GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY:			return "GL_UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY";
		case GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE:			return "GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE";
		case GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY:	return "GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY";
		case GLSL_ATTRIBUTE_SAMPLER_2D_MULTISAMPLE:						return "GL_SAMPLER_2D_MULTISAMPLE";
		case GLSL_ATTRIBUTE_INT_SAMPLER_2D_MULTISAMPLE:					return "GL_INT_SAMPLER_2D_MULTISAMPLE";
		case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:		return "GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE";
		case GLSL_ATTRIBUTE_SAMPLER_2D_MULTISAMPLE_ARRAY:				return "GL_SAMPLER_2D_MULTISAMPLE_ARRAY";
		case GLSL_ATTRIBUTE_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:			return "GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY";
		case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:	return "GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY";
		default:
			assert( false );
			return "";
		}
	}

	bool isImageBuffer( GlslAttributeType type )noexcept
	{
		return type == GLSL_ATTRIBUTE_IMAGE_BUFFER
			|| type == GLSL_ATTRIBUTE_INT_IMAGE_BUFFER
			|| type == GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_BUFFER;
	}

	bool isImage( GlslAttributeType type )noexcept
	{
		return type == GLSL_ATTRIBUTE_IMAGE_1D
			|| type == GLSL_ATTRIBUTE_IMAGE_2D
			|| type == GLSL_ATTRIBUTE_IMAGE_3D
			|| type == GLSL_ATTRIBUTE_IMAGE_2D_RECT
			|| type == GLSL_ATTRIBUTE_IMAGE_CUBE
			|| type == GLSL_ATTRIBUTE_IMAGE_1D_ARRAY
			|| type == GLSL_ATTRIBUTE_IMAGE_2D_ARRAY
			|| type == GLSL_ATTRIBUTE_IMAGE_CUBE_MAP_ARRAY
			|| type == GLSL_ATTRIBUTE_IMAGE_2D_MULTISAMPLE
			|| type == GLSL_ATTRIBUTE_IMAGE_2D_MULTISAMPLE_ARRAY
			|| type == GLSL_ATTRIBUTE_INT_IMAGE_1D
			|| type == GLSL_ATTRIBUTE_INT_IMAGE_2D
			|| type == GLSL_ATTRIBUTE_INT_IMAGE_3D
			|| type == GLSL_ATTRIBUTE_INT_IMAGE_2D_RECT
			|| type == GLSL_ATTRIBUTE_INT_IMAGE_CUBE
			|| type == GLSL_ATTRIBUTE_INT_IMAGE_1D_ARRAY
			|| type == GLSL_ATTRIBUTE_INT_IMAGE_2D_ARRAY
			|| type == GLSL_ATTRIBUTE_INT_IMAGE_CUBE_MAP_ARRAY
			|| type == GLSL_ATTRIBUTE_INT_IMAGE_2D_MULTISAMPLE
			|| type == GLSL_ATTRIBUTE_INT_IMAGE_2D_MULTISAMPLE_ARRAY
			|| type == GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_1D
			|| type == GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_2D
			|| type == GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_3D
			|| type == GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_2D_RECT
			|| type == GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_CUBE
			|| type == GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_1D_ARRAY
			|| type == GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_2D_ARRAY
			|| type == GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY
			|| type == GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE
			|| type == GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY;
	}

	bool isSamplerBuffer( GlslAttributeType type )noexcept
	{
		return type == GLSL_ATTRIBUTE_SAMPLER_BUFFER
			|| type == GLSL_ATTRIBUTE_INT_SAMPLER_BUFFER
			|| type == GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_BUFFER;
	}

	bool isSampler( GlslAttributeType type )noexcept
	{
		return type == GLSL_ATTRIBUTE_SAMPLER_1D
			|| type == GLSL_ATTRIBUTE_SAMPLER_2D
			|| type == GLSL_ATTRIBUTE_SAMPLER_3D
			|| type == GLSL_ATTRIBUTE_SAMPLER_CUBE
			|| type == GLSL_ATTRIBUTE_SAMPLER_1D_SHADOW
			|| type == GLSL_ATTRIBUTE_SAMPLER_2D_SHADOW
			|| type == GLSL_ATTRIBUTE_SAMPLER_2D_RECT
			|| type == GLSL_ATTRIBUTE_SAMPLER_2D_RECT_SHADOW
			|| type == GLSL_ATTRIBUTE_SAMPLER_1D_ARRAY
			|| type == GLSL_ATTRIBUTE_SAMPLER_2D_ARRAY
			|| type == GLSL_ATTRIBUTE_SAMPLER_CUBE_ARRAY
			|| type == GLSL_ATTRIBUTE_SAMPLER_1D_ARRAY_SHADOW
			|| type == GLSL_ATTRIBUTE_SAMPLER_2D_ARRAY_SHADOW
			|| type == GLSL_ATTRIBUTE_SAMPLER_CUBE_SHADOW
			|| type == GLSL_ATTRIBUTE_SAMPLER_CUBE_ARRAY_SHADOW
			|| type == GLSL_ATTRIBUTE_SAMPLER_2D_MULTISAMPLE
			|| type == GLSL_ATTRIBUTE_SAMPLER_2D_MULTISAMPLE_ARRAY
			|| type == GLSL_ATTRIBUTE_INT_SAMPLER_1D
			|| type == GLSL_ATTRIBUTE_INT_SAMPLER_2D
			|| type == GLSL_ATTRIBUTE_INT_SAMPLER_3D
			|| type == GLSL_ATTRIBUTE_INT_SAMPLER_CUBE
			|| type == GLSL_ATTRIBUTE_INT_SAMPLER_2D_RECT
			|| type == GLSL_ATTRIBUTE_INT_SAMPLER_1D_ARRAY
			|| type == GLSL_ATTRIBUTE_INT_SAMPLER_2D_ARRAY
			|| type == GLSL_ATTRIBUTE_INT_SAMPLER_CUBE_ARRAY
			|| type == GLSL_ATTRIBUTE_INT_SAMPLER_2D_MULTISAMPLE
			|| type == GLSL_ATTRIBUTE_INT_SAMPLER_2D_MULTISAMPLE_ARRAY
			|| type == GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_1D
			|| type == GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_2D
			|| type == GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_3D
			|| type == GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_CUBE
			|| type == GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_2D_RECT
			|| type == GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_1D_ARRAY
			|| type == GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_2D_ARRAY
			|| type == GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_CUBE_ARRAY
			|| type == GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE
			|| type == GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY;
	}

	ConstantFormat getConstantFormat( GlslAttributeType type )noexcept
	{
		assert( !isImage( type ) && !isSampler( type ) && !isSamplerBuffer( type ) );
		switch ( type )
		{
		case GLSL_ATTRIBUTE_INT:										return ConstantFormat::eInt;
		case GLSL_ATTRIBUTE_UNSIGNED_INT:								return ConstantFormat::eUInt;
		case GLSL_ATTRIBUTE_FLOAT:										return ConstantFormat::eFloat;
		case GLSL_ATTRIBUTE_FLOAT_VEC2:									return ConstantFormat::eVec2f;
		case GLSL_ATTRIBUTE_FLOAT_VEC3:									return ConstantFormat::eVec3f;
		case GLSL_ATTRIBUTE_FLOAT_VEC4:									return ConstantFormat::eVec4f;
		case GLSL_ATTRIBUTE_INT_VEC2:									return ConstantFormat::eVec2i;
		case GLSL_ATTRIBUTE_INT_VEC3:									return ConstantFormat::eVec3i;
		case GLSL_ATTRIBUTE_INT_VEC4:									return ConstantFormat::eVec4i;
		case GLSL_ATTRIBUTE_FLOAT_MAT2:									return ConstantFormat::eMat2f;
		case GLSL_ATTRIBUTE_FLOAT_MAT3x2:								return ConstantFormat::eMat3x2f;
		case GLSL_ATTRIBUTE_FLOAT_MAT4x2:								return ConstantFormat::eMat4x2f;
		case GLSL_ATTRIBUTE_FLOAT_MAT2x3:								return ConstantFormat::eMat2x3f;
		case GLSL_ATTRIBUTE_FLOAT_MAT3:									return ConstantFormat::eMat3f;
		case GLSL_ATTRIBUTE_FLOAT_MAT4x3:								return ConstantFormat::eMat4x3f;
		case GLSL_ATTRIBUTE_FLOAT_MAT2x4:								return ConstantFormat::eMat2x4f;
		case GLSL_ATTRIBUTE_FLOAT_MAT3x4:								return ConstantFormat::eMat3x4f;
		case GLSL_ATTRIBUTE_FLOAT_MAT4:									return ConstantFormat::eMat4f;
		case GLSL_ATTRIBUTE_UNSIGNED_INT_VEC2:							return ConstantFormat::eVec2ui;
		case GLSL_ATTRIBUTE_UNSIGNED_INT_VEC3:							return ConstantFormat::eVec3ui;
		case GLSL_ATTRIBUTE_UNSIGNED_INT_VEC4:							return ConstantFormat::eVec4ui;
		default:
			assert( false && "Unsupported GLSL attribute type" );
			return ConstantFormat::eVec4f;
		}
	}

	ImageFormat getImageFormat( GlslAttributeType type )noexcept
	{
		assert( isImage( type ) || isImageBuffer( type ) );
		switch ( type )
		{
		case GLSL_ATTRIBUTE_IMAGE_1D:									return ImageFormat::e1D;
		case GLSL_ATTRIBUTE_IMAGE_2D:									return ImageFormat::e2D;
		case GLSL_ATTRIBUTE_IMAGE_3D:									return ImageFormat::e3D;
		case GLSL_ATTRIBUTE_IMAGE_CUBE:									return ImageFormat::eCube;
		case GLSL_ATTRIBUTE_IMAGE_BUFFER:								return ImageFormat::eBuffer;
		case GLSL_ATTRIBUTE_IMAGE_2D_RECT:								return ImageFormat::e2DRect;
		case GLSL_ATTRIBUTE_IMAGE_1D_ARRAY:								return ImageFormat::e1DArray;
		case GLSL_ATTRIBUTE_IMAGE_2D_ARRAY:								return ImageFormat::e2DArray;
		case GLSL_ATTRIBUTE_IMAGE_CUBE_MAP_ARRAY:						return ImageFormat::eCubeArray;
		case GLSL_ATTRIBUTE_IMAGE_2D_MULTISAMPLE:						return ImageFormat::e2DMultisample;
		case GLSL_ATTRIBUTE_IMAGE_2D_MULTISAMPLE_ARRAY:					return ImageFormat::e2DMultisampleArray;
		case GLSL_ATTRIBUTE_INT_IMAGE_1D:								return ImageFormat::eInt1D;
		case GLSL_ATTRIBUTE_INT_IMAGE_2D:								return ImageFormat::eInt2D;
		case GLSL_ATTRIBUTE_INT_IMAGE_3D:								return ImageFormat::eInt3D;
		case GLSL_ATTRIBUTE_INT_IMAGE_CUBE:								return ImageFormat::eIntCube;
		case GLSL_ATTRIBUTE_INT_IMAGE_BUFFER:							return ImageFormat::eIntBuffer;
		case GLSL_ATTRIBUTE_INT_IMAGE_2D_RECT:							return ImageFormat::eInt2DRect;
		case GLSL_ATTRIBUTE_INT_IMAGE_1D_ARRAY:							return ImageFormat::eInt1DArray;
		case GLSL_ATTRIBUTE_INT_IMAGE_2D_ARRAY:							return ImageFormat::eInt2DArray;
		case GLSL_ATTRIBUTE_INT_IMAGE_CUBE_MAP_ARRAY:					return ImageFormat::eIntCubeArray;
		case GLSL_ATTRIBUTE_INT_IMAGE_2D_MULTISAMPLE:					return ImageFormat::eInt2DMultisample;
		case GLSL_ATTRIBUTE_INT_IMAGE_2D_MULTISAMPLE_ARRAY:				return ImageFormat::eInt2DMultisampleArray;
		case GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_1D:						return ImageFormat::eUInt1D;
		case GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_2D:						return ImageFormat::eUInt2D;
		case GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_3D:						return ImageFormat::eUInt3D;
		case GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_CUBE:					return ImageFormat::eUIntCube;
		case GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_BUFFER:					return ImageFormat::eUIntBuffer;
		case GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_2D_RECT:					return ImageFormat::eUInt2DRect;
		case GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_1D_ARRAY:				return ImageFormat::eUInt1DArray;
		case GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_2D_ARRAY:				return ImageFormat::eUInt2DArray;
		case GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY:			return ImageFormat::eUIntCubeArray;
		case GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE:			return ImageFormat::eUInt2DMultisample;
		case GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY:	return ImageFormat::eUInt2DMultisampleArray;
		default:
			assert( false && "Unsupported GLSL attribute type" );
			return ImageFormat::e2D;
		}
	}

	SamplerFormat getSamplerFormat( GlslAttributeType type )noexcept
	{
		assert( isSampler( type ) || isSamplerBuffer( type ) );
		switch ( type )
		{
		case GLSL_ATTRIBUTE_SAMPLER_1D:									return SamplerFormat::e1D;
		case GLSL_ATTRIBUTE_SAMPLER_2D:									return SamplerFormat::e2D;
		case GLSL_ATTRIBUTE_SAMPLER_3D:									return SamplerFormat::e3D;
		case GLSL_ATTRIBUTE_SAMPLER_CUBE:								return SamplerFormat::eCube;
		case GLSL_ATTRIBUTE_SAMPLER_1D_SHADOW:							return SamplerFormat::e1DShadow;
		case GLSL_ATTRIBUTE_SAMPLER_2D_SHADOW:							return SamplerFormat::e2DShadow;
		case GLSL_ATTRIBUTE_SAMPLER_2D_RECT:							return SamplerFormat::e2DRect;
		case GLSL_ATTRIBUTE_SAMPLER_2D_RECT_SHADOW:						return SamplerFormat::e2DRectShadow;
		case GLSL_ATTRIBUTE_SAMPLER_1D_ARRAY:							return SamplerFormat::e1DArray;
		case GLSL_ATTRIBUTE_SAMPLER_2D_ARRAY:							return SamplerFormat::e2DArray;
		case GLSL_ATTRIBUTE_SAMPLER_CUBE_ARRAY:							return SamplerFormat::eCubeArray;
		case GLSL_ATTRIBUTE_SAMPLER_BUFFER:								return SamplerFormat::eBuffer;
		case GLSL_ATTRIBUTE_SAMPLER_1D_ARRAY_SHADOW:					return SamplerFormat::e1DArrayShadow;
		case GLSL_ATTRIBUTE_SAMPLER_2D_ARRAY_SHADOW:					return SamplerFormat::e2DArrayShadow;
		case GLSL_ATTRIBUTE_SAMPLER_CUBE_SHADOW:						return SamplerFormat::eCubeShadow;
		case GLSL_ATTRIBUTE_SAMPLER_CUBE_ARRAY_SHADOW:					return SamplerFormat::eCubeArrayShadow;
		case GLSL_ATTRIBUTE_SAMPLER_2D_MULTISAMPLE:						return SamplerFormat::e2DMultisample;
		case GLSL_ATTRIBUTE_SAMPLER_2D_MULTISAMPLE_ARRAY:				return SamplerFormat::e2DMultisampleArray;
		case GLSL_ATTRIBUTE_INT_SAMPLER_1D:								return SamplerFormat::eInt1D;
		case GLSL_ATTRIBUTE_INT_SAMPLER_2D:								return SamplerFormat::eInt2D;
		case GLSL_ATTRIBUTE_INT_SAMPLER_3D:								return SamplerFormat::eInt3D;
		case GLSL_ATTRIBUTE_INT_SAMPLER_CUBE:							return SamplerFormat::eIntCube;
		case GLSL_ATTRIBUTE_INT_SAMPLER_2D_RECT:						return SamplerFormat::eInt2DRect;
		case GLSL_ATTRIBUTE_INT_SAMPLER_1D_ARRAY:						return SamplerFormat::eInt1DArray;
		case GLSL_ATTRIBUTE_INT_SAMPLER_2D_ARRAY:						return SamplerFormat::eInt2DArray;
		case GLSL_ATTRIBUTE_INT_SAMPLER_CUBE_ARRAY:						return SamplerFormat::eIntCubeArray;
		case GLSL_ATTRIBUTE_INT_SAMPLER_BUFFER:							return SamplerFormat::eIntBuffer;
		case GLSL_ATTRIBUTE_INT_SAMPLER_2D_MULTISAMPLE:					return SamplerFormat::eInt2DMultisample;
		case GLSL_ATTRIBUTE_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:			return SamplerFormat::eInt2DMultisampleArray;
		case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_1D:					return SamplerFormat::eUInt1D;
		case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_2D:					return SamplerFormat::eUInt2D;
		case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_3D:					return SamplerFormat::eUInt3D;
		case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_CUBE:					return SamplerFormat::eUIntCube;
		case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_2D_RECT:				return SamplerFormat::eUInt2DRect;
		case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_1D_ARRAY:				return SamplerFormat::eUInt1DArray;
		case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_2D_ARRAY:				return SamplerFormat::eUInt2DArray;
		case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_CUBE_ARRAY:			return SamplerFormat::eUIntCubeArray;
		case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_BUFFER:				return SamplerFormat::eUIntBuffer;
		case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:		return SamplerFormat::eUInt2DMultisample;
		case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:	return SamplerFormat::eUInt2DMultisampleArray;
		default:
			assert( false && "Unsupported GLSL attribute type" );
			return SamplerFormat::e2D;
		}
	}

	uint32_t getSize( GlslAttributeType type )noexcept
	{
		switch ( type )
		{
		case GLSL_ATTRIBUTE_HALF_FLOAT:
			return 2u;
		case GLSL_ATTRIBUTE_BOOL:
		case GLSL_ATTRIBUTE_INT:
		case GLSL_ATTRIBUTE_UNSIGNED_INT:
		case GLSL_ATTRIBUTE_FLOAT:
			return 4u;
		case GLSL_ATTRIBUTE_DOUBLE:
		case GLSL_ATTRIBUTE_BOOL_VEC2:
		case GLSL_ATTRIBUTE_FLOAT_VEC2:
		case GLSL_ATTRIBUTE_INT_VEC2:
		case GLSL_ATTRIBUTE_UNSIGNED_INT_VEC2:
			return 8u;
		case GLSL_ATTRIBUTE_BOOL_VEC3:
		case GLSL_ATTRIBUTE_FLOAT_VEC3:
		case GLSL_ATTRIBUTE_INT_VEC3:
		case GLSL_ATTRIBUTE_UNSIGNED_INT_VEC3:
			return 12u;
		case GLSL_ATTRIBUTE_BOOL_VEC4:
		case GLSL_ATTRIBUTE_FLOAT_VEC4:
		case GLSL_ATTRIBUTE_INT_VEC4:
		case GLSL_ATTRIBUTE_UNSIGNED_INT_VEC4:
		case GLSL_ATTRIBUTE_FLOAT_MAT2:
		case GLSL_ATTRIBUTE_DOUBLE_VEC2:
			return 16u;
		case GLSL_ATTRIBUTE_FLOAT_MAT2x3:
		case GLSL_ATTRIBUTE_FLOAT_MAT3x2:
		case GLSL_ATTRIBUTE_DOUBLE_VEC3:
			return 24u;
		case GLSL_ATTRIBUTE_FLOAT_MAT2x4:
		case GLSL_ATTRIBUTE_FLOAT_MAT4x2:
		case GLSL_ATTRIBUTE_DOUBLE_MAT2:
		case GLSL_ATTRIBUTE_DOUBLE_VEC4:
			return 32u;
		case GLSL_ATTRIBUTE_FLOAT_MAT3:
			return 36u;
		case GLSL_ATTRIBUTE_FLOAT_MAT3x4:
		case GLSL_ATTRIBUTE_FLOAT_MAT4x3:
		case GLSL_ATTRIBUTE_DOUBLE_MAT2x3:
		case GLSL_ATTRIBUTE_DOUBLE_MAT3x2:
			return 48u;
		case GLSL_ATTRIBUTE_FLOAT_MAT4:
		case GLSL_ATTRIBUTE_DOUBLE_MAT2x4:
		case GLSL_ATTRIBUTE_DOUBLE_MAT4x2:
			return 64u;
		case GLSL_ATTRIBUTE_DOUBLE_MAT3:
			return 72u;
		case GLSL_ATTRIBUTE_DOUBLE_MAT3x4:
		case GLSL_ATTRIBUTE_DOUBLE_MAT4x3:
			return 96u;
		case GLSL_ATTRIBUTE_DOUBLE_MAT4:
			return 128u;
		case GLSL_ATTRIBUTE_IMAGE_1D:
		case GLSL_ATTRIBUTE_IMAGE_2D:
		case GLSL_ATTRIBUTE_IMAGE_3D:
		case GLSL_ATTRIBUTE_IMAGE_2D_RECT:
		case GLSL_ATTRIBUTE_IMAGE_CUBE:
		case GLSL_ATTRIBUTE_IMAGE_BUFFER:
		case GLSL_ATTRIBUTE_IMAGE_1D_ARRAY:
		case GLSL_ATTRIBUTE_IMAGE_2D_ARRAY:
		case GLSL_ATTRIBUTE_IMAGE_CUBE_MAP_ARRAY:
		case GLSL_ATTRIBUTE_IMAGE_2D_MULTISAMPLE:
		case GLSL_ATTRIBUTE_IMAGE_2D_MULTISAMPLE_ARRAY:
		case GLSL_ATTRIBUTE_INT_IMAGE_1D:
		case GLSL_ATTRIBUTE_INT_IMAGE_2D:
		case GLSL_ATTRIBUTE_INT_IMAGE_3D:
		case GLSL_ATTRIBUTE_INT_IMAGE_2D_RECT:
		case GLSL_ATTRIBUTE_INT_IMAGE_CUBE:
		case GLSL_ATTRIBUTE_INT_IMAGE_BUFFER:
		case GLSL_ATTRIBUTE_INT_IMAGE_1D_ARRAY:
		case GLSL_ATTRIBUTE_INT_IMAGE_2D_ARRAY:
		case GLSL_ATTRIBUTE_INT_IMAGE_CUBE_MAP_ARRAY:
		case GLSL_ATTRIBUTE_INT_IMAGE_2D_MULTISAMPLE:
		case GLSL_ATTRIBUTE_INT_IMAGE_2D_MULTISAMPLE_ARRAY:
		case GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_1D:
		case GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_2D:
		case GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_3D:
		case GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_2D_RECT:
		case GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_CUBE:
		case GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_BUFFER:
		case GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_1D_ARRAY:
		case GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_2D_ARRAY:
		case GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY:
		case GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE:
		case GLSL_ATTRIBUTE_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY:
		case GLSL_ATTRIBUTE_SAMPLER_1D:
		case GLSL_ATTRIBUTE_SAMPLER_2D:
		case GLSL_ATTRIBUTE_SAMPLER_3D:
		case GLSL_ATTRIBUTE_SAMPLER_CUBE:
		case GLSL_ATTRIBUTE_SAMPLER_1D_SHADOW:
		case GLSL_ATTRIBUTE_SAMPLER_2D_SHADOW:
		case GLSL_ATTRIBUTE_SAMPLER_2D_RECT:
		case GLSL_ATTRIBUTE_SAMPLER_2D_RECT_SHADOW:
		case GLSL_ATTRIBUTE_SAMPLER_1D_ARRAY:
		case GLSL_ATTRIBUTE_SAMPLER_2D_ARRAY:
		case GLSL_ATTRIBUTE_SAMPLER_CUBE_ARRAY:
		case GLSL_ATTRIBUTE_SAMPLER_BUFFER:
		case GLSL_ATTRIBUTE_SAMPLER_1D_ARRAY_SHADOW:
		case GLSL_ATTRIBUTE_SAMPLER_2D_ARRAY_SHADOW:
		case GLSL_ATTRIBUTE_SAMPLER_CUBE_SHADOW:
		case GLSL_ATTRIBUTE_SAMPLER_CUBE_ARRAY_SHADOW:
		case GLSL_ATTRIBUTE_SAMPLER_2D_MULTISAMPLE:
		case GLSL_ATTRIBUTE_SAMPLER_2D_MULTISAMPLE_ARRAY:
		case GLSL_ATTRIBUTE_INT_SAMPLER_1D:
		case GLSL_ATTRIBUTE_INT_SAMPLER_2D:
		case GLSL_ATTRIBUTE_INT_SAMPLER_3D:
		case GLSL_ATTRIBUTE_INT_SAMPLER_CUBE:
		case GLSL_ATTRIBUTE_INT_SAMPLER_2D_RECT:
		case GLSL_ATTRIBUTE_INT_SAMPLER_1D_ARRAY:
		case GLSL_ATTRIBUTE_INT_SAMPLER_2D_ARRAY:
		case GLSL_ATTRIBUTE_INT_SAMPLER_CUBE_ARRAY:
		case GLSL_ATTRIBUTE_INT_SAMPLER_BUFFER:
		case GLSL_ATTRIBUTE_INT_SAMPLER_2D_MULTISAMPLE:
		case GLSL_ATTRIBUTE_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
		case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_1D:
		case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_2D:
		case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_3D:
		case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_CUBE:
		case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_2D_RECT:
		case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_1D_ARRAY:
		case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_2D_ARRAY:
		case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_CUBE_ARRAY:
		case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_BUFFER:
		case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:
		case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
			return 0u;
		default:
			assert( false && "Unsupported attribute type" );
			return 4u;
		}
	}

	VkFormat getAttributeFormat( GlslAttributeType type )noexcept
	{
		switch ( type )
		{
		case GLSL_ATTRIBUTE_INT:
			return VK_FORMAT_R32_SINT;
		case GLSL_ATTRIBUTE_UNSIGNED_INT:
			return VK_FORMAT_R32_UINT;
		case GLSL_ATTRIBUTE_FLOAT:
			return VK_FORMAT_R32_SFLOAT;
		case GLSL_ATTRIBUTE_FLOAT_VEC2:
			return VK_FORMAT_R32G32_SFLOAT;
		case GLSL_ATTRIBUTE_FLOAT_VEC3:
			return VK_FORMAT_R32G32B32_SFLOAT;
		case GLSL_ATTRIBUTE_FLOAT_VEC4:
			return VK_FORMAT_R32G32B32A32_SFLOAT;
		case GLSL_ATTRIBUTE_INT_VEC2:
			return VK_FORMAT_R32G32_SINT;
		case GLSL_ATTRIBUTE_INT_VEC3:
			return VK_FORMAT_R32G32B32_SINT;
		case GLSL_ATTRIBUTE_INT_VEC4:
			return VK_FORMAT_R32G32B32A32_SINT;
		case GLSL_ATTRIBUTE_UNSIGNED_INT_VEC2:
			return VK_FORMAT_R32G32_UINT;
		case GLSL_ATTRIBUTE_UNSIGNED_INT_VEC3:
			return VK_FORMAT_R32G32B32_UINT;
		case GLSL_ATTRIBUTE_UNSIGNED_INT_VEC4:
			return VK_FORMAT_R32G32B32A32_UINT;
		default:
			assert( false && "Unsupported attribute type" );
			return VK_FORMAT_R32_SFLOAT;
		}
	}

	InputsLayout getInputs( ContextLock const & context
		, VkShaderStageFlagBits stage
		, GLuint program )
	{
		return ( hasProgramInterfaceQuery( context.getDevice() )
			? gl4::getInputs( context, stage, program )
			: gl3::getInputs( context, stage, program ) );
	}

	ConstantsLayout & getPushConstants( ContextLock const & context
		, ConstantsLayout & constants
		, VkShaderStageFlagBits stage
		, GLuint program )
	{
		return ( hasProgramInterfaceQuery( context.getDevice() )
			? gl4::getPushConstants( context, constants, stage, program )
			: gl3::getPushConstants( context, constants, stage, program ) );
	}

	InterfaceBlocksLayout getUniformBuffers( ContextLock const & context
		, VkShaderStageFlagBits stage
		, GLuint program )
	{
		return ( hasProgramInterfaceQuery( context.getDevice() )
			? gl4::getUniformBuffers( context, stage, program )
			: gl3::getUniformBuffers( context, stage, program ) );
	}

	InterfaceBlocksLayout getStorageBuffers( ContextLock const & context
		, VkShaderStageFlagBits stage
		, GLuint program )
	{
		return ( hasProgramInterfaceQuery( context.getDevice() )
			? gl4::getStorageBuffers( context, stage, program )
			: gl3::getStorageBuffers( context, stage, program ) );
	}

	SamplersLayout getSamplerBuffers( ContextLock const & context
		, VkShaderStageFlagBits stage
		, GLuint program )
	{
		return ( hasProgramInterfaceQuery( context.getDevice() )
			? gl4::getSamplerBuffers( context, stage, program )
			: gl3::getSamplerBuffers( context, stage, program ) );
	}

	SamplersLayout getSamplers( ContextLock const & context
		, VkShaderStageFlagBits stage
		, GLuint program )
	{
		return ( hasProgramInterfaceQuery( context.getDevice() )
			? gl4::getSamplers( context, stage, program )
			: gl3::getSamplers( context, stage, program ) );
	}

	ImagesLayout getImageBuffers( ContextLock const & context
		, VkShaderStageFlagBits stage
		, GLuint program )
	{
		return ( hasProgramInterfaceQuery( context.getDevice() )
			? gl4::getImageBuffers( context, stage, program )
			: gl3::getImageBuffers( context, stage, program ) );
	}

	ImagesLayout getImages( ContextLock const & context
		, VkShaderStageFlagBits stage
		, GLuint program )
	{
		return ( hasProgramInterfaceQuery( context.getDevice() )
			? gl4::getImages( context, stage, program )
			: gl3::getImages( context, stage, program ) );
	}

	ShaderDesc getShaderDesc( ContextLock const & context
		, ConstantsLayout & constants
		, VkShaderStageFlagBits stage
		, GLuint programObject )
	{
		ShaderDesc result{ true
			, 0u
			, 0u
			, getInputs( context, stage, programObject )
			, getPushConstants( context, constants, stage, programObject )
			, getUniformBuffers( context, stage, programObject )
			, getStorageBuffers( context, stage, programObject )
			, getSamplerBuffers( context, stage, programObject )
			, getSamplers( context, stage, programObject )
			, getImageBuffers( context, stage, programObject )
			, getImages( context, stage, programObject ) };

		return result;
	}

	void validatePipeline( ContextLock const & context
		, GLuint program
		, VkPipelineVertexInputStateCreateInfo const & vertexInputState
		, VkRenderPass renderPass )
	{
		validateInputs( context, program, vertexInputState );
		validateOutputs( context, program, renderPass );
	}
}

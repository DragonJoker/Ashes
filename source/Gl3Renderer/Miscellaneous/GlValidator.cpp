#include "Miscellaneous/GlValidator.hpp"

#include "Core/GlDevice.hpp"
#include "Pipeline/GlPipelineLayout.hpp"
#include "RenderPass/GlRenderPass.hpp"

#include <Ashes/Pipeline/VertexInputAttributeDescription.hpp>
#include <Ashes/Pipeline/VertexInputState.hpp>

#include <algorithm>

#if defined( interface )
#	undef interface
#endif

namespace gl_renderer
{
	namespace
	{
		std::string const ValidationError = "VALIDATION ERROR: ";
		std::string const ValidationWarning = "VALIDATION WARNING: ";

		enum GlslInterface
			: GLenum
		{
			GLSL_INTERFACE_ATOMIC_COUNTER_BUFFER = 0x92C0,
			GLSL_INTERFACE_UNIFORM = 0x92E1,
			GLSL_INTERFACE_UNIFORM_BLOCK = 0x92E2,
			GLSL_INTERFACE_PROGRAM_INPUT = 0x92E3,
			GLSL_INTERFACE_PROGRAM_OUTPUT = 0x92E4,
			GLSL_INTERFACE_BUFFER_VARIABLE = 0x92E5,
			GLSL_INTERFACE_SHADER_STORAGE_BLOCK = 0x92E6,
			GLSL_INTERFACE_VERTEX_SUBROUTINE = 0x92E8,
			GLSL_INTERFACE_TESS_CONTROL_SUBROUTINE = 0x92E9,
			GLSL_INTERFACE_TESS_EVALUATION_SUBROUTINE = 0x92EA,
			GLSL_INTERFACE_GEOMETRY_SUBROUTINE = 0x92EB,
			GLSL_INTERFACE_FRAGMENT_SUBROUTINE = 0x92EC,
			GLSL_INTERFACE_COMPUTE_SUBROUTINE = 0x92ED,
			GLSL_INTERFACE_VERTEX_SUBROUTINE_UNIFORM = 0x92EE,
			GLSL_INTERFACE_TESS_CONTROL_SUBROUTINE_UNIFORM = 0x92EF,
			GLSL_INTERFACE_TESS_EVALUATION_SUBROUTINE_UNIFORM = 0x92F0,
			GLSL_INTERFACE_GEOMETRY_SUBROUTINE_UNIFORM = 0x92F1,
			GLSL_INTERFACE_FRAGMENT_SUBROUTINE_UNIFORM = 0x92F2,
			GLSL_INTERFACE_COMPUTE_SUBROUTINE_UNIFORM = 0x92F3,
		};

		enum GlslDataName
			: GLenum
		{
			GLSL_DATANAME_ACTIVE_RESOURCES = 0x92F5,
			GLSL_DATANAME_MAX_NAME_LENGTH = 0x92F6,
			GLSL_DATANAME_MAX_NUM_ACTIVE_VARIABLES = 0x92F7,
			GLSL_DATANAME_MAX_NUM_COMPATIBLE_SUBROUTINES = 0x92F8,
		};

		enum GlslProperty
			: GLenum
		{
			GLSL_PROPERTY_NUM_COMPATIBLE_SUBROUTINES = 0x8E4A,
			GLSL_PROPERTY_COMPATIBLE_SUBROUTINES = 0x8E4B,
			GLSL_PROPERTY_IS_PER_PATCH = 0x92E7,
			GLSL_PROPERTY_NAME_LENGTH = 0x92F9,
			GLSL_PROPERTY_TYPE = 0x92FA,
			GLSL_PROPERTY_ARRAY_SIZE = 0x92FB,
			GLSL_PROPERTY_OFFSET = 0x92FC,
			GLSL_PROPERTY_BLOCK_INDEX = 0x92FD,
			GLSL_PROPERTY_ARRAY_STRIDE = 0x92FE,
			GLSL_PROPERTY_MATRIX_STRIDE = 0x92FF,
			GLSL_PROPERTY_IS_ROW_MAJOR = 0x9300,
			GLSL_PROPERTY_ATOMIC_COUNTER_BUFFER_INDEX = 0x9301,
			GLSL_PROPERTY_BUFFER_BINDING = 0x9302,
			GLSL_PROPERTY_BUFFER_DATA_SIZE = 0x9303,
			GLSL_PROPERTY_NUM_ACTIVE_VARIABLES = 0x9304,
			GLSL_PROPERTY_ACTIVE_VARIABLES = 0x9305,
			GLSL_PROPERTY_REFERENCED_BY_VERTEX_SHADER = 0x9306,
			GLSL_PROPERTY_REFERENCED_BY_TESS_CONTROL_SHADER = 0x9307,
			GLSL_PROPERTY_REFERENCED_BY_TESS_EVALUATION_SHADER = 0x9308,
			GLSL_PROPERTY_REFERENCED_BY_GEOMETRY_SHADER = 0x9309,
			GLSL_PROPERTY_REFERENCED_BY_FRAGMENT_SHADER = 0x930A,
			GLSL_PROPERTY_REFERENCED_BY_COMPUTE_SHADER = 0x930B,
			GLSL_PROPERTY_TOP_LEVEL_ARRAY_SIZE = 0x930C,
			GLSL_PROPERTY_TOP_LEVEL_ARRAY_STRIDE = 0x930D,
			GLSL_PROPERTY_LOCATION = 0x930E,
			GLSL_PROPERTY_LOCATION_INDEX = 0x930F,
			GLSL_PROPERTY_LOCATION_COMPONENT = 0x934A,
		};

		enum GlslAttributeType
			: GLenum
		{
			GLSL_ATTRIBUTE_INT = 0x1404,
			GLSL_ATTRIBUTE_UNSIGNED_INT = 0x1405,
			GLSL_ATTRIBUTE_FLOAT = 0x1406,
			GLSL_ATTRIBUTE_DOUBLE = 0x140A,
			GLSL_ATTRIBUTE_HALF_FLOAT = 0x140B,
			GLSL_ATTRIBUTE_FLOAT_VEC2 = 0x8B50,
			GLSL_ATTRIBUTE_FLOAT_VEC3 = 0x8B51,
			GLSL_ATTRIBUTE_FLOAT_VEC4 = 0x8B52,
			GLSL_ATTRIBUTE_INT_VEC2 = 0x8B53,
			GLSL_ATTRIBUTE_INT_VEC3 = 0x8B54,
			GLSL_ATTRIBUTE_INT_VEC4 = 0x8B55,
			GLSL_ATTRIBUTE_BOOL = 0x8B56,
			GLSL_ATTRIBUTE_BOOL_VEC2 = 0x8B57,
			GLSL_ATTRIBUTE_BOOL_VEC3 = 0x8B58,
			GLSL_ATTRIBUTE_BOOL_VEC4 = 0x8B59,
			GLSL_ATTRIBUTE_FLOAT_MAT2 = 0x8B5A,
			GLSL_ATTRIBUTE_FLOAT_MAT3 = 0x8B5B,
			GLSL_ATTRIBUTE_FLOAT_MAT4 = 0x8B5C,
			GLSL_ATTRIBUTE_SAMPLER_1D = 0x8B5D,
			GLSL_ATTRIBUTE_SAMPLER_2D = 0x8B5E,
			GLSL_ATTRIBUTE_SAMPLER_3D = 0x8B5F,
			GLSL_ATTRIBUTE_SAMPLER_CUBE = 0X8B60,
			GLSL_ATTRIBUTE_SAMPLER_1D_SHADOW = 0x8B61,
			GLSL_ATTRIBUTE_SAMPLER_2D_SHADOW = 0x8B62,
			GLSL_ATTRIBUTE_SAMPLER_2D_RECT = 0X8B63,
			GLSL_ATTRIBUTE_SAMPLER_2D_RECT_SHADOW = 0x8B64,
			GLSL_ATTRIBUTE_FLOAT_MAT2x3 = 0x8B65,
			GLSL_ATTRIBUTE_FLOAT_MAT2x4 = 0x8B66,
			GLSL_ATTRIBUTE_FLOAT_MAT3x2 = 0x8B67,
			GLSL_ATTRIBUTE_FLOAT_MAT3x4 = 0x8B68,
			GLSL_ATTRIBUTE_FLOAT_MAT4x2 = 0x8B69,
			GLSL_ATTRIBUTE_FLOAT_MAT4x3 = 0x8B6A,
			GLSL_ATTRIBUTE_SAMPLER_1D_ARRAY = 0x8DC0,
			GLSL_ATTRIBUTE_SAMPLER_2D_ARRAY = 0x8DC1,
			GLSL_ATTRIBUTE_SAMPLER_BUFFER = 0x8DC2,
			GLSL_ATTRIBUTE_SAMPLER_1D_ARRAY_SHADOW = 0x8DC3,
			GLSL_ATTRIBUTE_SAMPLER_2D_ARRAY_SHADOW = 0x8DC4,
			GLSL_ATTRIBUTE_SAMPLER_CUBE_SHADOW = 0x8DC5,
			GLSL_ATTRIBUTE_UNSIGNED_INT_VEC2 = 0x8DC6,
			GLSL_ATTRIBUTE_UNSIGNED_INT_VEC3 = 0x8DC7,
			GLSL_ATTRIBUTE_UNSIGNED_INT_VEC4 = 0x8DC8,
			GLSL_ATTRIBUTE_INT_SAMPLER_1D = 0x8DC9,
			GLSL_ATTRIBUTE_INT_SAMPLER_2D = 0x8DCA,
			GLSL_ATTRIBUTE_INT_SAMPLER_3D = 0x8DCB,
			GLSL_ATTRIBUTE_INT_SAMPLER_CUBE = 0x8DCC,
			GLSL_ATTRIBUTE_INT_SAMPLER_2D_RECT = 0x8DCD,
			GLSL_ATTRIBUTE_INT_SAMPLER_1D_ARRAY = 0x8DCE,
			GLSL_ATTRIBUTE_INT_SAMPLER_2D_ARRAY = 0x8DCF,
			GLSL_ATTRIBUTE_INT_SAMPLER_BUFFER = 0x8DD0,
			GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_1D = 0x8DD1,
			GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_2D = 0x8DD2,
			GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_3D = 0x8DD3,
			GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_CUBE = 0x8DD4,
			GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_2D_RECT = 0x8DD5,
			GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_1D_ARRAY = 0x8DD6,
			GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_2D_ARRAY = 0x8DD7,
			GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_BUFFER = 0x8DD8,
			GLSL_ATTRIBUTE_DOUBLE_MAT2 = 0x8F46,
			GLSL_ATTRIBUTE_DOUBLE_MAT3 = 0x8F47,
			GLSL_ATTRIBUTE_DOUBLE_MAT4 = 0x8F48,
			GLSL_ATTRIBUTE_DOUBLE_MAT2x3 = 0x8F49,
			GLSL_ATTRIBUTE_DOUBLE_MAT2x4 = 0x8F4A,
			GLSL_ATTRIBUTE_DOUBLE_MAT3x2 = 0x8F4B,
			GLSL_ATTRIBUTE_DOUBLE_MAT3x4 = 0x8F4C,
			GLSL_ATTRIBUTE_DOUBLE_MAT4x2 = 0x8F4D,
			GLSL_ATTRIBUTE_DOUBLE_MAT4x3 = 0x8F4E,
			GLSL_ATTRIBUTE_DOUBLE_VEC2 = 0x8FFC,
			GLSL_ATTRIBUTE_DOUBLE_VEC3 = 0x8FFD,
			GLSL_ATTRIBUTE_DOUBLE_VEC4 = 0x8FFE,
			GLSL_ATTRIBUTE_SAMPLER_CUBE_ARRAY = 0x900C,
			GLSL_ATTRIBUTE_SAMPLER_2D_MULTISAMPLE = 0x9108,
			GLSL_ATTRIBUTE_INT_SAMPLER_2D_MULTISAMPLE = 0x9109,
			GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE = 0x910A,
			GLSL_ATTRIBUTE_SAMPLER_2D_MULTISAMPLE_ARRAY = 0x910B,
			GLSL_ATTRIBUTE_INT_SAMPLER_2D_MULTISAMPLE_ARRAY = 0x910C,
			GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY = 0x910D,
		};

		enum class GlslObject
			: GLenum
		{
			eActiveAttributes,
			eActiveUniforms,
		};

		std::string getName( GlslAttributeType type )
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
			case GLSL_ATTRIBUTE_SAMPLER_CUBE_ARRAY:							return "GL_SAMPLER_CUBEMAP_ARRAY";
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

		bool areCompatible( ashes::Format lhs, ashes::Format rhs )
		{
			if ( lhs == rhs )
			{
				return true;
			}

			switch ( lhs )
			{
			case ashes::Format::eR16_SFLOAT:
			case ashes::Format::eR32_SFLOAT:
			case ashes::Format::eR8_UNORM:
			case ashes::Format::eR8_SINT:
			case ashes::Format::eR8_SRGB:
			case ashes::Format::eR8_SSCALED:
				return rhs == ashes::Format::eR32_SFLOAT
					|| rhs == ashes::Format::eR32G32_SFLOAT
					|| rhs == ashes::Format::eR32G32B32_SFLOAT
					|| rhs == ashes::Format::eR32G32B32A32_SFLOAT
					|| rhs == ashes::Format::eR8_UNORM
					|| rhs == ashes::Format::eR8_SINT
					|| rhs == ashes::Format::eR8_SRGB
					|| rhs == ashes::Format::eR8_SSCALED;
			case ashes::Format::eR16G16_SFLOAT:
			case ashes::Format::eR32G32_SFLOAT:
			case ashes::Format::eR8G8_UNORM:
			case ashes::Format::eR8G8_SINT:
			case ashes::Format::eR8G8_SRGB:
			case ashes::Format::eR8G8_SSCALED:
				return rhs == ashes::Format::eR32G32_SFLOAT
					|| rhs == ashes::Format::eR32G32B32_SFLOAT
					|| rhs == ashes::Format::eR32G32B32A32_SFLOAT
					|| rhs == ashes::Format::eR8G8_UNORM
					|| rhs == ashes::Format::eR8G8_SINT
					|| rhs == ashes::Format::eR8G8_SRGB
					|| rhs == ashes::Format::eR8G8_SSCALED;
			case ashes::Format::eR16G16B16_SFLOAT:
			case ashes::Format::eR32G32B32_SFLOAT:
			case ashes::Format::eR8G8B8_UNORM:
			case ashes::Format::eR8G8B8_SINT:
			case ashes::Format::eR8G8B8_SRGB:
			case ashes::Format::eR8G8B8_SSCALED:
				return rhs == ashes::Format::eR32G32B32_SFLOAT
					|| rhs == ashes::Format::eR32G32B32A32_SFLOAT
					|| rhs == ashes::Format::eR8G8B8_UNORM
					|| rhs == ashes::Format::eR8G8B8_SINT
					|| rhs == ashes::Format::eR8G8B8_SRGB
					|| rhs == ashes::Format::eR8G8B8_SSCALED;
			case ashes::Format::eR16G16B16A16_SFLOAT:
			case ashes::Format::eR32G32B32A32_SFLOAT:
			case ashes::Format::eR8G8B8A8_UNORM:
			case ashes::Format::eB8G8R8A8_UNORM:
			case ashes::Format::eR8G8B8A8_SINT:
			case ashes::Format::eR8G8B8A8_SRGB:
			case ashes::Format::eR8G8B8A8_SSCALED:
				return rhs == ashes::Format::eR16G16B16A16_SFLOAT
					|| rhs == ashes::Format::eR32G32B32A32_SFLOAT
					|| rhs == ashes::Format::eR8G8B8A8_UNORM
					|| rhs == ashes::Format::eB8G8R8A8_UNORM
					|| rhs == ashes::Format::eR16G16B16_SFLOAT
					|| rhs == ashes::Format::eR32G32B32_SFLOAT
					|| rhs == ashes::Format::eR8G8B8_UNORM
					|| rhs == ashes::Format::eB8G8R8_UNORM
					|| rhs == ashes::Format::eR8G8B8A8_SINT
					|| rhs == ashes::Format::eR8G8B8A8_SRGB
					|| rhs == ashes::Format::eR8G8B8A8_SSCALED;
			case ashes::Format::eD16_UNORM:
			case ashes::Format::eD16_UNORM_S8_UINT:
			case ashes::Format::eD24_UNORM_S8_UINT:
			case ashes::Format::eD32_SFLOAT:
			case ashes::Format::eD32_SFLOAT_S8_UINT:
				return rhs == ashes::Format::eR32_SFLOAT;
			default:
				assert( false );
				return false;
			}
		}

		ashes::ConstantFormat getFormat( GlslAttributeType type )noexcept
		{
			switch ( type )
			{
			case GLSL_ATTRIBUTE_INT:										return ashes::ConstantFormat::eInt;
			case GLSL_ATTRIBUTE_UNSIGNED_INT:								return ashes::ConstantFormat::eUInt;
			case GLSL_ATTRIBUTE_FLOAT:										return ashes::ConstantFormat::eFloat;
			case GLSL_ATTRIBUTE_FLOAT_VEC2:									return ashes::ConstantFormat::eVec2f;
			case GLSL_ATTRIBUTE_FLOAT_VEC3:									return ashes::ConstantFormat::eVec3f;
			case GLSL_ATTRIBUTE_FLOAT_VEC4:									return ashes::ConstantFormat::eVec4f;
			case GLSL_ATTRIBUTE_INT_VEC2:									return ashes::ConstantFormat::eVec2i;
			case GLSL_ATTRIBUTE_INT_VEC3:									return ashes::ConstantFormat::eVec3i;
			case GLSL_ATTRIBUTE_INT_VEC4:									return ashes::ConstantFormat::eVec4i;
			case GLSL_ATTRIBUTE_FLOAT_MAT2:									return ashes::ConstantFormat::eMat2f;
			case GLSL_ATTRIBUTE_FLOAT_MAT3:									return ashes::ConstantFormat::eMat3f;
			case GLSL_ATTRIBUTE_FLOAT_MAT4:									return ashes::ConstantFormat::eMat4f;
			case GLSL_ATTRIBUTE_UNSIGNED_INT_VEC2:							return ashes::ConstantFormat::eVec2ui;
			case GLSL_ATTRIBUTE_UNSIGNED_INT_VEC3:							return ashes::ConstantFormat::eVec3ui;
			case GLSL_ATTRIBUTE_UNSIGNED_INT_VEC4:							return ashes::ConstantFormat::eVec4ui;
			default:
				assert( false && "Unsupported GLSL attribute type" );
				return ashes::ConstantFormat::eVec4f;
			}
		}

		uint32_t getSize( GlslAttributeType type )
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
			case GLSL_ATTRIBUTE_SAMPLER_BUFFER:
			case GLSL_ATTRIBUTE_SAMPLER_1D_ARRAY_SHADOW:
			case GLSL_ATTRIBUTE_SAMPLER_2D_ARRAY_SHADOW:
			case GLSL_ATTRIBUTE_SAMPLER_CUBE_SHADOW:
			case GLSL_ATTRIBUTE_INT_SAMPLER_1D:
			case GLSL_ATTRIBUTE_INT_SAMPLER_2D:
			case GLSL_ATTRIBUTE_INT_SAMPLER_3D:
			case GLSL_ATTRIBUTE_INT_SAMPLER_CUBE:
			case GLSL_ATTRIBUTE_INT_SAMPLER_2D_RECT:
			case GLSL_ATTRIBUTE_INT_SAMPLER_1D_ARRAY:
			case GLSL_ATTRIBUTE_INT_SAMPLER_2D_ARRAY:
			case GLSL_ATTRIBUTE_INT_SAMPLER_BUFFER:
			case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_1D:
			case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_2D:
			case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_3D:
			case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_CUBE:
			case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_2D_RECT:
			case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_1D_ARRAY:
			case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_2D_ARRAY:
			case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_BUFFER:
			case GLSL_ATTRIBUTE_SAMPLER_2D_MULTISAMPLE:
			case GLSL_ATTRIBUTE_INT_SAMPLER_2D_MULTISAMPLE:
			case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:
			case GLSL_ATTRIBUTE_SAMPLER_2D_MULTISAMPLE_ARRAY:
			case GLSL_ATTRIBUTE_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
			case GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
				return 0u;
				break;
			default:
				assert( false && "Unsupported attribute type" );
				return 4u;
			}
		}

		bool isSampler( GlslAttributeType type )
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
				|| type == GLSL_ATTRIBUTE_SAMPLER_BUFFER
				|| type == GLSL_ATTRIBUTE_SAMPLER_1D_ARRAY_SHADOW
				|| type == GLSL_ATTRIBUTE_SAMPLER_2D_ARRAY_SHADOW
				|| type == GLSL_ATTRIBUTE_SAMPLER_CUBE_SHADOW
				|| type == GLSL_ATTRIBUTE_INT_SAMPLER_1D
				|| type == GLSL_ATTRIBUTE_INT_SAMPLER_2D
				|| type == GLSL_ATTRIBUTE_INT_SAMPLER_3D
				|| type == GLSL_ATTRIBUTE_INT_SAMPLER_CUBE
				|| type == GLSL_ATTRIBUTE_INT_SAMPLER_2D_RECT
				|| type == GLSL_ATTRIBUTE_INT_SAMPLER_1D_ARRAY
				|| type == GLSL_ATTRIBUTE_INT_SAMPLER_2D_ARRAY
				|| type == GLSL_ATTRIBUTE_INT_SAMPLER_BUFFER
				|| type == GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_1D
				|| type == GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_2D
				|| type == GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_3D
				|| type == GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_CUBE
				|| type == GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_2D_RECT
				|| type == GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_1D_ARRAY
				|| type == GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_2D_ARRAY
				|| type == GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_BUFFER
				|| type == GLSL_ATTRIBUTE_SAMPLER_CUBE_ARRAY
				|| type == GLSL_ATTRIBUTE_SAMPLER_2D_MULTISAMPLE
				|| type == GLSL_ATTRIBUTE_INT_SAMPLER_2D_MULTISAMPLE
				|| type == GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE
				|| type == GLSL_ATTRIBUTE_SAMPLER_2D_MULTISAMPLE_ARRAY
				|| type == GLSL_ATTRIBUTE_INT_SAMPLER_2D_MULTISAMPLE_ARRAY
				|| type == GLSL_ATTRIBUTE_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY;
		}

		ashes::Format convertFormat( GlslAttributeType type )
		{
			switch ( type )
			{
			case GLSL_ATTRIBUTE_FLOAT:
				return ashes::Format::eR32_SFLOAT;
			case GLSL_ATTRIBUTE_FLOAT_VEC2:
				return ashes::Format::eR32G32_SFLOAT;
			case GLSL_ATTRIBUTE_FLOAT_VEC3:
				return ashes::Format::eR32G32B32_SFLOAT;
			case GLSL_ATTRIBUTE_FLOAT_VEC4:
				return ashes::Format::eR32G32B32A32_SFLOAT;
			default:
				assert( false );
				return ashes::Format::eR32G32B32A32_SFLOAT;
			}
		}

		bool areCompatibleInputs( ashes::Format lhs, ashes::Format rhs )
		{
			if ( lhs == rhs )
			{
				return true;
			}

			switch ( lhs )
			{
			case ashes::Format::eR32G32B32A32_SFLOAT:
				return rhs == ashes::Format::eR32G32B32_SFLOAT
					|| rhs == ashes::Format::eR32G32B32A32_SFLOAT
					|| rhs == ashes::Format::eR32G32B32A32_SINT
					|| rhs == ashes::Format::eR32G32B32A32_UINT
					|| rhs == ashes::Format::eR8G8B8A8_UNORM;
			case ashes::Format::eR32G32B32A32_SINT:
				return rhs == ashes::Format::eR32G32B32_SINT
					|| rhs == ashes::Format::eR32G32B32A32_SFLOAT
					|| rhs == ashes::Format::eR32G32B32A32_SINT
					|| rhs == ashes::Format::eR32G32B32A32_UINT
					|| rhs == ashes::Format::eR8G8B8A8_UNORM;
			case ashes::Format::eR32G32B32A32_UINT:
				return rhs == ashes::Format::eR32G32B32_UINT
					|| rhs == ashes::Format::eR32G32B32A32_SFLOAT
					|| rhs == ashes::Format::eR32G32B32A32_SINT
					|| rhs == ashes::Format::eR32G32B32A32_UINT
					|| rhs == ashes::Format::eR8G8B8A8_UNORM;
			case ashes::Format::eR8G8B8A8_UNORM:
				return rhs == ashes::Format::eR32G32B32_SFLOAT
					|| rhs == ashes::Format::eR32G32B32A32_SFLOAT
					|| rhs == ashes::Format::eR32G32B32A32_SINT
					|| rhs == ashes::Format::eR32G32B32A32_UINT
					|| rhs == ashes::Format::eR8G8B8A8_UNORM;
			case ashes::Format::eR32G32B32_SFLOAT:
				return rhs == ashes::Format::eR32G32B32A32_SFLOAT
					|| rhs == ashes::Format::eR32G32B32_SFLOAT
					|| rhs == ashes::Format::eR32G32B32_SINT
					|| rhs == ashes::Format::eR32G32B32_UINT;
			case ashes::Format::eR32G32B32_SINT:
				return rhs == ashes::Format::eR32G32B32A32_SINT
					|| rhs == ashes::Format::eR32G32B32_SFLOAT
					|| rhs == ashes::Format::eR32G32B32_SINT
					|| rhs == ashes::Format::eR32G32B32_UINT;
			case ashes::Format::eR32G32B32_UINT:
				return rhs == ashes::Format::eR32G32B32A32_UINT
					|| rhs == ashes::Format::eR32G32B32_SFLOAT
					|| rhs == ashes::Format::eR32G32B32_SINT
					|| rhs == ashes::Format::eR32G32B32_UINT;
			case ashes::Format::eR32G32_SFLOAT:
			case ashes::Format::eR32G32_SINT:
			case ashes::Format::eR32G32_UINT:
				return rhs == ashes::Format::eR32G32_SFLOAT
					|| rhs == ashes::Format::eR32G32_SINT
					|| rhs == ashes::Format::eR32G32_UINT;
			case ashes::Format::eR32_SINT:
			case ashes::Format::eR32_UINT:
			case ashes::Format::eR32_SFLOAT:
				return rhs == ashes::Format::eR32_SINT
					|| rhs == ashes::Format::eR32_UINT
					|| rhs == ashes::Format::eR32_SFLOAT;
			default:
				assert( false );
				return false;
			}
		}

		ashes::Format convertAttribute( GlslAttributeType type )
		{
			switch ( type )
			{
			case GLSL_ATTRIBUTE_INT:
				return ashes::Format::eR32_SINT;
			case GLSL_ATTRIBUTE_UNSIGNED_INT:
				return ashes::Format::eR32_UINT;
			case GLSL_ATTRIBUTE_FLOAT:
				return ashes::Format::eR32_SFLOAT;
			case GLSL_ATTRIBUTE_FLOAT_VEC2:
				return ashes::Format::eR32G32_SFLOAT;
			case GLSL_ATTRIBUTE_FLOAT_VEC3:
				return ashes::Format::eR32G32B32_SFLOAT;
			case GLSL_ATTRIBUTE_FLOAT_VEC4:
				return ashes::Format::eR32G32B32A32_SFLOAT;
			case GLSL_ATTRIBUTE_INT_VEC2:
				return ashes::Format::eR32G32_SINT;
			case GLSL_ATTRIBUTE_INT_VEC3:
				return ashes::Format::eR32G32B32_SINT;
			case GLSL_ATTRIBUTE_INT_VEC4:
				return ashes::Format::eR32G32B32A32_SINT;
			//case GLSL_ATTRIBUTE_FLOAT_MAT2:
			//	return ashes::Format::eMat2f;
			//case GLSL_ATTRIBUTE_FLOAT_MAT3:
			//	return ashes::Format::eMat3f;
			//case GLSL_ATTRIBUTE_FLOAT_MAT4:
			//	return ashes::Format::eMat4f;
			case GLSL_ATTRIBUTE_UNSIGNED_INT_VEC2:
				return ashes::Format::eR32G32_UINT;
			case GLSL_ATTRIBUTE_UNSIGNED_INT_VEC3:
				return ashes::Format::eR32G32B32_UINT;
			case GLSL_ATTRIBUTE_UNSIGNED_INT_VEC4:
				return ashes::Format::eR32G32B32A32_UINT;
			default:
				assert( false );
				return ashes::Format::eR32_SFLOAT;
			}
		}

		template< typename FuncType >
		void getProgramInterfaceInfos( ContextLock const & context
			, uint32_t program
			, GlslInterface interface
			, std::vector< GlslProperty > const & properties
			, FuncType function )
		{
			int count = 0;
			context->glGetProgramInterfaceiv( program
				, interface
				, GLSL_DATANAME_MAX_NAME_LENGTH
				, &count );
			std::vector< char > buffer( count );
			context->glGetProgramInterfaceiv( program
				, interface
				, GLSL_DATANAME_ACTIVE_RESOURCES
				, &count );
			std::vector< GLint > values;
			values.resize( properties.size() );
			std::vector< GLenum > props;

			for ( auto & prop : properties )
			{
				props.push_back( prop );
			}

			for ( int i = 0; i < count; ++i )
			{
				GLsizei length;
				context->glGetProgramResourceName( program
					, interface
					, i
					, uint32_t( buffer.size() )
					, &length
					, buffer.data() );
				std::string name( buffer.data(), length );
				context->glGetProgramResourceiv( program
					, interface
					, i
					, GLsizei( props.size() )
					, props.data()
					, GLsizei( values.size() )
					, nullptr
					, values.data() );
				function( name, values );
			}
		}

		template< typename BufFuncType, typename VarFuncType >
		void getProgramBufferInfos( ContextLock const & context
			, uint32_t program
			, GlslInterface bufferInterface
			, GlslInterface variableInterface
			, BufFuncType bufferFunction
			, VarFuncType variableFunction )
		{
			GLint maxNameLength = 0;
			context->glGetProgramInterfaceiv( program
				, bufferInterface
				, GLSL_DATANAME_MAX_NAME_LENGTH
				, &maxNameLength );
			std::vector< char > buffer( maxNameLength );
			GLint numBlocks;
			context->glGetProgramInterfaceiv( program
				, bufferInterface
				, GLSL_DATANAME_ACTIVE_RESOURCES
				, &numBlocks );
			GLenum const blockBinding[1] = { GLSL_PROPERTY_BUFFER_BINDING };
			GLenum const activeUniformsCount[1] = { GLSL_PROPERTY_NUM_ACTIVE_VARIABLES };
			GLenum const activeUniforms[1] = { GLSL_PROPERTY_ACTIVE_VARIABLES };
			GLenum const uniformProperties[3] = { GLSL_PROPERTY_NAME_LENGTH, GLSL_PROPERTY_TYPE, GLSL_PROPERTY_LOCATION };

			for ( int blockIx = 0; blockIx < numBlocks; ++blockIx )
			{
				GLsizei nameLength = 0;
				context->glGetProgramResourceName( program
					, bufferInterface
					, blockIx
					, uint32_t( buffer.size() )
					, &nameLength
					, buffer.data() );
				std::string bufferName( buffer.data(), nameLength );
				GLint binding = 0;
				context->glGetProgramResourceiv( program
					, bufferInterface
					, blockIx
					, 1
					, blockBinding
					, 1
					, nullptr
					, &binding );
				GLuint index = context->glGetProgramResourceIndex( program
					, bufferInterface
					, bufferName.c_str() );
				GLint numActiveUnifs = 0;
				context->glGetProgramResourceiv( program
					, bufferInterface
					, blockIx
					, 1
					, activeUniformsCount
					, 1
					, nullptr
					, &numActiveUnifs );
				bufferFunction( bufferName
					, binding
					, index
					, numActiveUnifs );

				if ( numActiveUnifs )
				{
					std::vector< GLint > blockUnifs( numActiveUnifs );
					context->glGetProgramResourceiv( program
						, bufferInterface
						, blockIx
						, 1
						, activeUniforms
						, numActiveUnifs
						, nullptr
						, blockUnifs.data() );

					for ( GLint unifIx = 0; unifIx < numActiveUnifs; ++unifIx )
					{
						GLint values[3];
						context->glGetProgramResourceiv( program
							, variableInterface
							, blockUnifs[unifIx]
							, 3
							, uniformProperties
							, 3
							, nullptr
							, values );
						std::vector< char > nameData( values[0] );
						context->glGetProgramResourceName( program
							, variableInterface
							, blockUnifs[unifIx]
							, GLsizei( nameData.size() )
							, nullptr
							, &nameData[0] );
						std::string variableName( nameData.begin(), nameData.end() - 1 );
						variableFunction( variableName, GlslAttributeType( values[1] ), values[2] );
					}
				}
			}
		}

		template< typename VarFuncType >
		void getVariableInfos( ContextLock const & context
			, uint32_t program
			, GlslInterface variableInterface
			, VarFuncType variableFunction )
		{
			static GLenum constexpr properties[]
			{
				GLSL_PROPERTY_BLOCK_INDEX,
				GLSL_PROPERTY_TYPE,
				GLSL_PROPERTY_NAME_LENGTH,
				GLSL_PROPERTY_LOCATION,
				GLSL_PROPERTY_ARRAY_SIZE,
				GLSL_PROPERTY_OFFSET
			};
			static uint32_t constexpr count = uint32_t( sizeof( properties ) / sizeof( *properties ) );
			GLint numUniforms = 0;
			context->glGetProgramInterfaceiv( program
				, variableInterface
				, GLSL_DATANAME_ACTIVE_RESOURCES
				, &numUniforms );

			for ( int unif = 0; unif < numUniforms; ++unif )
			{
				GLint values[count];
				context->glGetProgramResourceiv( program
					, variableInterface
					, unif
					, count
					, properties
					, count
					, nullptr
					, values );

				// Skip any uniforms that are in a block.
				if ( values[0] == -1 )
				{
					std::vector< char > nameData( values[2] );
					context->glGetProgramResourceName( program
						, variableInterface
						, unif
						, GLsizei( nameData.size() )
						, nullptr
						, &nameData[0] );
					std::string variableName( nameData.begin(), nameData.end() - 1 );
					variableFunction( variableName
						, GlslAttributeType( values[1] )
						, values[3]
						, values[4]
						, values[5] );
				}
			}
		}

		template< typename FuncType >
		void getUnnamedProgramInterfaceInfos( ContextLock const & context
			, uint32_t program
			, GlslInterface interface
			, GlslProperty property
			, FuncType function )
		{
			int count = 0;
			context->glGetProgramInterfaceiv( program
				, interface
				, GLSL_DATANAME_ACTIVE_RESOURCES
				, &count );
			std::vector< int > values( count );
			std::vector< int > lengths( count );

			for ( int i = 0; i < count; ++i )
			{
				GLenum prop = property;
				context->glGetProgramResourceiv( program
					, interface
					, i
					, 1
					, &prop
					, 1
					, &lengths[i]
					, &values[i] );
			}

			if ( count )
			{
				function( values );
			}
		}

		struct AttrSpec
		{
			ashes::Format format;
			uint32_t location;
		};

		void doValidateInputs( ContextLock const & context
			, GLuint program
			, ashes::VertexInputState const & vertexInputState )
		{
			std::vector< AttrSpec > attributes;

			for ( auto & attribute : vertexInputState.vertexAttributeDescriptions )
			{
				attributes.push_back( { attribute.format, attribute.location } );
			}

			auto findAttribute = [&attributes]( std::string const & name
				, GlslAttributeType glslType
				, uint32_t location )
			{
				auto it = std::find_if( attributes.begin()
					, attributes.end()
					, [&glslType, &location]( AttrSpec const & lookup )
					{
						return areCompatibleInputs( lookup.format, convertAttribute( glslType ) )
							&& lookup.location == location;
					} );

				if ( it != attributes.end() )
				{
					attributes.erase( it );
				}
				else if ( name.find( "gl_" ) != 0u )
				{
					std::stringstream stream;
					stream << ValidationError
						<< "Attribute [" << name
						<< "], of type: " << getName( glslType )
						<< ", at location: " << location
						<< " is used in the shader program, but is not listed in the vertex layouts" << std::endl;
					ashes::Logger::logError( stream.str() );
					throw std::logic_error{ stream.str() };
				}
			};

			getProgramInterfaceInfos( context
				, program
				, GLSL_INTERFACE_PROGRAM_INPUT
				, { GLSL_PROPERTY_TYPE, GLSL_PROPERTY_ARRAY_SIZE, GLSL_PROPERTY_LOCATION/*, GLSL_PROPERTY_LOCATION_COMPONENT*/ }
				, [&attributes, &findAttribute]( std::string const & name, std::vector< GLint > const & values )
				{
					auto glslType = GlslAttributeType( values[0] );
					auto location = uint32_t( values[2] );

					switch ( glslType )
					{
					case GLSL_ATTRIBUTE_FLOAT_MAT2:
						findAttribute( name, GLSL_ATTRIBUTE_FLOAT_VEC2, location + 0u );
						findAttribute( name, GLSL_ATTRIBUTE_FLOAT_VEC2, location + 1u );
						break;
					case GLSL_ATTRIBUTE_FLOAT_MAT3:
						findAttribute( name, GLSL_ATTRIBUTE_FLOAT_VEC3, location + 0u );
						findAttribute( name, GLSL_ATTRIBUTE_FLOAT_VEC3, location + 1u );
						findAttribute( name, GLSL_ATTRIBUTE_FLOAT_VEC3, location + 2u );
						break;
					case GLSL_ATTRIBUTE_FLOAT_MAT4:
						findAttribute( name, GLSL_ATTRIBUTE_FLOAT_VEC4, location + 0u );
						findAttribute( name, GLSL_ATTRIBUTE_FLOAT_VEC4, location + 1u );
						findAttribute( name, GLSL_ATTRIBUTE_FLOAT_VEC4, location + 2u );
						findAttribute( name, GLSL_ATTRIBUTE_FLOAT_VEC4, location + 3u );
						break;
					default:
						findAttribute( name, glslType, location );
						break;
					}
				} );

			for ( auto & attribute : attributes )
			{
				ashes::Logger::logWarning( std::stringstream{} << ValidationWarning
					<< "Vertex layout has attribute of type " << getName( attribute.format )
					<< ", at location " << attribute.location
					<< ", which is not used by the program" );
			}
		}

		void doValidateOutputs( ContextLock const & context
			, GLuint program
			, RenderPass const & renderPass )
		{
			std::set< ashes::AttachmentDescription const * > attaches;

			for ( auto & attach : renderPass.getAttachments() )
			{
				attaches.insert( &attach );
			}

			struct GlslOutput
			{
				std::string name;
				GlslAttributeType type;
				uint32_t location;
			};
			std::vector< GlslOutput > outputs;

			getProgramInterfaceInfos( context
				, program
				, GLSL_INTERFACE_PROGRAM_OUTPUT
				, { GLSL_PROPERTY_TYPE, GLSL_PROPERTY_ARRAY_SIZE, GLSL_PROPERTY_LOCATION/*, GLSL_PROPERTY_LOCATION_COMPONENT*/ }
				, [&outputs]( std::string name, std::vector< GLint > const & values )
				{
					outputs.push_back( { name, GlslAttributeType( values[0] ), uint32_t( values[2] ) } );
				} );

			for ( auto & output : outputs )
			{
				bool found = false;

				if ( output.location != ~( 0u ) )
				{
					if ( renderPass.getColourAttaches().size() > output.location )
					{
						auto & attach = renderPass.getColourAttaches()[output.location];

						if ( areCompatible( attach.attach.get().format, convertFormat( output.type ) ) )
						{
							found = true;
							attaches.erase( &attach.attach.get() );
						}
					}

					if ( !found )
					{
						ashes::Logger::logError( std::stringstream{} << ValidationError
							<< "Attachment [" << output.name
							<< "], of type: " << getName( output.type )
							<< ", at location: " << output.location
							<< " is used in the shader program, but is not listed in the render pass attachments" );
					}
				}
				else
				{
					auto it = std::find_if( attaches.begin()
						, attaches.end()
						, [&output]( ashes::AttachmentDescription const * lookup )
						{
							return areCompatible( lookup->format, convertFormat( output.type ) );
						} );

					if ( it != attaches.end() )
					{
						attaches.erase( it );
					}
				}
			}

			for ( auto & attach : attaches )
			{
				if ( !ashes::isDepthOrStencilFormat( attach->format ) )
				{
					ashes::Logger::logWarning( std::stringstream{} << ValidationWarning
						<< "Render pass has an attahment of type " << ashes::getName( attach->format )
						<< ", which is not used by the program" );
				}
			}
		}

		void doValidateUbos( ContextLock const & context
			, GLuint program )
		{
			getProgramBufferInfos( context
				, program
				, GLSL_INTERFACE_UNIFORM_BLOCK
				, GLSL_INTERFACE_UNIFORM
				, []( std::string name
					, GLint point
					, GLuint index
					, GLint variables )
				{
					ashes::Logger::logDebug( std::stringstream{} << "   Uniform block: " << name
						<< ", at point " << point
						<< ", and index " << index
						<< ", active variables " << variables );
				}
				, []( std::string name
					, GlslAttributeType type
					, GLint location )
				{
					ashes::Logger::logDebug( std::stringstream{} << "      variable: " << name
						<< ", type " << getName( type )
						<< ", at location " << location );
				} );
		}

		void doValidateSsbos( ContextLock const & context
			, GLuint program )
		{
			getProgramBufferInfos( context
				, program
				, GLSL_INTERFACE_SHADER_STORAGE_BLOCK
				, GLSL_INTERFACE_BUFFER_VARIABLE
				, []( std::string name
					, GLint point
					, GLuint index
					, GLint variables )
				{
					ashes::Logger::logDebug( std::stringstream{} << "   ShaderStorage block: " << name
						<< ", at point " << point
						<< ", and index " << index
						<< ", active variables " << variables );
				}
				, []( std::string name
					, GlslAttributeType type
					, GLint location )
				{
					ashes::Logger::logDebug( std::stringstream{} << "      variable: " << name
						<< ", type " << getName( type )
						<< ", at location " << location );
				} );
		}

		void doValidateUniforms( ContextLock const & context
			, GLuint program )
		{
			getVariableInfos( context
				, program
				, GLSL_INTERFACE_UNIFORM
				, []( std::string name
					, GlslAttributeType type
					, GLint location
					, GLint arraySize
					, GLint offset )
				{
					ashes::Logger::logDebug( std::stringstream{} << "   Uniform variable: " << name
						<< ", type: " << getName( type )
						<< ", location: " << location
						<< ", arraySize: " << arraySize
						<< ", offset: " << offset );
				} );
		}
	}

	InputLayout getInputLayout( ContextLock const & context
		, GLuint program )
	{
		InputLayout result;
		getProgramInterfaceInfos( context
			, program
			, GLSL_INTERFACE_PROGRAM_INPUT
			, { GLSL_PROPERTY_TYPE, GLSL_PROPERTY_ARRAY_SIZE, GLSL_PROPERTY_LOCATION/*, GLSL_PROPERTY_LOCATION_COMPONENT*/ }
			, [&result]( std::string const & name, std::vector< GLint > const & values )
			{
				auto type = GlslAttributeType( values[0] );
				auto location = uint32_t( values[2] );

				switch ( type )
				{
				case GLSL_ATTRIBUTE_FLOAT_MAT2:
					result.push_back( { name, ashes::ConstantFormat::eVec2f, location + 0u } );
					result.push_back( { name, ashes::ConstantFormat::eVec2f, location + 1u } );
					break;
				case GLSL_ATTRIBUTE_FLOAT_MAT3:
					result.push_back( { name, ashes::ConstantFormat::eVec3f, location + 0u } );
					result.push_back( { name, ashes::ConstantFormat::eVec3f, location + 1u } );
					result.push_back( { name, ashes::ConstantFormat::eVec3f, location + 2u } );
					break;
				case GLSL_ATTRIBUTE_FLOAT_MAT4:
					result.push_back( { name, ashes::ConstantFormat::eVec4f, location + 0u } );
					result.push_back( { name, ashes::ConstantFormat::eVec4f, location + 1u } );
					result.push_back( { name, ashes::ConstantFormat::eVec4f, location + 2u } );
					result.push_back( { name, ashes::ConstantFormat::eVec4f, location + 3u } );
					break;
				default:
					result.push_back( { name, getFormat( type ), location } );
					break;
				}
			} );
		return result;
	}

	InterfaceBlockLayout getInterfaceBlockLayout( ContextLock const & context
		, GLuint program )
	{
		InterfaceBlockLayout result;
		getProgramBufferInfos( context
			, program
			, GLSL_INTERFACE_UNIFORM_BLOCK
			, GLSL_INTERFACE_UNIFORM
			, [&result]( std::string name
				, GLint point
				, GLuint index
				, GLint variables )
			{
				result.push_back( { name, uint32_t( point ), 0u } );
			}
			, [&result]( std::string name
				, GlslAttributeType type
				, GLint location )
			{
				result.back().constants.push_back( { name, uint32_t( location ), getFormat( type ), getSize( type ) } );
			} );
		return result;
	}

	ConstantsLayout getConstantsLayout( ContextLock const & context
		, GLuint program )
	{
		ConstantsLayout result;
		getVariableInfos( context
			, program
			, GLSL_INTERFACE_UNIFORM
			, [&result]( std::string name
				, GlslAttributeType type
				, GLint location
				, GLint arraySize
				, GLint offset )
			{
				if ( !isSampler( type ) )
				{
					result.push_back( { name
						, uint32_t( location )
						, getFormat( type )
						, getSize( type )
						, uint32_t( arraySize )
						, ( offset == -1
							? 0u
							: uint32_t( offset ) ) } );
				}
			} );
		return result;
	}

	void validatePipeline( ContextLock const & context
		, PipelineLayout const & layout
		, GLuint program
		, ashes::VertexInputState const & vertexInputState
		, ashes::RenderPass const & renderPass )
	{
		doValidateInputs( context, program, vertexInputState );
		doValidateOutputs( context, program, static_cast< RenderPass const & >( renderPass ) );
		//doValidateUbos( context, program );
		//doValidateSsbos( context, program );
		//doValidateUniforms( context, program );
	}
}

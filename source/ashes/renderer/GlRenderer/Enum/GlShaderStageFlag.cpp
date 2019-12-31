#include "GlRendererPrerequisites.hpp"

namespace ashes::gl
{
	std::string getShaderStageName( GlShaderStage value )
	{
		switch ( value )
		{
		case GL_SHADER_STAGE_VERTEX:
			return "GL_VERTEX_SHADER";
		case GL_SHADER_STAGE_TESS_CONTROL:
			return "GL_TESS_CONTROL_SHADER";
		case GL_SHADER_STAGE_TESS_EVALUATION:
			return "GL_TESS_EVALUATION_SHADER";
		case GL_SHADER_STAGE_GEOMETRY:
			return "GL_GEOMETRY_SHADER";
		case GL_SHADER_STAGE_FRAGMENT:
			return "GL_FRAGMENT_SHADER";
		case GL_SHADER_STAGE_COMPUTE:
			return "GL_COMPUTE_SHADER";
		default:
			return "GL_UNKNOWN_SHADER";
		}
	}

	GlShaderStage getShaderStage( VkShaderStageFlagBits const & flags )
	{
		switch ( flags )
		{
		case VK_SHADER_STAGE_VERTEX_BIT:
			return GL_SHADER_STAGE_VERTEX;
		case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
			return GL_SHADER_STAGE_TESS_CONTROL;
		case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
			return GL_SHADER_STAGE_TESS_EVALUATION;
		case VK_SHADER_STAGE_GEOMETRY_BIT:
			return GL_SHADER_STAGE_GEOMETRY;
		case VK_SHADER_STAGE_FRAGMENT_BIT:
			return GL_SHADER_STAGE_FRAGMENT;
		case VK_SHADER_STAGE_COMPUTE_BIT:
			return GL_SHADER_STAGE_COMPUTE;
		default:
			return GL_SHADER_STAGE_VERTEX;
		}
	}

	std::string getShaderStageFlagsName( GlShaderStageFlags value )
	{
		std::string result;
		std::string sep;

		if ( ashes::checkFlag( value, GL_SHADER_STAGE_FLAG_VERTEX ) )
		{
			result += sep + " GL_VERTEX_SHADER_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GL_SHADER_STAGE_FLAG_TESS_CONTROL ) )
		{
			result += sep + "GL_TESS_CONTROL_SHADER_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GL_SHADER_STAGE_FLAG_TESS_EVALUATION ) )
		{
			result += sep + "GL_TESS_EVALUATION_SHADER_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GL_SHADER_STAGE_FLAG_GEOMETRY ) )
		{
			result += sep + "GL_GEOMETRY_SHADER_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GL_SHADER_STAGE_FLAG_FRAGMENT ) )
		{
			result += sep + "GL_FRAGMENT_SHADER_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GL_SHADER_STAGE_FLAG_COMPUTE ) )
		{
			result += sep + "GL_COMPUTE_SHADER_BIT";
		}

		return result;
	}

	GlShaderStageFlag convertShaderStageFlag( VkShaderStageFlagBits const & flag )
	{
		switch ( flag )
		{
		case VK_SHADER_STAGE_VERTEX_BIT:
			return GL_SHADER_STAGE_FLAG_VERTEX;

		case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
			return GL_SHADER_STAGE_FLAG_TESS_CONTROL;

		case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
			return GL_SHADER_STAGE_FLAG_TESS_EVALUATION;

		case VK_SHADER_STAGE_GEOMETRY_BIT:
			return GL_SHADER_STAGE_FLAG_GEOMETRY;

		case VK_SHADER_STAGE_FRAGMENT_BIT:
			return GL_SHADER_STAGE_FLAG_FRAGMENT;

		case VK_SHADER_STAGE_COMPUTE_BIT:
			return GL_SHADER_STAGE_FLAG_COMPUTE;

		default:
			assert( false && "Unsupported shader stage." );
			return GL_SHADER_STAGE_FLAG_VERTEX;
		}
	}

	GlShaderStageFlags convertShaderStageFlags( VkShaderStageFlags const & flags )
	{
		GlShaderStageFlags result{};

		if ( ashes::checkFlag( flags, VK_SHADER_STAGE_VERTEX_BIT ) )
		{
			result |= GL_SHADER_STAGE_FLAG_VERTEX;
		}

		if ( ashes::checkFlag( flags, VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT ) )
		{
			result |= GL_SHADER_STAGE_FLAG_TESS_CONTROL;
		}

		if ( ashes::checkFlag( flags, VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT ) )
		{
			result |= GL_SHADER_STAGE_FLAG_TESS_EVALUATION;
		}

		if ( ashes::checkFlag( flags, VK_SHADER_STAGE_GEOMETRY_BIT ) )
		{
			result |= GL_SHADER_STAGE_FLAG_GEOMETRY;
		}

		if ( ashes::checkFlag( flags, VK_SHADER_STAGE_FRAGMENT_BIT ) )
		{
			result |= GL_SHADER_STAGE_FLAG_FRAGMENT;
		}

		if ( ashes::checkFlag( flags, VK_SHADER_STAGE_COMPUTE_BIT ) )
		{
			result |= GL_SHADER_STAGE_FLAG_COMPUTE;
		}

		return result;
	}
}

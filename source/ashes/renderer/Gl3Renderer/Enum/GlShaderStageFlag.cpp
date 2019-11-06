#include "GlRendererPrerequisites.hpp"

namespace ashes::gl3
{
	std::string getShaderStageFlagName( GlShaderStageFlags value )
	{
		std::string result;
		std::string sep;

		if ( ashes::checkFlag( value, GlShaderStageFlag::GL_SHADER_STAGE_VERTEX ) )
		{
			result += sep + " GL_VERTEX_SHADER";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GlShaderStageFlag::GL_SHADER_STAGE_TESS_CONTROL ) )
		{
			result += sep + "GL_TESS_CONTROL_SHADER";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GlShaderStageFlag::GL_SHADER_STAGE_TESS_EVALUATION ) )
		{
			result += sep + "GL_TESS_EVALUATION_SHADER";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GlShaderStageFlag::GL_SHADER_STAGE_GEOMETRY ) )
		{
			result += sep + "GL_GEOMETRY_SHADER";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GlShaderStageFlag::GL_SHADER_STAGE_FRAGMENT ) )
		{
			result += sep + "GL_FRAGMENT_SHADER";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GlShaderStageFlag::GL_SHADER_STAGE_COMPUTE ) )
		{
			result += sep + "GL_COMPUTE_SHADER";
		}

		return result;
	}
	
	std::string getShaderStageFlagName( VkShaderStageFlags value )
	{
		std::string result;
		std::string sep;

		if ( ashes::checkFlag( value, VK_SHADER_STAGE_VERTEX_BIT ) )
		{
			result += sep + " VK_SHADER_STAGE_VERTEX_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT ) )
		{
			result += sep + "VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT ) )
		{
			result += sep + "VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, VK_SHADER_STAGE_GEOMETRY_BIT ) )
		{
			result += sep + "VK_SHADER_STAGE_GEOMETRY_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, VK_SHADER_STAGE_FRAGMENT_BIT ) )
		{
			result += sep + "VK_SHADER_STAGE_FRAGMENT_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, VK_SHADER_STAGE_COMPUTE_BIT ) )
		{
			result += sep + "VK_SHADER_STAGE_COMPUTE_BIT";
		}

		return result;
	}

	GlShaderStageFlags convertShaderStageFlags( VkShaderStageFlags const & flags )
	{
		GLenum result{ 0 };

		if ( ashes::checkFlag( flags, VK_SHADER_STAGE_VERTEX_BIT ) )
		{
			result |= GL_SHADER_STAGE_VERTEX;
		}

		if ( ashes::checkFlag( flags, VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT ) )
		{
			result |= GL_SHADER_STAGE_TESS_CONTROL;
		}

		if ( ashes::checkFlag( flags, VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT ) )
		{
			result |= GL_SHADER_STAGE_TESS_EVALUATION;
		}

		if ( ashes::checkFlag( flags, VK_SHADER_STAGE_GEOMETRY_BIT ) )
		{
			result |= GL_SHADER_STAGE_GEOMETRY;
		}

		if ( ashes::checkFlag( flags, VK_SHADER_STAGE_FRAGMENT_BIT ) )
		{
			result |= GL_SHADER_STAGE_FRAGMENT;
		}

		if ( ashes::checkFlag( flags, VK_SHADER_STAGE_COMPUTE_BIT ) )
		{
			result |= GL_SHADER_STAGE_COMPUTE;
		}

		return result;
	}

	GlShaderStageFlag convertShaderStageFlag( VkShaderStageFlagBits const & flag )
	{
		switch ( flag )
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
			assert( false && "Unsupported shader stage." );
			return GL_SHADER_STAGE_VERTEX;
		}
	}
}

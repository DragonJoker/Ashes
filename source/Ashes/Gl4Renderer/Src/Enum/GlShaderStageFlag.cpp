#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	std::string getName( GlShaderStageFlags value )
	{
		std::string result;
		std::string sep;

		if ( checkFlag( value, gl_renderer::GlShaderStageFlag::GL_SHADER_STAGE_VERTEX ) )
		{
			result += sep + " GL_VERTEX_SHADER";
			sep = " | ";
		}

		if ( checkFlag( value, gl_renderer::GlShaderStageFlag::GL_SHADER_STAGE_TESS_CONTROL ) )
		{
			result += sep + "GL_TESS_CONTROL_SHADER";
			sep = " | ";
		}

		if ( checkFlag( value, gl_renderer::GlShaderStageFlag::GL_SHADER_STAGE_TESS_EVALUATION ) )
		{
			result += sep + "GL_TESS_EVALUATION_SHADER";
			sep = " | ";
		}

		if ( checkFlag( value, gl_renderer::GlShaderStageFlag::GL_SHADER_STAGE_GEOMETRY ) )
		{
			result += sep + "GL_GEOMETRY_SHADER";
			sep = " | ";
		}

		if ( checkFlag( value, gl_renderer::GlShaderStageFlag::GL_SHADER_STAGE_FRAGMENT ) )
		{
			result += sep + "GL_FRAGMENT_SHADER";
			sep = " | ";
		}

		if ( checkFlag( value, gl_renderer::GlShaderStageFlag::GL_SHADER_STAGE_COMPUTE ) )
		{
			result += sep + "GL_COMPUTE_SHADER";
		}

		return result;
	}

	GlShaderStageFlags convert( ashes::ShaderStageFlags const & flags )
	{
		GLenum result{ 0 };

		if ( checkFlag( flags, ashes::ShaderStageFlag::eVertex ) )
		{
			result |= GL_SHADER_STAGE_VERTEX;
		}

		if ( checkFlag( flags, ashes::ShaderStageFlag::eTessellationControl ) )
		{
			result |= GL_SHADER_STAGE_TESS_CONTROL;
		}

		if ( checkFlag( flags, ashes::ShaderStageFlag::eTessellationEvaluation ) )
		{
			result |= GL_SHADER_STAGE_TESS_EVALUATION;
		}

		if ( checkFlag( flags, ashes::ShaderStageFlag::eGeometry ) )
		{
			result |= GL_SHADER_STAGE_GEOMETRY;
		}

		if ( checkFlag( flags, ashes::ShaderStageFlag::eFragment ) )
		{
			result |= GL_SHADER_STAGE_FRAGMENT;
		}

		if ( checkFlag( flags, ashes::ShaderStageFlag::eCompute ) )
		{
			result |= GL_SHADER_STAGE_COMPUTE;
		}

		return result;
	}

	GlShaderStageFlag convert( ashes::ShaderStageFlag const & flag )
	{
		switch ( flag )
		{
		case ashes::ShaderStageFlag::eVertex:
			return GL_SHADER_STAGE_VERTEX;

		case ashes::ShaderStageFlag::eTessellationControl:
			return GL_SHADER_STAGE_TESS_CONTROL;

		case ashes::ShaderStageFlag::eTessellationEvaluation:
			return GL_SHADER_STAGE_TESS_EVALUATION;

		case ashes::ShaderStageFlag::eGeometry:
			return GL_SHADER_STAGE_GEOMETRY;

		case ashes::ShaderStageFlag::eFragment:
			return GL_SHADER_STAGE_FRAGMENT;

		case ashes::ShaderStageFlag::eCompute:
			return GL_SHADER_STAGE_COMPUTE;

		default:
			assert( false && "Unsupported shader stage." );
			return GL_SHADER_STAGE_VERTEX;
		}
	}
}

#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	GLenum convert( renderer::ShaderStageFlags const & flags )
	{
		GLenum result{ 0 };

		if ( checkFlag( flags, renderer::ShaderStageFlag::eVertex ) )
		{
			result |= GL_VERTEX_SHADER;
		}

		if ( checkFlag( flags, renderer::ShaderStageFlag::eTessellationControl ) )
		{
			result |= GL_TESS_CONTROL_SHADER;
		}

		if ( checkFlag( flags, renderer::ShaderStageFlag::eTessellationEvaluation ) )
		{
			result |= GL_TESS_EVALUATION_SHADER;
		}

		if ( checkFlag( flags, renderer::ShaderStageFlag::eGeometry ) )
		{
			result |= GL_GEOMETRY_SHADER;
		}

		if ( checkFlag( flags, renderer::ShaderStageFlag::eFragment ) )
		{
			result |= GL_FRAGMENT_SHADER;
		}

		if ( checkFlag( flags, renderer::ShaderStageFlag::eCompute ) )
		{
			result |= GL_COMPUTE_SHADER;
		}

		return result;
	}

	GLenum convert( renderer::ShaderStageFlag const & flag )
	{
		switch ( flag )
		{
		case renderer::ShaderStageFlag::eVertex:
			return GL_VERTEX_SHADER;

		case renderer::ShaderStageFlag::eTessellationControl:
			return GL_TESS_CONTROL_SHADER;

		case renderer::ShaderStageFlag::eTessellationEvaluation:
			return GL_TESS_EVALUATION_SHADER;

		case renderer::ShaderStageFlag::eGeometry:
			return GL_GEOMETRY_SHADER;

		case renderer::ShaderStageFlag::eFragment:
			return GL_FRAGMENT_SHADER;

		case renderer::ShaderStageFlag::eCompute:
			return GL_COMPUTE_SHADER;

		default:
			assert( false && "Unsupported shader stage." );
			return GL_VERTEX_SHADER;
		}
	}
}

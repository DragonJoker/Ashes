#include "ShaderStageFlag.hpp"

namespace renderer
{
	VkShaderStageFlags convert( ShaderStageFlags const & flags )
	{
		VkShaderStageFlags result{ 0 };

		if ( checkFlag( flags, ShaderStageFlag::eVertex ) )
		{
			result |= VK_SHADER_STAGE_VERTEX_BIT;
		}

		if ( checkFlag( flags, ShaderStageFlag::eTessellationControl ) )
		{
			result |= VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		}

		if ( checkFlag( flags, ShaderStageFlag::eTessellationEvaluation ) )
		{
			result |= VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
		}

		if ( checkFlag( flags, ShaderStageFlag::eGeometry ) )
		{
			result |= VK_SHADER_STAGE_GEOMETRY_BIT;
		}

		if ( checkFlag( flags, ShaderStageFlag::eFragment ) )
		{
			result |= VK_SHADER_STAGE_FRAGMENT_BIT;
		}

		if ( checkFlag( flags, ShaderStageFlag::eCompute ) )
		{
			result |= VK_SHADER_STAGE_COMPUTE_BIT;
		}

		return result;
	}

	VkShaderStageFlagBits convert( ShaderStageFlag const & flag )
	{
		VkShaderStageFlags result{ 0 };

		switch ( flag )
		{
		case ShaderStageFlag::eVertex:
			return VK_SHADER_STAGE_VERTEX_BIT;

		case ShaderStageFlag::eTessellationControl:
			return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;

		case ShaderStageFlag::eTessellationEvaluation:
			return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;

		case ShaderStageFlag::eGeometry:
			return VK_SHADER_STAGE_GEOMETRY_BIT;

		case ShaderStageFlag::eFragment:
			return VK_SHADER_STAGE_FRAGMENT_BIT;

		case ShaderStageFlag::eCompute:
			return VK_SHADER_STAGE_COMPUTE_BIT;

		default:
			assert( false );
			return VK_SHADER_STAGE_ALL;
		}
	}
}

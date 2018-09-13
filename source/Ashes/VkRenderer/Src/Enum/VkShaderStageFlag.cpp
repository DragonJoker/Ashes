#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkShaderStageFlags convert( ashes::ShaderStageFlags const & flags )
	{
		VkShaderStageFlags result{ 0 };

		if ( checkFlag( flags, ashes::ShaderStageFlag::eVertex ) )
		{
			result |= VK_SHADER_STAGE_VERTEX_BIT;
		}

		if ( checkFlag( flags, ashes::ShaderStageFlag::eTessellationControl ) )
		{
			result |= VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		}

		if ( checkFlag( flags, ashes::ShaderStageFlag::eTessellationEvaluation ) )
		{
			result |= VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
		}

		if ( checkFlag( flags, ashes::ShaderStageFlag::eGeometry ) )
		{
			result |= VK_SHADER_STAGE_GEOMETRY_BIT;
		}

		if ( checkFlag( flags, ashes::ShaderStageFlag::eFragment ) )
		{
			result |= VK_SHADER_STAGE_FRAGMENT_BIT;
		}

		if ( checkFlag( flags, ashes::ShaderStageFlag::eCompute ) )
		{
			result |= VK_SHADER_STAGE_COMPUTE_BIT;
		}

		return result;
	}

	VkShaderStageFlagBits convert( ashes::ShaderStageFlag const & flag )
	{
		VkShaderStageFlags result{ 0 };

		switch ( flag )
		{
		case ashes::ShaderStageFlag::eVertex:
			return VK_SHADER_STAGE_VERTEX_BIT;

		case ashes::ShaderStageFlag::eTessellationControl:
			return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;

		case ashes::ShaderStageFlag::eTessellationEvaluation:
			return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;

		case ashes::ShaderStageFlag::eGeometry:
			return VK_SHADER_STAGE_GEOMETRY_BIT;

		case ashes::ShaderStageFlag::eFragment:
			return VK_SHADER_STAGE_FRAGMENT_BIT;

		case ashes::ShaderStageFlag::eCompute:
			return VK_SHADER_STAGE_COMPUTE_BIT;

		default:
			assert( false );
			return VK_SHADER_STAGE_ALL;
		}
	}
}

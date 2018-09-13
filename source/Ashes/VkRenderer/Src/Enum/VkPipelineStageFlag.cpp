#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkPipelineStageFlags convert( ashes::PipelineStageFlags const & flags )
	{
		VkPipelineStageFlags result{ 0 };

		if ( checkFlag( flags, ashes::PipelineStageFlag::eTopOfPipe ) )
		{
			result |= VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		}

		if ( checkFlag( flags, ashes::PipelineStageFlag::eDrawIndirect ) )
		{
			result |= VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;
		}

		if ( checkFlag( flags, ashes::PipelineStageFlag::eVertexInput ) )
		{
			result |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
		}

		if ( checkFlag( flags, ashes::PipelineStageFlag::eVertexShader ) )
		{
			result |= VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
		}

		if ( checkFlag( flags, ashes::PipelineStageFlag::eTessellationControlShader ) )
		{
			result |= VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT;
		}

		if ( checkFlag( flags, ashes::PipelineStageFlag::eTessellationEvaluationShader ) )
		{
			result |= VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT;
		}

		if ( checkFlag( flags, ashes::PipelineStageFlag::eGeometryShader ) )
		{
			result |= VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT;
		}

		if ( checkFlag( flags, ashes::PipelineStageFlag::eFragmentShader ) )
		{
			result |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}

		if ( checkFlag( flags, ashes::PipelineStageFlag::eEarlyFragmentTests ) )
		{
			result |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}

		if ( checkFlag( flags, ashes::PipelineStageFlag::eLateFragmentTests ) )
		{
			result |= VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		}

		if ( checkFlag( flags, ashes::PipelineStageFlag::eColourAttachmentOutput ) )
		{
			result |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		}

		if ( checkFlag( flags, ashes::PipelineStageFlag::eComputeShader ) )
		{
			result |= VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
		}

		if ( checkFlag( flags, ashes::PipelineStageFlag::eTransfer ) )
		{
			result |= VK_PIPELINE_STAGE_TRANSFER_BIT;
		}

		if ( checkFlag( flags, ashes::PipelineStageFlag::eBottomOfPipe ) )
		{
			result |= VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		}

		if ( checkFlag( flags, ashes::PipelineStageFlag::eHost ) )
		{
			result |= VK_PIPELINE_STAGE_HOST_BIT;
		}

		if ( checkFlag( flags, ashes::PipelineStageFlag::eAllGraphics ) )
		{
			result |= VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
		}

		if ( checkFlag( flags, ashes::PipelineStageFlag::eAllCommands ) )
		{
			result |= VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
		}

		return result;
	}

	VkPipelineStageFlagBits convert( ashes::PipelineStageFlag const & flag )
	{
		switch ( flag )
		{
		case ashes::PipelineStageFlag::eTopOfPipe:
			return VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

		case ashes::PipelineStageFlag::eDrawIndirect:
			return VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;

		case ashes::PipelineStageFlag::eVertexInput:
			return VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;

		case ashes::PipelineStageFlag::eVertexShader:
			return VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;

		case ashes::PipelineStageFlag::eTessellationControlShader:
			return VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT;

		case ashes::PipelineStageFlag::eTessellationEvaluationShader:
			return VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT;

		case ashes::PipelineStageFlag::eGeometryShader:
			return VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT;

		case ashes::PipelineStageFlag::eFragmentShader:
			return VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

		case ashes::PipelineStageFlag::eEarlyFragmentTests:
			return VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;

		case ashes::PipelineStageFlag::eLateFragmentTests:
			return VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;

		case ashes::PipelineStageFlag::eColourAttachmentOutput:
			return VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

		case ashes::PipelineStageFlag::eComputeShader:
			return VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;

		case ashes::PipelineStageFlag::eTransfer:
			return VK_PIPELINE_STAGE_TRANSFER_BIT;

		case ashes::PipelineStageFlag::eBottomOfPipe:
			return VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

		case ashes::PipelineStageFlag::eHost:
			return VK_PIPELINE_STAGE_HOST_BIT;

		case ashes::PipelineStageFlag::eAllGraphics:
			return VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;

		case ashes::PipelineStageFlag::eAllCommands:
			return VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;

		default:
			assert( false );
			return VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
		}
	}
}

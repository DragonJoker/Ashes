#include "PipelineStageFlag.hpp"

namespace renderer
{
	VkPipelineStageFlags convert( PipelineStageFlags const & flags )
	{
		VkPipelineStageFlags result{ 0 };

		if ( checkFlag( flags, PipelineStageFlag::eTopOfPipe ) )
		{
			result |= VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		}

		if ( checkFlag( flags, PipelineStageFlag::eDrawIndirect ) )
		{
			result |= VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;
		}

		if ( checkFlag( flags, PipelineStageFlag::eVertexInput ) )
		{
			result |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
		}

		if ( checkFlag( flags, PipelineStageFlag::eVertexShader ) )
		{
			result |= VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
		}

		if ( checkFlag( flags, PipelineStageFlag::eTessellationControlShader ) )
		{
			result |= VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT;
		}

		if ( checkFlag( flags, PipelineStageFlag::eTessellationEvaluationShader ) )
		{
			result |= VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT;
		}

		if ( checkFlag( flags, PipelineStageFlag::eGeometryShader ) )
		{
			result |= VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT;
		}

		if ( checkFlag( flags, PipelineStageFlag::eFragmentShader ) )
		{
			result |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}

		if ( checkFlag( flags, PipelineStageFlag::eEarlyFragmentTests ) )
		{
			result |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}

		if ( checkFlag( flags, PipelineStageFlag::eLateFragmentTests ) )
		{
			result |= VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		}

		if ( checkFlag( flags, PipelineStageFlag::eColourAttachmentOutput ) )
		{
			result |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		}

		if ( checkFlag( flags, PipelineStageFlag::eComputeShader ) )
		{
			result |= VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
		}

		if ( checkFlag( flags, PipelineStageFlag::eTransfer ) )
		{
			result |= VK_PIPELINE_STAGE_TRANSFER_BIT;
		}

		if ( checkFlag( flags, PipelineStageFlag::eBottomOfPipe ) )
		{
			result |= VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		}

		if ( checkFlag( flags, PipelineStageFlag::eHost ) )
		{
			result |= VK_PIPELINE_STAGE_HOST_BIT;
		}

		if ( checkFlag( flags, PipelineStageFlag::eAllGraphics ) )
		{
			result |= VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
		}

		if ( checkFlag( flags, PipelineStageFlag::eAllCommands ) )
		{
			result |= VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
		}

		return result;
	}
}

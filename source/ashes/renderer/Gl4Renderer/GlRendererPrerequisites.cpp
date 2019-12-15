#include "GlRendererPrerequisites.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	namespace
	{
		template< typename VkObjectT >
		void assertReport( VkObjectT object
			, bool condition
			, char const * msg )
		{
			if ( !condition )
			{
				reportError( object
					, VK_ERROR_VALIDATION_FAILED_EXT
					, "Validation Error."
					, msg );
			}
		}
	}

	uint32_t deduceMemoryType( uint32_t typeBits
		, VkMemoryPropertyFlags requirements )
	{
		return ashes::deduceMemoryType( typeBits
			, requirements
			, Instance::getMemoryProperties() );
	}

	bool areCompatible( VkCommandBuffer cmd
		, VkPipelineStageFlags pipelineFlags
		, VkAccessFlags accessFlags )
	{
		if ( ( pipelineFlags & VK_PIPELINE_STAGE_ALL_COMMANDS_BIT )
			|| ( pipelineFlags & VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT )
			|| ( accessFlags & VK_ACCESS_MEMORY_READ_BIT )
			|| ( accessFlags & VK_ACCESS_MEMORY_WRITE_BIT ) )
		{
			return true;
		}

		if ( ( accessFlags & VK_ACCESS_INDIRECT_COMMAND_READ_BIT ) )
		{
			assertReport( cmd
				, ( pipelineFlags & VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT )
				, "VK_ACCESS_INDIRECT_COMMAND_READ_BIT access flags are only compatible with VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT pipeline stage flags" );
			return ( pipelineFlags & VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT );
		}

		if ( ( accessFlags & VK_ACCESS_INDEX_READ_BIT )
			|| ( accessFlags & VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT ) )
		{
			assertReport( cmd
				, ( pipelineFlags & VK_PIPELINE_STAGE_VERTEX_INPUT_BIT )
				, "VK_ACCESS_INDEX_READ_BIT or VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT access flags only compatible with VK_PIPELINE_STAGE_VERTEX_INPUT_BIT pipeline stage flags" );
			return ( pipelineFlags & VK_PIPELINE_STAGE_VERTEX_INPUT_BIT );
		}

		if ( ( accessFlags & VK_ACCESS_UNIFORM_READ_BIT )
			|| ( accessFlags & VK_ACCESS_SHADER_READ_BIT )
			|| ( accessFlags & VK_ACCESS_SHADER_WRITE_BIT ) )
		{
			assertReport( cmd
				, ( ( pipelineFlags & VK_PIPELINE_STAGE_VERTEX_SHADER_BIT )
				|| ( pipelineFlags & VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT )
				|| ( pipelineFlags & VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT )
				|| ( pipelineFlags & VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT )
				|| ( pipelineFlags & VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT )
				|| ( pipelineFlags & VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT )
				|| ( pipelineFlags & VK_PIPELINE_STAGE_TASK_SHADER_BIT_NV )
				|| ( pipelineFlags & VK_PIPELINE_STAGE_MESH_SHADER_BIT_NV )
				|| ( pipelineFlags & VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_NV ) )
				, "VK_ACCESS_UNIFORM_READ_BIT, VK_ACCESS_SHADER_READ_BIT or VK_ACCESS_SHADER_WRITE_BIT access flags only compatible with VK_PIPELINE_STAGE_XXX_SHADER_BIT pipeline stage flags" );
			return ( pipelineFlags & VK_PIPELINE_STAGE_VERTEX_SHADER_BIT )
				|| ( pipelineFlags & VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT )
				|| ( pipelineFlags & VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT )
				|| ( pipelineFlags & VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT )
				|| ( pipelineFlags & VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT )
				|| ( pipelineFlags & VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT )
				|| ( pipelineFlags & VK_PIPELINE_STAGE_TASK_SHADER_BIT_NV )
				|| ( pipelineFlags & VK_PIPELINE_STAGE_MESH_SHADER_BIT_NV )
				|| ( pipelineFlags & VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_NV );
		}

		if ( ( accessFlags & VK_ACCESS_INPUT_ATTACHMENT_READ_BIT ) )
		{
			assertReport( cmd
				, ( pipelineFlags & VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT )
				, "VK_ACCESS_INPUT_ATTACHMENT_READ_BIT access flags are only compatible with VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT pipeline stage flags" );
			return ( pipelineFlags & VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT );
		}

		if ( ( accessFlags & VK_ACCESS_COLOR_ATTACHMENT_READ_BIT )
			|| ( accessFlags & VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT ) )
		{
			assertReport( cmd
				, ( pipelineFlags & VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT )
				, "VK_ACCESS_COLOR_ATTACHMENT_READ_BIT or VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT access flags only compatible with VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT pipeline stage flags" );
			return ( pipelineFlags & VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT );
		}

		if ( ( accessFlags & VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT )
			|| ( accessFlags & VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT ) )
		{
			assertReport( cmd
				, ( ( pipelineFlags & VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT )
				|| ( pipelineFlags & VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT ) )
				, "VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT or VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT access flags only compatible with VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT or VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT pipeline stage flags" );
			return ( pipelineFlags & VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT )
				|| ( pipelineFlags & VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT );
		}

		if ( ( accessFlags & VK_ACCESS_TRANSFER_READ_BIT )
			|| ( accessFlags & VK_ACCESS_TRANSFER_WRITE_BIT ) )
		{
			assertReport( cmd
				, ( pipelineFlags & VK_PIPELINE_STAGE_TRANSFER_BIT )
				, "VK_ACCESS_TRANSFER_READ_BIT or VK_ACCESS_TRANSFER_WRITE_BIT access flags only compatible with VK_PIPELINE_STAGE_TRANSFER_BIT pipeline stage flags" );
			return ( pipelineFlags & VK_PIPELINE_STAGE_TRANSFER_BIT );
		}

		if ( ( accessFlags & VK_ACCESS_HOST_READ_BIT )
			|| ( accessFlags & VK_ACCESS_HOST_WRITE_BIT ) )
		{
			assertReport( cmd
				, ( pipelineFlags & VK_PIPELINE_STAGE_HOST_BIT )
				, "VK_ACCESS_HOST_READ_BIT or VK_ACCESS_HOST_WRITE_BIT access flags only compatible with VK_PIPELINE_STAGE_HOST_BIT pipeline stage flags" );
			return ( pipelineFlags & VK_PIPELINE_STAGE_HOST_BIT );
		}

		assertReport( cmd
			, !accessFlags
			, "Unsupported combination for accessFlags and pipelineFlags." );
		return !accessFlags;
	}
}

namespace ashes::gl
{
	using gl4::get;

	uint32_t getScreenIndex( VkDisplayModeKHR displayMode )
	{
		return get( get( displayMode )->getDisplay() )->getScreenIndex();
	}

	VkDisplayModeParametersKHR getDisplayModeParameters( VkDisplayModeKHR displayMode )
	{
		return get( displayMode )->getParameters();
	}

	VkExtent2D getDisplayResolution( VkDisplayModeKHR displayMode )
	{
		return get( get( displayMode )->getDisplay() )->getResolution();
	}
}

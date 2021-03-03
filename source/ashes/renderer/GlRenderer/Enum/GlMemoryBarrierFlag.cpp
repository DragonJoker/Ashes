#include "GlRendererPrerequisites.hpp"

namespace ashes::gl
{
	std::string getName( GlMemoryBarrierFlags value )
	{
		std::string result;
		std::string sep;

		if ( ashes::checkFlag( value, GlMemoryBarrierFlag::GL_MEMORY_BARRIER_VERTEX_ATTRIB_ARRAY ) )
		{
			result += sep + "GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GlMemoryBarrierFlag::GL_MEMORY_BARRIER_ELEMENT_ARRAY ) )
		{
			result += sep + "GL_ELEMENT_ARRAY_BARRIER_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GlMemoryBarrierFlag::GL_MEMORY_BARRIER_UNIFORM ) )
		{
			result += sep + "GL_UNIFORM_BARRIER_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GlMemoryBarrierFlag::GL_MEMORY_BARRIER_TEXTURE_FETCH ) )
		{
			result += sep + "GL_TEXTURE_FETCH_BARRIER_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GlMemoryBarrierFlag::GL_MEMORY_BARRIER_SHADER_IMAGE_ACCESS ) )
		{
			result += sep + "GL_SHADER_IMAGE_ACCESS_BARRIER_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GlMemoryBarrierFlag::GL_MEMORY_BARRIER_COMMAND ) )
		{
			result += sep + "GL_COMMAND_BARRIER_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GlMemoryBarrierFlag::GL_MEMORY_BARRIER_PIXEL_BUFFER ) )
		{
			result += sep + "GL_PIXEL_BUFFER_BARRIER_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GlMemoryBarrierFlag::GL_MEMORY_BARRIER_TEXTURE_UPDATE ) )
		{
			result += sep + "GL_TEXTURE_UPDATE_BARRIER_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GlMemoryBarrierFlag::GL_MEMORY_BARRIER_BUFFER_UPDATE ) )
		{
			result += sep + "GL_BUFFER_UPDATE_BARRIER_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GlMemoryBarrierFlag::GL_MEMORY_BARRIER_FRAMEBUFFER ) )
		{
			result += sep + "GL_FRAMEBUFFER_BARRIER_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GlMemoryBarrierFlag::GL_MEMORY_BARRIER_TRANSFORM_FEEDBACK ) )
		{
			result += sep + "GL_TRANSFORM_FEEDBACK_BARRIER_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GlMemoryBarrierFlag::GL_MEMORY_BARRIER_ATOMIC_COUNTER ) )
		{
			result += sep + "GL_ATOMIC_COUNTER_BARRIER_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GlMemoryBarrierFlag::GL_MEMORY_BARRIER_SHADER_STORAGE ) )
		{
			result += sep + "GL_SHADER_STORAGE_BARRIER_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GlMemoryBarrierFlag::GL_MEMORY_BARRIER_CLIENT_MAPPED_BUFFER ) )
		{
			result += sep + "GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GlMemoryBarrierFlag::GL_MEMORY_BARRIER_QUERY_BUFFER ) )
		{
			result += sep + "GL_QUERY_BUFFER_BARRIER_BIT";
		}

		return result;
	}

	GlMemoryBarrierFlags getMemoryBarrierFlags( VkPipelineStageFlags const & flags )
	{
		GlMemoryBarrierFlags result{ 0 };

		if ( ashes::checkFlag( flags, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT ) )
		{
		}

		if ( ashes::checkFlag( flags, VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT ) )
		{
		}

		if ( ashes::checkFlag( flags, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT ) )
		{
			result |= GL_MEMORY_BARRIER_VERTEX_ATTRIB_ARRAY;
			result |= GL_MEMORY_BARRIER_ELEMENT_ARRAY;
			result |= GL_MEMORY_BARRIER_UNIFORM;
		}

		if ( ashes::checkFlag( flags, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT ) )
		{
			result |= GL_MEMORY_BARRIER_VERTEX_ATTRIB_ARRAY;
			result |= GL_MEMORY_BARRIER_ELEMENT_ARRAY;
			result |= GL_MEMORY_BARRIER_UNIFORM;
		}

		if ( ashes::checkFlag( flags, VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT ) )
		{
		}

		if ( ashes::checkFlag( flags, VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT ) )
		{
		}

		if ( ashes::checkFlag( flags, VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT ) )
		{
		}

		if ( ashes::checkFlag( flags, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT ) )
		{
			result |= GL_MEMORY_BARRIER_TEXTURE_FETCH;
			result |= GL_MEMORY_BARRIER_UNIFORM;
		}

		if ( ashes::checkFlag( flags, VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT ) )
		{
		}

		if ( ashes::checkFlag( flags, VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT ) )
		{
		}

		if ( ashes::checkFlag( flags, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT ) )
		{
			result |= GL_MEMORY_BARRIER_FRAMEBUFFER;
		}

		if ( ashes::checkFlag( flags, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT ) )
		{
			result |= GL_MEMORY_BARRIER_UNIFORM;
			result |= GL_MEMORY_BARRIER_SHADER_STORAGE;
		}

		if ( ashes::checkFlag( flags, VK_PIPELINE_STAGE_TRANSFER_BIT ) )
		{
			result |= GL_MEMORY_BARRIER_TEXTURE_UPDATE;
			result |= GL_MEMORY_BARRIER_BUFFER_UPDATE;
		}

		if ( ashes::checkFlag( flags, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT ) )
		{
		}

		if ( ashes::checkFlag( flags, VK_PIPELINE_STAGE_HOST_BIT ) )
		{
			result |= GL_MEMORY_BARRIER_CLIENT_MAPPED_BUFFER;
		}

		if ( ashes::checkFlag( flags, VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT ) )
		{
		}

		if ( ashes::checkFlag( flags, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT ) )
		{
			result |= GL_MEMORY_BARRIER_COMMAND;
		}

		return result;
	}
}

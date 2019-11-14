#include "GlRendererPrerequisites.hpp"

#include "Core/GlInstance.hpp"

namespace ashes::gl3
{
	uint32_t deduceMemoryType( uint32_t typeBits
		, VkMemoryPropertyFlags requirements )
	{
		return ashes::deduceMemoryType( typeBits
			, requirements
			, Instance::getMemoryProperties() );
	}
}

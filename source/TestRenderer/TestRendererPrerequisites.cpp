#include "TestRendererPrerequisites.hpp"

#include "Core/TestInstance.hpp"

#include <Ashes/Core/Exception.hpp>

namespace test_renderer
{
	uint32_t deduceMemoryType( uint32_t typeBits
		, ashes::MemoryPropertyFlags requirements )
	{
		auto & memoryProperties = Instance::getMemoryProperties();
		uint32_t result = 0xFFFFFFFFu;
		bool found{ false };

		// Recherche parmi les types de mémoire la première ayant les propriétés voulues.
		uint32_t i{ 0 };

		while ( i < memoryProperties.memoryTypes.size() && !found )
		{
			if ( ( typeBits & 1 ) == 1 )
			{
				// Le type de mémoire est disponible, a-t-il les propriétés demandées?
				if ( ( memoryProperties.memoryTypes[i].propertyFlags & requirements ) == requirements )
				{
					result = i;
					found = true;
				}
			}

			typeBits >>= 1;
			++i;
		}

		if ( !found )
		{
			throw ashes::Exception{ ashes::Result::eErrorRenderer, "Could not deduce memory type" };
		}

		return result;
	}
}

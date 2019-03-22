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

		// Recherche parmi les types de m�moire la premi�re ayant les propri�t�s voulues.
		uint32_t i{ 0 };

		while ( i < memoryProperties.memoryTypes.size() && !found )
		{
			if ( ( typeBits & 1 ) == 1 )
			{
				// Le type de m�moire est disponible, a-t-il les propri�t�s demand�es?
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

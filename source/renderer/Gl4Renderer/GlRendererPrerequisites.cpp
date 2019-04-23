#include "GlRendererPrerequisites.hpp"

#include "Buffer/GlBuffer.hpp"
#include "Buffer/GlBufferView.hpp"
#include "Command/GlCommandBuffer.hpp"
#include "Command/GlCommandPool.hpp"
#include "Command/GlQueue.hpp"
#include "Core/GlContext.hpp"
#include "Core/GlDevice.hpp"
#include "Core/GlInstance.hpp"
#include "Core/GlPhysicalDevice.hpp"
#include "Core/GlSurface.hpp"
#include "Core/GlSwapChain.hpp"
#include "Descriptor/GlDescriptorSetLayout.hpp"
#include "Descriptor/GlDescriptorPool.hpp"
#include "Descriptor/GlDescriptorSet.hpp"
#include "Image/GlImage.hpp"
#include "Image/GlImageView.hpp"
#include "Image/GlSampler.hpp"
#include "Miscellaneous/GlDeviceMemory.hpp"
#include "Miscellaneous/GlQueryPool.hpp"
#include "Pipeline/GlPipeline.hpp"
#include "Pipeline/GlPipelineLayout.hpp"
#include "RenderPass/GlFramebuffer.hpp"
#include "RenderPass/GlRenderPass.hpp"
#include "Shader/GlShaderModule.hpp"
#include "Sync/GlFence.hpp"
#include "Sync/GlSemaphore.hpp"
#include "Sync/GlEvent.hpp"

#include "ashesgl4_api.hpp"

#include <common/Exception.hpp>

namespace ashes::gl4
{
	uint32_t deduceMemoryType( uint32_t typeBits
		, VkMemoryPropertyFlags requirements )
	{
		auto & memoryProperties = Instance::getMemoryProperties();
		uint32_t result = 0xFFFFFFFFu;
		bool found{ false };

		// Recherche parmi les types de m�moire la premi�re ayant les propri�t�s voulues.
		uint32_t i{ 0 };

		while ( i < memoryProperties.memoryTypeCount && !found )
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
			throw Exception{ VK_ERROR_INITIALIZATION_FAILED, "Could not deduce memory type" };
		}

		return result;
	}
}

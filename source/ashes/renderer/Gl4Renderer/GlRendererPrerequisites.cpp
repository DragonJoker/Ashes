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

		// Recherche parmi les types de mémoire la première ayant les propriétés voulues.
		uint32_t i{ 0 };
		uint32_t lookup = 1u;

		while ( i < memoryProperties.memoryTypeCount && !found )
		{
			if ( ( typeBits & lookup ) == lookup )
			{
				// Le type de mémoire est disponible, a-t-il les propriétés demandées?
				if ( ( memoryProperties.memoryTypes[i].propertyFlags & requirements ) == requirements )
				{
					result = i;
					found = true;
				}
			}

			++lookup;
			++i;
		}

		if ( !found )
		{
			throw ashes::Exception{ VK_ERROR_OUT_OF_DEVICE_MEMORY, "Could not deduce memory type" };
		}

		return result;
	}
}

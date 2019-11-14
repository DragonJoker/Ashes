/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestCommandBase.hpp"

namespace ashes::test
{
	class MemoryBarrierCommand
		: public CommandBase
	{
	public:
		MemoryBarrierCommand( VkDevice device
			, VkPipelineStageFlags after
			, VkPipelineStageFlags before
			, VkMemoryBarrierArray const & memoryBarriers
			, VkBufferMemoryBarrierArray const & bufferBarriers
			, VkImageMemoryBarrierArray const & imageBarriers );

		void apply()const;
		CommandPtr clone()const;

	private:
		struct BufferLock
		{
			VkDeviceSize offset;
			VkDeviceSize size;
			VkBuffer buffer;
		};
		using BufferLockArray = std::vector< BufferLock >;
		BufferLockArray m_uploadBuffers;
		BufferLockArray m_downloadBuffers;
	};
}

/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/XBoxRenderer/Command/Commands/XBoxCommandBase.hpp"

namespace ashes::xbox
{
	class UpdateBufferCommand
		: public CommandBase
	{
	public:
		UpdateBufferCommand( VkDevice device
			, VkBuffer buffer
			, VkDeviceSize bufferOffset
			, ArrayView< uint8_t const > const & data );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		VkBuffer m_buffer;
		VkDeviceMemory m_memory;
		VkDeviceSize m_memoryOffset;
		ByteArray m_data;
		bool m_mappable;
		D3D11_BOX m_dstBox{};
	};
}

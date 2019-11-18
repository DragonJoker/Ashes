/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::d3d11
{
	class UpdateBufferCommand
		: public CommandBase
	{
	public:
		UpdateBufferCommand( VkDevice device
			, VkBuffer buffer
			, VkDeviceSize bufferOffset
			, ArrayView< uint8_t const > const & data );

		void apply( Context const & context )const;
		CommandPtr clone()const;

	private:
		VkDeviceMemory m_memory;
		VkDeviceSize m_memoryOffset;
		ByteArray m_data;
	};
}

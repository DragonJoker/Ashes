/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::d3d11
{
	class FillBufferCommand
		: public CommandBase
	{
	public:
		FillBufferCommand( VkDevice device
			, VkBuffer buffer
			, VkDeviceSize bufferOffset
			, VkDeviceSize size
			, uint32_t data );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		VkDeviceMemory m_memory;
		VkDeviceSize m_memoryOffset;
		VkDeviceSize m_size;
		uint32_t m_data;
	};
}

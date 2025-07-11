/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::D3D11_NAMESPACE
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

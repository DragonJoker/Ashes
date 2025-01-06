/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/XBoxRenderer/Command/Commands/XBoxCommandBase.hpp"

namespace ashes::xbox
{
	class DrawIndexedIndirectCommand
		: public CommandBase
	{
	public:
		DrawIndexedIndirectCommand( VkDevice device
			, VkBuffer buffer
			, VkDeviceSize offset
			, uint32_t drawCount
			, uint32_t stride
			, VkPrimitiveTopology mode
			, VkIndexType type
			, VbosBindingArray const & vbos );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		VkBuffer m_buffer;
		UINT m_offset;
		uint32_t m_drawCount;
		uint32_t m_stride;
		D3D11_PRIMITIVE_TOPOLOGY m_mode;
		DXGI_FORMAT m_type;
		VbosBindingArray m_vbos;
	};
}

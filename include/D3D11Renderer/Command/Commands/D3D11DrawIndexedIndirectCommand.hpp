/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::d3d11
{
	class DrawIndexedIndirectCommand
		: public CommandBase
	{
	public:
		DrawIndexedIndirectCommand( Device const & device
			, ashes::BufferBase const & buffer
			, uint32_t offset
			, uint32_t drawCount
			, uint32_t stride
			, VkPrimitiveTopology mode
			, VkIndexType type
			, VbosBindingArray const & vbos );

		void apply( Context const & context )const;
		CommandPtr clone()const;

	private:
		Buffer const & m_buffer;
		uint32_t m_offset;
		uint32_t m_drawCount;
		uint32_t m_stride;
		D3D11_PRIMITIVE_TOPOLOGY m_mode;
		DXGI_FORMAT m_type;
		VbosBindingArray m_vbos;
	};
}

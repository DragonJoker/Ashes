/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::d3d11
{
	class DrawIndirectCommand
		: public CommandBase
	{
	public:
		DrawIndirectCommand( VkDevice device
			, VkBuffer buffer
			, uint32_t offset
			, uint32_t drawCount
			, uint32_t stride
			, VkPrimitiveTopology mode
			, VbosBindingArray const & vbos );

		void apply( Context const & context )const;
		CommandPtr clone()const;

	private:
		Buffer const & m_buffer;
		uint32_t m_offset;
		uint32_t m_drawCount;
		uint32_t m_stride;
		D3D11_PRIMITIVE_TOPOLOGY m_mode;
		VbosBindingArray m_vbos;
	};
}

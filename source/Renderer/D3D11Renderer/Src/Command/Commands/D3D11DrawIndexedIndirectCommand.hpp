/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11CommandBase.hpp"

namespace d3d11_renderer
{
	class DrawIndexedIndirectCommand
		: public CommandBase
	{
	public:
		DrawIndexedIndirectCommand( Device const & device
			, renderer::BufferBase const & buffer
			, uint32_t offset
			, uint32_t drawCount
			, uint32_t stride
			, renderer::PrimitiveTopology mode
			, renderer::IndexType type
			, VbosBindingArray const & vbos );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

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

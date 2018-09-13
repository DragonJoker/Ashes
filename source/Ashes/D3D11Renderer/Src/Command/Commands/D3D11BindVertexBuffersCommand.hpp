/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11CommandBase.hpp"

namespace d3d11_renderer
{
	class BindVertexBuffersCommand
		: public CommandBase
	{
	public:
		BindVertexBuffersCommand( Device const & device
			, uint32_t firstBinding
			, ashes::BufferCRefArray const & buffers
			, ashes::UInt64Array const & offsets );

		void apply( Context const & context )const override;
		void remove( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		uint32_t m_firstBinding;
		BufferCRefArray m_vbos;
		std::vector< ID3D11Buffer * > m_buffers;
		std::vector< UINT > m_offsets;
		std::vector< UINT > m_strides;
	};
}

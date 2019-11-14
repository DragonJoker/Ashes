/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::d3d11
{
	class BindVertexBuffersCommand
		: public CommandBase
	{
	public:
		BindVertexBuffersCommand( VkDevice device
			, uint32_t firstBinding
			, VkBufferArray const & buffers
			, UInt64Array const & offsets );

		void apply( Context const & context )const;
		void remove( Context const & context )const;
		CommandPtr clone()const;

	private:
		uint32_t m_firstBinding;
		VkBufferArray m_vbos;
		std::vector< ID3D11Buffer * > m_buffers;
		std::vector< UINT > m_offsets;
		std::vector< UINT > m_strides;
	};
}

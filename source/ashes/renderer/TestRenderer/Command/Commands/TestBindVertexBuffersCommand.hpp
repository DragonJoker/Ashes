/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestCommandBase.hpp"

namespace ashes::test
{
	class BindVertexBuffersCommand
		: public CommandBase
	{
	public:
		BindVertexBuffersCommand( VkDevice device
			, uint32_t firstBinding
			, VkBufferArray const & buffers
			, UInt64Array const & offsets );

		void apply()const;
		CommandPtr clone()const;

	private:
		uint32_t m_firstBinding;
		VkBufferArray m_vbos;
		std::vector< uint32_t > m_offsets;
		std::vector< uint32_t > m_strides;
	};
}

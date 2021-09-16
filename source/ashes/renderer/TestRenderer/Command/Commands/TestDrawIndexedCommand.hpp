/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestCommandBase.hpp"

namespace ashes::test
{
	class DrawIndexedCommand
		: public CommandBase
	{
	public:
		DrawIndexedCommand( VkDevice device
			, uint32_t indexCount
			, uint32_t instCount
			, uint32_t firstIndex
			, uint32_t vertexOffset
			, uint32_t firstInstance
			, VkPrimitiveTopology mode
			, VkIndexType type
			, VbosBindingArray const & vbos );

		void apply()const override;
		CommandPtr clone()const override;

	private:
		VbosBindingArray m_vbos;
	};
}

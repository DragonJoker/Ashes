/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/XBoxRenderer/Command/Commands/XBoxCommandBase.hpp"

namespace ashes::xbox
{
	class CopyQueryPoolResultsCommand
		: public CommandBase
	{
	public:
		CopyQueryPoolResultsCommand( VkDevice device
			, VkQueryPool queryPool
			, uint32_t firstQuery
			, uint32_t queryCount
			, VkBuffer dstBuffer
			, VkDeviceSize dstOffset
			, VkDeviceSize stride
			, VkQueryResultFlags flags );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		VkQueryPool m_queryPool;
		uint32_t m_firstQuery;
		uint32_t m_queryCount;
		VkBuffer m_dstBuffer;
		VkDeviceMemory m_dstMemory;
		VkDeviceSize m_dstOffset;
		VkDeviceSize m_stride;
		VkQueryResultFlags m_flags;
		bool m_mappable;
		mutable ByteArray m_data;
		D3D11_BOX m_dstBox{};
	};
}

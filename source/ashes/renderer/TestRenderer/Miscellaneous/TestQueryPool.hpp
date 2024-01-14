/*
See LICENSE file in root folder
*/
#ifndef ___TestRenderer_QueryPool_H___
#define ___TestRenderer_QueryPool_H___

#include "renderer/TestRenderer/TestRendererPrerequisites.hpp"

namespace ashes::test
{
	class QueryPool
	{
	public:
		QueryPool( VkDevice device
			, VkQueryPoolCreateInfo createInfo );

		VkResult getResults( uint32_t firstQuery
			, uint32_t queryCount
			, VkDeviceSize stride
			, VkQueryResultFlags flags
			, UInt32Array & data )const;
		VkResult getResults( uint32_t firstQuery
			, uint32_t queryCount
			, VkDeviceSize stride
			, VkQueryResultFlags flags
			, UInt64Array & data )const;

		VkDevice getDevice()const noexcept
		{
			return m_device;
		}

	private:
		VkDevice m_device;
		VkQueryPoolCreateInfo m_createInfo;
		mutable ByteArray m_data;
		std::function< uint32_t( uint32_t ) > getUint32;
		std::function< uint64_t( uint32_t ) > getUint64;
	};
}

#endif

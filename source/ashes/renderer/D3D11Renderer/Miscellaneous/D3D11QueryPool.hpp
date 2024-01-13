/*
See LICENSE file in root folder
*/
#ifndef ___D3D11Renderer_QueryPool_H___
#define ___D3D11Renderer_QueryPool_H___

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

namespace ashes::d3d11
{
	class QueryPool
		: public NonCopyable
	{
	public:
		QueryPool( VkDevice device
			, VkQueryPoolCreateInfo createInfo );
		~QueryPool()noexcept;

		VkResult getResults( uint32_t firstQuery
			, uint32_t queryCount
			, VkDeviceSize stride
			, VkQueryResultFlags flags
			, VkDeviceSize dataSize
			, void * data )const;

		auto begin()const noexcept
		{
			return m_queries.begin();
		}

		auto end()const noexcept
		{
			return m_queries.end();
		}

		VkDevice getDevice()const noexcept
		{
			return m_device;
		}

	private:
		VkResult getPipelineStatisticsResults( uint32_t firstQuery
			, uint32_t queryCount
			, VkDeviceSize stride
			, VkQueryResultFlags flags
			, VkDeviceSize dataSize
			, void * data )const;
		VkResult getOtherResults( uint32_t firstQuery
			, uint32_t queryCount
			, VkDeviceSize stride
			, VkQueryResultFlags flags
			, void * data )const;

	private:
		VkDevice m_device;
		VkQueryPoolCreateInfo m_createInfo;
		std::vector< ID3D11Query * > m_queries;
		mutable ByteArray m_data;
		std::function< uint32_t( uint32_t ) > getUint32;
		std::function< uint64_t( uint32_t ) > getUint64;
	};
}

#endif

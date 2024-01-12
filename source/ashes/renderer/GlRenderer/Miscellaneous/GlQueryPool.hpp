/*
See LICENSE file in root folder
*/
#ifndef ___GlRenderer_QueryPool_H___
#define ___GlRenderer_QueryPool_H___

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"

namespace ashes::gl
{
	class QueryPool
		: public AutoIdIcdObject< QueryPool >
	{
	public:
		QueryPool( VkAllocationCallbacks const * allocInfo
			, VkDevice device
			, VkQueryPoolCreateInfo const & createInfo );
		~QueryPool()noexcept;

		VkResult getResults( ContextLock const & context
			, uint32_t firstQuery
			, uint32_t queryCount
			, VkDeviceSize stride
			, VkQueryResultFlags flags
			, void * buffer )const;

		auto begin()const
		{
			return m_names.begin();
		}

		auto end()const
		{
			return m_names.end();
		}

		auto getType()const
		{
			return m_queryType;
		}

		auto getTypes()const
		{
			return m_pipelineStatistics;
		}

		VkDevice getDevice()const
		{
			return m_device;
		}

	private:
		VkDevice m_device;
		VkQueryType m_queryType;
		uint32_t m_queryCount;
		std::vector< GlQueryType > m_pipelineStatistics;
		std::vector< GLuint > m_names;
	};
}

#endif

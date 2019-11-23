/*
See LICENSE file in root folder
*/
#ifndef ___GlRenderer_QueryPool_H___
#define ___GlRenderer_QueryPool_H___

#include "renderer/Gl4Renderer/GlRendererPrerequisites.hpp"

namespace ashes::gl4
{
	class QueryPool
	{
	public:
		QueryPool( VkDevice device
			, VkQueryPoolCreateInfo createInfo );
		~QueryPool();

		VkResult getResults( ContextLock const & context
			, uint32_t firstQuery
			, uint32_t queryCount
			, VkDeviceSize stride
			, VkQueryResultFlags flags
			, size_t dataSize
			, void * buffer )const;

		inline auto begin()const
		{
			return m_names.begin();
		}

		inline auto end()const
		{
			return m_names.end();
		}

		inline auto getType()const
		{
			return m_queryType;
		}

		inline auto getTypes()const
		{
			return m_pipelineStatistics;
		}

	protected:
		VkDevice m_device;
		VkQueryPoolCreateFlags m_flags;
		VkQueryType m_queryType;
		uint32_t m_queryCount;
		std::vector< GlQueryType > m_pipelineStatistics;
		std::vector< GLuint > m_names;
	};
}

#endif

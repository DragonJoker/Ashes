/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___TestRenderer_PipelineCache_HPP___
#define ___TestRenderer_PipelineCache_HPP___
#pragma once

#include "renderer/TestRenderer/TestRendererPrerequisites.hpp"

namespace ashes::test
{
	/**
	*\brief
	*	Un pipeline de rendu.
	*/
	class PipelineCache
	{
	public:
		/**
		*name
		*	Construction / Destruction.
		*/
		/**@{*/
		PipelineCache( VkDevice device
			, VkPipelineCacheCreateInfo createInfo );
		/**@}*/

		VkResult merge( VkPipelineCacheArray pipelines );

		inline ByteArray const & getData()const
		{
			return m_data;
		}

		VkDevice getDevice()const
		{
			return m_device;
		}

	private:
		VkDevice m_device;
		ByteArray m_data;
	};
}

#endif

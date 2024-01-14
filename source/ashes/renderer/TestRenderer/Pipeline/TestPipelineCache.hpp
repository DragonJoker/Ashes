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
			, VkPipelineCacheCreateInfo const & createInfo );
		/**@}*/

		VkResult merge( VkPipelineCacheArray const & pipelines );

		ByteArray const & getData()const noexcept
		{
			return m_data;
		}

		VkDevice getDevice()const noexcept
		{
			return m_device;
		}

	private:
		VkDevice m_device;
		ByteArray m_data;
	};
}

#endif

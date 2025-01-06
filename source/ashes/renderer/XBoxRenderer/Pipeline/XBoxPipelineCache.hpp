/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___XBOXRenderer_PipelineCache_HPP___
#define ___XBOXRenderer_PipelineCache_HPP___
#pragma once

#include "renderer/XBoxRenderer/XBoxRendererPrerequisites.hpp"

namespace ashes::xbox
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

		VkResult merge()const;

		ByteArray const & getData()const
		{
			return m_data;
		}

		VkDevice getDevice()const
		{
			return m_device;
		}

	private:
		VkDevice m_device;
		VkPipelineCacheCreateInfo m_createInfo;
		ByteArray m_data;
	};
}

#endif

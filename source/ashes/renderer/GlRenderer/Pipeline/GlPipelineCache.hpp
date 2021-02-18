/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_PipelineCache_HPP___
#define ___GlRenderer_PipelineCache_HPP___
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"

namespace ashes::gl
{
	/**
	*\brief
	*	Un pipeline de rendu.
	*/
	class PipelineCache
		: public AutoIdIcdObject< PipelineCache >
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

		inline VkDevice getDevice()const
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

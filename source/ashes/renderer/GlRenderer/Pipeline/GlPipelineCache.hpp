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

#include <common/ArrayView.hpp>

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
		PipelineCache( VkAllocationCallbacks const * allocInfo
			, VkDevice device
			, VkPipelineCacheCreateInfo createInfo );
		~PipelineCache();
		/**@}*/

		VkResult merge( ArrayView< VkPipelineCache const > pipelines );

		inline ByteArray const & getData()const
		{
			return m_data;
		}

		inline VkDevice getDevice()const
		{
			return m_device;
		}

	private:
		struct Header
		{
			uint32_t headerLength;
			uint32_t headerVersion;
			uint32_t vendorID;
			uint32_t deviceID;
			uint8_t pipelineCacheUUID[VK_UUID_SIZE];
		};

	private:
		Header m_header;
		VkDevice m_device;
		VkPipelineCacheCreateInfo m_createInfo;
		ByteArray m_data;
	};
}

#endif

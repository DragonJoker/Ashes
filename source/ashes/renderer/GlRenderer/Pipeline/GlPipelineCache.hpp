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
		~PipelineCache()noexcept;
		/**@}*/

		VkResult merge( ArrayView< VkPipelineCache const > pipelines )const;

		ByteArray const & getData()const
		{
			return m_data;
		}

		VkDevice getDevice()const
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
			std::array< uint8_t, VK_UUID_SIZE > pipelineCacheUUID;
		};

	private:
		Header m_header;
		VkDevice m_device;
		VkPipelineCacheCreateInfo m_createInfo;
		ByteArray m_data;
	};
}

#endif

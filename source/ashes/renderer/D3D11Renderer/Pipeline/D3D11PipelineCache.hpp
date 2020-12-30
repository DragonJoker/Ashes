/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___D3D11Renderer_PipelineCache_HPP___
#define ___D3D11Renderer_PipelineCache_HPP___
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

namespace ashes::d3d11
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
		VkDevice m_device;
		VkPipelineCacheCreateInfo m_createInfo;
		ByteArray m_data;
	};
}

#endif

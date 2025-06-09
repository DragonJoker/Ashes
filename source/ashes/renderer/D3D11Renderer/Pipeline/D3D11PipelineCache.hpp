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

namespace ashes::D3D11_NAMESPACE
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

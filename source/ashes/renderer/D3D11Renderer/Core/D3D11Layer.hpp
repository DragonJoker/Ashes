/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

namespace ashes::d3d11
{
	class Layer
	{
	public:
		virtual bool onCopyToImageCommand( VkCommandBuffer cmd
			, VkBufferImageCopyArray const & copyInfo
			, VkBuffer src
			, VkImage dst )const
		{
			return true;
		}

		virtual bool onCheckHResultCommand( HRESULT hresult
			, std::string message )const
		{
			return true;
		}

		virtual void onReportMessage( VkDebugReportFlagsEXT flags
			, VkDebugReportObjectTypeEXT objectType
			, uint64_t object
			, size_t location
			, int32_t messageCode
			, const char * pLayerPrefix
			, const char * pMessage )const
		{
		}
	};

	class LayerException
		: public std::exception
	{
	public:
		LayerException( Layer const & layer
			, std::string message )
			: m_layer{ layer }
			, m_message{ std::move( message ) }
		{
		}

		Layer const & getLayer()const
		{
			return m_layer;
		}

		const char * what()const
		{
			return m_message.c_str();
		}

	private:
		Layer const & m_layer;
		std::string m_message;
	};
}

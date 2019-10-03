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
		bool bufferImageCommand( VkCommandBuffer cmd
			, VkBufferImageCopy const & copyInfo
			, VkBuffer buffer
			, VkImage image )const;
		bool copyToImageCommand( VkCommandBuffer cmd
			, VkBufferImageCopyArray const & copyInfo
			, VkBuffer src
			, VkImage dst )const;
		bool checkHResultCommand( HRESULT hresult
			, std::string message )const;
#if VK_EXT_debug_utils
		void submitDebugUtilsMessenger( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity
			, VkDebugUtilsMessageTypeFlagsEXT messageTypes
			, VkDebugUtilsMessengerCallbackDataEXT const & callbackData )const;
#endif
#if VK_EXT_debug_report
		void reportMessage( VkDebugReportFlagsEXT flags
			, VkDebugReportObjectTypeEXT objectType
			, uint64_t object
			, size_t location
			, int32_t messageCode
			, const char * pLayerPrefix
			, const char * pMessage )const;
#endif

	private:
		virtual bool onBufferImageCommand( VkCommandBuffer cmd
			, VkBufferImageCopy const & copyInfo
			, VkBuffer buffer
			, VkImage image )const
		{
			return true;
		}

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

#if VK_EXT_debug_utils

		virtual void onSubmitDebugUtilsMessenger( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity
			, VkDebugUtilsMessageTypeFlagsEXT messageTypes
			, VkDebugUtilsMessengerCallbackDataEXT const & callbackData )const
		{
		}

#endif
#if VK_EXT_debug_report

		virtual void onReportMessage( VkDebugReportFlagsEXT flags
			, VkDebugReportObjectTypeEXT objectType
			, uint64_t object
			, size_t location
			, int32_t messageCode
			, const char * pLayerPrefix
			, const char * pMessage )const
		{
		}

#endif
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

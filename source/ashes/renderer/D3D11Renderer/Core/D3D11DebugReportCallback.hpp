/**
*\file
*	Instance.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "renderer/D3D11Renderer/Core/D3D11Layer.hpp"

namespace ashes::d3d11
{
	struct ReportData
	{
		VkDebugReportFlagsEXT flags;
		VkDebugReportObjectTypeEXT objectType;
		uint64_t object;
		size_t location;
		int32_t messageCode;
		std::string layerPrefix;
		std::string message;
	};

	class DebugReportCallbackEXT;

	class DebugLayer
		: public Layer
	{
	public:
		DebugLayer( DebugReportCallbackEXT & callback );
		bool onBufferImageCommand( VkCommandBuffer cmd
			, VkBufferImageCopy const & copyInfo
			, VkBuffer buffer
			, VkImage image )const;
		bool onCopyToImageCommand( VkCommandBuffer cmd
			, VkBufferImageCopyArray const & copyInfos
			, VkBuffer src
			, VkImage dst )const override;
		bool onCheckHResultCommand( HRESULT hresult
			, std::string message )const override;
		void onReportMessage( VkDebugReportFlagsEXT flags
			, VkDebugReportObjectTypeEXT objectType
			, uint64_t object
			, size_t location
			, int32_t messageCode
			, const char * pLayerPrefix
			, const char * pMessage )const override;

	private:
		DebugReportCallbackEXT & m_callback;
	};

	class DebugReportCallbackEXT
	{
	public:
		DebugReportCallbackEXT( VkInstance instance
			, VkDebugReportCallbackCreateInfoEXT createInfo );
		~DebugReportCallbackEXT();

		bool report( ReportData report );

		inline DebugLayer const & getLayer()const
		{
			return m_layer;
		}

	private:
		VkInstance m_instance;
		VkDebugReportCallbackCreateInfoEXT m_createInfo;
		DebugLayer m_layer;
	};
}

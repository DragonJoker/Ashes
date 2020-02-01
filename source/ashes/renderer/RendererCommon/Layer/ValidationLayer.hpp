///**
//*\file
//*	Instance.h
//*\author
//*	Sylvain Doremus
//*/
//#pragma once
//
//#include "renderer/RendererCommon/Layer/Layer.hpp"
//
//namespace ashes
//{
//	struct ReportData
//	{
//		VkDebugReportFlagsEXT flags;
//		VkDebugReportObjectTypeEXT objectType;
//		uint64_t object;
//		size_t location;
//		int32_t messageCode;
//		std::string layerPrefix;
//		std::string message;
//	};
//
//	class DebugReportCallback;
//
//	class DebugLayer
//		: public Layer
//	{
//	public:
//		DebugLayer( DebugReportCallback & callback );
//		bool onBufferImageCommand( VkCommandBuffer cmd
//			, VkBufferImageCopy const & copyInfo
//			, VkBuffer buffer
//			, VkImage image )const;
//		bool onCopyToImageCommand( VkCommandBuffer cmd
//			, ArrayView< VkBufferImageCopy const > const & copyInfos
//			, VkBuffer src
//			, VkImage dst )const override;
//
//#ifdef _WIN32
//		bool onCheckHResultCommand( HRESULT hresult
//			, std::string message )const override;
//#endif
//
//	private:
//		DebugReportCallback & m_callback;
//	};
//
//	class DebugReportCallback
//		: public ashes::DebugReportCallback
//	{
//	public:
//		DebugReportCallback( VkInstance instance
//			, VkDebugReportCallbackCreateInfoEXT createInfo );
//		~DebugReportCallback();
//
//		bool report( ReportData report );
//
//		inline DebugLayer const & getLayer()const
//		{
//			return m_layer;
//		}
//
//	private:
//		DebugLayer m_layer;
//	};
//}

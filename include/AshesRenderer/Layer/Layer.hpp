///*
//This file belongs to Ashes.
//See LICENSE file in root folder
//*/
//#pragma once
//
//#include "AshesRenderer/AshesRendererPrerequisites.hpp"
//
//namespace ashes
//{
//	class Layer
//	{
//	public:
//		virtual bool onCopyToImageCommand( VkCommandBuffer cmd
//			, VkBufferImageCopyArray const & copyInfo
//			, VkBuffer src
//			, VkImage dst )const = 0;
//
//#ifdef VK_USE_PLATFORM_WIN32_KHR
//		virtual bool onCheckHResultCommand( HRESULT hresult
//			, std::string message )const = 0;
//#endif
//	};
//
//	class LayerException
//		: public std::exception
//	{
//	public:
//		LayerException( Layer const & layer
//			, std::string message )
//			: m_layer{ layer }
//			, m_message{ std::move( message ) }
//		{
//		}
//
//		Layer const & getLayer()const
//		{
//			return m_layer;
//		}
//
//		const char * what()const override
//		{
//			return m_message.c_str();
//		}
//
//	private:
//		Layer const & m_layer;
//		std::string m_message;
//	};
//}

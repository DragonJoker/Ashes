/**
*\file
*	Instance.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "D3D11Renderer/Core/D3D11Layer.hpp"

#include <Ashes/Core/DebugReportCallback.hpp>

namespace ashes::d3d11
{
	struct ReportData
	{
		ashes::DebugReportFlags flags;
		VkDebugReportObjectTypeEXT objectType;
		uint64_t object;
		size_t location;
		int32_t messageCode;
		std::string layerPrefix;
		std::string message;
	};

	class DebugReportCallback;

	class DebugLayer
		: public Layer
	{
	public:
		DebugLayer( DebugReportCallback & callback );
		bool onBufferImageCommand( VkCommandBuffer cmd
			, ashes::BufferImageCopy const & copyInfo
			, VkBuffer buffer
			, VkImage image )const;
		bool onCopyToImageCommand( VkCommandBuffer cmd
			, ashes::VkBufferImageCopyArray const & copyInfos
			, VkBuffer src
			, VkImage dst )const;
		bool onCheckHResultCommand( HRESULT hresult
			, std::string message )const;

	private:
		DebugReportCallback & m_callback;
	};

	class DebugReportCallback
		: public ashes::DebugReportCallback
	{
	public:
		DebugReportCallback( Instance const & instance
			, ashes::DebugReportCallbackCreateInfo createInfo );
		~DebugReportCallback();

		bool report( ReportData report );

		inline DebugLayer const & getLayer()const
		{
			return m_layer;
		}

	private:
		DebugLayer m_layer;
	};
}

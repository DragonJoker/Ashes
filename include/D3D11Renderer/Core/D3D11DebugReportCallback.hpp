/**
*\file
*	Instance.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "D3D11Renderer/Core/D3D11Layer.hpp"

#include <Ashes/Core/DebugReportCallback.hpp>

namespace d3d11_renderer
{
	struct ReportData
	{
		ashes::DebugReportFlags flags;
		ashes::DebugReportObjectType objectType;
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
		bool onBufferImageCommand( ashes::CommandBuffer const & cmd
			, ashes::BufferImageCopy const & copyInfo
			, ashes::BufferBase const & buffer
			, ashes::Image const & image )const;
		bool onCopyToImageCommand( ashes::CommandBuffer const & cmd
			, ashes::BufferImageCopyArray const & copyInfos
			, ashes::BufferBase const & src
			, ashes::Image const & dst )const override;
		bool onCheckHResultCommand( HRESULT hresult
			, std::string message )const override;

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

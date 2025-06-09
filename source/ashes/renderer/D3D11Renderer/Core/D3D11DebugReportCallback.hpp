/**
*\file
*	Instance.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "renderer/D3D11Renderer/Core/D3D11Layer.hpp"

namespace ashes::D3D11_NAMESPACE
{
#if VK_EXT_debug_utils

	struct MessageData
	{
		~MessageData()noexcept = default;

		MessageData( MessageData const & rhs )
			: messageSeverity{ rhs.messageSeverity }
			, messageTypes{ rhs.messageTypes }
			, callbackData{ rhs.callbackData }
			, message{ rhs.message }
			, objects{ rhs.objects }
		{
			callbackData.pMessage = message.data();
			callbackData.objectCount = uint32_t( objects.size() );
			callbackData.pObjects = objects.data();
		}

		MessageData & operator=( MessageData const & rhs )
		{
			messageSeverity = rhs.messageSeverity;
			messageTypes = rhs.messageTypes;
			callbackData = rhs.callbackData;
			message = rhs.message;
			objects = rhs.objects;

			callbackData.pMessage = message.data();
			callbackData.objectCount = uint32_t( objects.size() );
			callbackData.pObjects = objects.data();

			return *this;
		}

		MessageData( MessageData && rhs )noexcept
			: messageSeverity{ rhs.messageSeverity }
			, messageTypes{ rhs.messageTypes }
			, callbackData{ std::move( rhs.callbackData ) }
			, message{ std::move( rhs.message ) }
			, objects{ std::move( rhs.objects ) }
		{
			callbackData.pMessage = message.data();
			callbackData.objectCount = uint32_t( objects.size() );
			callbackData.pObjects = objects.data();

			rhs.callbackData.objectCount = 0u;
			rhs.callbackData.pObjects = nullptr;
			rhs.callbackData.queueLabelCount = 0u;
			rhs.callbackData.pQueueLabels = nullptr;
			rhs.callbackData.cmdBufLabelCount = 0u;
			rhs.callbackData.pCmdBufLabels = nullptr;
		}

		MessageData & operator=( MessageData && rhs )noexcept
		{
			messageSeverity = rhs.messageSeverity;
			messageTypes = rhs.messageTypes;
			callbackData = std::move( rhs.callbackData );
			message = std::move( rhs.message );
			objects = std::move( rhs.objects );

			callbackData.pMessage = message.data();
			callbackData.objectCount = uint32_t( objects.size() );
			callbackData.pObjects = objects.data();

			rhs.callbackData.objectCount = 0u;
			rhs.callbackData.pObjects = nullptr;
			rhs.callbackData.queueLabelCount = 0u;
			rhs.callbackData.pQueueLabels = nullptr;
			rhs.callbackData.cmdBufLabelCount = 0u;
			rhs.callbackData.pCmdBufLabels = nullptr;

			return *this;
		}

		MessageData( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity
			, VkDebugUtilsMessageTypeFlagsEXT messageTypes
			, VkDebugUtilsMessengerCallbackDataEXT callbackData )
			: messageSeverity{ messageSeverity }
			, messageTypes{ messageTypes }
			, callbackData{ std::move( callbackData ) }
			, message( this->callbackData.pMessage ? this->callbackData.pMessage : std::string{} )
			, objects( this->callbackData.pObjects, this->callbackData.pObjects + this->callbackData.objectCount )
		{
		}

		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity;
		VkDebugUtilsMessageTypeFlagsEXT messageTypes;
		VkDebugUtilsMessengerCallbackDataEXT callbackData;
		std::string message;
		std::vector< VkDebugUtilsObjectNameInfoEXT > objects;
	};

	class DebugUtilsMessengerEXT;

	class DebugUtilsLayer
		: public Layer
	{
	public:
		explicit DebugUtilsLayer( DebugUtilsMessengerEXT & callback );
		bool onBufferImageCommand( VkCommandBuffer cmd
			, VkBufferImageCopy const & copyInfo
			, VkBuffer buffer
			, VkImage image )const override;
		bool onCopyToImageCommand( VkCommandBuffer cmd
			, ArrayView< VkBufferImageCopy const > const & copyInfos
			, VkBuffer src
			, VkImage dst )const override;
		bool onCheckHResultCommand( HRESULT hresult
			, std::string const & message )const override;
#	if VK_EXT_debug_report
		void onReportMessage( VkDebugReportFlagsEXT flags
			, VkDebugReportObjectTypeEXT objectType
			, uint64_t object
			, size_t location
			, int32_t messageCode
			, const char * pLayerPrefix
			, const char * pMessage )const override;
#	endif
		void onSubmitDebugUtilsMessenger( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity
			, VkDebugUtilsMessageTypeFlagsEXT messageTypes
			, VkDebugUtilsMessengerCallbackDataEXT const & callbackData )const override;

	private:
		DebugUtilsMessengerEXT & m_callback;
	};

	class DebugUtilsMessengerEXT
		: public NonCopyable
	{
	public:
		DebugUtilsMessengerEXT( VkInstance instance
			, VkDebugUtilsMessengerCreateInfoEXT createInfo );
		~DebugUtilsMessengerEXT()noexcept;

		bool report( MessageData const & report )noexcept;

		inline DebugUtilsLayer const & getLayer()const noexcept
		{
			return m_layer;
		}

		VkInstance getInstance()const noexcept
		{
			return m_instance;
		}

	private:
		VkInstance m_instance;
		VkDebugUtilsMessengerCreateInfoEXT m_createInfo;
		DebugUtilsLayer m_layer;
	};

#endif
#if VK_EXT_debug_report

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

	class DebugReportLayer
		: public Layer
	{
	public:
		explicit DebugReportLayer( DebugReportCallbackEXT & callback );
		bool onBufferImageCommand( VkCommandBuffer cmd
			, VkBufferImageCopy const & copyInfo
			, VkBuffer buffer
			, VkImage image )const override;
		bool onCopyToImageCommand( VkCommandBuffer cmd
			, ArrayView< VkBufferImageCopy const > const & copyInfos
			, VkBuffer src
			, VkImage dst )const override;
		bool onCheckHResultCommand( HRESULT hresult
			, std::string const & message )const override;
		void onReportMessage( VkDebugReportFlagsEXT flags
			, VkDebugReportObjectTypeEXT objectType
			, uint64_t object
			, size_t location
			, int32_t messageCode
			, const char * pLayerPrefix
			, const char * pMessage )const override;
#	if VK_EXT_debug_utils
		void onSubmitDebugUtilsMessenger( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity
			, VkDebugUtilsMessageTypeFlagsEXT messageTypes
			, VkDebugUtilsMessengerCallbackDataEXT const & callbackData )const override;
#	endif

	private:
		DebugReportCallbackEXT & m_callback;
	};

	class DebugReportCallbackEXT
		: public NonCopyable
	{
	public:
		DebugReportCallbackEXT( VkInstance instance
			, VkDebugReportCallbackCreateInfoEXT createInfo );
		~DebugReportCallbackEXT()noexcept;

		bool report( ReportData const & report )noexcept;

		inline DebugReportLayer const & getLayer()const noexcept
		{
			return m_layer;
		}

		VkInstance getInstance()const noexcept
		{
			return m_instance;
		}

	private:
		VkInstance m_instance;
		VkDebugReportCallbackCreateInfoEXT m_createInfo;
		DebugReportLayer m_layer;
	};

#endif
}

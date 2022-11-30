/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "renderer/D3D11Renderer/Core/D3D11DebugReportCallback.hpp"

#include "renderer/D3D11Renderer/Core/D3D11Instance.hpp"
#include "renderer/D3D11Renderer/Image/D3D11Image.hpp"

#include "ashesd3d11_api.hpp"

#include <sstream>

namespace ashes::d3d11
{
	//*************************************************************************
	
	namespace
	{
		template< typename T >
		std::string toString( T const & value )
		{
			std::stringstream stream;
			stream.imbue( std::locale{ "C" } );
			stream << value;
			return stream.str();
		}

		bool isOneBitSet( uint32_t flags )
		{
			static std::set< uint32_t > const flagsRegister
			{
				0x00000001, 0x00000002, 0x00000004, 0x00000008,
				0x00000010, 0x00000020, 0x00000040, 0x00000080,
				0x00000100, 0x00000200, 0x00000400, 0x00000800,
				0x00001000, 0x00002000, 0x00004000, 0x00008000,
				0x00010000, 0x00020000, 0x00040000, 0x00080000,
				0x00100000, 0x00200000, 0x00400000, 0x00800000,
				0x01000000, 0x02000000, 0x04000000, 0x08000000,
				0x10000000, 0x20000000, 0x40000000, 0x80000000,
			};
			return flagsRegister.find( flags ) != flagsRegister.end();
		}

		template< typename CheckFuncT
			, typename DataT
			, typename LayerT
			, typename CallbackT >
			bool checkValid( CallbackT & callback
				, LayerT const & layer
				, VkBufferImageCopy const & copyInfo
				, VkImage image
				, DataT & data
				, CheckFuncT check )
		{
			auto format = get( image )->getFormat();
			VkExtent3D texelBlockExtent{ 1u, 1u, 1u };

			if ( ashes::isCompressedFormat( format ) )
			{
				auto extent = ashes::getMinimalExtent2D( format );
				texelBlockExtent.width = extent.width;
				texelBlockExtent.height = extent.height;
			}
			else
			{
				texelBlockExtent.width = ashes::getSize( format );
			}

			VkDeviceSize texelBlockSize;

			if ( !ashes::isDepthStencilFormat( format ) )
			{
				texelBlockSize = ashes::getSize( texelBlockExtent, format );
			}
			else
			{
				texelBlockSize = ashes::getSize( format );
			}

			auto texelBlockWidth = texelBlockSize / ( texelBlockExtent.height * texelBlockExtent.depth );
			auto texelBlockHeight = texelBlockSize / ( texelBlockExtent.width * texelBlockExtent.depth );
			auto texelBlockDepth = texelBlockSize / ( texelBlockExtent.width * texelBlockExtent.height );

			auto result = VK_SUCCESS == check( callback
				, layer
				, data
				, 0u == ( copyInfo.bufferOffset % texelBlockSize )
				, "bufferOffset must be a multiple of the format's texel block size" );
			result = result && VK_SUCCESS == check( callback
				, layer
				, data
				, ( copyInfo.bufferOffset == 0u || 0u == ( copyInfo.bufferOffset % 4u ) )
				, "bufferOffset must be a multiple of 4" );
			result = result && VK_SUCCESS == check( callback
				, layer
				, data
				, ( copyInfo.bufferRowLength == 0 ) || ( copyInfo.bufferRowLength >= copyInfo.imageExtent.width )
				, "bufferRowLength must be 0, or greater than or equal to the width member of imageExtent" );
			result = result && VK_SUCCESS == check( callback
				, layer
				, data
				, ( copyInfo.bufferImageHeight == 0 ) || ( copyInfo.bufferImageHeight >= copyInfo.imageExtent.height )
				, "bufferImageHeight must be 0, or greater than or equal to the height member of imageExtent" );
			auto subresourceWidth = texelBlockWidth * get( image )->getDimensions().width >> copyInfo.imageSubresource.mipLevel;
			result = result && VK_SUCCESS == check( callback
				, layer
				, data
				, ( copyInfo.imageOffset.x >= 0 && copyInfo.imageOffset.x <= int32_t( subresourceWidth ) )
				&& ( copyInfo.imageExtent.width + copyInfo.imageOffset.x >= 0
					&& copyInfo.imageExtent.width + copyInfo.imageOffset.x <= int32_t( subresourceWidth ) )
				, "imageOffset.x and (imageExtent.width + imageOffset.x) must both be greater than or equal to 0 and less than or equal to the image subresource width" );
			auto subresourceHeight = texelBlockHeight * get( image )->getDimensions().height >> copyInfo.imageSubresource.mipLevel;
			result = result && VK_SUCCESS == check( callback
				, layer
				, data
				, ( copyInfo.imageOffset.y >= 0 && copyInfo.imageOffset.y <= int32_t( subresourceHeight ) )
				&& ( copyInfo.imageExtent.height + copyInfo.imageOffset.y >= 0
					&& copyInfo.imageExtent.height + copyInfo.imageOffset.y <= int32_t( subresourceHeight ) )
				, "imageOffset.y and (imageExtent.height + imageOffset.y) must both be greater than or equal to 0 and less than or equal to the image subresource height" );
			auto subresourceDepth = texelBlockDepth * get( image )->getDimensions().depth;
			result = result && VK_SUCCESS == check( callback
				, layer
				, data
				, ( copyInfo.imageOffset.z >= 0 && copyInfo.imageOffset.z <= int32_t( subresourceDepth ) )
				&& ( copyInfo.imageExtent.depth + copyInfo.imageOffset.z >= 0
					&& copyInfo.imageExtent.depth + copyInfo.imageOffset.z <= int32_t( subresourceDepth ) )
				, "imageOffset.z and (imageExtent.depth + imageOffset.z) must both be greater than or equal to 0 and less than or equal to the image subresource depth" );
			auto imageType = get( image )->getType();
			result = result && VK_SUCCESS == check( callback
				, layer
				, data
				, ( imageType != VK_IMAGE_TYPE_1D
					|| ( copyInfo.imageOffset.y == 0
						&& copyInfo.imageExtent.height == 1u ) )
				, "if the calling command's VkImage parameter is of type VK_IMAGE_TYPE_1D, then imageOffset.y must be 0 and imageExtent.height must be 1." );
			result = result && VK_SUCCESS == check( callback
				, layer
				, data
				, ( ( imageType != VK_IMAGE_TYPE_1D && imageType != VK_IMAGE_TYPE_2D )
					|| ( copyInfo.imageOffset.z == 0
						&& copyInfo.imageExtent.depth == 1u ) )
				, "if the calling command's VkImage parameter is of type VK_IMAGE_TYPE_1D or VK_IMAGE_TYPE_2D, then imageOffset.z must be 0 and imageExtent.depth must be 1." );
			result = result && VK_SUCCESS == check( callback
				, layer
				, data
				, ( imageType != VK_IMAGE_TYPE_3D
					|| ( copyInfo.imageSubresource.baseArrayLayer == 0
						&& copyInfo.imageSubresource.layerCount == 1u ) )
				, "if the calling command's VkImage parameter is of type VK_IMAGE_TYPE_1D or VK_IMAGE_TYPE_2D, then imageOffset.z must be 0 and imageExtent.depth must be 1." );
			result = result && VK_SUCCESS == check( callback
				, layer
				, data
				, ( ( !ashes::isCompressedFormat( format ) )
					|| ( ( copyInfo.bufferRowLength % texelBlockExtent.width ) == 0 ) )
				, "If the calling command's VkImage parameter is a compressed image, or a single-plane, _422 image format, bufferRowLength must be a multiple of the compressed texel block width." );
			result = result && VK_SUCCESS == check( callback
				, layer
				, data
				, ( ( !ashes::isCompressedFormat( format ) )
					|| ( ( copyInfo.bufferImageHeight % texelBlockExtent.height ) == 0 ) )
				, "If the calling command's VkImage parameter is a compressed image, or a single-plane, _422 image format, bufferImageHeight must be a multiple of the compressed texel block height." );
			result = result && VK_SUCCESS == check( callback
				, layer
				, data
				, ( ( !ashes::isCompressedFormat( format ) )
					|| ( ( copyInfo.imageOffset.x % texelBlockExtent.width ) == 0
						&& ( copyInfo.imageOffset.y % texelBlockExtent.height ) == 0
						&& ( copyInfo.imageOffset.z % 1u ) == 0 ) )
				, "If the calling command's VkImage parameter is a compressed image, or a single-plane, _422 image format, all members of imageOffset must be a multiple of the corresponding dimensions of the compressed texel block." );
			result = result && VK_SUCCESS == check( callback
				, layer
				, data
				, ( ( !ashes::isCompressedFormat( format ) )
					|| ( ( copyInfo.bufferOffset % texelBlockSize ) == 0 ) )
				, "If the calling command's VkImage parameter is a compressed image, or a single-plane, _422 image format, bufferOffset must be a multiple of the compressed texel block size in bytes." );
			result = result && VK_SUCCESS == check( callback
				, layer
				, data
				, ( ( !ashes::isCompressedFormat( format ) )
					|| ( copyInfo.imageExtent.width % texelBlockExtent.width ) == 0
					|| ( copyInfo.imageExtent.width + copyInfo.imageOffset.x ) == subresourceWidth )
				, "If the calling command's VkImage parameter is a compressed image, or a single-plane, _422 image format, imageExtent.width must be a multiple of the compressed texel block width or (imageExtent.width + imageOffset.x) must equal the image subresource width." );
			result = result && VK_SUCCESS == check( callback
				, layer
				, data
				, ( ( !ashes::isCompressedFormat( format ) )
					|| ( copyInfo.imageExtent.height % texelBlockExtent.height ) == 0
					|| ( copyInfo.imageExtent.height + copyInfo.imageOffset.y ) == subresourceHeight )
				, "If the calling command's VkImage parameter is a compressed image, or a single-plane, _422 image format, imageExtent.height must be a multiple of the compressed texel block height or (imageExtent.height + imageOffset.y) must equal the image subresource height." );
			result = result && VK_SUCCESS == check( callback
				, layer
				, data
				, ( ( !ashes::isCompressedFormat( format ) )
					|| ( copyInfo.imageExtent.depth % texelBlockExtent.depth ) == 0
					|| ( copyInfo.imageExtent.depth + copyInfo.imageOffset.z ) == subresourceDepth )
				, "If the calling command's VkImage parameter is a compressed image, or a single-plane, _422 image format, imageExtent.depth must be a multiple of the compressed texel block depth or (imageExtent.depth + imageOffset.z) must equal the image subresource depth." );
			result = result && VK_SUCCESS == check( callback
				, layer
				, data
				, ( checkFlag( copyInfo.imageSubresource.aspectMask, ashes::getAspectMask( format ) ) != 0u )
				, "The aspectMask member of imageSubresource must specify aspects present in the calling command's VkImage parameter." );
			result = result && VK_SUCCESS == check( callback
				, layer
				, data
				, ( isOneBitSet( copyInfo.imageSubresource.aspectMask ) )
				, "The aspectMask member of imageSubresource must specify aspects present in the calling command's VkImage parameter." );
			return !result;
		}

		struct Error
		{
			std::string name;
			VkResult result;
		};

		using ErrorsMap = std::map< HRESULT, Error >;

		ErrorsMap const & getErrors()
		{
#if !defined( DXGI_ERROR_INVALID_CALL )
#	define DXGI_ERROR_INVALID_CALL D3DERR_INVALIDCALL
#endif
#if !defined( DXGI_ERROR_WAS_STILL_DRAWING )
#	define DXGI_ERROR_WAS_STILL_DRAWING D3DERR_WASSTILLDRAWING
#endif
			static ErrorsMap Errors
			{
				{ D3D11_ERROR_FILE_NOT_FOUND, { "The file was not found.", VK_ERROR_INITIALIZATION_FAILED } },
				{ D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS, { "There are too many unique instances of a particular type of state object.", VK_ERROR_INITIALIZATION_FAILED } },
				{ D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS, { "There are too many unique instances of a particular type of view object.", VK_ERROR_INITIALIZATION_FAILED } },
				{ D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD, { "The first call to ID3D11DeviceContext::Map after either ID3D11Device::CreateDeferredContext or ID3D11DeviceContext::FinishCommandList per Resource was not D3D11_MAP_WRITE_DISCARD.", VK_ERROR_INITIALIZATION_FAILED } },
				{ E_FAIL, { "Attempted to create a device with the debug layer enabled and the layer is not installed.", VK_ERROR_INITIALIZATION_FAILED } },
				{ E_INVALIDARG, { "An invalid parameter was passed to the returning function.", VK_ERROR_INITIALIZATION_FAILED } },
				{ E_OUTOFMEMORY, { "Direct3D could not allocate sufficient memory to complete the call.", VK_ERROR_OUT_OF_DEVICE_MEMORY } },
				{ E_NOTIMPL, { "The method call isn't implemented with the passed parameter combination.", VK_ERROR_INITIALIZATION_FAILED } },
				{ S_FALSE, { "Alternate success value, indicating a successful but nonstandard completion( the precise meaning depends on context ).", VK_ERROR_INITIALIZATION_FAILED } },
				{ DXGI_ERROR_INVALID_CALL, { "The method call is invalid.For example, a method's parameter may not be a valid pointer.", VK_ERROR_INITIALIZATION_FAILED } },
				{ DXGI_ERROR_ACCESS_DENIED, { "You tried to use a resource to which you did not have the required access privileges.This error is most typically caused when you write to a shared resource with read - only access.", VK_ERROR_INITIALIZATION_FAILED } },
				{ DXGI_ERROR_ACCESS_LOST, { "The desktop duplication interface is invalid.The desktop duplication interface typically becomes invalid when a different type of image is displayed on the desktop.", VK_ERROR_INITIALIZATION_FAILED } },
#if defined( DXGI_ERROR_ALREADY_EXISTS )
				{ DXGI_ERROR_ALREADY_EXISTS, { "The desired element already exists.This is returned by DXGIDeclareAdapterRemovalSupport if it is not the first time that the function is called.", VK_ERROR_INITIALIZATION_FAILED } },
#endif
				{ DXGI_ERROR_CANNOT_PROTECT_CONTENT, { "DXGI can't provide content protection on the swap chain. This error is typically caused by an older driver, or when you use a swap chain that is incompatible with content protection.", VK_ERROR_INITIALIZATION_FAILED } },
				{ DXGI_ERROR_DEVICE_HUNG, { "The application's device failed due to badly formed commands sent by the application. This is an design-time issue that should be investigated and fixed.", VK_ERROR_INITIALIZATION_FAILED } },
				{ DXGI_ERROR_DEVICE_REMOVED, { "The video card has been physically removed from the system, or a driver upgrade for the video card has occurred.The application should destroy and recreate the device.For help debugging the problem, call ID3D10Device::GetDeviceRemovedReason.", VK_ERROR_DEVICE_LOST } },
				{ DXGI_ERROR_DEVICE_RESET, { "The device failed due to a badly formed command.This is a run - time issue; The application should destroy and recreate the device.", VK_ERROR_DEVICE_LOST } },
				{ DXGI_ERROR_DRIVER_INTERNAL_ERROR, { "The driver encountered a problem and was put into the device removed state.", VK_ERROR_INITIALIZATION_FAILED } },
				{ DXGI_ERROR_FRAME_STATISTICS_DISJOINT, { "An event( for example, a power cycle ) interrupted the gathering of presentation statistics.", VK_ERROR_INITIALIZATION_FAILED } },
				{ DXGI_ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE, { "The application attempted to acquire exclusive ownership of an output, but failed because some other application( or device within the application ) already acquired ownership.", VK_ERROR_INITIALIZATION_FAILED } },
				{ DXGI_ERROR_MORE_DATA, { "The buffer supplied by the application is not big enough to hold the requested data.", VK_ERROR_INITIALIZATION_FAILED } },
				{ DXGI_ERROR_NAME_ALREADY_EXISTS, { "The supplied name of a resource in a call to IDXGIResource1::CreateSharedHandle is already associated with some other resource.", VK_ERROR_INITIALIZATION_FAILED } },
				{ DXGI_ERROR_NONEXCLUSIVE, { "A global counter resource is in use, and the Direct3D device can't currently use the counter resource.", VK_ERROR_INITIALIZATION_FAILED } },
				{ DXGI_ERROR_NOT_CURRENTLY_AVAILABLE, { "The resource or request is not currently available, but it might become available later.", VK_ERROR_INITIALIZATION_FAILED } },
				{ DXGI_ERROR_NOT_FOUND, { "When calling IDXGIObject::GetPrivateData, the GUID passed in is not recognized as one previously passed to IDXGIObject::SetPrivateData or IDXGIObject::SetPrivateDataInterface.When calling IDXGIFactory::EnumAdapters or IDXGIAdapter::EnumOutputs, the enumerated ordinal is out of range.", VK_ERROR_INITIALIZATION_FAILED } },
				{ DXGI_ERROR_REMOTE_CLIENT_DISCONNECTED, { "Reserved", VK_ERROR_INITIALIZATION_FAILED } },
				{ DXGI_ERROR_REMOTE_OUTOFMEMORY, { "Reserved", VK_ERROR_INITIALIZATION_FAILED } },
				{ DXGI_ERROR_RESTRICT_TO_OUTPUT_STALE, { "The DXGI output( monitor ) to which the swap chain content was restricted is now disconnected or changed.", VK_ERROR_INITIALIZATION_FAILED } },
				{ DXGI_ERROR_SDK_COMPONENT_MISSING, { "The operation depends on an SDK component that is missing or mismatched.", VK_ERROR_INITIALIZATION_FAILED } },
				{ DXGI_ERROR_SESSION_DISCONNECTED, { "The Remote Desktop Services session is currently disconnected.", VK_ERROR_INITIALIZATION_FAILED } },
				{ DXGI_ERROR_UNSUPPORTED, { "The requested functionality is not supported by the device or the driver.", VK_ERROR_INITIALIZATION_FAILED } },
				{ DXGI_ERROR_WAIT_TIMEOUT, { "The time - out interval elapsed before the next desktop frame was available.", VK_TIMEOUT } },
				{ DXGI_ERROR_WAS_STILL_DRAWING, { "The GPU was busy at the moment when a call was made to perform an operation, and did not execute or schedule the operation.", VK_ERROR_INITIALIZATION_FAILED } },
				{ S_OK, { "No error occurred.", VK_SUCCESS } },
			};
			return Errors;
		}
	}

	//*************************************************************************

#if VK_EXT_debug_utils
#	if VK_EXT_debug_report

	namespace dudr
	{
		VkDebugUtilsMessageSeverityFlagBitsEXT getDebugUtilsSeverity( VkDebugReportFlagsEXT flags )
		{
			switch ( flags )
			{
			case VK_DEBUG_REPORT_INFORMATION_BIT_EXT:
				return VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
			case VK_DEBUG_REPORT_WARNING_BIT_EXT:
				return VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
			case VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT:
				return VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
			case VK_DEBUG_REPORT_ERROR_BIT_EXT:
				return VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			case VK_DEBUG_REPORT_DEBUG_BIT_EXT:
				return VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
			default:
				return VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
			}
		}
		
		VkDebugReportFlagsEXT getDebugUtilsSeverity( VkDebugUtilsMessageSeverityFlagBitsEXT flags )
		{
			switch ( flags )
			{
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
				return VK_DEBUG_REPORT_INFORMATION_BIT_EXT;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
				return VK_DEBUG_REPORT_WARNING_BIT_EXT;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
				return VK_DEBUG_REPORT_ERROR_BIT_EXT;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
				return VK_DEBUG_REPORT_DEBUG_BIT_EXT;
			default:
				return VK_DEBUG_REPORT_DEBUG_BIT_EXT;
			}
		}

		VkObjectType getObjectType( VkDebugReportObjectTypeEXT type )
		{
			return VkObjectType( type );
		}

		VkDebugReportObjectTypeEXT getObjectType( VkObjectType type )
		{
			return VkDebugReportObjectTypeEXT( type );
		}
	}

#	endif

	namespace du
	{
		VkResult check( DebugUtilsMessengerEXT & callback
			, DebugUtilsLayer const & layer
			, MessageData report
			, bool condition
			, std::string message )
		{
			if ( !condition )
			{
				report.message += "Condition failed: " + message;
				callback.report( report );
				return VkResult( report.callbackData.messageIdNumber );
			}

			return VK_SUCCESS;
		}

		template< typename T >
		VkResult checkEqual( DebugUtilsMessengerEXT callback
			, ReportData report
			, T const & lhs
			, T const & rhs )
		{
			return check( callback
				, report
				, !( lhs == rhs )
				, toString( lhs ) + " != " + toString( rhs ) );
		}
	}

	DebugUtilsLayer::DebugUtilsLayer( DebugUtilsMessengerEXT & callback )
		: m_callback{ callback }
	{
	}

	bool DebugUtilsLayer::onBufferImageCommand( VkCommandBuffer cmd
		, VkBufferImageCopy const & copyInfo
		, VkBuffer buffer
		, VkImage image )const
	{
		static MessageData const baseMessage
		{
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT,
			VkDebugUtilsMessengerCallbackDataEXT
			{
				VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CALLBACK_DATA_EXT,
				nullptr,
				0u,
				"VALIDATION",
				0u,
			},
		};
		MessageData message{ baseMessage };
		message.objects.push_back( 
			{
				VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
				nullptr,
				VK_OBJECT_TYPE_COMMAND_BUFFER,
				uint64_t( cmd ),
				nullptr,
			} );
		return !checkValid( m_callback, *this, copyInfo, image, message, du::check );
	}

	bool DebugUtilsLayer::onCopyToImageCommand( VkCommandBuffer cmd
		, ArrayView< VkBufferImageCopy const > const & copyInfos
		, VkBuffer src
		, VkImage dst )const
	{
		for ( auto & copyInfo : copyInfos )
		{
			onBufferImageCommand( cmd, copyInfo, src, dst );
		}

		return false;
	}

	bool DebugUtilsLayer::onCheckHResultCommand( HRESULT hresult
		, std::string message )const
	{
		static MessageData const baseMessage
		{
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
			VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT,
			VkDebugUtilsMessengerCallbackDataEXT
			{
				VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CALLBACK_DATA_EXT,
				nullptr,
				0u,
				"VALIDATION",
				0u,
			},
		};
		MessageData messageData{ baseMessage };
		bool result = hresult == S_OK;

		if ( !result )
		{
			std::stringstream stream;
			stream.imbue( std::locale{ "C" } );
			stream << "Error calling [" << message << "]: " << std::hex << hresult;
			auto it = getErrors().find( hresult );
			VkResult ret{ VK_ERROR_INITIALIZATION_FAILED };

			if ( it != getErrors().end() )
			{
				stream << "\n    Direct3D: " << it->second.name;
				ret = it->second.result;
			}

			stream << "\n    Windows: " << getLastErrorText();
			messageData.callbackData.messageIdNumber = hresult;
			du::check( m_callback
				, *this
				, std::move( messageData )
				, false
				, stream.str() );
		}

		return !result;
	}

#	if VK_EXT_debug_report

	void DebugUtilsLayer::onReportMessage( VkDebugReportFlagsEXT flags
		, VkDebugReportObjectTypeEXT objectType
		, uint64_t object
		, size_t location
		, int32_t messageCode
		, const char * pLayerPrefix
		, const char * pMessage )const
	{
		static MessageData const baseMessage
		{
			dudr::getDebugUtilsSeverity( flags ),
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT,
			VkDebugUtilsMessengerCallbackDataEXT
			{
				VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CALLBACK_DATA_EXT,
				nullptr,
				0u,
				pLayerPrefix,
				messageCode,
				pMessage,
				0u,
				nullptr,
				0u,
				nullptr,
			},
		};
		MessageData message{ baseMessage };
		message.objects.push_back(
			{
				VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
				nullptr,
				dudr::getObjectType( objectType ),
				object,
				nullptr,
			} );
		m_callback.report( message );
	}

#	endif

	void DebugUtilsLayer::onSubmitDebugUtilsMessenger( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity
		, VkDebugUtilsMessageTypeFlagsEXT messageTypes
		, VkDebugUtilsMessengerCallbackDataEXT const & callbackData )const
	{
		m_callback.report( MessageData
			{
				messageSeverity,
				messageTypes,
				callbackData,
			} );
	}

	//*************************************************************************

	DebugUtilsMessengerEXT::DebugUtilsMessengerEXT( VkInstance instance
		, VkDebugUtilsMessengerCreateInfoEXT createInfo )
		: m_instance{ instance }
		, m_createInfo{ std::move( createInfo ) }
		, m_layer{ *this }
	{
		get( m_instance )->registerLayer( &m_layer );
	}

	DebugUtilsMessengerEXT::~DebugUtilsMessengerEXT()
	{
		get( m_instance )->unregisterLayer( &m_layer );
	}

	bool DebugUtilsMessengerEXT::report( MessageData report )
	{
		if ( report.messageTypes & m_createInfo.messageType
			&& m_createInfo.messageSeverity & report.messageSeverity )
		{
			return m_createInfo.pfnUserCallback( report.messageSeverity
				, report.messageTypes
				, &report.callbackData
				, m_createInfo.pUserData ) != 0;
		}

		return false;
	}

	//*************************************************************************

#endif
#if VK_EXT_debug_report

	//*************************************************************************

	namespace dr
	{
		VkResult check( DebugReportCallbackEXT & callback
			, DebugReportLayer const & layer
			, ReportData report
			, bool condition
			, std::string message )
		{
			if ( !condition )
			{
				report.message += "Condition failed: " + message;
				callback.report( report );
				return VkResult( report.messageCode );
			}

			return VK_SUCCESS;
		}

		template< typename T >
		VkResult checkEqual( DebugReportCallbackEXT callback
			, ReportData report
			, T const & lhs
			, T const & rhs )
		{
			return check( callback
				, report
				, !( lhs == rhs )
				, toString( lhs ) + " != " + toString( rhs ) );
		}
	}

	//*************************************************************************

	DebugReportLayer::DebugReportLayer( DebugReportCallbackEXT & callback )
		: m_callback{ callback }
	{
	}

	bool DebugReportLayer::onBufferImageCommand( VkCommandBuffer cmd
		, VkBufferImageCopy const & copyInfo
		, VkBuffer buffer
		, VkImage image )const
	{
		static ReportData const baseReport
		{
			VK_DEBUG_REPORT_ERROR_BIT_EXT,
			VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT,
			0u,
			0u,
			0u,
			"VALIDATION",
		};
		ReportData report{ baseReport };
		report.object = uint64_t( &cmd );
		return !checkValid( m_callback, *this, copyInfo, image, report, dr::check );
	}

	bool DebugReportLayer::onCopyToImageCommand( VkCommandBuffer cmd
		, ArrayView< VkBufferImageCopy const > const & copyInfos
		, VkBuffer src
		, VkImage dst )const
	{
		for ( auto & copyInfo : copyInfos )
		{
			onBufferImageCommand( cmd, copyInfo, src, dst );
		}

		return false;
	}

	bool DebugReportLayer::onCheckHResultCommand( HRESULT hresult
		, std::string message )const
	{
		static ReportData const baseReport
		{
			VK_DEBUG_REPORT_ERROR_BIT_EXT,
			VK_DEBUG_REPORT_OBJECT_TYPE_UNKNOWN_EXT,
			0u,
			0u,
			0u,
			"VALIDATION",
		};
		ReportData report{ baseReport };
		bool result = hresult == S_OK;

		if ( !result )
		{
			std::stringstream stream;
			stream.imbue( std::locale{ "C" } );
			stream << "Error calling [" << message << "]: " << std::hex << hresult;
			auto it = getErrors().find( hresult );
			VkResult ret{ VK_ERROR_INITIALIZATION_FAILED };

			if ( it != getErrors().end() )
			{
				stream << "\n    Direct3D: " << it->second.name;
				ret = it->second.result;
			}

			stream << "\n    Windows: " << getLastErrorText();
			report.messageCode = hresult;
			dr::check( m_callback
				, *this
				, std::move( report )
				, false
				, stream.str() );
		}

		return !result;
	}

#	if VK_EXT_debug_utils

	void DebugReportLayer::onSubmitDebugUtilsMessenger( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity
		, VkDebugUtilsMessageTypeFlagsEXT messageTypes
		, VkDebugUtilsMessengerCallbackDataEXT const & callbackData )const
	{
		for ( uint32_t i = 0u; i < callbackData.objectCount; ++i )
		{
			auto & object = callbackData.pObjects[i];
			m_callback.report( ReportData
				{
					dudr::getDebugUtilsSeverity( messageSeverity ),
					dudr::getObjectType( object.objectType ),
					object.objectHandle,
					0u,
					callbackData.messageIdNumber,
					callbackData.pMessageIdName,
					callbackData.pMessage,
				} );
		}
	}

#	endif

	void DebugReportLayer::onReportMessage( VkDebugReportFlagsEXT flags
		, VkDebugReportObjectTypeEXT objectType
		, uint64_t object
		, size_t location
		, int32_t messageCode
		, const char * pLayerPrefix
		, const char * pMessage )const
	{
		m_callback.report( ReportData
		{
			flags,
			objectType,
			object,
			location,
			messageCode,
			pLayerPrefix,
			pMessage,
		} );
	}

	//*************************************************************************

	DebugReportCallbackEXT::DebugReportCallbackEXT( VkInstance instance
		, VkDebugReportCallbackCreateInfoEXT createInfo )
		: m_instance{ instance }
		, m_createInfo{ std::move( createInfo ) }
		, m_layer{ *this }
	{
		get( m_instance )->registerLayer( &m_layer );
	}

	DebugReportCallbackEXT::~DebugReportCallbackEXT()
	{
		get( m_instance )->unregisterLayer( &m_layer );
	}

	bool DebugReportCallbackEXT::report( ReportData report )
	{
		if ( report.flags & m_createInfo.flags )
		{
			return m_createInfo.pfnCallback( report.flags
				, report.objectType
				, report.object
				, report.location
				, report.messageCode
				, report.layerPrefix.c_str()
				, report.message.c_str()
				, m_createInfo.pUserData ) != 0;
		}

		return false;
	}

#endif

	//*************************************************************************
}

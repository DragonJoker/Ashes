/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include <Utils/UtilsDebug.hpp>

#include <Ashes/Core/DebugReportCallback.hpp>
#include <Ashes/Core/Instance.hpp>
#include <Ashes/Miscellaneous/DebugReportCallbackCreateInfo.hpp>

#if !defined( NDEBUG )
#	define LOAD_VALIDATION_LAYERS 1
#else
#	define LOAD_VALIDATION_LAYERS 1
#endif

namespace utils
{
	namespace
	{
		static char const * const VK_EXT_DEBUG_REPORT_EXTENSION_NAME = "VK_EXT_debug_report";

		bool ASHES_API debugReportCallback( ashes::DebugReportFlags flags
			, ashes::DebugReportObjectType objectType
			, uint64_t object
			, size_t location
			, int32_t messageCode
			, const char * pLayerPrefix
			, const char * pMessage
			, void* pUserData )
		{
			// Select prefix depending on flags passed to the callback
			// Note that multiple flags may be set for a single validation message
			std::locale loc{ "C" };
			std::stringstream stream;
			stream.imbue( loc );
			stream << "Vulkan ";

			// Error that may result in undefined behaviour
			if ( ashes::checkFlag( flags, ashes::DebugReportFlag::eError ) )
			{
				stream << "Error:\n";
			};
			// Warnings may hint at unexpected / non-spec API usage
			if ( ashes::checkFlag( flags, ashes::DebugReportFlag::eWarning ) )
			{
				stream << "Warning:\n";
			};
			// May indicate sub-optimal usage of the API
			if ( ashes::checkFlag( flags, ashes::DebugReportFlag::ePerformanceWarning ) )
			{
				stream << "Performance:\n";
			};
			// Informal messages that may become handy during debugging
			if ( ashes::checkFlag( flags, ashes::DebugReportFlag::eInformation ) )
			{
				stream << "Info:\n";
			}
			// Diagnostic info from the Vulkan loader and layers
			// Usually not helpful in terms of API usage, but may help to debug layer and loader problems 
			if ( ashes::checkFlag( flags, ashes::DebugReportFlag::eDebug ) )
			{
				stream << "Debug:\n";
			}

			// Display message to default output (console/logcat)
			stream << "    Layer: " << pLayerPrefix << "\n";
			stream << "    Code: 0x" << std::hex << messageCode << "\n";
			stream << "    Object: (" << std::hex << object << ") " << getName( objectType ) << "\n";
			stream << "    Message: " << pMessage;

			auto result = false;

#if ASHES_ANDROID

			if ( ashes::checkFlag( flags, ashes::DebugReportFlag::eError ) )
			{
				LOGE( "%s", stream.str().c_str() );
			}
			else
			{
				LOGD( "%s", stream.str().c_str() );
			}

			fflush( stdout );

#else

			if ( ashes::checkFlag( flags, ashes::DebugReportFlag::eError ) )
			{
				ashes::Logger::logError( stream );
			}
			else if ( ashes::checkFlag( flags, ashes::DebugReportFlag::eWarning )
				|| ashes::checkFlag( flags, ashes::DebugReportFlag::ePerformanceWarning ) )
			{
				ashes::Logger::logWarning( stream );
			}
			else if ( ashes::checkFlag( flags, ashes::DebugReportFlag::eInformation ) )
			{
				ashes::Logger::logTrace( stream );
			}
			else
			{
				ashes::Logger::logDebug( stream );
			}

#endif

			// The return value of this callback controls wether the Vulkan call that caused
			// the validation message will be aborted or not
			// Return VK_FALSE if we DON'T want Vulkan calls that cause a validation message 
			// (and return a VkResult) to abort
			// Return VK_TRUE if you instead want to have calls abort, and the function will 
			// return VK_ERROR_VALIDATION_FAILED_EXT 
			return result;
		}
	}

	ashes::DebugReportCallbackPtr setupDebugging( ashes::Instance const & instance
		, void * userData )
	{
#if LOAD_VALIDATION_LAYERS
		// The report flags determine what type of messages for the layers will be displayed
		// For validating (debugging) an appplication the error and warning bits should suffice
		ashes::DebugReportFlags debugReportFlags = 0u
			/*| DebugReportFlag::eInformation*/
			| ashes::DebugReportFlag::eWarning
			| ashes::DebugReportFlag::ePerformanceWarning
			| ashes::DebugReportFlag::eError
			| ashes::DebugReportFlag::eDebug;

		ashes::DebugReportCallbackCreateInfo dbgCreateInfo
		{
			debugReportFlags,
			debugReportCallback,
			userData
		};

		return instance.createDebugReportCallback( dbgCreateInfo );
#else

		return nullptr;

#endif
	}
}

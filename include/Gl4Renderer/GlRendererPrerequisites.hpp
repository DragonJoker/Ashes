/**
*\file
*	RenderLibPrerequisites.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include <ashes/ashes.h>

#include <cassert>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Gl4Renderer/Miscellaneous/OpenGLDefines.hpp"
#include "Gl4Renderer/Miscellaneous/GlDebug.hpp"
#include "Gl4Renderer/Miscellaneous/GlCallLogger.hpp"

#include <AshesRenderer/AshesRendererPrerequisites.hpp>
#include <AshesRenderer/Util/ArrayView.hpp>
#include <AshesRenderer/Util/ConstantFormat.hpp>
#include <AshesRenderer/Util/Format.hpp>
#include <AshesRenderer/Util/FlagCombination.hpp>
#include <AshesRenderer/Util/Signal.hpp>

#define BufferOffset( n ) ( ( uint8_t * )nullptr + ( n ) )

#if defined( _WIN32 ) && !defined( Gl4Renderer_STATIC )
#	ifdef Gl4Renderer_EXPORTS
#		define Gl4Renderer_API __declspec( dllexport )
#	else
#		define Gl4Renderer_API __declspec( dllimport )
#	endif
#else
#	define Gl4Renderer_API
#endif

#if defined( __GNUG__ )
#	define ASHES_COMPILER_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#	if ASHES_COMPILER_VERSION < 40900
#		error "Unsupported version of GCC"
#	elif ASHES_COMPILER_VERSION < 70400
#		include <experimental/optional>
namespace ashes::gl4
{
	template< typename T >
	using Optional = std::experimental::optional< T >;
	using std::experimental::nullopt;
}
#	else
#		include <optional>
namespace ashes::gl4
{
	template< typename T >
	using Optional = std::optional< T >;
	using std::nullopt;
}
#	endif
#else
#	include <optional>
namespace ashes::gl4
{
	template< typename T >
	using Optional = std::optional< T >;
	using std::nullopt;
}
#	if defined( MemoryBarrier )
#		undef MemoryBarrier
#	endif
#endif

#ifndef NDEBUG
#	define declareDebugVariable( Type, Name, Value )\
	mutable Type Name{ Value }
#	define setDebugValue( Name, Value )\
	Name = Value;
#	define assertDebugValue( Name, Value )\
	assert( Name == Value )
#else
#	define declareDebugVariable( Type, Name, Value )
#	define setDebugValue( Name, Value )
#	define assertDebugValue( Name, Value )
#endif

namespace ashes::gl4
{
	inline constexpr uint32_t getMajor( uint32_t version )
	{
		return ( ( uint32_t )( version ) >> 22 );
	}

	inline constexpr uint32_t getMinor( uint32_t version )
	{
		return ( ( ( uint32_t )( version ) >> 12 ) & 0x3ff );
	}

	inline constexpr uint32_t getPatch( uint32_t version )
	{
		return ( ( uint32_t )( version ) & 0xfff );
	}

	inline constexpr uint32_t makeVersion( uint32_t major
		, uint32_t minor
		, uint32_t patch )
	{
		return ( ( ( major ) << 22 ) | ( ( minor ) << 12 ) | ( patch ) );
	}

	using StringArray = std::vector< std::string >;
	using UInt32Array = std::vector< uint32_t >;
	using UInt64Array = std::vector< uint64_t >;

	struct DebugReportCallbackData
	{
		VkDebugReportCallbackEXT debugReport;
		PFNGLDEBUGPROC callback;
		void * userParam;
	};
	
	struct DebugReportAMDCallbackData
	{
		VkDebugReportCallbackEXT debugReport;
		PFNGLDEBUGAMDPROC callback;
		void * userParam;
	};

	class Buffer;
	class BufferView;
	class CommandBase;
	class CommandBuffer;
	class CommandPool;
	class DebugReportCallbackEXT;
	class DescriptorPool;
	class DescriptorSet;
	class DescriptorSetLayout;
	class Device;
	class DeviceMemory;
	class Event;
	class Fence;
	class Framebuffer;
	class Image;
	class ImageView;
	class Instance;
	class PhysicalDevice;
	class Pipeline;
	class PipelineCache;
	class PipelineLayout;
	class QueryPool;
	class Queue;
	class RenderPass;
	class Sampler;
	class Semaphore;
	class ShaderModule;
	class SurfaceKHR;
	class SwapchainKHR;

	class SamplerYcbcrConversion;
	class DescriptorUpdateTemplate;
	class DisplayKHR;
	class DisplayModeKHR;
	class ObjectTableNVX;
	class IndirectCommandsLayoutNVX;
	class DebugUtilsMessengerEXT;
	class ValidationCacheEXT;

	class FrameBufferAttachment;
	class GeometryBuffers;
	class ShaderProgram;

	class Context;
	class ContextLock;
	using ContextPtr = std::unique_ptr< Context >;

	using CommandPtr = std::unique_ptr< CommandBase >;
	using CommandArray = std::vector< CommandPtr >;

	using GeometryBuffersRef = std::reference_wrapper< GeometryBuffers >;
	using GeometryBuffersPtr = std::unique_ptr< GeometryBuffers >;
	using GeometryBuffersRefArray = std::vector< GeometryBuffersRef >;

	struct AttachmentDescription
	{
		uint32_t index;
		std::reference_wrapper< VkAttachmentDescription const > attach;
	};

	using AttachmentDescriptionArray = std::vector< AttachmentDescription >;

	struct BufferObjectBinding
	{
		GLuint bo;
		uint64_t offset;
		VkBuffer buffer;
	};

	using VboBindings = std::map< uint32_t, BufferObjectBinding >;
	using IboBinding = Optional< BufferObjectBinding >;

	using BufferDestroyFunc = std::function< void( GLuint ) >;
	using BufferDestroySignal = Signal< BufferDestroyFunc >;
	using BufferDestroyConnection = SignalConnection< BufferDestroySignal >;

	uint32_t deduceMemoryType( uint32_t typeBits
		, VkMemoryPropertyFlags requirements );
}

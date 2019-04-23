/**
*\file
*	RenderLibPrerequisites.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include <ashes/ashes.h>

#include "renderer/Gl4Renderer/Miscellaneous/GlDebug.hpp"
#include "renderer/Gl4Renderer/Miscellaneous/OpenGLDefines.hpp"

#include <renderer/RendererCommon/AshesRendererPrerequisites.hpp>
#include <renderer/RendererCommon/Helper/ConstantFormat.hpp>

#include <common/ArrayView.hpp>
#include <common/Format.hpp>
#include <common/FlagCombination.hpp>
#include <common/Signal.hpp>

#include <cassert>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#if defined( _WIN32 ) && !defined( Gl4Renderer_STATIC )
#	ifdef Gl4Renderer_EXPORTS
#		define Gl4Renderer_API __declspec( dllexport )
#	else
#		define Gl4Renderer_API __declspec( dllimport )
#	endif
#else
#	define Gl4Renderer_API
#endif

namespace ashes::gl4
{
	PFN_vkVoidFunction getFunction( char const * const name );

	template< typename FuncT >
	inline bool getFunction( char const * const name, FuncT & function )
	{
		function = FuncT( getFunction( name ) );
		return function != nullptr;
	}

	template< typename FuncT >
	inline bool getFunction( std::string const & name, FuncT & function )
	{
		function = FuncT( getFunction( name.c_str() ) );
		return function != nullptr;
	}

	inline void * getBufferOffset( intptr_t value )
	{
		return reinterpret_cast< void * >( reinterpret_cast< uint8_t * >( 0u ) + value );
	}

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
	struct ContextState;
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

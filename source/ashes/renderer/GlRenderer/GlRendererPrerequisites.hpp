/**
*\file
*	RenderLibPrerequisites.h
*\author
*	Sylvain Doremus
*/
#pragma once

#ifndef VK_NO_PROTOTYPES
#	define VK_NO_PROTOTYPES
#endif

#include <ashes/ashes.h>

#include <vulkan/vk_icd.h>

#include "renderer/GlRenderer/Core/GlAutoIdIcdObject.hpp"
#include "renderer/GlRenderer/Miscellaneous/GlDebug.hpp"
#include "renderer/GlRenderer/Miscellaneous/OpenGLDefines.hpp"
#include "renderer/GlRenderer/Core/GlContextState.hpp"

#include <renderer/RendererCommon/AshesRendererPrerequisites.hpp>
#include <renderer/RendererCommon/Helper/ConstantFormat.hpp>

#include <ashes/common/Format.hpp>

#include <cassert>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#if defined( _WIN32 ) && !defined( GlRenderer_STATIC )
#	ifdef GlRenderer_EXPORTS
#		define GlRenderer_API __declspec( dllexport )
#	else
#		define GlRenderer_API __declspec( dllimport )
#	endif
#else
#	define GlRenderer_API
#endif

#define Ashes_GlRemoveExtensions 0

namespace ashes::gl
{
#if VK_EXT_debug_utils

	struct DebugUtilsMessengerData
	{
		VkDebugUtilsMessengerEXT debugMessenger;
		PFNGLDEBUGPROC callback;
		void * userParam;
	};

	struct DebugUtilsAMDMessengerData
	{
		VkDebugUtilsMessengerEXT debugMessenger;
		PFNGLDEBUGAMDPROC callback;
		void * userParam;
	};

#endif
#if VK_EXT_debug_report

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

#endif

	class ExtensionNotPresentException
		: public ashes::Exception
	{
	public:
		ExtensionNotPresentException( std::string const & name )
			: Exception{ VK_ERROR_EXTENSION_NOT_PRESENT, name }
		{
		}
	};

	class Buffer;
	class BufferView;
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
	class RenderWindow;
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

	struct ContextState;

	class CommandBase;
	class Context;
	class ContextImpl;
	class ContextLock;
	class ContextStateStack;
	class ExtensionsHandler;
	class FrameBufferAttachment;
	class GeometryBuffers;
	class ShaderProgram;

	using ContextPtr = std::unique_ptr< Context >;
	using CommandPtr = std::unique_ptr< CommandBase >;
	using ContextImplPtr = std::unique_ptr< ContextImpl >;

	using CommandArray = std::vector< CommandPtr >;
	using ContextStateArray = std::vector< ContextState >;

	using ShaderProgramPtr = std::unique_ptr< ShaderProgram >;
	
	using GeometryBuffersRef = std::reference_wrapper< GeometryBuffers >;
	using GeometryBuffersPtr = std::unique_ptr< GeometryBuffers >;
	using GeometryBuffersRefArray = std::vector< GeometryBuffersRef >;

	struct GlPhysicalDeviceFeatures
	{
		VkBool32 has420PackExtensions;
		VkBool32 hasCopyImage;
		VkBool32 hasImmutableStorage;
		VkBool32 hasProgramPipelines;
		VkBool32 hasTextureStorage;
		VkBool32 hasTextureViews;
		VkBool32 hasViewportArrays;
	};

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

	struct FboAttachment
	{
		uint32_t referenceIndex;
		GlAttachmentPoint point;
		GLuint object;
		GlAttachmentType type;
		GlTextureType target;
		uint32_t mipLevel;
		uint32_t index;
		uint32_t baseArrayLayer;
		uint32_t imgLayerCount;
		uint32_t viewLayerCount;
		GLuint originalObject;
		GLuint originalMipLevel;
	};

	using FboAttachmentArray = std::vector< FboAttachment >;

	using DeviceMemoryDestroyFunc = std::function< void( GLuint ) >;
	using DeviceMemoryDestroySignal = Signal< DeviceMemoryDestroyFunc >;
	using DeviceMemoryDestroyConnection = SignalConnection< DeviceMemoryDestroySignal >;

	using CmdBuffer = UInt32Array;
	using CmdList = std::vector< CmdBuffer >;
	using PreExecuteAction = std::function< void( CmdList &, ContextStateStack const & ) >;
	using PreExecuteActions = std::vector< PreExecuteAction >;

	PFN_vkVoidFunction getFunction( char const * const name );

	namespace details
	{
		template< typename FuncT >
		inline bool getFunctionRec( std::string const & name
			, FuncT & function )
		{
			function = FuncT( getFunction( name.c_str() ) );
			return function != nullptr;
		}

		template< typename FuncT, typename ParamT >
		inline bool getFunctionRec( std::string const & name
			, FuncT & function
			, char const * const lastShort
			, VkExtensionProperties const & lastExtension )
		{
			function = FuncT( getFunction( ( name + lastShort ).c_str() ) );
			return function != nullptr;
		}

		template< typename FuncT, typename ... ParamsT >
		inline bool getFunctionRec( std::string const & name
			, FuncT & function
			, char const * const currentShort
			, VkExtensionProperties const & currentExtension
			, ParamsT ... params )
		{
			function = FuncT( getFunction( ( name + currentShort ).c_str() ) );

			if ( !function )
			{
				return getFunctionRec( name, function, params... );
			}

			return function != nullptr;
		}
	}

	template< typename FuncT, typename ... ParamsT >
	inline bool getFunction( char const * const name
		, FuncT & function
		, ParamsT ... params )
	{
		return details::getFunctionRec( std::string{ name ? name : "" }, function, params... );
	}

	template< typename FuncT, typename ... ParamsT >
	inline bool getFunction( std::string const & name
		, FuncT & function
		, ParamsT ... params )
	{
		return details::getFunctionRec( name, function, params... );
	}

	inline void * getBufferOffset( intptr_t value )
	{
		return reinterpret_cast< void * >( reinterpret_cast< uint8_t * >( 0u ) + value );
	}

	uint32_t deduceMemoryType( VkDevice device
		, uint32_t typeBits
		, VkMemoryPropertyFlags requirements );
	bool areCompatible( VkCommandBuffer cmd
		, VkPipelineStageFlags pipelineFlags
		, VkAccessFlags accessFlags );
	uint32_t getScreenIndex( VkDisplayModeKHR displayMode );
	VkDisplayModeParametersKHR getDisplayModeParameters( VkDisplayModeKHR displayMode );
	VkExtent2D getDisplayResolution( VkDisplayModeKHR displayMode );
}

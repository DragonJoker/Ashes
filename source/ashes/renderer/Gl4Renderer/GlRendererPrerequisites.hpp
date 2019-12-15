/**
*\file
*	RenderLibPrerequisites.h
*\author
*	Sylvain Doremus
*/
#pragma once

#define VK_NO_PROTOTYPES
#include <ashes/ashes.h>

#include <vulkan/vk_icd.h>

#include "renderer/Gl4Renderer/Miscellaneous/GlDebug.hpp"
#include "renderer/Gl4Renderer/Miscellaneous/OpenGLDefines.hpp"

#include <renderer/RendererCommon/AshesRendererPrerequisites.hpp>
#include <renderer/RendererCommon/Helper/ConstantFormat.hpp>

#include <renderer/GlRendererCommon/GlAutoIdIcdObject.hpp>

#include <renderer/GlRendererCommon/GlContextState.hpp>

#include <ashes/common/Format.hpp>

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
	using gl::ContextState;
	using gl::ExtensionsHandler;
	using gl::getFunction;
	using gl::getBufferOffset;
	using gl::IcdObject;
	using gl::AutoIdIcdObject;

#if VK_EXT_debug_utils

	struct DebugUtilsMessengerData
	{
		VkDebugUtilsMessengerEXT debugMessenger;
		PFNGLDEBUGPROC callback;
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

#endif

	struct ContextState;

	class Buffer;
	class BufferView;
	class CommandBase;
	class CommandBuffer;
	class CommandPool;
	class Context;
	class ContextLock;
	class ContextStateStack;
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

	struct FboAttachment
	{
		GlAttachmentPoint point;
		GLuint object;
		GlAttachmentType type;
		GlTextureType target;
		uint32_t mipLevel;
		uint32_t index;
		uint32_t layerCount;
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

	uint32_t deduceMemoryType( uint32_t typeBits
		, VkMemoryPropertyFlags requirements );
	bool areCompatible( VkCommandBuffer cmd
		, VkPipelineStageFlags pipelineFlags
		, VkAccessFlags accessFlags );
}

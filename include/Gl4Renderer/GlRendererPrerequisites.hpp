/**
*\file
*	RenderLibPrerequisites.h
*\author
*	Sylvain Doremus
*/
#pragma once

#define VK_NO_PROTOTYPES 1
#include "vulkan/vulkan.h"

#include <Ashes/AshesPrerequisites.hpp>

#include "Gl4Renderer/Miscellaneous/OpenGLDefines.hpp"

#include "Gl4Renderer/Enum/GlAccessFlag.hpp"
#include "Gl4Renderer/Enum/GlAttachmentPoint.hpp"
#include "Gl4Renderer/Enum/GlAttachmentType.hpp"
#include "Gl4Renderer/Enum/GlBaseType.hpp"
#include "Gl4Renderer/Enum/GlBlendFactor.hpp"
#include "Gl4Renderer/Enum/GlBlendOp.hpp"
#include "Gl4Renderer/Enum/GlBorderColour.hpp"
#include "Gl4Renderer/Enum/GlBufferTarget.hpp"
#include "Gl4Renderer/Enum/GlClearTarget.hpp"
#include "Gl4Renderer/Enum/GlClipInfo.hpp"
#include "Gl4Renderer/Enum/GlColourComponentFlag.hpp"
#include "Gl4Renderer/Enum/GlCompareOp.hpp"
#include "Gl4Renderer/Enum/GlComponentSwizzle.hpp"
#include "Gl4Renderer/Enum/GlConstantFormat.hpp"
#include "Gl4Renderer/Enum/GlCullModeFlag.hpp"
#include "Gl4Renderer/Enum/GlDebugReportObjectType.hpp"
#include "Gl4Renderer/Enum/GlFenceWaitFlag.hpp"
#include "Gl4Renderer/Enum/GlFilter.hpp"
#include "Gl4Renderer/Enum/GlFormat.hpp"
#include "Gl4Renderer/Enum/GlFrameBufferTarget.hpp"
#include "Gl4Renderer/Enum/GlFrontFace.hpp"
#include "Gl4Renderer/Enum/GlGetParameter.hpp"
#include "Gl4Renderer/Enum/GlImageAspectFlag.hpp"
#include "Gl4Renderer/Enum/GlImageLayout.hpp"
#include "Gl4Renderer/Enum/GlImageTiling.hpp"
#include "Gl4Renderer/Enum/GlIndexType.hpp"
#include "Gl4Renderer/Enum/GlLogicOp.hpp"
#include "Gl4Renderer/Enum/GlMemoryBarrierFlag.hpp"
#include "Gl4Renderer/Enum/GlMemoryMapFlag.hpp"
#include "Gl4Renderer/Enum/GlMemoryPropertyFlag.hpp"
#include "Gl4Renderer/Enum/GlMipmapMode.hpp"
#include "Gl4Renderer/Enum/GlPolygonMode.hpp"
#include "Gl4Renderer/Enum/GlPrimitiveTopology.hpp"
#include "Gl4Renderer/Enum/GlQueryResultFlag.hpp"
#include "Gl4Renderer/Enum/GlQueryType.hpp"
#include "Gl4Renderer/Enum/GlSampleCountFlag.hpp"
#include "Gl4Renderer/Enum/GlSamplerParameter.hpp"
#include "Gl4Renderer/Enum/GlShaderBinaryFormat.hpp"
#include "Gl4Renderer/Enum/GlShaderInfo.hpp"
#include "Gl4Renderer/Enum/GlShaderStageFlag.hpp"
#include "Gl4Renderer/Enum/GlStencilOp.hpp"
#include "Gl4Renderer/Enum/GlTexLevelParameter.hpp"
#include "Gl4Renderer/Enum/GlTexParameter.hpp"
#include "Gl4Renderer/Enum/GlTextureType.hpp"
#include "Gl4Renderer/Enum/GlTextureViewType.hpp"
#include "Gl4Renderer/Enum/GlTextureUnit.hpp"
#include "Gl4Renderer/Enum/GlTweak.hpp"
#include "Gl4Renderer/Enum/GlWrapMode.hpp"

#include "Gl4Renderer/Miscellaneous/GlCallLogger.hpp"
#include "Gl4Renderer/Pipeline/GlSpecialisationInfo.hpp"

#include "Gl4Renderer/Miscellaneous/GlDebug.hpp"

#define BufferOffset( n ) ( ( uint8_t * )nullptr + ( n ) )

#if defined( _WIN32 ) && !defined( Gl3Renderer_STATIC )
#	ifdef Gl4Renderer_EXPORTS
#		define Gl4Renderer_API __declspec( dllexport )
#	else
#		define Gl4Renderer_API __declspec( dllimport )
#	endif
#else
#	define Gl4Renderer_API
#endif

namespace gl_renderer
{
	struct DebugReportCallbackData
	{
		PFNGLDEBUGPROC callback;
		void * userParam;
	};
	
	struct DebugReportAMDCallbackData
	{
		PFNGLDEBUGAMDPROC callback;
		void * userParam;
	};

	struct AttachmentDescription;

	class Buffer;
	class BufferView;
	class CommandBase;
	class CommandBuffer;
	class ComputePipeline;
	class Context;
	class ContextLock;
	class DebugReportCallback;
	class DescriptorSet;
	class Device;
	class DeviceMemory;
	class FrameBuffer;
	class GeometryBuffers;
	class Image;
	class ImageView;
	class Instance;
	class PhysicalDevice;
	class Pipeline;
	class PipelineLayout;
	class QueryPool;
	class Queue;
	class RenderPass;
	class ShaderModule;
	class ShaderProgram;
	class Surface;
	class UniformBuffer;
	class WindowHandle;

	using ContextPtr = std::unique_ptr< Context >;
	using CommandPtr = std::unique_ptr< CommandBase >;
	using DebugReportCallbackPtr = std::unique_ptr< DebugReportCallback >;
	using DevicePtr = std::unique_ptr< Device >;
	using GeometryBuffersPtr = std::unique_ptr< GeometryBuffers >;
	using ImageViewPtr = std::unique_ptr< ImageView >;
	using PhysicalDevicePtr = std::unique_ptr< PhysicalDevice >;
	using QueuePtr = std::unique_ptr< Queue >;
	using SurfacePtr = std::unique_ptr< Surface >;

	using DeviceMemoryPtr = std::shared_ptr< DeviceMemory >;

	using GeometryBuffersRef = std::reference_wrapper< GeometryBuffers >;

	using ShaderModuleCRef = std::reference_wrapper< ShaderModule const >;

	using GeometryBuffersRefArray = std::vector< GeometryBuffersRef >;

	using ShaderModuleCRefArray = std::vector< ShaderModuleCRef >;

	using PhysicalDevicePtrArray = std::vector< PhysicalDevicePtr >;

	using CommandArray = std::vector< CommandPtr >;
	using AttachmentDescriptionArray = std::vector< AttachmentDescription >;
	using ViewportArray = std::vector< VkViewport >;
	using ScissorArray = std::vector< VkRect2D >;

	struct BufferObjectBinding
	{
		GLuint bo;
		uint64_t offset;
		Buffer const * buffer;
	};

	using VboBindings = std::map< uint32_t, BufferObjectBinding >;
	using IboBinding = ashes::Optional< BufferObjectBinding >;

	using BufferDestroyFunc = std::function< void( GLuint ) >;
	using BufferDestroySignal = ashes::Signal< BufferDestroyFunc >;
	using BufferDestroyConnection = ashes::SignalConnection< BufferDestroySignal >;

	uint32_t deduceMemoryType( uint32_t typeBits
		, ashes::MemoryPropertyFlags requirements );

	inline uint32_t checkFlag( uint32_t a, uint32_t b )
	{
		return ( a & b ) == b;
	}
}

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct AshRendererFeatures
	{
		bool hasTexBufferRange;
		bool hasImageTexture;
		bool hasBaseInstance;
		bool hasClearTexImage;
		bool hasComputeShaders;
		bool hasStorageBuffers;
		bool supportsPersistentMapping;
	} AshRendererFeatures;

#ifdef __cplusplus
}
#endif

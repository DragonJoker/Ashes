/**
*\file
*	RenderLibPrerequisites.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include <Ashes/AshesPrerequisites.hpp>

#include "Gl3Renderer/Miscellaneous/OpenGLDefines.hpp"

#include "Gl3Renderer/Enum/GlAccessFlag.hpp"
#include "Gl3Renderer/Enum/GlAttachmentPoint.hpp"
#include "Gl3Renderer/Enum/GlAttachmentType.hpp"
#include "Gl3Renderer/Enum/GlBaseType.hpp"
#include "Gl3Renderer/Enum/GlBlendFactor.hpp"
#include "Gl3Renderer/Enum/GlBlendOp.hpp"
#include "Gl3Renderer/Enum/GlBorderColour.hpp"
#include "Gl3Renderer/Enum/GlBufferTarget.hpp"
#include "Gl3Renderer/Enum/GlClearTarget.hpp"
#include "Gl3Renderer/Enum/GlClipInfo.hpp"
#include "Gl3Renderer/Enum/GlColourComponentFlag.hpp"
#include "Gl3Renderer/Enum/GlCompareOp.hpp"
#include "Gl3Renderer/Enum/GlComponentSwizzle.hpp"
#include "Gl3Renderer/Enum/GlConstantFormat.hpp"
#include "Gl3Renderer/Enum/GlCullModeFlag.hpp"
#include "Gl3Renderer/Enum/GlDebugReportObjectType.hpp"
#include "Gl3Renderer/Enum/GlFenceWaitFlag.hpp"
#include "Gl3Renderer/Enum/GlFilter.hpp"
#include "Gl3Renderer/Enum/GlFormat.hpp"
#include "Gl3Renderer/Enum/GlFrameBufferTarget.hpp"
#include "Gl3Renderer/Enum/GlFrontFace.hpp"
#include "Gl3Renderer/Enum/GlGetParameter.hpp"
#include "Gl3Renderer/Enum/GlImageAspectFlag.hpp"
#include "Gl3Renderer/Enum/GlImageLayout.hpp"
#include "Gl3Renderer/Enum/GlImageTiling.hpp"
#include "Gl3Renderer/Enum/GlIndexType.hpp"
#include "Gl3Renderer/Enum/GlLogicOp.hpp"
#include "Gl3Renderer/Enum/GlMemoryBarrierFlag.hpp"
#include "Gl3Renderer/Enum/GlMemoryMapFlag.hpp"
#include "Gl3Renderer/Enum/GlMemoryPropertyFlag.hpp"
#include "Gl3Renderer/Enum/GlMipmapMode.hpp"
#include "Gl3Renderer/Enum/GlPolygonMode.hpp"
#include "Gl3Renderer/Enum/GlPrimitiveTopology.hpp"
#include "Gl3Renderer/Enum/GlQueryResultFlag.hpp"
#include "Gl3Renderer/Enum/GlQueryType.hpp"
#include "Gl3Renderer/Enum/GlSampleCountFlag.hpp"
#include "Gl3Renderer/Enum/GlSamplerParameter.hpp"
#include "Gl3Renderer/Enum/GlShaderBinaryFormat.hpp"
#include "Gl3Renderer/Enum/GlShaderInfo.hpp"
#include "Gl3Renderer/Enum/GlShaderStageFlag.hpp"
#include "Gl3Renderer/Enum/GlStencilOp.hpp"
#include "Gl3Renderer/Enum/GlTexLevelParameter.hpp"
#include "Gl3Renderer/Enum/GlTexParameter.hpp"
#include "Gl3Renderer/Enum/GlTextureType.hpp"
#include "Gl3Renderer/Enum/GlTextureViewType.hpp"
#include "Gl3Renderer/Enum/GlTextureUnit.hpp"
#include "Gl3Renderer/Enum/GlTweak.hpp"
#include "Gl3Renderer/Enum/GlWrapMode.hpp"

#include "Gl3Renderer/Miscellaneous/GlCallLogger.hpp"
#include "Gl3Renderer/Pipeline/GlSpecialisationInfo.hpp"

#include "Gl3Renderer/Miscellaneous/GlDebug.hpp"

#define BufferOffset( n ) ( ( uint8_t * )nullptr + ( n ) )

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
	class ComputePipeline;
	class Context;
	class ContextLock;
	class DescriptorSet;
	class Device;
	class FrameBuffer;
	class GeometryBuffers;
	class PhysicalDevice;
	class Pipeline;
	class PipelineLayout;
	class QueryPool;
	class Queue;
	class Instance;
	class RenderPass;
	class ShaderModule;
	class ShaderProgram;
	class Texture;
	class TextureView;

	using ContextPtr = std::unique_ptr< Context >;
	using CommandPtr = std::unique_ptr< CommandBase >;
	using GeometryBuffersPtr = std::unique_ptr< GeometryBuffers >;
	using QueuePtr = std::unique_ptr< Queue >;
	using TextureViewPtr = std::unique_ptr< TextureView >;

	using GeometryBuffersRef = std::reference_wrapper< GeometryBuffers >;

	using ShaderModuleCRef = std::reference_wrapper< ShaderModule const >;

	using GeometryBuffersRefArray = std::vector< GeometryBuffersRef >;

	using ShaderModuleCRefArray = std::vector< ShaderModuleCRef >;

	using CommandArray = std::vector< CommandPtr >;
	using AttachmentDescriptionArray = std::vector< AttachmentDescription >;

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
}

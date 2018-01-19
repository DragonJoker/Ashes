/**
*\file
*	RenderLibPrerequisites.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "GL/glew.h"

#include <RendererPrerequisites.hpp>

#include "GlAccessFlag.hpp"
#include "GlAttributeFormat.hpp"
#include "GlBlendFactor.hpp"
#include "GlBlendOp.hpp"
#include "GlBorderColour.hpp"
#include "GlBufferTarget.hpp"
#include "GlColourComponentFlag.hpp"
#include "GlCompareOp.hpp"
#include "GlCullModeFlag.hpp"
#include "GlFilter.hpp"
#include "GlFrontFace.hpp"
#include "GlImageAspectFlag.hpp"
#include "GlImageLayout.hpp"
#include "GlImageTiling.hpp"
#include "GlIndexType.hpp"
#include "GlLogicOp.hpp"
#include "GlMemoryMapFlag.hpp"
#include "GlMipmapMode.hpp"
#include "GlPixelFormat.hpp"
#include "GlPolygonMode.hpp"
#include "GlPrimitiveTopology.hpp"
#include "GlQueryResultFlag.hpp"
#include "GlQueryType.hpp"
#include "GlSampleCountFlag.hpp"
#include "GlShaderStageFlag.hpp"
#include "GlStencilOp.hpp"
#include "GlTextureType.hpp"
#include "GlWrapMode.hpp"

namespace gl_renderer
{
	class Buffer;
	class BufferView;
	class CommandBase;
	class Context;
	class DescriptorSet;
	class Device;
	class FrameBuffer;
	class GeometryBuffers;
	class Pipeline;
	class PipelineLayout;
	class QueryPool;
	class RenderPass;
	class ShaderProgram;
	class Texture;
	class TextureView;

	using ContextPtr = std::unique_ptr< Context >;
	using CommandPtr = std::unique_ptr< CommandBase >;

	using CommandArray = std::vector< CommandPtr >;
}

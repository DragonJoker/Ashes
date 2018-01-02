/**
*\file
*	RenderLibPrerequisites.h
*\author
*	Sylvain Doremus
*/
#pragma once

#define GLEW_STATIC 1
#include <GL/glew.h>

#include "GlAccessFlag.hpp"
#include "GlAttributeFormat.hpp"
#include "GlBlendFactor.hpp"
#include "GlBlendOp.hpp"
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
#include "GlSampleCountFlag.hpp"
#include "GlShaderStageFlag.hpp"
#include "GlStencilOp.hpp"
#include "GlWrapMode.hpp"

#include <Renderer/RendererPrerequisites.hpp>

namespace gl_renderer
{
	class CommandBase;
	class Context;
	class FrameBuffer;
	class Pipeline;
	class RenderPass;
	class Texture;

	using ContextPtr = std::unique_ptr< Context >;
	using CommandPtr = std::unique_ptr< CommandBase >;

	using CommandArray = std::vector< CommandPtr >;
}

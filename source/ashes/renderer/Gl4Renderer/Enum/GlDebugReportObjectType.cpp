#include "GlRendererPrerequisites.hpp"

namespace ashes::gl4
{
	GlDebugReportObjectType convert( VkDebugReportObjectTypeEXT const & value )
	{
		GlDebugReportObjectType result = GlDebugReportObjectType::eUnknown;

		switch ( value )
		{
		case VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT:
			result = GlDebugReportObjectType::eBuffer;
			break;
		case VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT:
			result = GlDebugReportObjectType::eTexture;
			break;
		case VK_DEBUG_REPORT_OBJECT_TYPE_QUERY_POOL_EXT:
			result = GlDebugReportObjectType::eQuery;
			break;
		case VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT:
			result = GlDebugReportObjectType::eShaderModule;
			break;
		case VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_EXT:
			result = GlDebugReportObjectType::eSampler;
			break;
		case VK_DEBUG_REPORT_OBJECT_TYPE_FRAMEBUFFER_EXT:
			result = GlDebugReportObjectType::eFrameBuffer;
			break;
		}

		return result;
	}

	GlDebugReportObjectType convert( VkObjectType const & value )
	{
		GlDebugReportObjectType result = GlDebugReportObjectType::eUnknown;

		switch ( value )
		{
		case VK_OBJECT_TYPE_BUFFER:
			result = GlDebugReportObjectType::eBuffer;
			break;
		case VK_OBJECT_TYPE_IMAGE:
			result = GlDebugReportObjectType::eTexture;
			break;
		case VK_OBJECT_TYPE_QUERY_POOL:
			result = GlDebugReportObjectType::eQuery;
			break;
		case VK_OBJECT_TYPE_SHADER_MODULE:
			result = GlDebugReportObjectType::eShaderModule;
			break;
		case VK_OBJECT_TYPE_SAMPLER:
			result = GlDebugReportObjectType::eSampler;
			break;
		case VK_OBJECT_TYPE_FRAMEBUFFER:
			result = GlDebugReportObjectType::eFrameBuffer;
			break;
		}

		return result;
	}
}

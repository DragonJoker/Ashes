#include "GlDebugReportObjectType.hpp"

namespace gl_renderer
{
	GlDebugReportObjectType convert( ashes::DebugReportObjectType const & value )
	{
		GlDebugReportObjectType result = GlDebugReportObjectType::eUnknown;

		switch ( value )
		{
		case ashes::DebugReportObjectType::eBuffer:
			result = GlDebugReportObjectType::eBuffer;
			break;
		case ashes::DebugReportObjectType::eImage:
			result = GlDebugReportObjectType::eTexture;
			break;
		case ashes::DebugReportObjectType::eQueryPool:
			result = GlDebugReportObjectType::eQuery;
			break;
		case ashes::DebugReportObjectType::eShaderModule:
			result = GlDebugReportObjectType::eShaderModule;
			break;
		case ashes::DebugReportObjectType::eSampler:
			result = GlDebugReportObjectType::eSampler;
			break;
		case ashes::DebugReportObjectType::eFramebuffer:
			result = GlDebugReportObjectType::eFrameBuffer;
			break;
		}

		return result;
	}
}

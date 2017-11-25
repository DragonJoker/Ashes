#include "Renderer.hpp"

#include "RenderingResources.hpp"

#include "Connection.hpp"

#include <VkLib/PhysicalDevice.hpp>
#include <VkLib/LogicalDevice.hpp>
#include <VkLib/SwapChain.hpp>

# if RENDERLIB_GLSL_TO_SPV
#	include <glslang/Public/ShaderLang.h>
#endif

namespace renderer
{
	Renderer::Renderer()
	{
# if RENDERLIB_GLSL_TO_SPV

		glslang::InitializeProcess();

#endif
	}

	Renderer::~Renderer()
	{
# if RENDERLIB_GLSL_TO_SPV

		glslang::FinalizeProcess();

#endif
	}

	DevicePtr Renderer::createDevice( Connection && connection )const
	{
		DevicePtr result;

		try
		{
			result = std::make_unique< Device >( *this, std::move( connection ) );
		}
		catch ( std::exception & exc )
		{
			std::cerr << "Could not initialise logical device:\n" << exc.what() << std::endl;
		}
		catch ( ... )
		{
			std::cerr << "Could not initialise logical device:\nUnknown error" << std::endl;
		}

		return result;
	}
}

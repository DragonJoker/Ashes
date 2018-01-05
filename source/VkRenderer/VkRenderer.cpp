#include "VkRenderer.hpp"

#include "VkConnection.hpp"
#include "VkDevice.hpp"

#include <VkLib/PhysicalDevice.hpp>
#include <VkLib/LogicalDevice.hpp>
#include <VkLib/SwapChain.hpp>

# if VKRENDERER_GLSL_TO_SPV
#	include <glslang/Public/ShaderLang.h>
#endif

namespace vk_renderer
{
	Renderer::Renderer()
		: renderer::Renderer{}
	{
# if VKRENDERER_GLSL_TO_SPV

		glslang::InitializeProcess();

#endif
	}

	Renderer::~Renderer()
	{
# if VKRENDERER_GLSL_TO_SPV

		glslang::FinalizeProcess();

#endif
	}

	renderer::DevicePtr Renderer::createDevice( renderer::ConnectionPtr && connection )const
	{
		renderer::DevicePtr result;

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

	renderer::ConnectionPtr Renderer::createConnection( uint32_t deviceIndex
		, renderer::WindowHandle && handle )const
	{
		return std::make_unique< Connection >( *this
			, deviceIndex
			, std::move( handle ) );
	}
}

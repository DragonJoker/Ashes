#include "Renderer.hpp"

#include "RenderingResources.hpp"

#include "Connection.hpp"

#include <VkLib/PhysicalDevice.hpp>
#include <VkLib/LogicalDevice.hpp>
#include <VkLib/SwapChain.hpp>

#include <glslang/Public/ShaderLang.h>

namespace renderer
{
	Renderer::Renderer()
	{
		glslang::InitializeProcess();
	}

	Renderer::~Renderer()
	{
		glslang::FinalizeProcess();
	}

	DevicePtr Renderer::createDevice( Connection & connection )
	{
		DevicePtr result;

		try
		{
			result = std::make_unique< Device >( *this, connection );
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

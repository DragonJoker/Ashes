#include "RenderPanel.hpp"

#include "Application.hpp"
#include "MainFrame.hpp"

#include <Core/Surface.hpp>
#include <Core/Device.hpp>
#include <Core/Exception.hpp>

#include <Transform.hpp>

#include <chrono>

namespace vkapp
{
	RenderPanel::RenderPanel( wxWindow * parent
		, wxSize const & size
		, utils::Instance const & instance )
		: wxPanel{ parent, wxID_ANY, wxDefaultPosition, size }
	{
		try
		{
			m_surface = doCreateSurface( instance );
			std::cout << "Surface created." << std::endl;
			doCreateDevice( instance, *m_surface );
			std::cout << "Logical device created." << std::endl;
		}
		catch ( std::exception & )
		{
			doCleanup();
			throw;
		}
	}

	RenderPanel::~RenderPanel()
	{
		doCleanup();
	}

	void RenderPanel::doCleanup()
	{
		if ( m_device )
		{
			m_device->waitIdle();
			m_device.reset();
		}
	}

	ashes::SurfacePtr RenderPanel::doCreateSurface( utils::Instance const & instance )
	{
		auto handle = common::makeWindowHandle( *this );
		auto & gpu = instance.getPhysicalDevice( 0u );
		return instance.getInstance().createSurface( gpu
			, std::move( handle ) );
	}

	void RenderPanel::doCreateDevice( utils::Instance const & instance
		, ashes::Surface const & surface )
	{
		ashes::DeviceCreateInfo createInfo;
		createInfo.enabledFeatures = surface.getGpu().getFeatures();
		createInfo.enabledLayerNames = instance.getLayerNames();
		createInfo.enabledExtensionNames = instance.getExtensionNames();
		createInfo.queueCreateInfos.push_back( ashes::DeviceQueueCreateInfo
			{
				0u,
				0u,
				{ 1.0f },
			} );
		m_device = instance.getInstance().createDevice( surface.getGpu()
			, std::move( createInfo ) );

		if ( m_device )
		{
			std::cout << m_device->getPhysicalDevice().dumpProperties() << std::endl;
		}
	}
}

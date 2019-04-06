#include "RenderPanel.hpp"

#include "Application.hpp"
#include "MainFrame.hpp"

#include <AshesPP/Core/Surface.hpp>
#include <AshesPP/Core/Device.hpp>
#include <AshesRenderer/Util/Exception.hpp>

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
		ashes::DeviceQueueCreateInfoArray queueCreateInfos;
		queueCreateInfos.emplace_back( 0u, 0u, ashes::FloatArray{ 1.0f } );
		ashes::DeviceCreateInfo createInfo
		{
			0u,
			std::move( queueCreateInfos ),
			instance.getLayerNames(),
			instance.getExtensionNames(),
			surface.getGpu().getFeatures(),
		};
		m_device = instance.getInstance().createDevice( surface.getGpu()
			, std::move( createInfo ) );

		if ( m_device )
		{
			std::cout << m_device->getPhysicalDevice().dumpProperties() << std::endl;
		}
	}
}

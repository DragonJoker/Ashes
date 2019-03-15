#include "RenderPanel.hpp"

#include "Application.hpp"
#include "MainFrame.hpp"

#include <Core/Connection.hpp>
#include <Core/Device.hpp>
#include <Core/Exception.hpp>

#include <Transform.hpp>

#include <chrono>

namespace vkapp
{
	RenderPanel::RenderPanel( wxWindow * parent
		, wxSize const & size
		, ashes::Instance const & instance )
		: wxPanel{ parent, wxID_ANY, wxDefaultPosition, size }
	{
		try
		{
			auto surface = doCreateSurface( instance );
			std::cout << "Surface created." << std::endl;
			doCreateDevice( instance, std::move( surface ) );
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

	ashes::ConnectionPtr RenderPanel::doCreateSurface( ashes::Instance const & instance )
	{
		auto handle = common::makeWindowHandle( *this );
		auto & gpu = instance.getPhysicalDevice( 0u );
		return instance.createConnection( gpu
			, std::move( handle ) );
	}

	void RenderPanel::doCreateDevice( ashes::Instance const & instance
		, ashes::ConnectionPtr surface )
	{
		m_device = instance.createDevice( std::move( surface ), ~( 0u ), ~( 0u ) );
		std::cout << m_device->getPhysicalDevice().dumpProperties() << std::endl;
	}
}

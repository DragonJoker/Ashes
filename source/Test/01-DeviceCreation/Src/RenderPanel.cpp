#include "RenderPanel.hpp"

#include "Application.hpp"
#include "MainFrame.hpp"

#include <Renderer/Connection.hpp>
#include <Renderer/Device.hpp>

#include <Utils/Transform.hpp>

#include <chrono>

namespace vkapp
{
	RenderPanel::RenderPanel( wxWindow * parent
		, wxSize const & size
		, renderer::Renderer const & vulkan )
		: wxPanel{ parent, wxID_ANY, wxDefaultPosition, size }
	{
		try
		{
			doCreateDevice( vulkan );
			std::cout << "Logical device created." << std::endl;
		}
		catch ( std::exception & )
		{
			if ( m_device )
			{
				m_device->waitIdle();
			}

			m_device.reset();
			throw;
		}

		DEBUG_WRITE( AppName.ToStdString() + ".log" );
	}

	RenderPanel::~RenderPanel()
	{
		m_device.reset();
	}

	void RenderPanel::doCreateDevice( renderer::Renderer const & vulkan )
	{
		m_device = vulkan.createDevice( common::makeConnection( this, vulkan ) );
	}
}

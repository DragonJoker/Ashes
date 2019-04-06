#pragma once

#include "Prerequisites.hpp"

#include <AshesPP/Core/Device.hpp>

#include <AshesPP/Utils/Signal.hpp>

#include <wx/panel.h>

#include <array>

namespace vkapp
{
	class RenderPanel
		: public wxPanel
	{
	public:
		RenderPanel( wxWindow * parent
			, wxSize const & size
			, utils::Instance const & instance );
		~RenderPanel();

	private:
		void doCleanup();
		ashes::SurfacePtr doCreateSurface( utils::Instance const & instance );
		void doCreateDevice( utils::Instance const & instance
			, ashes::Surface const & surface );

	private:
		ashes::SurfacePtr m_surface;
		ashes::DevicePtr m_device;
	};
}

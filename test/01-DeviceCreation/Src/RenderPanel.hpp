#pragma once

#include "Prerequisites.hpp"

#include <Core/Device.hpp>

#include <Utils/Signal.hpp>

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
			, ashes::Instance const & instance );
		~RenderPanel();

	private:
		void doCleanup();
		ashes::SurfacePtr doCreateSurface( ashes::Instance const & instance );
		void doCreateDevice( ashes::Instance const & instance
			, ashes::SurfacePtr surface );

	private:
		ashes::DevicePtr m_device;
	};
}

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
			, ashes::Renderer const & renderer );
		~RenderPanel();

	private:
		void doCleanup();
		void doCreateDevice( ashes::Renderer const & renderer );

	private:
		ashes::DevicePtr m_device;
	};
}

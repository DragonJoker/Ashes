#pragma once

#include "Prerequisites.hpp"

#include <Core/Device.hpp>

#include <Utils/UtilsSignal.hpp>

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
			, renderer::Renderer const & renderer );
		~RenderPanel();

	private:
		void doCleanup();
		void doCreateDevice( renderer::Renderer const & renderer );

	private:
		renderer::DevicePtr m_device;
	};
}

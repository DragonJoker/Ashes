#pragma once

#include "Prerequisites.hpp"

#include <ashespp/Core/Device.hpp>

#include <ashes/common/Signal.hpp>

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
		~RenderPanel()noexcept override;

	private:
		void doCleanup()noexcept;
		void doCreateSurface( utils::Instance const & instance );
		void doCreateDevice( utils::Instance const & instance );

	private:
		ashes::SurfacePtr m_surface;
		ashes::DevicePtr m_device;
	};
}

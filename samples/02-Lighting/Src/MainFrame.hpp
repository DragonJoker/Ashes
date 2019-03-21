#pragma once

#include "Prerequisites.hpp"

#include <Ashes/Core/Instance.hpp>

#include <MainFrame.hpp>

namespace vkapp
{
	class MainFrame
		: public common::MainFrame
	{
	public:
		MainFrame( wxString const & rendererName
			, utils::InstanceFactory & factory );

	private:
		common::RenderPanel * doCreatePanel( wxSize const & size, utils::Instance const & instance )override;
	};
}

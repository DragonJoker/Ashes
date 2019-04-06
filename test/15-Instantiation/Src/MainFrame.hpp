#pragma once

#include "Prerequisites.hpp"

#include <AshesPP/Core/Instance.hpp>

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
		wxPanel * doCreatePanel( wxSize const & size, utils::Instance const & instance )override;
	};
}

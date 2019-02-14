#pragma once

#include "Prerequisites.hpp"

namespace vkapp
{
	void readFile( std::string const & name
		, ashes::Extent3D const & size
		, ashes::ByteArray & data );
}

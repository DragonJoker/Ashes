#pragma once

#include "Prerequisites.hpp"

namespace vkapp
{
	void readFile( std::string const & name
		, renderer::Extent3D const & size
		, renderer::ByteArray & data );
}

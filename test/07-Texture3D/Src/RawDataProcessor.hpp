#pragma once

#include "Prerequisites.hpp"

namespace vkapp
{
	void readFile( std::string const & name
		, VkExtent3D const & size
		, ashes::ByteArray & data );
}

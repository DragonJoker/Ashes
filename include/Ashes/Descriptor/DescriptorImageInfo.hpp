/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_DescriptorImageInfo_HPP___
#define ___Ashes_DescriptorImageInfo_HPP___
#pragma once

#include "Ashes/AshesPrerequisites.hpp"

namespace ashes
{
	struct DescriptorImageInfo
	{
		Optional< SamplerCRef > sampler;
		Optional< TextureViewCRef > imageView;
		ImageLayout imageLayout;
	};
}

#endif

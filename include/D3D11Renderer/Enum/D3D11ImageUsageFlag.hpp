/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <Ashes/AshesPrerequisites.hpp>

namespace d3d11_renderer
{
	bool isRenderTarget( ashes::ImageUsageFlags const & flags );
	bool isStorage( ashes::ImageUsageFlags const & flags );
	bool isSampled( ashes::ImageUsageFlags const & flags );
	bool isRenderable( ashes::ImageUsageFlags const & flags
		, VkFormat format
		, uint32_t mipLevels );
	bool isMipmapped( ashes::ImageUsageFlags const & flags
		, VkFormat format
		, uint32_t mipLevels );
}

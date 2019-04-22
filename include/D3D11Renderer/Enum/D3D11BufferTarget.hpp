/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

namespace ashes::d3d11
{
	bool isPipelineBindable( VkBufferUsageFlags const & targets );
	UINT convert( VkBufferUsageFlags const & targets );
}

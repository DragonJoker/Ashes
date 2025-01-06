/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

namespace ashes::xbox
{
	bool isPipelineBindable( VkBufferUsageFlags const & targets );
	UINT getBindFlags( VkBufferUsageFlags const & targets );
}

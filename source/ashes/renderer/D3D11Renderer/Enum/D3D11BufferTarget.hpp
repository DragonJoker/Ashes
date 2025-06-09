/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

namespace ashes::D3D11_NAMESPACE
{
	bool isPipelineBindable( VkBufferUsageFlags const & targets );
	UINT getBindFlags( VkBufferUsageFlags const & targets );
}

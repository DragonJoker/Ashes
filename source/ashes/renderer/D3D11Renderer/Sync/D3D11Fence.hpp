/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/D3D11RendererPrerequisites.hpp"

#include <Ashes/Sync/Fence.hpp>

namespace ashes::d3d11
{
	class Fence
		: public ashes::Fence
	{
	public:
		Fence( Device const & device
			, ashes::FenceCreateFlags flags = 0 );
		~Fence();

		ashes::WaitResult wait( uint64_t timeout )const;
		void reset()const;
	};
}

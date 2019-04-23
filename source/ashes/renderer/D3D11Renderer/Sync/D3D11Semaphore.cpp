/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Sync/D3D11Semaphore.hpp"

#include "Core/D3D11Device.hpp"

namespace ashes::d3d11
{
	Semaphore::Semaphore( Device const & device )
		: ashes::Semaphore{ device }
	{
	}

	Semaphore::~Semaphore()
	{
	}
}

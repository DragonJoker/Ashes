/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/D3D11DeviceContextLock.hpp"

#include "Core/D3D11Device.hpp"

#include <d3dcommon.h>

namespace ashes::D3D11_NAMESPACE
{
	DeviceContextLock::DeviceContextLock( ID3D11DeviceContext * context
		, std::mutex & mutex )
		: m_context{ context }
		, m_lock{ std::make_unique< std::unique_lock< std::mutex > >( mutex ) }
	{
	}

	DeviceContextLock::DeviceContextLock( ID3D11DeviceContext * context )noexcept
		: m_context{ context }
	{
	}
}

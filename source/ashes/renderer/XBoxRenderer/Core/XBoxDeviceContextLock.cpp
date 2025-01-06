/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/XBoxDeviceContextLock.hpp"

#include "Core/XBoxDevice.hpp"

#include <D3DCommon.h>

namespace ashes::xbox
{
	DeviceContextLock::DeviceContextLock( Device const * device )
		: m_device{ device }
		, m_context{ m_device->lockImmediateContext() }
	{
	}

	DeviceContextLock::DeviceContextLock( ID3D11DeviceContext * context )noexcept
		: m_context{ context }
	{
	}

	DeviceContextLock::DeviceContextLock( DeviceContextLock && rhs )noexcept
		: m_device{ rhs.m_device }
		, m_context{ rhs.m_context }
	{
		rhs.m_device = nullptr;
		rhs.m_context = nullptr;
	}

	DeviceContextLock & DeviceContextLock::operator=( DeviceContextLock && rhs )noexcept
	{
		m_device = rhs.m_device;
		m_context = rhs.m_context;

		rhs.m_device = nullptr;
		rhs.m_context = nullptr;

		return *this;
	}

	DeviceContextLock::~DeviceContextLock()noexcept
	{
		if ( m_device )
		{
			m_device->unlockImmediateContext();
		}
	}
}

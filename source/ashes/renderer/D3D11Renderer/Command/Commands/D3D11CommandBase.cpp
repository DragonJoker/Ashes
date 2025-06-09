/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11CommandBase.hpp"

#include "Core/D3D11Device.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::D3D11_NAMESPACE
{
	//*************************************************************************

	DeviceContextLock Context::getImmediateContext( VkDevice device )
	{
		return get( device )->getImmediateContext();
	}

	//*************************************************************************

	CommandBase::CommandBase( VkDevice device )
		: m_device{ device }
	{
	}

	//*************************************************************************
}

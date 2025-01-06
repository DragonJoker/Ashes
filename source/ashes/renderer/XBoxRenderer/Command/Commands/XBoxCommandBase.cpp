/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/XBoxCommandBase.hpp"

#include "Core/XBoxDevice.hpp"

#include "ashesxbox_api.hpp"

namespace ashes::xbox
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

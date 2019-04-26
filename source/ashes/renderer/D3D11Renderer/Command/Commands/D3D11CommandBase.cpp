/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11CommandBase.hpp"

#include "Core/D3D11Device.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
{
	//*************************************************************************

	ID3D11DeviceContext * Context::getImmediateContext( VkDevice device )
	{
		ID3D11DeviceContext * result;
		get( device )->getDevice()->GetImmediateContext( &result );
		return result;
	}

	//*************************************************************************

	CommandBase::CommandBase( VkDevice device )
		: m_device{ device }
	{
	}

	CommandBase::~CommandBase()noexcept
	{
	}

	//*************************************************************************
}

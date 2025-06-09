/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

namespace ashes::D3D11_NAMESPACE
{
	class DeviceContextLock
	{
	public:
		explicit DeviceContextLock( ID3D11DeviceContext * context, std::mutex & mutex );
		explicit DeviceContextLock( ID3D11DeviceContext * context )noexcept;

		ID3D11DeviceContext * operator->()const
		{
			return m_context;
		}

		ID3D11DeviceContext * operator*()const
		{
			return m_context;
		}

	private:
		ID3D11DeviceContext * m_context{};
		std::unique_ptr< std::unique_lock< std::mutex > > m_lock;
	};
}

/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Sync/Semaphore.hpp"

#include "ashespp/Core/Device.hpp"
#include "ashespp/Sync/Fence.hpp"

namespace ashes
{
	Semaphore::Semaphore( Device const & device )
		: m_device{ device }
	{
		VkSemaphoreCreateInfo createInfo
		{
			VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
			nullptr,
			0                                         // flags
		};
		DEBUG_DUMP( createInfo );
		auto res = m_device.vkCreateSemaphore( m_device
			, &createInfo
			, nullptr
			, &m_internal );
		checkError( res, "Semaphore creation" );
		registerObject( m_device, "Semaphore", this );
	}

	Semaphore::~Semaphore()
	{
		unregisterObject( m_device, this );
		m_device.vkDestroySemaphore( m_device
			, m_internal
			, nullptr );
	}

	void Semaphore::wait()const
	{
		ashesSyncCheck( m_state == State::eWaitable
			, "You probably expect too much from this semaphore ;)"
			, *this );
		m_state = State::eSignalable;
	}

	void Semaphore::signal( Fence const * fence )const
	{
		if ( fence )
		{
			ashesSyncCheck( !m_connection
				, "You probably expect too much from this semaphore ;)" 
				, *this );
			m_connection = fence->onWaitEnd.connect( [this]( Fence const & fence, WaitResult result )
				{
					signal();
					m_connection.disconnect();
				} );
		}
		else
		{
			signal();
		}
	}

	void Semaphore::signal()const
	{
		ashesSyncCheck( m_state == State::eSignalable
			, "You probably expect too much from this semaphore ;)"
			, *this );
		m_state = State::eWaitable;
	}
}

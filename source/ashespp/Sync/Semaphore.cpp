/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Sync/Semaphore.hpp"

#include "ashespp/Core/Device.hpp"
#include "ashespp/Sync/Fence.hpp"
#include "ashespp/Sync/SyncException.hpp"

namespace ashes
{
	Semaphore::Semaphore( Device const & device )
		: Semaphore{ device, "Semaphore" }
	{
	}

	Semaphore::Semaphore( Device const & device
		, std::string const & debugName )
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
			, m_device.getAllocationCallbacks()
			, &m_internal );
		checkError( res, "Semaphore creation" );
		registerObject( m_device, debugName, *this );
	}

	Semaphore::~Semaphore()
	{
		unregisterObject( m_device, *this );
		m_device.vkDestroySemaphore( m_device
			, m_internal
			, m_device.getAllocationCallbacks() );
	}

	void Semaphore::wait( std::set< Semaphore const * > & list )const
	{
#if Ashes_DebugSync
		ashesSyncCheck( m_state == State::eWaitable
			, "You probably expect too much from this semaphore ;)"
			, *this );
		m_list = &list;
		m_state = State::eSignalable;
#endif
	}

	void Semaphore::signal( Fence const * fence )const
	{
		signal();
	}

	void Semaphore::signal()const
	{
#if Ashes_DebugSync
		ashesSyncCheck( m_state == State::eSignalable
			, "You probably expect too much from this semaphore ;)"
			, *this );
		m_state = State::eWaitable;

		if ( m_list )
		{
			auto it = m_list->find( this );
			ashesSyncCheck( it != m_list->end()
				, "The list isn't waiting for the semaphore anymore"
				, *this );
			m_list->erase( it );
		}
#endif
	}
}

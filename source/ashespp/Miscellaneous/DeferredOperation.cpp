#include "ashespp/Miscellaneous/DeferredOperation.hpp"

#include "ashespp/Core/Device.hpp"

namespace ashes
{
	DeferredOperation::DeferredOperation( Device const & device )
		: DeferredOperation{ device, "DeferredOperation" }
	{
	}

	DeferredOperation::DeferredOperation( Device const & device
		, std::string const & debugName )
		: VkObject{ debugName }
		, m_device{ device }
	{
		auto res = m_device.vkCreateDeferredOperationKHR( device
			, m_device.getAllocationCallbacks()
			, &m_internal );
		checkError( res, "DeferredOperation creation" );
		registerObject( m_device, debugName, *this );
	}
	
	DeferredOperation::~DeferredOperation()
	{
		unregisterObject( m_device, *this );
		m_device.vkDestroyDeferredOperationKHR( m_device
			, m_internal
			, m_device.getAllocationCallbacks() );
	}

	uint32_t DeferredOperation::getMaxConcurrency()const
	{
		return m_device.vkGetDeferredOperationMaxConcurrencyKHR( m_device
			, m_internal );
	}

	VkResult DeferredOperation::getResult()const
	{
		return m_device.vkGetDeferredOperationResultKHR( m_device
			, m_internal );
	}

	void DeferredOperation::join()const
	{
		auto res = m_device.vkDeferredOperationJoinKHR( m_device
			, m_internal );
		checkError( res, "DeferredOperation joining" );
	}
}

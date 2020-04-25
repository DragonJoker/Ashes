/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_QueueShare_HPP___
#define ___AshesPP_QueueShare_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
{
	struct QueueShare
	{
		QueueShare( UInt32Array queueFamilyIndices = {} )
			: queueFamilyIndices{ std::move( queueFamilyIndices ) }
		{
			doInit();
		}

		QueueShare( QueueShare const & rhs )
			: queueFamilyIndices{ rhs.queueFamilyIndices }
		{
			doInit();
		}
		
		QueueShare( QueueShare && rhs )noexcept
			: queueFamilyIndices{ std::move( rhs.queueFamilyIndices ) }
		{
			rhs.queueFamilyIndexCount = 0u;
			rhs.pQueueFamilyIndices = nullptr;
			doInit();
		}

		QueueShare & operator=( QueueShare const & rhs )
		{
			queueFamilyIndices = rhs.queueFamilyIndices;
			doInit();
			return *this;
		}

		QueueShare & operator=( QueueShare && rhs )noexcept
		{
			queueFamilyIndices = std::move( rhs.queueFamilyIndices );
			rhs.queueFamilyIndexCount = 0u;
			rhs.pQueueFamilyIndices = nullptr;
			doInit();
			return *this;
		}

		UInt32Array queueFamilyIndices;
		VkSharingMode sharingMode;
		uint32_t queueFamilyIndexCount;
		uint32_t * pQueueFamilyIndices;

	private:
		void doInit()
		{
			auto end = queueFamilyIndices.end();
			end = std::unique( queueFamilyIndices.begin(), end );
			auto count = ( end == queueFamilyIndices.end() )
				? int( queueFamilyIndices.size() )
				: int( std::distance( queueFamilyIndices.begin(), end ) );
			sharingMode = ( count > 1 )
				? VK_SHARING_MODE_CONCURRENT
				: VK_SHARING_MODE_EXCLUSIVE;
			queueFamilyIndexCount = uint32_t( count );
			pQueueFamilyIndices = ( count == 0u )
				? nullptr
				: queueFamilyIndices.data();
		}
	};
}

#endif

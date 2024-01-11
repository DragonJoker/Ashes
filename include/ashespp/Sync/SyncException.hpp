/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_SyncException_HPP___
#define ___AshesPP_SyncException_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

#include <exception>
#include <functional>
#include <string>
#include <sstream>
#include <iomanip>

#define ashesSyncCheck( condition, text, semaphore )\
	ashes::syncCheck( condition, #condition, text, semaphore )

namespace ashes
{
#if !defined( NDEBUG )
	static bool constexpr DebugSync = false;
#else
	static bool constexpr DebugSync = false;
#endif

	class SyncException
		: public std::exception
	{
	public:
		SyncException( std::string const & condition
			, std::string const & text
			, VkSemaphore semaphore )
			: std::exception{}
			, m_text{ "Sync error: " + condition + " - " + text + getName( semaphore ) }
		{
		}

		char const * what() const noexcept override
		{
			return m_text.c_str();
		}

	private:
		static std::string getName( VkSemaphore semaphore )
		{
			std::stringstream stream;
			stream.imbue( std::locale{ "C" } );
			stream << "0x" << std::hex << std::setw( 8u ) << std::setfill( '0' ) << uint64_t( semaphore );
			return stream.str();
		}

	private:
		std::string m_text;
	};

	inline void syncCheck( bool result
		, char const * const condition
		, char const * const text
		, VkSemaphore const & semaphore )
	{
		if ( !result )
		{
			throw ashes::SyncException{ condition, text, semaphore };
		}
	}
}

#endif

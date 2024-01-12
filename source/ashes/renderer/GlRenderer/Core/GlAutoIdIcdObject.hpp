/**
*\file
*	InlineUniformBlocks.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "renderer/GlRenderer/Core/GlIcdObject.hpp"

#include <cassert>

namespace ashes::gl
{
	template< class TypeT >
	class AutoIdIcdObject
		: public gl::IcdObject
	{
	protected:
		AutoIdIcdObject()
		{
			m_internal = getNext();
		}

		~AutoIdIcdObject()noexcept = default;

	private:
		static uint32_t getNext()
		{
			static uint32_t value{ 0u };
			return ++value;
		}
	};
}

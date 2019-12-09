/**
*\file
*	InlineUniformBlocks.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "renderer/GlRendererCommon/GlIcdObject.hpp"

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

		~AutoIdIcdObject()
		{
		}

	private:
		static uint32_t getNext()
		{
			static uint32_t value{ 0u };
			return ++value;
		}
	};
}

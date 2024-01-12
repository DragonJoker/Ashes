/**
*\file
*	InlineUniformBlocks.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include <renderer/RendererCommon/IcdObject.hpp>

#include <cassert>

namespace ashes::gl
{
	class IcdObject
		: public ashes::IcdObject
	{
	private:
		static constexpr uint32_t InvalidValue = ~0u;

	public:
		inline uint32_t getInternal()const noexcept
		{
			assert( m_internal != InvalidValue );
			return m_internal;
		}

		inline bool hasInternal()const noexcept
		{
			return m_internal != InvalidValue;
		}

	protected:
		IcdObject()noexcept = default;
		~IcdObject()noexcept = default;

	protected:
		uint32_t m_internal{ InvalidValue };
	};
}

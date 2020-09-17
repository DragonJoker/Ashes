/*
See LICENSE file in root folder
*/
#pragma once

#include "GlContext.hpp"

#include <cassert>

namespace ashes::gl
{
	template< typename ValueT >
	struct ValueGetter;

	template<>
	struct ValueGetter< int32_t >
	{
		using ValueT = int32_t;

		static void get( ContextLock & context, GlValueName name, ValueT * value );
		static void get( ContextLock & context, GlValueName name, GLint index, ValueT * value );
	};

	template<>
	struct ValueGetter< int64_t >
	{
		using ValueT = int64_t;

		static void get( ContextLock & context, GlValueName name, ValueT * value );
		static void get( ContextLock & context, GlValueName name, GLint index, ValueT * value );
	};

	template<>
	struct ValueGetter< float >
	{
		using ValueT = float;

		static void get( ContextLock & context, GlValueName name, ValueT * value );
		static void get( ContextLock & context, GlValueName name, GLint index, ValueT * value );
	};

	template<>
	struct ValueGetter< uint32_t >
	{
		using ValueT = uint32_t;
		using SignedT = int32_t;

		static void get( ContextLock & context, GlValueName name, ValueT * value )
		{
			ValueGetter< SignedT >::get( context, name, reinterpret_cast< SignedT * >( value ) );
		}

		static void get( ContextLock & context, GlValueName name, GLint index, ValueT * value )
		{
			ValueGetter< SignedT >::get( context, name, index, reinterpret_cast< SignedT * >( value ) );
		}
	};

	template<>
	struct ValueGetter< uint64_t >
	{
		using ValueT = uint64_t;
		using SignedT = int64_t;

		static void get( ContextLock & context, GlValueName name, ValueT * value )
		{
			ValueGetter< SignedT >::get( context, name, reinterpret_cast< SignedT * >( value ) );
		}

		static void get( ContextLock & context, GlValueName name, GLint index, ValueT * value )
		{
			ValueGetter< SignedT >::get( context, name, index, reinterpret_cast< SignedT * >( value ) );
		}
	};

	class ContextLock
	{
	public:
		ContextLock( ContextLock const & ) = delete;
		ContextLock & operator=( ContextLock const & ) = delete;

		inline ContextLock( Context & context
			, VkDevice device = VK_NULL_HANDLE )
			: m_context{ &context }
			, m_device{ device }
			, m_disable{ !context.isEnabled() }
		{
			if ( m_disable )
			{
				m_context->lock();
			}
		}

		inline ContextLock( ContextLock && rhs )
			: m_context{ rhs.m_context }
			, m_disable{ rhs.m_disable }
		{
			rhs.m_context = nullptr;
		}

		inline ContextLock & operator=( ContextLock && rhs )
		{
			m_context = rhs.m_context;
			m_disable = rhs.m_disable;
			rhs.m_context = nullptr;
			return *this;
		}

		inline ~ContextLock()
		{
			if ( m_context && m_disable )
			{
				m_context->unlock();
			}
		}

		inline Context * operator->()const
		{
			return m_context;
		}

		inline Context const & getContext()const
		{
			assert( m_context != nullptr );
			return *m_context;
		}

		inline VkDevice getDevice()const
		{
			assert( m_device != nullptr );
			return m_device;
		}

		template< typename ValueT >
		void getValue( GlValueName name, ValueT & value )
		{
			ValueGetter< ValueT >::get( *this, name, &value );
		}

		template< typename ValueT, size_t CountT >
		void getValues( GlValueName name, ValueT ( & value )[CountT] )
		{
			ValueGetter< ValueT >::get( *this, name, value );
		}

		template< typename ValueT, size_t CountT >
		void getValuesI( GlValueName name, ValueT ( & value )[CountT] )
		{
			for ( size_t i = 0u; i < CountT; ++i )
			{
				ValueGetter< ValueT >::get( *this, name, i, value );
			}
		}

	private:
		Context * m_context;
		VkDevice m_device;
		bool m_disable;
	};
}

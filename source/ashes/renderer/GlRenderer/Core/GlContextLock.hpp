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

		static void get( ContextLock const & context, GlValueName name, ValueT * value );
		static void get( ContextLock const & context, GlValueName name, GLint index, ValueT * value );
	};

	template<>
	struct ValueGetter< int64_t >
	{
		using ValueT = int64_t;

		static void get( ContextLock const & context, GlValueName name, ValueT * value );
		static void get( ContextLock const & context, GlValueName name, GLint index, ValueT * value );
	};

	template<>
	struct ValueGetter< float >
	{
		using ValueT = float;

		static void get( ContextLock const & context, GlValueName name, ValueT * value );
		static void get( ContextLock const & context, GlValueName name, GLint index, ValueT * value );
	};

	template<>
	struct ValueGetter< uint32_t >
	{
		using ValueT = uint32_t;
		using SignedT = int32_t;

		static void get( ContextLock const & context, GlValueName name, ValueT * value )
		{
			ValueGetter< SignedT >::get( context, name, reinterpret_cast< SignedT * >( value ) );
		}

		static void get( ContextLock const & context, GlValueName name, GLint index, ValueT * value )
		{
			ValueGetter< SignedT >::get( context, name, index, reinterpret_cast< SignedT * >( &value[index] ) );
		}
	};

	template<>
	struct ValueGetter< uint64_t >
	{
		using ValueT = uint64_t;
		using SignedT = int64_t;

		static void get( ContextLock const & context, GlValueName name, ValueT * value )
		{
			ValueGetter< SignedT >::get( context, name, reinterpret_cast< SignedT * >( value ) );
		}

		static void get( ContextLock const & context, GlValueName name, GLint index, ValueT * value )
		{
			ValueGetter< SignedT >::get( context, name, index, reinterpret_cast< SignedT * >( &value[index] ) );
		}
	};

	class ContextLock
	{
	public:
		ContextLock( ContextLock const & ) = delete;
		ContextLock & operator=( ContextLock const & ) = delete;

		ContextLock( Context & context
			, VkDevice device = {} )
			: m_context{ &context }
			, m_device{ device }
			, m_disable{ !context.isEnabled() }
		{
			if ( m_disable )
			{
				m_context->lock();
			}
		}

		ContextLock( ContextLock && rhs )noexcept
			: m_context{ rhs.m_context }
			, m_device{ rhs.m_device }
			, m_disable{ rhs.m_disable }
		{
			rhs.m_context = nullptr;
		}

		ContextLock & operator=( ContextLock && rhs )noexcept
		{
			m_context = rhs.m_context;
			m_disable = rhs.m_disable;
			rhs.m_context = nullptr;
			return *this;
		}

		~ContextLock()noexcept
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
		ValueT & getValue( GlValueName name, ValueT & value )const
		{
			ValueGetter< ValueT >::get( *this, name, &value );
			return value;
		}

		template< typename ValueT, size_t CountT >
		void getValues( GlValueName name, ValueT ( & value )[CountT] )const
		{
			ValueGetter< ValueT >::get( *this, name, value );
		}

		template< typename ValueT, size_t CountT >
		void getValues( GlValueName name, std::array< ValueT, CountT > & value )const
		{
			ValueGetter< ValueT >::get( *this, name, value.data() );
		}

		template< typename ValueT, size_t CountT >
		void getValuesI( GlValueName name, ValueT ( & value )[CountT] )const
		{
			for ( GLint i = 0u; i < GLint( CountT ); ++i )
			{
				ValueGetter< ValueT >::get( *this, name, i, value );
			}
		}

		template< typename ValueT, size_t CountT >
		void getValuesI( GlValueName name, std::array< ValueT, CountT > & value )const
		{
			for ( GLint i = 0u; i < GLint( CountT ); ++i )
			{
				ValueGetter< ValueT >::get( *this, name, i, value.data() );
			}
		}

		template< typename ValueT >
		ValueT & getValue( GlValueName name, ValueT & value, ValueT const & min )const
		{
			ValueGetter< ValueT >::get( *this, name, &value );
			value = std::max( value, min );
			return value;
		}

		template< typename ValueT >
		ValueT & getBitfieldValue( GlValueName name, ValueT & value, ValueT const & min )const
		{
			ValueT bitfield{};
			ValueGetter< ValueT >::get( *this, name, &bitfield );
			value = ValueT{};

			while ( bitfield )
			{
				value |= bitfield;
				bitfield >>= 1;
			}

			value = std::max( value, min );
			return value;
		}

		template< typename ValueT, size_t CountT >
		void getValues( GlValueName name, ValueT ( & value )[CountT], std::array< ValueT, CountT > const & min )const
		{
			ValueGetter< ValueT >::get( *this, name, value );

			for ( size_t i = 0u; i < CountT; ++i )
			{
				value[i] = std::max( value[i], min[i] );
			}
		}

		template< typename ValueT, size_t CountT >
		void getValues( GlValueName name, std::array< ValueT, CountT > & value, std::array< ValueT, CountT > const & min )const
		{
			ValueGetter< ValueT >::get( *this, name, value.data() );

			for ( size_t i = 0u; i < CountT; ++i )
			{
				value[i] = std::max( value[i], min[i] );
			}
		}

		template< typename ValueT, size_t CountT >
		void getValuesI( GlValueName name, ValueT( &value )[CountT], std::array< ValueT, CountT > const & min )const
		{
			for ( size_t i = 0u; i < CountT; ++i )
			{
				ValueGetter< ValueT >::get( *this, name, GLint( i ), value );
				value[i] = std::max( value[i], min[i] );
			}
		}

		template< typename ValueT, size_t CountT >
		void getValuesI( GlValueName name, std::array< ValueT, CountT > & value, std::array< ValueT, CountT > const & min )const
		{
			for ( size_t i = 0u; i < CountT; ++i )
			{
				ValueGetter< ValueT >::get( *this, name, GLint( i ), value.data() );
				value[i] = std::max( value[i], min[i] );
			}
		}

	private:
		Context * m_context;
		VkDevice m_device;
		bool m_disable;
	};
}

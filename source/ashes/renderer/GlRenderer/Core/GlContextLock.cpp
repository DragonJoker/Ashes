#include "GlContextLock.hpp"

namespace ashes::gl
{
	void ValueGetter< int32_t >::get( ContextLock & context, GlValueName name, int32_t * value )
	{
		glLogCall( context, glGetIntegerv, name, value );
	}

	void ValueGetter< int32_t >::get( ContextLock & context, GlValueName name, GLint index, int32_t * value )
	{
		glLogCall( context, glGetIntegeri_v, name, index, value );
	}

	void ValueGetter< int64_t >::get( ContextLock & context, GlValueName name, int64_t * value )
	{
		glLogCall( context, glGetInteger64v, name, value );
	}

	void ValueGetter< int64_t >::get( ContextLock & context, GlValueName name, GLint index, int64_t * value )
	{
		glLogCall( context, glGetInteger64i_v, name, index, value );
	}

	void ValueGetter< float >::get( ContextLock & context, GlValueName name, float * value )
	{
		glLogCall( context, glGetFloatv, name, value );
	}

	void ValueGetter< float >::get( ContextLock & context, GlValueName name, GLint index, float * value )
	{
		glLogCall( context, glGetFloati_v, name, index, value );
	}
}
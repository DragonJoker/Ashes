/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Utils/UtilsInstance.hpp"

#include "Utils/UtilsDebug.hpp"

namespace utils
{
	Instance::Instance( RendererFactory const & factory
		, std::string const & name
		, ashes::Instance::Configuration const & configuration )
	{
		m_instance = factory.create( name, configuration );
		m_debugCallback = setupDebugging( *m_instance
			, this );
	}
}

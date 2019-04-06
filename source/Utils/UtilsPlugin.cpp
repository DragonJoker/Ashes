#include "Utils/UtilsPlugin.hpp"

#include "Utils/UtilsFile.hpp"
#include "Utils/Factory.hpp"

#include <AshesRenderer/Util/Exception.hpp>

#include <functional>

namespace utils
{
	Plugin::Plugin( AshPluginDescription desc )
		: m_desc{ std::move( desc ) }
	{
	}
}

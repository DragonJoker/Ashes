/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "UtilsPlugin.hpp"

#include <Ashes/Core/DebugReportCallback.hpp>
#include <Ashes/Core/Instance.hpp>

namespace utils
{
	class Instance
	{
	public:
		Instance( RendererFactory const & factory
			, std::string const & name
			, ashes::Instance::Configuration const & configuration );

		inline ashes::Instance const & getInstance()const
		{
			return *m_instance;
		}

	private:
		ashes::InstancePtr m_instance;
		ashes::DebugReportCallbackPtr m_debugCallback;
	};
	using InstancePtr = std::unique_ptr< Instance >;
}


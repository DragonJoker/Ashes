/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Ashes/Core/Instance.hpp"

#include "Ashes/Core/Surface.hpp"
#include "Ashes/Miscellaneous/QueueCreateInfo.hpp"
#include "Ashes/Utils/CallStack.hpp"

#include <cmath>

namespace ashes
{
	Instance::Instance( ClipDirection clipDirection
		, std::string const & name
		, InstanceCreateInfo createInfo )
		: m_createInfo{ std::move( createInfo ) }
		, m_clipDirection{ clipDirection }
		, m_name{ name }
	{
#ifndef NDEBUG

		callstack::initialise();

#endif
	}

	Instance::~Instance()
	{
#ifndef NDEBUG

		callstack::cleanup();

#endif
	}

	std::array< float, 16 > Instance::infinitePerspective( float radiansFovY
		, float aspect
		, float zNear )const
	{
		float const range = tan( radiansFovY / float( 2 ) ) * zNear;
		float const left = -range * aspect;
		float const right = range * aspect;
		float const bottom = -range;
		float const top = range;

		std::array< float, 16 > result{ 0.0f };
		result[0] = ( float( 2 ) * zNear ) / ( right - left );
		result[5] = ( float( 2 ) * zNear ) / ( top - bottom );
		result[10] = -float( 1 );
		result[11] = -float( 1 );
		result[14] = -float( 2 ) * zNear;
		return result;
	}
}

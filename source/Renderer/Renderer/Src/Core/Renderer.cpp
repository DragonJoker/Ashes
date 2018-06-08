/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Core/Renderer.hpp"

#include "Utils/CallStack.hpp"

#include <cmath>

namespace renderer
{
	Renderer::Renderer( ClipDirection clipDirection
		, std::string const & name
		, Configuration const & configuration )
		: m_configuration{ configuration }
		, m_clipDirection{ clipDirection }
		, m_name{ name }
	{
#ifndef NDEBUG

		Debug::initialise();

#endif
	}

	Renderer::~Renderer()
	{
#ifndef NDEBUG

		Debug::cleanup();

#endif
	}

	PhysicalDevice & Renderer::getPhysicalDevice( uint32_t gpuIndex )const
	{
		if ( gpuIndex >= m_gpus.size() )
		{
			throw std::logic_error( "Invalid GPU index" );
		}

		return *m_gpus[gpuIndex];
	}

	std::array< float, 16 > Renderer::infinitePerspective( float radiansFovY
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

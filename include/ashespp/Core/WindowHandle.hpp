/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_WindowHandle_HPP___
#define ___AshesPP_WindowHandle_HPP___

#include "ashespp/AshesPPPrerequisites.hpp"

#include <memory>
#include <string>

namespace ashes
{
	/**
	*\brief
	*	A window handle, platform dependant.
	*/
	class IWindowHandle
	{
	protected:
		/**
		*\brief
		*	Constructor.
		*/
		IWindowHandle( std::string surfaceName )
			: m_surfaceName{ std::move( surfaceName ) }
		{
		}

	public:
		virtual ~IWindowHandle() = default;
		/**
		*\brief
		*	Boolean convertion operator.
		*/
		virtual operator bool() = 0;
		/**
		*\return
		*	The surface type name.
		*/
		inline std::string const & getSurfaceName()const
		{
			return m_surfaceName;
		}

	private:
		std::string const m_surfaceName;
	};
	using IWindowHandlePtr = std::unique_ptr< IWindowHandle >;
	/**
	*\brief
	*	A window handle, platform independant.
	*/
	class WindowHandle
	{
	public:
		WindowHandle( WindowHandle const & ) = delete;
		WindowHandle & operator=( WindowHandle const & ) = delete;
		WindowHandle( WindowHandle && rhs ) = default;
		WindowHandle & operator=( WindowHandle && rhs ) = default;
		/**
		*\brief
		*	Constructor.
		*\param[in] handle
		*	The allocated handle.
		*/
		explicit WindowHandle( IWindowHandlePtr handle );
		/**
		*\brief
		*	Destructor.
		*/
		~WindowHandle() = default;
		/**
		*\brief
		*	Boolean convertion operator.
		*/
		operator bool();
		/**
		*\return
		*	The window handle, cast in wanted format (IMswWindowHandle, IXWindowHandle, ...).
		*/
		template< class T >
		inline T const & getInternal()const
		{
			return static_cast< T const & >( *m_handle );
		}
		/**
		*\return
		*	The surface type name.
		*/
		inline std::string const & getSurfaceName()const
		{
			return m_handle->getSurfaceName();
		}
		/**
		*\return
		*	The surface.
		*/
		inline Surface const & getSurface()const
		{
			return *m_surface;
		}
		/**
		*\param[in] surface
		*	The surface.
		*/
		inline void setSurface( Surface const & surface )
		{
			m_surface = &surface;
		}

	private:
		IWindowHandlePtr m_handle;
		Surface const * m_surface;
	};
}

#endif

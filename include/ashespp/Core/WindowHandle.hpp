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
		explicit IWindowHandle( std::string surfaceName )noexcept
			: m_surfaceName{ std::move( surfaceName ) }
		{
		}

	public:
		virtual ~IWindowHandle()noexcept = default;
		/**
		*\brief
		*	Boolean convertion operator.
		*/
		virtual operator bool()noexcept = 0;
		/**
		*\return
		*	The surface type name.
		*/
		std::string const & getSurfaceName()const noexcept
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
		WindowHandle( WindowHandle && rhs )noexcept = default;
		WindowHandle & operator=( WindowHandle && rhs )noexcept = default;
		/**
		*\brief
		*	Constructor.
		*\param[in] handle
		*	The allocated handle.
		*/
		explicit WindowHandle( IWindowHandlePtr handle )noexcept;
		/**
		*\brief
		*	Destructor.
		*/
		~WindowHandle()noexcept = default;
		/**
		*\brief
		*	Boolean convertion operator.
		*/
		operator bool()noexcept;
		/**
		*\return
		*	The window handle, cast in wanted format (IMswWindowHandle, IXWindowHandle, ...).
		*/
		template< class T >
		T const & getInternal()const noexcept
		{
			return static_cast< T const & >( *m_handle );
		}
		/**
		*\return
		*	The surface type name.
		*/
		std::string const & getSurfaceName()const noexcept
		{
			static std::string const dummy;
			return ( m_handle
				? m_handle->getSurfaceName()
				: dummy );
		}
		/**
		*\return
		*	The surface.
		*/
		Surface const & getSurface()const noexcept
		{
			return *m_surface;
		}
		/**
		*\param[in] surface
		*	The surface.
		*/
		void setSurface( Surface const & surface )noexcept
		{
			m_surface = &surface;
		}

	private:
		IWindowHandlePtr m_handle;
		Surface const * m_surface{};
	};
}

#endif

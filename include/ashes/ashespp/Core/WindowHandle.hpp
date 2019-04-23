/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_WindowHandle_HPP___
#define ___Ashes_WindowHandle_HPP___

#include <memory>
#include <string>

namespace ashes
{
	/**
	*\~english
	*\brief
	*	A window handle, platform dependant.
	*\~french
	*\brief
	*	Implémentation d'un handle de fenêtre, dépendant de l'OS.
	*/
	class IWindowHandle
	{
	protected:
		/**
		*\~english
		*\brief
		*	Constructor.
		*\~french
		*\brief
		*	Constructeur.
		*/
		IWindowHandle( std::string surfaceName )
			: m_surfaceName{ std::move( surfaceName ) }
		{
		}

	public:
		/**
		*\~english
		*\brief
		*	Boolean convertion operator.
		*\~french
		*\brief
		*	Opérateur de conversion en booléen.
		*/
		virtual operator bool() = 0;
		/**
		*\~english
		*\return
		*	The surface type name.
		*\~french
		*\return
		*	Le nom du type de la surface.
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
	*\~english
	*\brief
	*	A window handle, platform independant.
	*\~french
	*\brief
	*	Implémentation d'un handle de fenêtre, indépendant de la plateforme.
	*/
	class WindowHandle
	{
	public:
		WindowHandle( WindowHandle const & ) = delete;
		WindowHandle & operator=( WindowHandle const & ) = delete;
		WindowHandle( WindowHandle && rhs ) = default;
		WindowHandle & operator=( WindowHandle && rhs ) = default;
		/**
		*\~english
		*\brief
		*	Constructor.
		*\param[in] handle
		*	The allocated handle.
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] handle
		*	Le handle alloué.
		*/
		explicit WindowHandle( IWindowHandlePtr handle );
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		~WindowHandle() = default;
		/**
		*\~english
		*\brief
		*	Boolean convertion operator.
		*\~french
		*\brief
		*	Opérateur de conversion en booléen.
		*/
		operator bool();
		/**
		*\~english
		*\return
		*	The window handle, cast in wanted format (IMswWindowHandle, IXWindowHandle, ...).
		*\~french
		*\return
		*	Le handle de la fenêtre, casté dans le format voulu (IMswWindowHandle, IXWindowHandle, ...).
		*/
		template< class T >
		inline T const & getInternal()const
		{
			return static_cast< T const & >( *m_handle );
		}
		/**
		*\~english
		*\return
		*	The surface type name.
		*\~french
		*\return
		*	Le nom du type de la surface.
		*/
		inline std::string const & getSurfaceName()const
		{
			return m_handle->getSurfaceName();
		}

	private:
		IWindowHandlePtr m_handle;
	};
}

#endif

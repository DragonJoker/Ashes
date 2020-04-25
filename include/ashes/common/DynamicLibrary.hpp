/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_common_DynamicLibrary_HPP___
#define ___Ashes_common_DynamicLibrary_HPP___
#pragma once

#include <string>

namespace ashes
{
	/**
	*\brief
	*	Platform independant shared library (.dll or .so).
	*\remarks
	*	Loads a library and gives access to it's functions in a platform independant way.
	*/
	class DynamicLibrary
	{
	public:
		DynamicLibrary( DynamicLibrary const & ) = delete;
		DynamicLibrary & operator=( DynamicLibrary const & ) = delete;
		DynamicLibrary( DynamicLibrary && rhs )noexcept;
		DynamicLibrary & operator=( DynamicLibrary && rhs )noexcept;
		/**
		*\brief
		*	Constructor, ppens a library from a path.
		*\remarks
		*	If the library couldn't be loaded, throws a std::runtime_error.
		*\param[in] path
		*	The file path.
		*/
		DynamicLibrary( std::string const & path );
		/**
		*\brief
		*	Destructor.
		*/
		~DynamicLibrary()noexcept;
		/**
		*\brief
		*	Retrieves a function.
		*\param[in] name
		*	The function name.
		*\param[out] function
		*	Receives the function.
		*\return
		*	\p true if the function was successfully retrieved.
		*/
		template< typename FuncType >
		bool getFunction( std::string const & name, FuncType & function )noexcept
		{
			function = reinterpret_cast< FuncType >( doGetFunction( name ) );
			return function != nullptr;
		}
		/**
		*\return
		*	The file path to the library.
		*/
		inline std::string const & getPath()const
		{
			return m_path;
		}

	private:
		void doOpen();
		void doClose()noexcept;
		void * doGetFunction( std::string const & p_name )noexcept;

	private:
		std::string m_path;
		void * m_library{ nullptr };
	};
}

#endif

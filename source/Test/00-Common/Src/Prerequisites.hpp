#pragma once

#include <Renderer/Connection.hpp>

#include <Utils/Mat4.hpp>

#pragma warning( disable: 4996 )
#include <wx/wx.h>
#include <wx/windowptr.h>

#include <array>
#include <chrono>

namespace common
{
	static wxSize const WindowSize{ 800, 600 };
	/**
	*\~french
	*\brief
	*	Crée une vk::Connection associant une fenêtre et un GPU.
	*\param[in] window
	*	Fenêtre utilisée pour récupérer les informations dépendantes de la plateforme cible.
	*\param[in] vulkan
	*	L'instance Vulkan.
	*\param[in] gpu
	*	Le GPU choisi pour associer Vulkan à la fenêtre.
	*\~english
	*\brief
	*	Creates a vk::Connection binding a window to a GPU.
	*\param[in] window
	*	The window used to retrieve the platform dependant informations.
	*\param[in] vulkan
	*	The Vulkan instance.
	*\param[in] gpu
	*	The GPU chosen to bind the window and the GPU.
	*/
	renderer::ConnectionPtr makeConnection( wxWindow * window
		, renderer::Renderer const & vulkan );
	/**
	*\~french
	*\brief
	*	Charge le contenu d'un fichier texte dans un tableau d'octets.
	*\param[in] file
	*	Le chemin d'accès au fichier.
	*\return
	*	Le contenu du fichier.
	*\~english
	*\brief
	*	Loads the content of a text file into a byte array.
	*\param[in] file
	*	The file path.
	*\return
	*	The file content.
	*/
	std::string dumpTextFile( std::string const & file );
	/**
	*\~french
	*\brief
	*	Charge le contenu d'un fichier binaire dans un tableau d'octets.
	*\param[in] file
	*	Le chemin d'accès au fichier.
	*\return
	*	Le contenu du fichier.
	*\~english
	*\brief
	*	Loads the content of a binary file into a byte array.
	*\param[in] file
	*	The file path.
	*\return
	*	The file content.
	*/
	renderer::ByteArray dumpBinaryFile( std::string const & file );
	/**
	*\~french
	*\brief
	*	Charge le contenu d'un fichier binaire dans un tableau d'octets.
	*\param[in] file
	*	Le chemin d'accès au fichier.
	*\return
	*	Le contenu du fichier.
	*\~english
	*\brief
	*	Loads the content of a binary file into a byte array.
	*\param[in] file
	*	The file path.
	*\return
	*	The file content.
	*/
	renderer::UInt32Array dumpSpvFile( std::string const & file );
	/**
	*\~french
	*\return
	*	Le dossier d'exécution du programme.
	*\~english
	*\return
	*	The executable folder.
	*/
	std::string getExecutableDirectory();
	/**
	*\~french
	*\brief
	*	Récupère le dossier parent du chemin donné.
	*\param[in] path
	*	Le chemin.
	*\return
	*	Le dossier parent.
	*\~english
	*\brief
	*	Retrieves the given path's parent folder.
	*\param[in] path
	*	The path.
	*\return
	*	The parent folder.
	*/
	std::string getPath( std::string const & path );

	class Application;
	class MainFrame;
}
/**
*\~french
*\brief
*	Concatène deux chemins en utilisant le séparateur de dossier.
*\param[in] lhs, rhs
*	Les chemins à concaténer.
*\return
*	Le chemin concaténé (lhs / rhs).
*\~english
*\brief
*	Concatenates two paths, adding the folder separator.
*\param[in] lhs, rhs
*	The paths to concatenate.
*\return
*	The concatenated path (lhs / rhs).
*/
std::string operator/( std::string const & lhs, std::string const & rhs );
/**
*\~french
*\brief
*	Concatène deux chemins en utilisant le séparateur de dossier.
*\param[in] lhs, rhs
*	Les chemins à concaténer.
*\return
*	Le chemin concaténé (lhs / rhs).
*\~english
*\brief
*	Concatenates two paths, adding the folder separator.
*\param[in] lhs, rhs
*	The paths to concatenate.
*\return
*	The concatenated path (lhs / rhs).
*/
inline std::string operator/( std::string const & lhs, char const * const rhs )
{
	return lhs / std::string{ rhs };
}
/**
*\~french
*\brief
*	Concatène deux chemins en utilisant le séparateur de dossier.
*\param[in] lhs, rhs
*	Les chemins à concaténer.
*\return
*	Le chemin concaténé (lhs / rhs).
*\~english
*\brief
*	Concatenates two paths, adding the folder separator.
*\param[in] lhs, rhs
*	The paths to concatenate.
*\return
*	The concatenated path (lhs / rhs).
*/
inline std::string operator/( char const * const lhs, std::string const & rhs )
{
	return std::string{ lhs } / rhs;
}

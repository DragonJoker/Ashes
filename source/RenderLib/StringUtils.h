/**
*\file
*	FileUtils.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_StringUtils_H___
#define ___RenderLib_StringUtils_H___
#pragma once

#include <string>

namespace render
{
	//! Un tableau de chaines de caractères.
	using StringArray = std::vector< std::string >;
	/**
	*\brief
	*	Remplace toutes les occurences de toReplace par replacement dans text.
	*\param[in] text
	*	Le texte source.
	*\param[in] toReplace
	*	Le texte à remplacer.
	*\param[in] replacement
	*	Le texte de remplacement.
	*\return
	*	Le text modifié.
	*/
	std::string replace( std::string const & text
		, std::string const & toReplace
		, std::string const & replacement );
	/**
	*\brief
	*	Supprime les caractères blancs (' ', '\\t', '\\r') du début du texte.
	*\param[in] text
	*	Le texte source.
	*\return
	*	Le text modifié.
	*/
	std::string trimLeft( std::string const & text );
	/**
	*\brief
	*	Supprime les caractères blancs (' ', '\\t', '\\r') de la fin du texte.
	*\param[in] text
	*	Le texte source.
	*\return
	*	Le text modifié.
	*/
	std::string trimRight( std::string const & text );
	/**
	*\brief
	*	Récupère le texte donné avec tous les caractères majuscules remplacés
	*	par les minuscules correspondantes.
	*\param[in] text
	*	Le texte source.
	*\return
	*	Le text modifié.
	*/
	std::string lowerCase( std::string const & text );
	/**
	*\brief
	*	Récupère le texte donné avec tous les caractères minuscules remplacés
	*	par les majuscules correspondantes.
	*\param[in] text
	*	Le texte source.
	*\return
	*	Le text modifié.
	*/
	std::string upperCase( std::string const & text );
	/**
	*\brief
	*	Découpe une chaîne en plusieurs sous-chaînes, en utilisant un/des
	*	délimiteur(s).
	*\param[in] str
	*	La chaîne à découper.
	*\param[in] delims
	*	Le(s) délimiteur(s).
	*\param[in] maxSplits
	*	Le nombre maximal de découpes (le retour contiendra 
	*	0 < x < maxSplits sous-chaînes).
	*\param[in] keepVoid
	*	Dit si la fonction garde les sous-chaînes vides ou pas.
	*\return
	*	Le tableau contenant les sous-chaînes.
	*/
	StringArray split( std::string const & str
		, std::string const & delims
		, uint32_t maxSplits = 10
		, bool keepVoid = true );
	/**
	*\brief
	*	Remplace toutes les occurences de toReplace par replacement dans text.
	*\param[in] text
	*	Le texte source.
	*\param[in] toReplace
	*	Le texte à remplacer.
	*\param[in] replacement
	*	Le texte de remplacement.
	*\return
	*	Le text modifié.
	*/
	inline std::string replace( std::string const & text
		, char toReplace
		, char replacement )
	{
		char find[2] = { toReplace, '\0' };
		char replaced[2] = { replacement, '\0' };
		return replace( text, find, replaced );
	}
	/**
	*\brief
	*	Remplace toutes les occurences de toReplace par replacement dans text.
	*\param[in] text
	*	Le texte source.
	*\param[in] toReplace
	*	Le texte à remplacer.
	*\param[in] replacement
	*	Le texte de remplacement.
	*\return
	*	Le text modifié.
	*/
	inline std::string replace( std::string const & text
		, std::string const & toReplace
		, char replacement )
	{
		char replaced[2] = { replacement, '\0' };
		return replace( text, toReplace, replaced );
	}
	/**
	*\brief
	*	Remplace toutes les occurences de toReplace par replacement dans text.
	*\param[in] text
	*	Le texte source.
	*\param[in] toReplace
	*	Le texte à remplacer.
	*\param[in] replacement
	*	Le texte de remplacement.
	*\return
	*	Le text modifié.
	*/
	inline std::string replace( std::string const & text
		, char toReplace
		, std::string const & replacement )
	{
		char find[2] = { toReplace, '\0' };
		return replace( text, find, replacement );
	}
	/**
	*\brief
	*	Supprime les caractères blancs (' ', '\\t', '\\r') du début et de la
	*	fin du texte.
	*\param[in] text
	*	Le texte source.
	*\return
	*	Le text modifié.
	*/
	inline std::string trim( std::string const & text )
	{
		return trimLeft( trimRight( text ) );
	}
}

#endif

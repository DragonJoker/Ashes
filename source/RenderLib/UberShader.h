/**
*\file
*	UberShader.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_UberShader_HPP___
#define ___RenderLib_UberShader_HPP___
#pragma once

#include "FlagCombination.h"

#include <GlLib/OpenGL.h>

namespace render
{
	/**
	*\brief
	*	Enumération des types de programme de rendu.
	*/
	enum class RenderType
		: uint8_t
	{
		//! Programme pour rendu de scène.
		eScene,
		//! Programme pour le picking.
		ePicking,
	};
	/**
	*\brief
	*	Enumération des types de programme de rendu.
	*/
	enum class TextureFlag
		: uint8_t
	{
		//! Programme sans texture.
		eNone = 0x00,
		//! Programme avec texture de diffuse.
		eDiffuse = 0x01,
		//! Programme avec texture d'opacité.
		eOpacity = 0x02,
	};
	RenderLib_ImplementFlag( TextureFlag )
	/**
	*\brief
	*	Enumération des types de programme de rendu.
	*/
	enum class OpacityType
		: uint8_t
	{
		//! Programme pour rendu opaque.
		eOpaque,
		//! Programme pour rendu transparent avec alpha blending.
		eAlphaBlend,
		//! Programme pour rendu transparent sans alpha test.
		eAlphaTest,
	};
	/**
	*\brief
	*	Enumération des types d'objets.
	*/
	enum class ObjectType
	{
		//! Objet avec maillage.
		eObject,
		//! Billboard.
		eBillboard,
		//! Liste de lignes.
		ePolyLine,
		//! Incrustation panneau.
		ePanelOverlay,
		//! Incrustation texte.
		eTextOverlay,
		//! Texture en plein écran.
		eTexture,
		GlLib_EnumBounds( eObject )
	};
	/**
	*\brief
	*	Classe permettant de récupérer les sources des shaders.
	*/
	class UberShader
	{
	public:
		/**
		*\brief
		*	Récupère le programme pour les shaders donnés.
		*\param[in] vtx
		*	Le vertex shader.
		*\param[in] pxl
		*	Le pixel shader.
		*\return
		*	Le programme.
		*/
		static gl::ShaderProgramPtr createShaderProgram( std::string vtx
			, std::string pxl );
		/**
		*\brief
		*	Récupère le programme pour les informations données.
		*\param[in] render
		*	Le type de rendu.
		*\param[in] textures
		*	Combinaison de TextureFlag.
		*\param[in] opacity
		*	Le type d'opacité.
		*\param[in] object
		*	Le type d'objet.
		*\return
		*	Le programme.
		*/
		static gl::ShaderProgramPtr createShaderProgram( RenderType render
			, TextureFlags textures
			, OpacityType opacity
			, ObjectType object );
		/**
		*\brief
		*	Récupère les indicateurs de programme pour les infos données.
		*\param[in] render
		*	Le type de rendu.
		*\param[in] opacity
		*	Le type d'opacité.
		*\param[in] object
		*	Le type d'objet.
		*\param[in] textures
		*	Combinaison de TextureFlag.
		*\return
		*	Les indicateurs.
		*/
		static uint32_t programFlags( RenderType render
			, ObjectType object
			, OpacityType opacity
			, TextureFlags textures )
		{
			return ( uint32_t( render ) << 24 )
				| ( uint32_t( object ) << 16 )
				| ( uint32_t( opacity ) << 8 )
				| ( uint32_t( textures ) << 0 );
		}
		/**
		*\brief
		*	Récupère le type de noeud associé aux paramètres.
		*\param[in] opacity
		*	Le type d'opacité.
		*\param[in] textures
		*	La combinaison de TextureFlag.
		*\return
		*	Le type de noeud.
		*/
		static NodeType nodeType( OpacityType opacity
			, TextureFlags textures );
		/**
		*\brief
		*	Récupère les indicateurs de texture depuis le type de noeud.
		*\param[in] type
		*	Le type de noeud.
		*\return
		*	Les indicateurs trouvés.
		*/
		static TextureFlags textureFlags( NodeType type );
		/**
		*\brief
		*	Récupère les type d'opacité depuis le type de noeud.
		*\param[in] type
		*	Le type de noeud.
		*\return
		*	Le type d'opacité.
		*/
		static OpacityType opacityType( NodeType type );

	private:
		/**
		*\brief
		*	Récupère le vertex program pour le type donné.
		*\param[in] render
		*	Le type de rendu.
		*\param[in] textures
		*	Combinaison de TextureFlag.
		*\param[in] opacity
		*	Le type d'opacité.
		*\param[in] object
		*	Le type d'objet.
		*\return
		*	Le vertex shader.
		*/
		static std::string doGetVertexSource( RenderType render
			, TextureFlags textures
			, OpacityType opacity
			, ObjectType object );
		/**
		*\brief
		*	Récupère le pixel program pour le type donné.
		*\param[in] render
		*	Le type de rendu.
		*\param[in] textures
		*	Combinaison de TextureFlag.
		*\param[in] opacity
		*	Le type d'opacité.
		*\param[in] object
		*	Le type d'objet.
		*\return
		*	Le pixel shader.
		*/
		static std::string doGetPixelSource( RenderType render
			, TextureFlags textures
			, OpacityType opacity
			, ObjectType object );
	};
}

#endif

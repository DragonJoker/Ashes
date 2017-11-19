/**
*\file
*	Material.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_Material_HPP___
#define ___RenderLib_Material_HPP___
#pragma once

#include "Texture.h"
#include "UberShader.h"

namespace render
{
	/**
	*\brief
	*	Contient les données d'un matériau.
	*/
	class Material
	{
	public:
		/**
		*\return
		*	\p true si ce matériau possède une texture.
		*/
		inline bool hasTexture()const noexcept
		{
			return hasOpacityMap()
				|| hasDiffuseMap();
		}
		/**
		*\return
		*	La combinaison de TextureFlag pour ce matériau.
		*/
		inline TextureFlags textureFlags()const noexcept
		{
			return TextureFlag::eNone
				| ( hasOpacityMap()
					? TextureFlag::eOpacity
					: TextureFlag::eNone )
				| ( hasDiffuseMap()
					? TextureFlag::eDiffuse
					: TextureFlag::eNone );
		}
		/**
		*\return
		*	\p true si ce matériau possède une texture diffuse.
		*/
		inline bool hasDiffuseMap()const noexcept
		{
			return m_diffuseMap != nullptr;
		}
		/**
		*\return
		*	La texture de diffuse de ce matériau.
		*\remarks
		*	La texture de diffuse *DOIT* exister (utiliser la fonction
		*	hasDiffuseMap() avant d'appeler cette fonction).
		*/
		inline Texture & diffuseMap()const noexcept
		{
			assert( m_diffuseMap );
			return *m_diffuseMap;
		}
		/**
		*\brief
		*	Définit la texture de diffuse de ce matériau.
		*\param[in] texture
		*	La nouvelle texture.
		*/
		inline void diffuseMap( TexturePtr texture )noexcept
		{
			m_diffuseMap = texture;
		}
		/**
		*\return
		*	\p true si ce matériau possède une texture d'opacité.
		*/
		inline bool hasOpacityMap()const noexcept
		{
			return m_opacityMap != nullptr;
		}
		/**
		*\return
		*	La texture d'opacité de ce matériau.
		*\remarks
		*	La texture d'opacité *DOIT* exister (utiliser la fonction
		*	hasOpacityMap() avant d'appeler cette fonction).
		*/
		inline Texture & opacityMap()const noexcept
		{
			assert( m_opacityMap );
			return *m_opacityMap;
		}
		/**
		*\brief
		*	Définit la texture d'opacité de ce matériau.
		*\param[in] texture
		*	La nouvelle texture.
		*/
		inline void opacityMap( TexturePtr texture )noexcept
		{
			m_opacityMap = texture;
		}
		/**
		*\brief
		*	Définit la couleur ambiante de ce matériau.
		*\param[in] colour
		*	La nouvelle valeur.
		*/
		inline void ambient( gl::RgbColour const & colour )noexcept
		{
			m_ambient = colour;
		}
		/**
		*\brief
		*	Définit la couleur diffuse de ce matériau.
		*\param[in] colour
		*	La nouvelle valeur.
		*/
		inline void diffuse( gl::RgbColour const & colour )noexcept
		{
			m_diffuse = colour;
		}
		/**
		*\brief
		*	Définit la couleur spéculaire de ce matériau.
		*\param[in] colour
		*	La nouvelle valeur.
		*/
		inline void specular( gl::RgbColour const & colour )noexcept
		{
			m_specular = colour;
		}
		/**
		*\brief
		*	Définit la couleur émissive de ce matériau.
		*\param[in] colour
		*	La nouvelle valeur.
		*/
		inline void emissive( gl::RgbColour const & colour )noexcept
		{
			m_emissive = colour;
		}
		/**
		*\brief
		*	Définit l'exposant spéculaire de ce matériau.
		*\param[in] exponent
		*	La nouvelle valeur.
		*/
		inline void exponent( float exponent )noexcept
		{
			m_exponent = exponent;
		}
		/**
		*\brief
		*	Définit l'opacité de ce matériau.
		*\param[in] opacity
		*	La nouvelle valeur.
		*/
		inline void opacity( float opacity )noexcept
		{
			m_opacity = opacity;
		}
		/**
		*\return
		*	La couleur ambiante de ce matériau.
		*/
		inline gl::RgbColour const & ambient()const noexcept
		{
			return m_ambient;
		}
		/**
		*\return
		*	La couleur diffuse de ce matériau.
		*/
		inline gl::RgbColour const & diffuse()const noexcept
		{
			return m_diffuse;
		}
		/**
		*\return
		*	La couleur spéculaire de ce matériau.
		*/
		inline gl::RgbColour const & specular()const noexcept
		{
			return m_specular;
		}
		/**
		*\return
		*	La couleur émissive de ce matériau.
		*/
		inline gl::RgbColour const & emissive()const noexcept
		{
			return m_emissive;
		}
		/**
		*\return
		*	L'exposant spéculaire de ce matériau.
		*/
		inline float exponent()const noexcept
		{
			return m_exponent;
		}
		/**
		*\return
		*	L'opacité de ce matériau.
		*/
		inline float opacity()const noexcept
		{
			return m_opacity;
		}
		/**
		*\return
		*	\p true si l'opacité du matériau est de type alpha testing.
		*/
		inline bool alphaTest()const noexcept
		{
			return m_alphaTest;
		}
		/**
		*\brief
		*	Active ou désactive l'alpha testing pour l'opacité du matériau.
		*\param[in] value
		*	\p true pour activer.
		*/
		inline void alphaTest( bool value )noexcept
		{
			m_alphaTest = value;
		}
		/**
		*\brief
		*	Dit si le matériau est opaque.
		*\return
		*	\p true si l'opacité est à 1 et s'il n'y a pas de texture
		*	d'opacité.
		*/
		inline bool opaque()const noexcept
		{
			return ( m_opacity >= 1.0 )
				&& ( !m_opacityMap );
		}
		/**
		*\return
		*	Le type d'opacité de ce matériau.
		*/
		inline OpacityType opacityType()const noexcept
		{
			return ( opaque()
				? OpacityType::eOpaque
				: ( alphaTest()
					? OpacityType::eAlphaTest
					: OpacityType::eAlphaBlend ) );
		}

	private:
		//! La couleur ambiante.
		gl::RgbColour m_ambient{ 0.0f, 0.0f, 0.0f };
		//! La couleur diffuse.
		gl::RgbColour m_diffuse{ 1.0f, 1.0f, 1.0f };
		//! La couleur spéculaire.
		gl::RgbColour m_specular{ 1.0f, 1.0f, 1.0f };
		//! La couleur émissive.
		gl::RgbColour m_emissive{ 0.0f, 0.0f, 0.0f };
		//! L'exposant spéculaire.
		float m_exponent{ 50.0f };
		//! L'opacité globale du matériau.
		float m_opacity{ 1.0f };
		//! La texture de diffuse.
		TexturePtr m_diffuseMap{ nullptr };
		//! La texture d'opacité.
		TexturePtr m_opacityMap{ nullptr };
		//! Dit si l'opacité du matériau est de type alpha test.
		bool m_alphaTest{ false };
	};
}

#endif

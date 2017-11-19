/**
*\file
*	Overlay.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_OverlayCategory_H___
#define ___RenderLib_OverlayCategory_H___
#pragma once

#include "RenderLibPrerequisites.h"

namespace render
{
	/**
	*\brief
	*	Contient les membres spécifiques à une catégorie d'incrustation.
	*/
	class Overlay
	{
	public:
		/**
		*\brief
		*	Enumération des types d'incrustations
		*/
		enum class Type
			: uint8_t
		{
			//! Un simple panneau.
			ePanel,
			//! Un panneau borduré.
			eBorderPanel,
			//! Un texte.
			eText,
		};
		/**
		*\brief
		*	Contient les données spécifiques de sommet pour une incrustation.
		*/
		struct Vertex
		{
			//! Sa position.
			gl::Vec2 coords;
			//! Ses coordonnées de texture.
			gl::Vec2 text;
		};
		//! Un quad permettant d'afficher un caractère.
		using Quad = std::array< Vertex, 6 >;
		//! Liste de sommets composant l'incrustation.
		using QuadArray = std::vector< Quad >;

	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] type
		*	Le type d'incrustation.
		*/
		Overlay( Type type );
		/**
		*\brief		Destructeur
		*/
		virtual ~Overlay();
		/**
		*\brief
		*	Met à jour la position, taille...
		*/
		void update();
		/**
		*\brief
		*	Dessine l'incrustation.
		*\param[in] renderer
		*	Le renderer utilisé pour dessiner cette incrustation.
		*/
		virtual void render( OverlayRenderer & renderer )const = 0;
		/**
		*\return
		*	Le matériau de l'incrustation.
		*/
		inline Material const & material()const noexcept
		{
			assert( m_material );
			return *m_material;
		}
		/**
		*\brief
		*	Définit le matériau de l'incrustation.
		*\param[in] material
		*	La nouvelle valeur.
		*/
		inline void material( MaterialPtr material )noexcept
		{
			m_material = material;
		}
		/**
		*\brief
		*	Définit la position de l'incrustation.
		*\param[in] position
		*	La nouvelle valeur.
		*/
		inline void position( gl::IVec2 const & position )noexcept
		{
			m_position = position;
			m_positionChanged = true;
		}
		/**
		*\return
		*	La position de l'incrustation.
		*/
		inline gl::IVec2 const & position()const noexcept
		{
			return m_position;
		}
		/**
		*\return
		*	La taille de l'incrustation.
		*/
		inline gl::IVec2 const & size()const noexcept
		{
			return m_size;
		}
		/**
		*\return
		*	Le type d'incrustation.
		*/
		inline Type type()const
		{
			return m_type;
		}
		/**
		*\return
		*	La matrice de transformation de l'incrustation.
		*/
		inline gl::Mat4 const & transform()const noexcept
		{
			return m_transform;
		}
		/**
		*\return
		*	Le statut de visibilité de l'objet.
		*/
		inline bool visible()const noexcept
		{
			return m_visible;
		}
		/**
		*\brief
		*	Définit le statut de visibilité de l'objet.
		*\param[in] show
		*	Le nouveau statut.
		*/
		inline void show( bool show = true )noexcept
		{
			m_visible = show;
		}
		/**
		*\return
		*	Le Z-index de l'incrustation.
		*/
		inline uint32_t index()const
		{
			return m_index;
		}
		/**
		*\brief
		*	Définit le Z-index de l'incrustation.
		*\remarks
		*	Un Z-index élevé implique que l'incrustation sera affichée par
		*	dessus les incrustations avec un Z-index moins élevé.
		*\param[in] index
		*	Le nouvel indice.
		*/
		inline void index( uint32_t index )
		{
			m_index = index;
		}

	protected:
		/**
		*\brief
		*	Met à jour la matrice de transformation de l'incrustation.
		*/
		void doUpdatePosition();
		/**
		*\brief
		*	Met à jour le tampon de sommets si nécessaire.
		*/
		virtual void doUpdate() = 0;

	protected:
		//! Le type d'incrustation
		Type m_type;
		//! La taille en pixels.
		gl::IVec2 m_size;
		//! La position en pixels.
		gl::IVec2 m_position;
		//! Dit si la position de cette incrustation a changé.
		bool m_positionChanged{ true };
		//! Le matériau de l'incrustation.
		MaterialPtr m_material;
		//! La matrice de transformation de l'incrustation.
		gl::Mat4 m_transform;
		//! Le statut de visibilité.
		bool m_visible{ true };
		//! Le Z-index de l'incrustation.
		uint32_t m_index{ 0 };
	};
}

#endif

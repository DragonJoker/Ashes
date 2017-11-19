/**
*\file
*	Picking.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_Picking_HPP___
#define ___RenderLib_Picking_HPP___
#pragma once

#include "PickingRenderer.h"
#include "RenderSignal.h"

#include <GlLib/GlFrameBuffer.h>
#include <GlLib/GlRenderBuffer.h>
#include <GlLib/GlTexture.h>

namespace render
{
	/**
	*\brief
	*	Passe de picking, utilisant les FBO.
	*\remarks
	*	Les noeuds de rendu sont organisés comme suit:
	*	\li Liste par type d'objet (billboard ou maillage).
	*	\li		Liste des noeuds par type NodeType.
	*
	*	Pour les billboards, il y a un niveau supplémentaire:
	*	\li			Liste des instances.
	*
	*	A partir de là, on packe les pixels de la manière suivante:
	*	\li Type d'objet => 2 bits.
	*	\li Type de noeud => 4 bits.
	*
	*	Pour les billboards on a ceci:
	*	\li Index du billboard => 8 bits.
	*	\li Index de l'instance => 18 bits.
	*
	*	Pour les maillages on a ceci:
	*	\li Index du maillage => 26 bits.
	*
	*	L'index du billboard correspondant à sa couleur, cela signifie qu'il
	*	ne peut y avoir que 256 couleurs différentes, pour les billboards.\n
	*	Lors du chargement des étoiles, j'en définis 58, il en reste donc
	*	198 à la discrétion de l'utilisateur.
	*/
	class Picking
	{
	public:
		/**
		*\brief
		*	Types d'éléments sélectionnables.
		*/
		enum class NodeType
		{
			//! Aucun élément sélectionné.
			eNone,
			//! Un objet complexe a été sélectionné.
			eObject,
			//! Un billboard a été sélectionné.
			eBillboard,
		};
		/**
		*\brief
		*	Structure d'un pixel.
		*/
		struct Pixel
		{
			//! Composante rouge.
			uint8_t r{ 0u };
			//! Composante verte.
			uint8_t g{ 0u };
			//! Composante bleue.
			uint8_t b{ 0u };
			//! Composante alpha.
			uint8_t a{ 0u };
		};
		//! Un tableau de pixels.
		using PixelArray = std::vector< Pixel >;

	public:
		/**
		 *\brief
		 *	Constructeur.
		*\param[in] size
		*	La taille voulue pour le FBO.
		 */
		Picking( gl::IVec2 const & size );
		/**
		*\brief
		*	Destructeur.
		*/
		~Picking();
		/**
		*\brief
		*	Sélectionne la géométrie à la position de souris donnée.
		*\param[in] position
		*	La position de la souris.
		*\param[in] camera
			La caméra.
		*\param[in] zoomPercent
			Le pourcentage du zoom actuel.
		*\param[in] objects
			Les objets à dessiner.
		*\param[in] billboards
			Les billboards à dessiner.
		*\return
		*	Picking::NodeType::eNone si rien n'a été pické.
		*/
		NodeType pick( gl::IVec2 const & position
			, Camera const & camera
			, float zoomPercent
			, RenderSubmeshArray const & objects
			, RenderBillboardArray const & billboards )const;
		/**
		*\return
		*	La texture de couleurs.
		*/
		inline gl::Texture const & texture()const noexcept
		{
			assert( m_colour != nullptr );
			return *m_colour;
		}

	public:
		//! Le signal émis lorsque l'on sélectionne un objet.
		mutable OnObjectPicked onObjectPicked;
		//! Le signal émis lorsque l'on sélectionne un billboard.
		mutable OnBillboardPicked onBillboardPicked;
		//! Le signal émis lorsque rien n'est sélectionné.
		mutable OnUnpick onUnpick;

	private:
		/**
		*\brief
		*	Exécute le picking FBO.
		*\param[in] position
		*	La position de la souris.
		*\param[in] camera
			La caméra.
		*\param[in] zoomPercent
			Le pourcentage du zoom actuel.
		*\param[in] objects
			Les objets à dessiner.
		*\param[in] billboards
			Les billboards à dessiner.
		*\return
		*	La couleur de la sélection.
		*/
		Pixel doFboPick( gl::IVec2 const & position
			, Camera const & camera
			, float zoomPercent
			, RenderSubmeshArray const & objects
			, RenderBillboardArray const & billboards )const;
		/**
		*\brief
		*	Sélectionne l'objet correspondant à la couleur donnée.
		*\param[in] pixel
		*	La couleur sélectionnée.
		*\param[in] objects
			Les objets dessinés.
		*\param[in] billboards
			Les billboards dessinés.
		*\return
		*	Picking::NodeType::eNone si rien n'a été pické.
		*/
		Picking::NodeType doPick( Pixel const & pixel
			, RenderSubmeshArray const & objects
			, RenderBillboardArray const & billboards )const;
		/**
		*\brief
		*	Unpacke les données d'un pixel dans un gl::IVec4.
		*\remarks
		*	Utilise les informations listées dans la documentation de la
		*	classe.
		*\param[in] pixel
		*	Le pixel packé.
		*\return
		*	Le données unpacked.
		*/
		static gl::IVec4 doUnpackPixel( Pixel pixel );
		/**
		*\brief
		*	Unpacke les données d'un pixel de billboard dans un gl::IVec2.
		*\remarks
		*	Utilise les informations listées dans la documentation de la
		*	classe.
		*\param[in] pixel
		*	Le pixel packé.
		*\return
		*	Le données unpacked (index de billboard, et index d'instance).
		*/
		static gl::IVec2 doUnpackBillboardPixel( Pixel pixel );
		/**
		*\brief
		*	Unpacke les données d'un pixel dans un gl::IVec4.
		*\remarks
		*	Utilise les informations listées dans la documentation de la
		*	classe.
		*\param[in] pixel
		*	Le pixel packé.
		*\return
		*	Le données unpacked (index de maillage).
		*/
		static gl::IVec2 doUnpackObjectPixel( Pixel pixel );

	private:
		//! Le renderer.
		PickingRenderer m_renderer;
		//! Les dimensions de l'image.
		gl::IVec2 m_size;
		//! La texture recevant le rendu couleur.
		gl::TexturePtr m_colour;
		//! Le tampon recevant le rendu profondeur.
		gl::RenderBufferPtr m_depth;
		//! Le tampon d'image.
		gl::FrameBufferPtr m_fbo;
		//! Le tampon dans lequel on va recevoir l'image.
		mutable PixelArray m_buffer;
	};
}

#endif

/**
*\file
*	OverlayRenderer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_OverlayRenderer_H___
#define ___RenderLib_OverlayRenderer_H___
#pragma once

#include "FontTexture.h"
#include "OverlayCategory.h"
#include "Viewport.h"
#include "UberShader.h"

#include <GlLib/GlPipeline.h>
#include <GlLib/GlUniformBuffer.h>

namespace render
{
	/**
	*\brief
	*	Noeud de rendu utilisé pour dessiner une incrustation.
	*/
	struct OverlayNode
	{
		/**
		*\brief
		*	Constructeur.
		*\param[in] text
		*	Dit si c'est un programme pour les incrustations texte.
		*/
		OverlayNode( bool text
			, OpacityType opacity
			, TextureFlags textures );

		//! Le programme sshader utilisé pour dessiner les incrustations.
		gl::ShaderProgramPtr m_program;
		//! Le tampon de variables uniformes pour les incrustations.
		gl::UniformBuffer m_overlayUbo;
		//! La variable uniforme contenant la matrice modèle - projection.
		gl::Mat4Uniform * m_mpUniform;
		//! La variable uniforme contenant la couleur de l'incrustation.
		gl::Vec4Uniform * m_colour;
		//! La variable uniforme contenant l'échantillonneur de la texture couleur.
		gl::IntUniformPtr m_mapColour;
		//! La variable uniforme contenant l'échantillonneur de la texture d'opacité.
		gl::IntUniformPtr m_mapOpacity;
		//! L'attribut de position.
		gl::Vec2AttributePtr m_position;
		//! L'attribut de coordonnées de texture.
		gl::Vec2AttributePtr m_texture;
	};
	//! Tableau de noeuds de rendu d'incrustations.
	using OverlayNodeArray = std::array< OverlayNode, size_t( NodeType::eCount ) >;
	/**
	*\brief
	*	Le renderer d'incrustations.
	*/
	class OverlayRenderer
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] maxCharsPerBuffer
		*	Le nombre maximal de caractères par tampon de sommets de texte.
		*/
		explicit OverlayRenderer( uint32_t maxCharsPerBuffer = 600 );
		/**
		*brief
		*	Destructeur.
		*/
		~OverlayRenderer();
		/**
		*brief
		*	Fonction de dessin des TextOverlays.
		*\param[in] overlays
		*	Les incrustations texte à dessiner.
		*/
		void draw( OverlayList const & overlays );
		/**
		*brief
		*	Dessine un PanelOverlay.
		*\param[in] overlay
		*	L'incrustation à dessiner.
		*/
		void drawPanel( PanelOverlay const & overlay );
		/**
		*brief
		*	Dessine un BorderPanelOverlay.
		*\param[in] overlay
		*	L'incrustation à dessiner.
		*/
		void drawBorderPanel( BorderPanelOverlay const & overlay );
		/**
		*brief
		*	Dessine un TextOverlay.
		*\param[in] overlay
		*	L'incrustation à dessiner.
		*/
		void drawText( TextOverlay const & overlay );
		/**
		*brief
		*	Commence le rendu des incrustations.
		*param[in] size
		*	Les dimensions de la fenêtre de rendu.
		*/
		void beginRender( gl::IVec2 const & size );
		/**
		*brief
		*	Termine le rendu des incrustations.
		*/
		void endRender();

	private:
		/**
		*\brief
		*	Crée un tampon de sommets et ses attrobite pour les incrustations
		*	texte, et l'ajoute à la liste.
		*/
		gl::Buffer< Overlay::Quad > const & doCreateTextBuffer();
		/**
		*brief
		*	Fonction de dessin d'une incrustation.
		*param[in] buffer
		*	Le tampon de la géométrie de l'incrustation.
		*param[in] count
		*	Le nombre de sommets.
		*param[in] transform
		*	La matrice de transformation de l'incrustation.
		*param[in] material
		*	Le matériau de l'incrustation.
		*param[in] node
		*	Le noeud de rendu.
		*/
		void doDrawBuffer( gl::Buffer< Overlay::Quad > const & buffer
			, uint32_t count
			, gl::Mat4 const & transform
			, Material const & material
			, OverlayNode const & node );
		/**
		*brief
		*	Fonction de dessin d'une incrustation.
		*param[in] buffer
		*	Le tampon de la géométrie de l'incrustation.
		*param[in] count
		*	Le nombre de sommets.
		*param[in] transform
		*	La matrice de transformation de l'incrustation.
		*param[in] material
		*	Le matériau de l'incrustation.
		*param[in] textOpacity
		*	La texture de police.
		*param[in] node
		*	Le noeud de rendu.
		*/
		void doDrawBuffer( gl::Buffer< Overlay::Quad > const & buffer
			, uint32_t count
			, gl::Mat4 const & transform
			, Material const & material
			, Texture const & textOpacity
			, OverlayNode const & node );
		/**
		*brief
		*	Remplit un GeometryBuffers d'une partie d'un tableau de sommets
		*	pour texte.
		*param[in] count
		*	Le nombre de sommets à copier dans le GeometryBuffers.
		*param[in] it
		*	Le début du tableau de sommets.
		*param[in] index
		*	L'indice courant dans le tableau de GeometryBuffers de texte.
		*return
		*	Le GeometryBuffers utilisé.
		*/
		gl::Buffer< Overlay::Quad > const & doFillTextPart( uint32_t count
			, uint32_t & offset
			, Overlay::QuadArray::const_iterator & it
			, uint32_t & index );

	private:
		//! Les programmes shader utilisé pour dessiner les incrustations panneau.
		OverlayNodeArray m_panelNodes;
		//! Le programme shader utilisé pour dessiner les incrustations texte.
		OverlayNode m_textNode;
		//! Le pipeline utilisé pour le dessin des incrustations texte.
		gl::Pipeline m_pipeline;
		//! Les tampons de sommets utilisés pour rendre les panneaux.
		gl::BufferPtr< Overlay::Quad > m_panelBuffer;
		//! Les tampons de sommets utilisés pour rendre les bordures.
		gl::BufferPtr< Overlay::Quad > m_borderBuffer;
		//! Les tampons de sommets utilisés pour rendre les textes.
		std::vector< gl::BufferPtr< Overlay::Quad > > m_textBuffers;
		//! Le nombre maximal de caractères par tampon de sommets texte.
		uint32_t m_maxCharsPerBuffer;
		//! Dit si les dimension du rendu ont changé.
		bool m_sizeChanged{ true };
		//! La matrice de projection.
		gl::Mat4 m_transform;
		//! Le viewport de rendu des incrustations.
		Viewport m_viewport;
	};
}

#endif

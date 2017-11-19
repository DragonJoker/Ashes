#pragma once

#include <DesktopUtils/MsWindow.h>

#include <RenderLib/CameraState.h>
#include <RenderLib/FontTexture.h>
#include <RenderLib/RenderWindow.h>
#include <RenderLib/Scene.h>

class Engine
	: public utils::MsWindow
{
public:
	Engine();
	~Engine();

public:
	/**
	*\brief
	*	Mise à jour du contenu.
	*/
	void onDraw()override;
	/**
	*\brief
	*	Création de la fenêtre.
	*/
	void onCreate()override;
	/**
	*\brief
	*	Destruction de la fenêtre.
	*/
	void onDestroy()override;
	/**
	*\brief
	*	Minimisation de la fenêtre.
	*/
	void onMinimise()override;
	/**
	*\brief
	*	Réaffichage de la fenêtre après une minimisation.
	*/
	void onRestore( gl::IVec2 const & event )override;
	/**
	*\brief
	*	Initialisation de la fenêtre.
	*/
	void onResize( gl::IVec2 const & event )override;
	/**
	*\brief
	*	Déplacement de la souris.
	*/
	void onMouseMove( utils::MouseEvent const & event )override;
	/**
	*\brief
	*	Bouton gauche de la souris cliqué.
	*/
	void onLButtonDown( utils::MouseEvent const & event )override;
	/**
	*\brief
	*	Bouton gauche de la souris cliqué.
	*/
	void onLButtonClick( utils::MouseEvent const & event )override;
	/**
	*\brief
	*	Bouton gauche de la souris double cliqué.
	*/
	void onLButtonDblClick( utils::MouseEvent const & event )override;
	/**
	*\brief
	*	Un évènement de molette.
	*/
	void onMouseWheel( utils::MouseEvent const & event )override;
	/**
	*\brief
	*	Gestion du signal de sélection d'objet.
	*/
	void onObjectPicked( render::Object & object );
	/**
	*\brief
	*	Gestion du signal de sélection d'objet.
	*/
	void onBillboardPicked( render::Billboard & billboard, uint32_t index );
	/**
	*\brief
	*	Gestion du signal de désélection.
	*/
	void onUnpick();

private:
	/**
	*\brief
	*	Initialisation des éléments 3D affichés.
	*/
	void doInitialise3DElements();
	/**
	*\brief
	*	Destruction des éléments 3D affichés.
	*/
	void doCleanup3DElements();
	/**
	*\brief
	*	Initialise la texture de police.
	*/
	void doInitialiseFontTexture();
	/**
	*\brief
	*	Crée un matériau depuis une couleur et une opacité.
	*\param[in] name
	*	Le nom du matériau.
	*\param[in] colour
	*	La couleur du matériau.
	*\param[in] opacity
	*	L'opacité du matériau.
	*/
	render::MaterialPtr doCreateOverlayMaterial( std::string const & name
		, gl::RgbColour const & colour
		, float opacity );
	/**
	*\brief
	*	Met à jour la position et la taille de l'objet sélectionné.
	*\param[in] movable
	*	Les informations de position.
	*/
	void doUpdatePicked( render::Movable const & movable );
	/**
	*\brief
	*	Met à jour la position et la taille de l'objet sélectionné.
	*\param[in] object
	*	Les informations de position et taille.
	*/
	void doUpdatePicked( render::Object const & object );
	/**
	*\brief
	*	Met à jour la position et la taille de l'objet sélectionné.
	*\param[in] billboard
	*	Les informations de position et taille.
	*/
	void doUpdatePicked( render::Billboard const & billboard
		, uint32_t index );

private:
	std::streambuf * m_cout;
	std::streambuf * m_cerr;
	std::streambuf * m_clog;

	render::RenderWindowPtr m_window;

	gl::IVec2 m_mouse;

	render::Connection< render::OnObjectPicked > m_onObjectPicked;
	render::Connection< render::OnBillboardPicked > m_onBillboardPicked;
	render::Connection< render::OnUnpick > m_onUnpick;

	render::Object * m_pickedObject{ nullptr };
	render::Billboard * m_pickedBillboard{ nullptr };
	render::BillboardPtr m_picked;
	render::FontTexturePtr m_fontTexture;
};

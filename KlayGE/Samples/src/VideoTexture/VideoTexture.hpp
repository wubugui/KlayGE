#ifndef _VIDEOTEXTURE_HPP
#define _VIDEOTEXTURE_HPP

#include <KlayGE/App3D.hpp>
#include <KlayGE/Font.hpp>
#include <KlayGE/CameraController.hpp>

class VideoTextureApp : public KlayGE::App3DFramework
{
public:
	VideoTextureApp(std::string const & name, KlayGE::RenderSettings const & settings);

private:
	void InitObjects();

	void DoUpdateOverlay();
	KlayGE::uint32_t DoUpdate(KlayGE::uint32_t pass);

	void InputHandler(KlayGE::InputEngine const & sender, KlayGE::InputAction const & action);

	KlayGE::FontPtr font_;

	KlayGE::SceneObjectPtr ground_;

	KlayGE::FirstPersonCameraController fpcController_;

	KlayGE::ShowEnginePtr show_eng_; 
};

#endif		// _VIDEOTEXTURE_HPP

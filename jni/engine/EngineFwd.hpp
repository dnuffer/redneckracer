// Copyright 2011 Nuffer Brothers Software LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef engine_EngineFwd_hpp_INCLUDED_
#define engine_EngineFwd_hpp_INCLUDED_

#include "EngineConfig.hpp"
#include "boost/smart_ptr/intrusive_ptr.hpp"

namespace engine
{

	class Drawable;
	typedef boost::intrusive_ptr<Drawable> DrawablePtr;

	class Action;
	typedef boost::intrusive_ptr<Action> ActionPtr;

	class Texture;
	typedef boost::intrusive_ptr<Texture> TexturePtr;

	class Resource;
	typedef boost::intrusive_ptr<Resource> ResourcePtr;

	class Scene;
	typedef boost::intrusive_ptr<Scene> ScenePtr;

	class Sprite;
	typedef boost::intrusive_ptr<Sprite> SpritePtr;

	class Animation;
	typedef boost::intrusive_ptr<Animation> AnimationPtr;

	class TexturedFont;
	typedef boost::intrusive_ptr<TexturedFont> TexturedFontPtr;

	class TexturedQuad;
	typedef boost::intrusive_ptr<TexturedQuad> TexturedQuadPtr;

	class MenuItem;
	typedef boost::intrusive_ptr<MenuItem> MenuItemPtr;

	class Menu;
	typedef boost::intrusive_ptr<Menu> MenuPtr;

	class TextureLibrary;
	typedef boost::intrusive_ptr<TextureLibrary> TextureLibraryPtr;

	class Collidable;
	typedef boost::intrusive_ptr<Collidable> CollidablePtr;

	class Collider;
	typedef boost::intrusive_ptr<Collider> ColliderPtr;
	
	class Label;
	typedef boost::intrusive_ptr<Label> LabelPtr;

	class TouchEvent;
	
	class TouchButton;
	typedef boost::intrusive_ptr<TouchButton> TouchButtonPtr;
	
	class TouchHandler;
	typedef boost::intrusive_ptr<TouchHandler> TouchHandlerPtr;

	class ProgressBar;
	typedef boost::intrusive_ptr<ProgressBar> ProgressBarPtr;
	
	class DrawableRectangle;
	typedef boost::intrusive_ptr<DrawableRectangle> DrawableRectanglePtr;
	
	class DrawableLine;
	typedef boost::intrusive_ptr<DrawableLine> DrawableLinePtr;

    class Sound;
    typedef boost::intrusive_ptr<Sound> SoundPtr;
}

#endif

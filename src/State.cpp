#include "State.h"
#include "Alien.h"
#include "Resources.h"
#include "InputManager.h"
#include "Camera.h"
#include "CameraFollower.h"
#include "PenguinBody.h"
#include "PenguinCannon.h"
#include "Collider.h"
#include "Collision.h"

State::State() {
    quitRequested = false;
	started = false;
}
State::~State() {
    objectArray.clear();
	Resources::ClearImages();
	Resources::ClearMusics();
	Resources::ClearSounds();
}
void State::Start(){
	LoadAssets();
	for (unsigned int i = 0; i < objectArray.size(); i++) {
		objectArray[i]->Start();
	}
	started = true;
}

weak_ptr<GameObject> State::AddObject(GameObject *go){
	shared_ptr<GameObject> sptr(go);
	objectArray.push_back(sptr);
	if(started == true){
		sptr->Start();
	}

	return weak_ptr<GameObject>(sptr);
}
weak_ptr<GameObject> State::GetObjectPtr(GameObject* go){
	for (unsigned int i = 0; i < objectArray.size(); i++) {
		if (objectArray[i].get() == go)
			return weak_ptr<GameObject>(objectArray[i]);
	}
	return weak_ptr<GameObject>();
}
void State::LoadAssets(){
	GameObject* bg = new GameObject;
	
	Sprite* sprite = new Sprite(*bg, "assets/img/ocean.jpg");
	bg->box.x = 0;
	bg->box.y = 0;
	bg->box.w = sprite->GetWidth();
	bg->box.h = sprite->GetHeight();
	bg->AddComponent(sprite);
	CameraFollower* update = new CameraFollower(*bg);
	bg->AddComponent(update);
	objectArray.emplace_back(bg);
	
    music = new Music("assets/audio/stageState.ogg");
    music->Play();

	GameObject* gt = new GameObject;
	TileSet* ts = new TileSet(*gt, 64, 64, "assets/img/tileset.png");
	TileMap* tm = new TileMap(*gt, "assets/map/tileMap.txt", ts);
	gt->AddComponent(tm);
	objectArray.emplace_back(gt);

	GameObject* alien = new GameObject();
	alien->AddComponent(new Alien(*alien, 3,100));
	alien->box.x = 512 - (alien->box.w/2);
	alien->box.y = 300 - (alien->box.h/2);
	objectArray.emplace_back(alien);

	GameObject* penguin = new GameObject();
	Sprite* penguinsprite = new Sprite(*penguin, "assets/img/penguin.png");
	penguin->box.SetCentro(704, 640);
	penguin->box.h = penguinsprite->GetHeight();
	penguin->box.w = penguinsprite->GetWidth();
	penguin->AddComponent(penguinsprite);
	penguin->AddComponent(new PenguinBody(*penguin));
	objectArray.emplace_back(penguin);
}
void State::Update(float dt) {
	Camera::Update(dt);
	quitRequested = false;
	if (InputManager::GetInstance().KeyPress(ESCAPE_KEY) || InputManager::GetInstance().QuitRequested() == true) {
		quitRequested = true;
	}

	for (unsigned int i = 0; i < objectArray.size(); i++) {
		objectArray[i]->Update(dt);
		if (objectArray[i]->IsDead()) {
			objectArray.erase(objectArray.begin() + i);
		}
	}

	for (unsigned int i = 0; i < objectArray.size(); i++) {
		Collider* a = (Collider*)objectArray[i]->GetComponent("Collider");
		if (a != nullptr) {
			for (unsigned int j = i + 1; j < objectArray.size(); j++) {
				Collider* b = (Collider*)objectArray[j]->GetComponent("Collider");
				if (b != nullptr) {
					if (Collision::IsColliding(a->box, b->box, a->associated.angleDeg, b->associated.angleDeg)) {
						objectArray[i]->NotifyCollision(*objectArray[j]);
						objectArray[j]->NotifyCollision(*objectArray[i]);
					}
				}
			}
 		}
	}
	
}
void State::Render() {

	for (unsigned int i = 0; i < objectArray.size(); i++) {
		objectArray[i]->Render();
		// if(objectArray[i]->GetComponent("TileMap")){
		// 	objectArray[i]->
		// }
	}
}
bool State::QuitRequested() {
    return quitRequested;
}

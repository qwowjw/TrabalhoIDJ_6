#include "PenguinBody.h"
#include "Alien.h"
#include "Bullet.h"
#include "Camera.h"
#define MAXSPEED 300
#include "Game.h"
#include "InputManager.h"
#include "PenguinCannon.h"
#include "Collider.h"

PenguinBody* PenguinBody::player = nullptr;

PenguinBody::PenguinBody(GameObject& associated) : Component(associated){
    speed = Vec2(0,0);
	angle = 0;
	hp = 20;
	dead = false;
	player = this;
	Camera::Follow(&associated);
    associated.AddComponent(new Collider(associated));

}
PenguinBody::~PenguinBody() {
	player = nullptr;
}

void PenguinBody::Start() {
	auto penguinPass = Game::GetInstance().GetState().GetObjectPtr(&associated);
	GameObject* tmp = new GameObject();
	Sprite* sprite = new Sprite(*tmp, "assets/img/cubngun.png");
	tmp->box.h = sprite->GetHeight();
	tmp->box.w = sprite->GetWidth();
	tmp->box.SetCentro(associated.box.GetVecCentralizado());
	tmp->AddComponent(sprite);
	tmp->AddComponent(new PenguinCannon(penguinPass, *tmp));
	pcannon = Game::GetInstance().GetState().AddObject(tmp);
}
void PenguinBody::Update(float dt){
	if (InputManager::GetInstance().IsKeyDown(W_KEY) && linearSpeed < MAXSPEED)
		linearSpeed += 10;
	else if (InputManager::GetInstance().IsKeyDown(S_KEY) && linearSpeed > -MAXSPEED)
		linearSpeed -= 10;
	if (InputManager::GetInstance().IsKeyDown(A_KEY))
		angle -= 1 * dt;
	else if (InputManager::GetInstance().IsKeyDown(D_KEY))
		angle += 1 * dt;
	speed = Vec2(linearSpeed, 0).Rotate(angle);



	associated.angleDeg = angle * 180 / PI;

	Vec2 deslocamento = speed*dt;

	associated.box.x += deslocamento.x;
	associated.box.y += deslocamento.y;

	if (hp <= 0 && !dead) {
		Camera::Unfollow();

		auto cannon = pcannon.lock();
	
		Sprite* sprite = new Sprite(associated, "assets/img/penguindeath.png", 5, 0.2, 1);
		associated.box.w = sprite->GetWidth();
		associated.box.h = sprite->GetHeight();
		associated.box.SetCentro(associated.box.GetVecCentralizado());
		associated.AddComponent(sprite);
		Sound* boom = new Sound(associated, "assets/audio/boom.wav");
		boom->Play();
		associated.AddComponent(boom);
		dead = true;
		linearSpeed = 0;
	}
}
void PenguinBody::Render(){}

bool PenguinBody::Is(string type) {
	return type == "PenguinBody";
}
void PenguinBody::NotifyCollision(GameObject& other) {
	Bullet* bullet = (Bullet*)other.GetComponent("Bullet");
	if (bullet != nullptr && bullet->targetsPlayer) {
		hp -= bullet->GetDamage();
	}
	/*Alien* alien = (Alien*)other.GetComponent("Alien");
	if (alien != nullptr)
		linearSpeed = 0;*/
}
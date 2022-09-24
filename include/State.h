#ifndef STATE_H
#define STATE_H
#include "Sprite.h"
#include "Music.h"
#include "Vec2.h"
#include "Sound.h"
#include "TileMap.h"
#include "TileSet.h"
#include <memory>
#include <vector>
#if defined (_WIN64) || defined (_WIN32)
#include "SDL2/SDL.h"
#else
#include <SDL2/SDL.h>
#endif
using namespace std;

class State {
    
    bool started;
    void Input();
    Music* music;
    bool quitRequested;
    vector<shared_ptr<GameObject>> objectArray;

public:
    void Start();
    weak_ptr<GameObject> AddObject(GameObject* go);
    weak_ptr<GameObject> GetObjectPtr(GameObject* go);
    ~State();
    State();
    bool QuitRequested();
    void LoadAssets();
    void Update(float dt);
    void Render();
};
#endif
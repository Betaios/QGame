#ifndef _Q_BASIC_H_
#define _Q_BASIC_H_

#include <lua.hpp>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

extern SDL_Window* window;
extern SDL_Renderer* renderer;

// ��ʼ��QGame��
// �޲���
// �޷���ֵ
extern "C" int init(lua_State * L);

// �˳�QGame��
// �޲���
// �޷���ֵ
extern "C" int quit(lua_State * L);

// ��ͣ����
// �޲���
// �޷���ֵ
extern "C" int pause(lua_State * L);


#endif // !_Q_BASIC_H_


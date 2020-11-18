#ifndef _MAIN_H_
#define _MAIN_H_

#include <lua.hpp>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "Q_Basic.h"
#include "Q_Window.h"
#include "Q_Graphic.h"
#include "Q_Media.h"
#include "Q_Interactivity.h"
#include "Q_Time.h"
#include "Q_Others.h"

#include <vector>
#include <string>
using namespace std;

SDL_Event event;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Color font_color = { 255, 255, 255, 255 };
TimerCallbackParam* pTimerCallbackParam = NULL;

vector<Mix_Music*>			vMusic;		// �Ѽ��ص������ļ�
vector<Mix_Chunk*>			vSound;		// �Ѽ��ص���Ч�ļ�
vector<QGImage>				vImage;		// �Ѽ��ص�ͼ���ļ�
vector<TTF_Font*>			vFont;		// �Ѽ��ص������ļ�
vector<TimerCallbackParam*>	vParam;		// ��ʱ���ص����������ṹ��

#endif // !_MAIN_H_


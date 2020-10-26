#include <lua.hpp>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <vector>
#include <string>
using namespace std;

struct QGImage
{
	SDL_Texture* image = NULL;
	int width = 0;
	int height = 0;
};	// ͼ�����ݽṹ��

SDL_Event event;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Color font_color = { 255, 255, 255, 255 };

vector<Mix_Music*> vMusic;	// �Ѽ��ص������ļ�
vector<Mix_Chunk*> vSound;	// �Ѽ��ص���Ч�ļ�
vector<QGImage> vImage;		// �Ѽ��ص�ͼ���ļ�
vector<TTF_Font*> vFont;	// �Ѽ��ص������ļ�

// ��ʼ��QGame��
// �޲���
// �޷���ֵ
extern "C" int init(lua_State * L)
{
	TTF_Init();
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF);
	Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	return 0;
}

// �˳�QGame��
// �޲���
// �޷���ֵ
extern "C" int quit(lua_State * L)
{
	IMG_Quit();
	Mix_CloseAudio();
	Mix_Quit();
	TTF_Quit();
	SDL_Quit();
	return 0;
}

// ��ȡ�������е�Ŀ¼
// �޲���
// 1����ֵ����������Ŀ¼
extern "C" int getBasePath(lua_State * L)
{
	lua_pushstring(L, SDL_GetBasePath());
	return 1;
}

// ���ü��а��ı�
// 1���������а��ı�
// �޷���ֵ
extern "C" int setClipboardText(lua_State * L)
{
	const char* text = lua_tostring(L, 1);
	SDL_SetClipboardText(text);
	return 1;
}

// ��ȡ���а��ı�
// �޲���
// 1����ֵ�����а��ı�
extern "C" int getClipboardText(lua_State * L)
{
	lua_pushstring(L, SDL_GetClipboardText());
	return 1;
}

// ��ȡƽ̨����
// �޲���
// 1����ֵ��ƽ̨���ͣ��ַ�����
extern "C" int getPlatform(lua_State * L)
{
	lua_pushstring(L, SDL_GetPlatform());
	return 1;
}

// ��ȡϵͳ���ڴ��С
// �޲���
// 1����ֵ��ϵͳ�ڴ棨MB��
extern "C" int getSystemRAM(lua_State * L)
{
	lua_pushnumber(L, SDL_GetSystemRAM());
	return 1;
}

// ��ʾģ̬����ʾ��Ϣ����
// 3�������������ͣ����󡢾��桢��ʾ�������⣬��ʾ��Ϣ����
// �޷���ֵ
extern "C" int showMessageBox(lua_State * L)
{
	SDL_MessageBoxFlags flag;
	if (!strcmp(lua_tostring(L, 1), "ERROR"))
	{
		flag = SDL_MESSAGEBOX_ERROR;
	}
	else if (!strcmp(lua_tostring(L, 1), "WARNING"))
	{
		flag = SDL_MESSAGEBOX_WARNING;
	}
	else
	{
		flag = SDL_MESSAGEBOX_INFORMATION;
	}
	SDL_ShowSimpleMessageBox(flag, lua_tostring(L, 2), lua_tostring(L, 3), window);
	return 0;
}

// �������ڣ������˴�����Ϊͼ�λ��ƵȲ�����������
// 5���������ڱ��⣬����x��y���꣨��ֵΪĬ��λ�ã������ڿ�Ⱥ͸߶ȣ��������Ա�־����Ӧ��ϵ���·�����ע�ͣ�
// �޷���ֵ
extern "C" int createWindow(lua_State * L)
{
	const char* title = lua_tostring(L, 1);
	int x = lua_tonumber(L, 2) < 0 ? SDL_WINDOWPOS_UNDEFINED : lua_tonumber(L, 2);
	int y = lua_tonumber(L, 3) < 0 ? SDL_WINDOWPOS_UNDEFINED : lua_tonumber(L, 3);
	int width = lua_tonumber(L, 4);
	int height = lua_tonumber(L, 5);
	const char* flags_str = lua_tostring(L, 6);
	int flags = SDL_WINDOW_SHOWN;
	if (flags_str)
	{
		char flags_array[10]; strcpy_s(flags_array, 10, flags_str);
		for (auto flag : flags_array)
		{
			switch (toupper(flag))
			{
			case 'F':
				flags |= SDL_WINDOW_FULLSCREEN;	// fullscreen window	
				break;
			case 'D':
				flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;	// fullscreen window at the current desktop resolution
				break;
			case 'B':
				flags |= SDL_WINDOW_BORDERLESS;	// no window decoration
				break;
			case 'R':
				flags |= SDL_WINDOW_RESIZABLE;	// window can be resized
				break;
			case 'M':
				flags |= SDL_WINDOW_MAXIMIZED;	// window is maximized
				break;
			case 'N':
				flags |= SDL_WINDOW_MINIMIZED;	// window is minimized
				break;
			default:
				break;
			}
		}
	}
	window = SDL_CreateWindow(title, x, y, width, height, flags);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);;
	return 0;
}

// ���ô��ڱ���
// 1��������������
// �޷���ֵ
extern "C" int setWindowTitle(lua_State * L)
{
	SDL_SetWindowTitle(window, lua_tostring(L, 1));
	return 0;
}

// ��ȡ���ڱ���
// �޲���
// 1����ֵ����������
extern "C" int getWindowTitle(lua_State * L)
{
	lua_pushstring(L, SDL_GetWindowTitle(window));
	return 1;
}

// ���ô���ģʽ
// 1������ȫ����־��0������ģʽ��1����Ƶģʽ�ı����ȫ����2��ʹ�õ�ǰ��Ļ�ֱ��ʵļ�ȫ����
// �޷���ֵ
extern "C" int setWindowMode(lua_State * L)
{
	int flag = lua_tonumber(L, 1);
	switch (flag)
	{
	case 0:
		SDL_SetWindowFullscreen(window, 0);
		break;
	case 1:
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
		break;
	case 2:
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		break;
	}
	return 0;
}

// ���ô����Ƿ��С�ɱ�
// 1�������Ƿ��С�ɱ�
// �޷���ֵ
extern "C" int setWindowResizable(lua_State * L)
{
	SDL_SetWindowResizable(window, lua_toboolean(L, 1) ? SDL_TRUE : SDL_FALSE);
	return 0;
}

// ���ô���͸����
// 1������͸���ȴ�С��0-1��
// �޷���ֵ
extern "C" int setWindowOpacity(lua_State * L)
{
	SDL_SetWindowOpacity(window, lua_tonumber(L, 1));
	return 0;
}

// ���ô��ڴ�С
// 2���������ڵĿ�͸�
// �޷���ֵ
extern "C" int setWindowSize(lua_State * L)
{
	SDL_SetWindowSize(window, lua_tonumber(L, 1), lua_tonumber(L, 2));
	return 0;
}

// ��ȡ���ڴ�С
// �޲���
// 2����ֵ�����ڵĿ�͸�
extern "C" int getWindowSize(lua_State * L)
{
	int width = 0;
	int height = 0;
	SDL_GetWindowSize(window, &width, &height);
	lua_pushnumber(L, width);
	lua_pushnumber(L, height);
	return 2;
}

// ���ô������ߴ�Ĵ�С
// 2���������ڿ�ȵ����ֵ�͸߶ȵ����ֵ
// �޷���ֵ
extern "C" int setWindowMaxSize(lua_State * L)
{
	SDL_SetWindowMaximumSize(window, lua_tonumber(L, 1), lua_tonumber(L, 2));
	return 0;
}

// ��ȡ�������ߴ�Ĵ�С
// �޲���
// 2����ֵ�����ڶȵ����ֵ�͸߶ȵ����ֵ
extern "C" int getWindowMaxSize(lua_State * L)
{
	int width = 0;
	int height = 0;
	SDL_GetWindowMaximumSize(window, &width, &height);
	lua_pushnumber(L, width);
	lua_pushnumber(L, height);
	return 2;
}

// ���ô�����С�ߴ�Ĵ�С
// 2���������ڿ�ȵ���Сֵ�͸߶ȵ���Сֵ
// �޷���ֵ
extern "C" int setWindowMinSize(lua_State * L)
{
	SDL_SetWindowMinimumSize(window, lua_tonumber(L, 1), lua_tonumber(L, 2));
	return 0;
}

// ��ȡ������С�ߴ�Ĵ�С
// �޲���
// 2����ֵ�����ڶȵ���Сֵ�͸߶ȵ���Сֵ
extern "C" int getWindowMinSize(lua_State * L)
{
	int width = 0;
	int height = 0;
	SDL_GetWindowMinimumSize(window, &width, &height);
	lua_pushnumber(L, width);
	lua_pushnumber(L, height);
	return 2;
}

// ���ô�������
// 2����������λ�õ�x��y����
// �޷���ֵ
extern "C" int setWindowPosition(lua_State * L)
{
	SDL_SetWindowPosition(window, lua_tonumber(L, 1), lua_tonumber(L, 2));
	return 0;
}

// ��ȡ��������
// �޲���
// 2����ֵ������λ�õ�x��y����
extern "C" int getWindowPosition(lua_State * L)
{
	int x = 0;
	int y = 0;
	SDL_GetWindowSize(window, &x, &y);
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	return 2;
}

// ʹ�õ�ǰ��ͼ��ɫ��մ�������
// �޲���
// �޷���ֵ
extern "C" int clear(lua_State * L)
{
	SDL_RenderClear(renderer);
 
	return 0;
}

// ��������Ƿ���ʾ
// 1�������Ƿ���ʾ
// �޷���ֵ
extern "C" int setCursorShow(lua_State * L)
{
	SDL_ShowCursor(lua_toboolean(L, 1) ? SDL_ENABLE : SDL_DISABLE);
	return 0;
}

// ��ȡ���λ��
// �޲���
// 2����ֵ��λ�õ�x��y����
extern "C" int getCursorPosition(lua_State * L)
{
	lua_pushnumber(L, event.motion.x);
	lua_pushnumber(L, event.motion.y);
	return 2;
}

// ��ȡ�����ֹ�������
// �޲���
// 1����ֵ�����ֹ�������
extern "C" int getScrollValue(lua_State * L)
{
	lua_pushnumber(L, abs(event.wheel.y) > 0 ? abs(event.wheel.y) : abs(event.wheel.x));
	return 1;
}

// ����ͼ���ļ�
// 1������ͼ���ļ�·��
// 1����ֵ��-1�������ļ�ʧ�ܣ��������Ѽ��ص�ͼ���ļ���ͼ����еı�־
extern "C" int loadImage(lua_State * L)
{
	const char* path = lua_tostring(L, 1);
	SDL_Surface* surface = IMG_Load(path);
	if (!surface)
	{
		lua_pushnumber(L, -1);
	}
	else
	{
		QGImage image;
		image.image = SDL_CreateTextureFromSurface(renderer, surface);
		image.width = surface->w;
		image.height = surface->h;
		vImage.push_back(image);
		lua_pushnumber(L, vImage.size() ? vImage.size() - 1 : 0);
	}
	SDL_FreeSurface(surface);
	return 1;
}

// ����ʹ��ColorKey��ʾ͸�������ͼ���ļ�
// 4������ͼ���ļ�·����ColorKey��R��G��B����
// 1����ֵ��-1�������ļ�ʧ�ܣ��������Ѽ��ص�ͼ���ļ���ͼ����еı�־
extern "C" int loadColorKeyImage(lua_State * L)
{
	const char* path = lua_tostring(L, 1);
	int R = lua_tonumber(L, 2);
	int G = lua_tonumber(L, 3);
	int B = lua_tonumber(L, 4);
	SDL_Surface* surface = IMG_Load(path);
	if (!surface)
	{
		lua_pushnumber(L, -1);
	}
	else
	{
		QGImage image;
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, R, G, B));
		image.image = SDL_CreateTextureFromSurface(renderer, surface);
		image.width = surface->w;
		image.height = surface->h;
		vImage.push_back(image);
		lua_pushnumber(L, vImage.size() ? vImage.size() - 1 : 0);
	}
	SDL_FreeSurface(surface);
	return 1;
}

// �ͷ��Ѽ��ص�ͼ��
// 1�������Ѽ��ص�ͼ���ļ���ͼ����еı�־
// 1����ֵ��-1���ͷ�ͼ��ʧ�ܣ�0���ͷ�ͼ��ɹ�
extern "C" int unloadImage(lua_State * L)
{
	int index = lua_tonumber(L, 1);
	if (index >= vImage.size() || index < 0)
	{
		lua_pushnumber(L, -1);
	}
	else
	{
		SDL_DestroyTexture(vImage[index].image);
		lua_pushnumber(L, 0);
	}
	return 1;
}

// �����Ѽ���ͼ���͸����
// 2�������Ѽ��ص�ͼ���ļ���ͼ����еı�־��͸������ֵ��0-255��
// 1����ֵ��-1������ͼ��͸����ʧ�ܣ�0������ͼ��͸���ȳɹ�
extern "C" int setImageAlpha(lua_State * L)
{
	int index = lua_tonumber(L, 1);
	int alpha = lua_tonumber(L, 2);
	if (index >= vImage.size() || index < 0)
	{
		lua_pushnumber(L, -1);
	}
	else
	{
		SDL_SetTextureBlendMode(vImage[index].image, SDL_BLENDMODE_BLEND);
		SDL_SetTextureAlphaMod(vImage[index].image, alpha);
		lua_pushnumber(L, 0);
	}
	return 1;
}

// ��ȡ�Ѽ���ͼ��ߴ�
// 1�������Ѽ��ص�ͼ���ļ���ͼ����еı�־
// 2����ֵ��-1��-1����ȡͼ��ߴ�ʧ�ܣ��ɹ��򷵻�ͼ��ĳ��Ϳ�
extern "C" int getImageSize(lua_State * L)
{
	int index = lua_tonumber(L, 1);
	if (index >= vImage.size() || index < 0)
	{
		lua_pushnumber(L, -1);
		lua_pushnumber(L, -1);
	}
	else
	{
		lua_pushnumber(L, vImage[index].width);
		lua_pushnumber(L, vImage[index].height);
	}
	return 2;
}

// ��ʾ�Ѽ��ص�ͼ��
// 5�������Ѽ��ص�ͼ���ļ���ͼ����еı�־����ʾλ��x����ʾλ��y����ʾ��ȣ���ʾ�߶ȣ�-1ΪĬ�ϳߴ磩
// 1����ֵ��-1����ʾͼ��ʧ�ܣ�0����ʾͼ��ɹ�
extern "C" int showImage(lua_State * L)
{
	int index = lua_tonumber(L, 1);
	if (index >= vImage.size() || index < 0)
	{
		lua_pushnumber(L, -1);
	}
	else
	{
		SDL_Rect rect = { lua_tonumber(L, 2) , lua_tonumber(L, 3) , lua_tonumber(L, 4) < 0 ? vImage[index].width : lua_tonumber(L, 4) , lua_tonumber(L, 5) < 0 ? vImage[index].height : lua_tonumber(L, 5) };
		SDL_RenderCopy(renderer, vImage[index].image, NULL, &rect);
	 
		lua_pushnumber(L, 0);
	}
	return 1;
}

// ��ʾ��ת����Ѽ���ͼ��
// 9�������Ѽ��ص�ͼ���ļ���ͼ����еı�־��˳ʱ����ת�Ƕȣ���ת����x���꣬��ת����y���꣬��תģʽ����ʾλ��x����ʾλ��y����ʾ��ȣ�-1ΪĬ�ϳߴ磩����ʾ�߶ȣ�-1ΪĬ�ϳߴ磩
// 1����ֵ��-1����ʾͼ��ʧ�ܣ�0����ʾͼ��ɹ�
extern "C" int showRotateImage(lua_State * L)
{
	int index = lua_tonumber(L, 1);
	if (index >= vImage.size() || index < 0)
	{
		lua_pushnumber(L, -1);
	}
	else
	{
		SDL_Rect rect = { lua_tonumber(L, 6) , lua_tonumber(L, 7) , lua_tonumber(L, 8) < 0 ? vImage[index].width : lua_tonumber(L, 8) , lua_tonumber(L, 9) < 0 ? vImage[index].height : lua_tonumber(L, 9) };
		SDL_Point point = { lua_tonumber(L, 3), lua_tonumber(L, 4) };
		SDL_RendererFlip flip;
		
		if (!strcmp(lua_tostring(L, 5), "HORIZONTAL"))
		{
			flip = SDL_FLIP_HORIZONTAL;
		}
		else if (!strcmp(lua_tostring(L, 5), "VERTICAL"))
		{
			flip = SDL_FLIP_VERTICAL;
		}
		else
		{
			flip = SDL_FLIP_NONE;
		}

		SDL_RenderCopyEx(renderer, vImage[index].image, NULL, &rect, lua_tonumber(L, 2), &point, flip);

		lua_pushnumber(L, 0);
	}
	return 1;
}

// ��ʾ�ü�����Ѽ���ͼ��
// 9�������Ѽ��ص�ͼ���ļ���ͼ����еı�־���ü�����x���ü�����y���ü������ȣ�-1ΪĬ�ϳߴ磩���ü�����߶ȣ�-1ΪĬ�ϳߴ磩����ʾλ��x����ʾλ��y����ʾ��ȣ�-1ΪĬ�ϳߴ磩����ʾ�߶ȣ�-1ΪĬ�ϳߴ磩
// 1����ֵ��-1����ʾͼ��ʧ�ܣ�0����ʾͼ��ɹ�
extern "C" int showReshapeImage(lua_State * L)
{
	int index = lua_tonumber(L, 1);
	if (index >= vImage.size() || index < 0)
	{
		lua_pushnumber(L, -1);
	}
	else
	{
		SDL_Rect reshapeRect = { lua_tonumber(L, 2) , lua_tonumber(L, 3) , lua_tonumber(L, 4) < 0 ? vImage[index].width : lua_tonumber(L, 4) , lua_tonumber(L, 5) < 0 ? vImage[index].height : lua_tonumber(L, 5) };
		SDL_Rect showRect = { lua_tonumber(L, 6) , lua_tonumber(L, 7) , lua_tonumber(L, 8) < 0 ? vImage[index].width : lua_tonumber(L, 8) , lua_tonumber(L, 9) < 0 ? vImage[index].width : lua_tonumber(L, 9) };
		SDL_RenderCopy(renderer, vImage[index].image, &reshapeRect, &showRect);

		lua_pushnumber(L, 0);
	}
	return 1;
}

// ��ʾ��ת�Ҳü�����Ѽ���ͼ��
// 13�������Ѽ��ص�ͼ���ļ���ͼ����еı�־����ת�Ƕȣ���ת����x���꣬��ת����y���꣬��תģʽ���ü�����x���ü�����y���ü������ȣ�-1ΪĬ�ϳߴ磩���ü�����߶ȣ�-1ΪĬ�ϳߴ磩����ʾλ��x����ʾλ��y����ʾ��ȣ�-1ΪĬ�ϳߴ磩����ʾ�߶ȣ�-1ΪĬ�ϳߴ磩
// 1����ֵ��-1����ʾͼ��ʧ�ܣ�0����ʾͼ��ɹ�
extern "C" int showRotateReshapeImage(lua_State * L)
{
	int index = lua_tonumber(L, 1);
	if (index >= vImage.size() || index < 0)
	{
		lua_pushnumber(L, -1);
	}
	else
	{
		SDL_Rect reshapeRect = { lua_tonumber(L, 6) , lua_tonumber(L, 7) , lua_tonumber(L, 8) < 0 ? vImage[index].width : lua_tonumber(L, 8) , lua_tonumber(L, 9) < 0 ? vImage[index].height : lua_tonumber(L, 9) };
		SDL_Rect showRect = { lua_tonumber(L, 10) , lua_tonumber(L, 11) , lua_tonumber(L, 12) < 0 ? vImage[index].width : lua_tonumber(L, 12) , lua_tonumber(L, 13) < 0 ? vImage[index].width : lua_tonumber(L, 13) };
		SDL_Point point = { lua_tonumber(L, 3), lua_tonumber(L, 4) };
		SDL_RendererFlip flip;

		if (!strcmp(lua_tostring(L, 5), "HORIZONTAL"))
		{
			flip = SDL_FLIP_HORIZONTAL;
		}
		else if (!strcmp(lua_tostring(L, 5), "VERTICAL"))
		{
			flip = SDL_FLIP_VERTICAL;
		}
		else
		{
			flip = SDL_FLIP_NONE;
		}

		SDL_RenderCopyEx(renderer, vImage[index].image, &reshapeRect, &showRect, lua_tonumber(L, 2), &point, flip);
	 
		lua_pushnumber(L, 0);
	}
	return 1;
}

// ���ô��ڻ�ͼ��ɫ
// 4��������ɫ��R��G��B��A������ȡֵ��Χ��Ϊ0-255��
// �޷���ֵ
extern "C" int setDrawColor(lua_State * L)
{
	int R = lua_tonumber(L, 1);
	int G = lua_tonumber(L, 2);
	int B = lua_tonumber(L, 3);
	int A = lua_tonumber(L, 4);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, R, G, B, A);
 
	return 0;
}

// ��ָ��λ�û��Ƶ�
// 2�����������ڵ�x��y����
// �޷���ֵ
extern "C" int point(lua_State * L)
{
	int x = lua_tonumber(L, 1);
	int y = lua_tonumber(L, 2);
	SDL_RenderDrawPoint(renderer, x, y);
 
	return 0;
}

// ��ָ��λ�û���ֱ��
// 4���������x��y���꣬�յ�x��y����
// �޷���ֵ
extern "C" int line(lua_State * L)
{
	int x1 = lua_tonumber(L, 1);
	int y1 = lua_tonumber(L, 2);
	int x2 = lua_tonumber(L, 3);
	int y2 = lua_tonumber(L, 4);
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
 
	return 0;
}

// ��ָ��λ�û�����������
// 4�������������Ͻ�x��y���꣬���ο�͸�	// 1������-1δ�����������
// �޷���ֵ
extern "C" int rectangle(lua_State * L)
{
	SDL_Rect rect = { lua_tonumber(L, 1) , lua_tonumber(L, 2) , lua_tonumber(L, 3) , lua_tonumber(L, 4) };
	SDL_RenderDrawRect(renderer, &rect);
 
	return 0;
}

// ��ָ��λ�û���������
// 4�������������Ͻ�x��y���꣬���ο�͸�
// �޷���ֵ
extern "C" int fillRectangle(lua_State * L)
{
	SDL_Rect rect = { lua_tonumber(L, 1) , lua_tonumber(L, 2) , lua_tonumber(L, 3) , lua_tonumber(L, 4) };
	SDL_RenderFillRect(renderer, &rect);
 
	return 0;
}

// ����Ⱦ������������ˢ�µ���Ļ��
// �޲���
// �޷���ֵ
extern "C" int update(lua_State * L)
{
	SDL_RenderPresent(renderer);
	return 0;
}

// ���������ļ�
// 1�����������ļ�·��
// 1����ֵ��-1�������ļ�ʧ�ܣ��������Ѽ��ص������ļ������ֿ��еı�־
extern "C" int loadMusic(lua_State * L)
{
	const char* path = lua_tostring(L, 1);
	Mix_Music* mm = Mix_LoadMUS(path);
	if (!mm)
	{
		lua_pushnumber(L, -1);
	}
	else
	{
		vMusic.push_back(mm);
		lua_pushnumber(L, vMusic.size() ? vMusic.size() - 1 : 0);
	}
	return 1;
}

// �ͷ��Ѽ��ص�����
// 1�������Ѽ��ص������ļ������ֿ��еı�־
// 1����ֵ��-1���ͷ�����ʧ�ܣ�0���ͷ����ֳɹ�
extern "C" int unloadMusic(lua_State * L)
{
	int index = lua_tonumber(L, 1);
	if (index >= vMusic.size() || index < 0)
	{
		lua_pushnumber(L, -1);
	}
	else
	{
		Mix_FreeMusic(vMusic[index]);
		lua_pushnumber(L, 0);
	}
	return 1;
}

// �����Ѽ��ص�����
// 2�������Ѽ��ص������ļ������ֿ��еı�־�����ֲ��ŵĴ�����-1Ϊѭ�����ţ�
// 1����ֵ��-1����������ʧ�ܣ�0���������ֳɹ�
extern "C" int playMusic(lua_State * L)
{
	int index = lua_tonumber(L, 1);
	int loops = lua_tonumber(L, 2);
	if (index >= vMusic.size() || index < 0)
	{
		lua_pushnumber(L, -1);
	}
	else
	{
		Mix_PlayMusic(vMusic[index], loops);
		lua_pushnumber(L, 0);
	}
	return 1;
}

// ���ŵ�������
// 3�������Ѽ��ص������ļ������ֿ��еı�־�����ֲ��ŵĴ�����������Ч����ʱ�䣨ms��
// 1������-1����������ʧ�ܣ�0���������ֳɹ�
extern "C" int fadeInMusic(lua_State * L)
{
	int index = lua_tonumber(L, 1);
	int loops = lua_tonumber(L, 2);
	int delay = lua_tonumber(L, 3);
	if (index >= vMusic.size() || index < 0)
	{
		lua_pushnumber(L, -1);
	}
	else
	{
		Mix_FadeInMusic(vMusic[index], loops, delay);
		lua_pushnumber(L, 0);
	}
	return 1;
}

// ������ֹͣ��ǰ���ڲ��ŵ�����
// 1������������Ч����ʱ�䣨ms��
// �޷���ֵ
extern "C" int fadeOutMusic(lua_State * L)
{
	int delay = lua_tonumber(L, 1);
	Mix_FadeOutMusic(delay);
	return 0;
}

// �������ֲ��ŵ�����
// 1������������С��0-128��
// �޷���ֵ
extern "C" int setMusicVolume(lua_State * L)
{
	int value = lua_tonumber(L, 1);
	Mix_VolumeMusic(value);
	return 0;
}

// ��ȡ��ǰ��������
// �޲���
// 1����ֵ����ǰ����������С
extern "C" int getMusicVolume(lua_State * L)
{
	int value = Mix_VolumeMusic(-1);
	lua_pushnumber(L, value);
	return 1;
}

// ��ͣ��ǰ���ڲ��ŵ�����
// �޲���
// �޷���ֵ
extern "C" int pauseMusic(lua_State * L)
{
	Mix_PauseMusic();
	return 0;
}

// �ָ���ǰ��ͣ״̬������
// �޲���
// �޷���ֵ
extern "C" int resumeMusic(lua_State * L)
{
	Mix_ResumeMusic();
	return 0;
}

// �ز���ǰ���ڲ��ŵ�����
// �޲���
// �޷���ֵ
extern "C" int rewindMusic(lua_State * L)
{
	Mix_RewindMusic();
	return 0;
}

// ��ȡָ�����ֵ�����
// 1�������Ѽ��ص������ļ������ֿ��еı�־��-1��ʾ���ڲ��ŵ����֣�
// 1����ֵ���ɹ��򷵻�ָ�����ֵ����ͣ��ַ�������δ֪�򷵻�"UNKONWN"��ʧ���򷵻�"ERROR"
extern "C" int getMusicType(lua_State * L)
{
	int index = lua_tonumber(L, 1);
	Mix_MusicType type;
	if (index < 0)
	{
		type = Mix_GetMusicType(NULL);
	}
	else
	{
		if (index >= vMusic.size())
		{
			lua_pushstring(L, "ERROR");
		}
		else
		{
			type = Mix_GetMusicType(vMusic[index]);
		}
	}
	switch (type)
	{
	case MUS_WAV:
		lua_pushstring(L, "WAV");
		break;
	case MUS_MP3:
		lua_pushstring(L, "MP3");
		break;
	case MUS_OGG:
		lua_pushstring(L, "OGG");
		break;
	case MUS_CMD:
		lua_pushstring(L, "CMD");
		break;
	case MUS_MOD:
		lua_pushstring(L, "MOD");
		break;
	case MUS_MID:
		lua_pushstring(L, "MID");
		break;
	default:
		lua_pushstring(L, "UNKONWN");
		break;
	}
	return 1;
}

// ������Ч�ļ�
// 1��������Ч�ļ�·��
// 1����ֵ��-1��������Ч�ļ�ʧ�ܣ��������Ѽ��ص���Ч�ļ�����Ч���еı�־
// ��ע����Ч�ļ�֧�����¸�ʽ��WAVE, AIFF, RIFF, OGG, VOC
extern "C" int loadSound(lua_State * L)
{
	const char* path = lua_tostring(L, 1);
	Mix_Chunk* mc = Mix_LoadWAV(path);
	if (!mc)
	{
		lua_pushnumber(L, -1);
	}
	else
	{
		vSound.push_back(mc);
		lua_pushnumber(L, vSound.size() ? vSound.size() - 1 : 0);
	}
	return 1;
}

// �ͷ��Ѽ��ص���Ч
// 1�������Ѽ��ص���Ч�ļ������ֿ��еı�־
// 1����ֵ��-1���ͷ���Чʧ�ܣ�0���ͷ���Ч�ɹ�
extern "C" int unloadSound(lua_State * L)
{
	int index = lua_tonumber(L, 1);
	if (index >= vSound.size() || index < 0)
	{
		lua_pushnumber(L, -1);
	}
	else
	{
		Mix_FreeChunk(vSound[index]);
		lua_pushnumber(L, 0);
	}
	return 1;
}

// �����Ѽ��ص���Ч
// 2�������Ѽ��ص���Ч�ļ�����Ч���еı�־����Ч���ŵĴ�����-1Ϊѭ�����ţ�
// 1����ֵ��-1����������ʧ�ܣ�0���������ֳɹ�
extern "C" int playSound(lua_State * L)
{
	int index = lua_tonumber(L, 1);
	int loops = lua_tonumber(L, 2);

	if (index >= vMusic.size())
	{
		lua_pushnumber(L, -1);
	}
	else
	{
		if (loops > 0)
		{
			Mix_PlayChannel(-1, vSound[index], loops - 1);
		}
		else
		{
			Mix_PlayChannel(-1, vSound[index], loops);
		}
		lua_pushnumber(L, 0);
	}
	return 1;
}

// ���������ļ�
// 2�����������ļ�·���������С
// 1����ֵ��-1�������ļ�ʧ�ܣ��������Ѽ��ص������ļ���������еı�־
extern "C" int loadFont(lua_State * L)
{
	const char* path = lua_tostring(L, 1);
	int size = lua_tonumber(L, 2);
	TTF_Font* tf = TTF_OpenFont(path, size);
	if (!tf)
	{
		lua_pushnumber(L, -1);
	}
	else
	{
		vFont.push_back(tf);
		lua_pushnumber(L, vFont.size() ? vFont.size() - 1 : 0);
	}
	return 1;
}

// �ͷ��Ѽ��ص�����
// 1�������Ѽ��ص������ļ���������еı�־
// 1����ֵ��-1���ͷ�����ʧ�ܣ�0���ͷ�����ɹ�
extern "C" int unloadFont(lua_State * L)
{
	int index = lua_tonumber(L, 1);
	if (index >= vFont.size() || index < 0)
	{
		lua_pushnumber(L, -1);
	}
	else
	{
		TTF_CloseFont(vFont[index]);
		lua_pushnumber(L, 0);
	}
	return 1;
}

// �������������ɫ
// 4������������ɫ��R��G��B��A������0-255��
// �޷���ֵ
extern "C" int setTextColor(lua_State * L)
{
	font_color.r = lua_tonumber(L, 1);
	font_color.g = lua_tonumber(L, 2);
	font_color.b = lua_tonumber(L, 3);
	font_color.a = lua_tonumber(L, 4);
	return 0;
}

// ��ָ��λ�û�������
// 6�������Ѽ��ص������ļ���������еı�־���ı����ݣ�����λ��x��y���꣬���ƿ�Ⱥ͸߶ȣ�-1ΪĬ�ϳߴ磩
// 1����ֵ��-1����������ʧ�ܣ�0����������ɹ�
extern "C" int drawText(lua_State * L)
{
	int index = lua_tonumber(L, 1);
	const char* text = lua_tostring(L, 2);
	int x = lua_tonumber(L, 3);
	int y = lua_tonumber(L, 4);
	int width = lua_tonumber(L, 5);
	int height = lua_tonumber(L, 6);
	if (index >= vFont.size() || index < 0)
	{
		lua_pushnumber(L, -1);
	}
	else
	{
		SDL_Surface* surface = TTF_RenderUTF8_Solid(vFont[index], text, font_color);
		SDL_Rect rect = { x, y, width < 0 ? surface->w : width, height < 0 ? surface->h : height };
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_RenderCopy(renderer, texture, NULL, &rect);
	 
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(surface);
		lua_pushnumber(L, 0);
	}
	return 1;
}


// ��ȡ�������¼�
// �޲���
// 1����ֵ��-1����ǰ�¼������޴�����¼���0����ȡ�����¼�
extern "C" int updateEvent(lua_State * L)
{
	if (SDL_PollEvent(&event))
	{
		lua_pushnumber(L, 0);
	}
	else
	{
		lua_pushnumber(L, -1);
	}
	return 1;
}

// ��ȡ��ǰ�¼�����
// �޲���
// 1����ֵ���¼����ͣ��ַ�����
// ע�⣺���ڲ����ַ������¼���Ӧ�������ڴ����������ֻ��ͨ����ϼ���ɣ�����Щ�ַ��İ����¼��ڴ���������Ͽ����޷�ͨ������������
extern "C" int getEventType(lua_State * L)
{
	string prefix = "";
	string suffix = "";
	string keyResult = "";
	switch (event.type)
	{
	case SDL_QUIT:
		lua_pushstring(L, "QUIT");
		break;
	case SDL_MOUSEMOTION:
		lua_pushstring(L, "MOUSEMOTION");
		break;
	case SDL_MOUSEBUTTONDOWN:
		switch (event.button.button)
		{
		case SDL_BUTTON_LEFT:
			lua_pushstring(L, "MOUSEBUTTON_D_L");
			break;
		case SDL_BUTTON_RIGHT:
			lua_pushstring(L, "MOUSEBUTTON_D_R");
			break;
		case SDL_BUTTON_MIDDLE:
			lua_pushstring(L, "MOUSEBUTTON_D_M");
			break;
		default:
			break;
		}
		break;
	case SDL_MOUSEBUTTONUP:
		switch (event.button.button)
		{
		case SDL_BUTTON_LEFT:
			lua_pushstring(L, "MOUSEBUTTON_U_L");
			break;
		case SDL_BUTTON_RIGHT:
			lua_pushstring(L, "MOUSEBUTTON_U_R");
			break;
		case SDL_BUTTON_MIDDLE:
			lua_pushstring(L, "MOUSEBUTTON_U_M");
			break;
		default:
			break;
		}
		break;
	case SDL_MOUSEWHEEL:
		if (event.wheel.y > 0)
		{
			lua_pushstring(L, "MOUSESCROLL_U");
		}
		else if (event.wheel.y < 0)
		{
			lua_pushstring(L, "MOUSESCROLL_D");
		}
		else if (event.wheel.x > 0)
		{
			lua_pushstring(L, "MOUSESCROLL_R");
		}
		else if (event.wheel.x < 0)
		{
			lua_pushstring(L, "MOUSESCROLL_L");
		}
		break;
	case SDL_WINDOWEVENT:
		switch (event.window.event)
		{
		case SDL_WINDOWEVENT_SHOWN:
			lua_pushstring(L, "WINDOWSHOW");
			break;
		case SDL_WINDOWEVENT_HIDDEN:
			lua_pushstring(L, "WINDOWHIDE");
			break;
		case SDL_WINDOWEVENT_MOVED:
			lua_pushstring(L, "WINDOWMOVE");
			break;
		case SDL_WINDOWEVENT_RESIZED:
			lua_pushstring(L, "WINDOWRESIZE");
			break;
		case SDL_WINDOWEVENT_MINIMIZED:
			lua_pushstring(L, "WINDOWMINSIZE");
			break;
		case SDL_WINDOWEVENT_MAXIMIZED:
			lua_pushstring(L, "WINDOWMAXSIZE");
			break;
		case SDL_WINDOWEVENT_ENTER:
			lua_pushstring(L, "WINDOWENTER");
			break;
		case SDL_WINDOWEVENT_LEAVE:
			lua_pushstring(L, "WINDOWLEAVE");
			break;
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			lua_pushstring(L, "WINDOWFOCUS");
			break;
		case SDL_WINDOWEVENT_FOCUS_LOST:
			lua_pushstring(L, "WINDOWLOST");
			break;
		case SDL_WINDOWEVENT_CLOSE:
			lua_pushstring(L, "WINDOWCLOSE");
			break;
		default:
			break;
		}
		break;
	case SDL_KEYUP:
	case SDL_KEYDOWN:
		prefix = event.type == SDL_KEYUP ? "KEYUP_" : "KEYDOWN_";
		switch (event.key.keysym.sym)
		{
		case SDLK_0:
		case SDLK_KP_0:
			keyResult = prefix + "0";
			break;
		case SDLK_KP_00:
			keyResult = prefix + "00";
			break;
		case SDLK_KP_000:
			keyResult = prefix + "000";
			break;
		case SDLK_1:
		case SDLK_KP_1:
			keyResult = prefix + "1";
			break;
		case SDLK_2:
		case SDLK_KP_2:
			keyResult = prefix + "2";
			break;
		case SDLK_3:
		case SDLK_KP_3:
			keyResult = prefix + "3";
			break;
		case SDLK_4:
		case SDLK_KP_4:
			keyResult = prefix + "4";
			break;
		case SDLK_5:
		case SDLK_KP_5:
			keyResult = prefix + "5";
			break;
		case SDLK_6:
		case SDLK_KP_6:
			keyResult = prefix + "6";
			break;
		case SDLK_7:
		case SDLK_KP_7:
			keyResult = prefix + "7";
			break;
		case SDLK_8:
		case SDLK_KP_8:
			keyResult = prefix + "8";
			break;
		case SDLK_9:
		case SDLK_KP_9:
			keyResult = prefix + "9";
			break;
		case SDLK_F1:
			keyResult = prefix + "F1";
			break;
		case SDLK_F2:
			keyResult = prefix + "F2";
			break;
		case SDLK_F3:
			keyResult = prefix + "F3";
			break;
		case SDLK_F4:
			keyResult = prefix + "F4";
			break;
		case SDLK_F5:
			keyResult = prefix + "F5";
			break;
		case SDLK_F6:
			keyResult = prefix + "F6";
			break;
		case SDLK_F7:
			keyResult = prefix + "F7";
			break;
		case SDLK_F8:
			keyResult = prefix + "F8";
			break;
		case SDLK_F9:
			keyResult = prefix + "F9";
			break;
		case SDLK_F10:
			keyResult = prefix + "F10";
			break;
		case SDLK_F11:
			keyResult = prefix + "F11";
			break;
		case SDLK_F12:
			keyResult = prefix + "F12";
			break;
		case SDLK_F13:
			keyResult = prefix + "F13";
			break;
		case SDLK_F14:
			keyResult = prefix + "F14";
			break;
		case SDLK_F15:
			keyResult = prefix + "F15";
			break;
		case SDLK_F16:
			keyResult = prefix + "F16";
			break;
		case SDLK_F17:
			keyResult = prefix + "F17";
			break;
		case SDLK_F18:
			keyResult = prefix + "F18";
			break;
		case SDLK_F19:
			keyResult = prefix + "F19";
			break;
		case SDLK_F20:
			keyResult = prefix + "F20";
			break;
		case SDLK_F21:
			keyResult = prefix + "F21";
			break;
		case SDLK_F22:
			keyResult = prefix + "F22";
			break;
		case SDLK_F23:
			keyResult = prefix + "F23";
			break;
		case SDLK_F24:
			keyResult = prefix + "F24";
			break;
		case SDLK_a:
		case SDLK_KP_A:
			keyResult = prefix + "A";
			break;
		case SDLK_b:
		case SDLK_KP_B:
			keyResult = prefix + "B";
			break;
		case SDLK_c:
		case SDLK_KP_C:
			keyResult = prefix + "C";
			break;
		case SDLK_d:
		case SDLK_KP_D:
			keyResult = prefix + "D";
			break;
		case SDLK_e:
		case SDLK_KP_E:
			keyResult = prefix + "E";
			break;
		case SDLK_f:
		case SDLK_KP_F:
			keyResult = prefix + "F";
			break;
		case SDLK_g:
			keyResult = prefix + "G";
			break;
		case SDLK_h:
			keyResult = prefix + "H";
			break;
		case SDLK_i:
			keyResult = prefix + "I";
			break;
		case SDLK_j:
			keyResult = prefix + "J";
			break;
		case SDLK_k:
			keyResult = prefix + "K";
			break;
		case SDLK_l:
			keyResult = prefix + "L";
			break;
		case SDLK_m:
			keyResult = prefix + "M";
			break;
		case SDLK_n:
			keyResult = prefix + "N";
			break;
		case SDLK_o:
			keyResult = prefix + "O";
			break;
		case SDLK_p:
			keyResult = prefix + "P";
			break;
		case SDLK_q:
			keyResult = prefix + "Q";
			break;
		case SDLK_r:
			keyResult = prefix + "R";
			break;
		case SDLK_s:
			keyResult = prefix + "S";
			break;
		case SDLK_t:
			keyResult = prefix + "T";
			break;
		case SDLK_u:
			keyResult = prefix + "U";
			break;
		case SDLK_v:
			keyResult = prefix + "V";
			break;
		case SDLK_w:
			keyResult = prefix + "W";
			break;
		case SDLK_x:
			keyResult = prefix + "X";
			break;
		case SDLK_y:
			keyResult = prefix + "Y";
			break;
		case SDLK_z:
			keyResult = prefix + "Z";
			break;
		case SDLK_ESCAPE:
			keyResult = prefix + "ESC";
			break;
		case SDLK_RETURN:
		case SDLK_KP_ENTER:
			keyResult = prefix + "ENTER";
			break;
		case SDLK_BACKSPACE:
		case SDLK_KP_BACKSPACE:
			keyResult = prefix + "BACKSPACE";
			break;
		case SDLK_UP:
			keyResult = prefix + "UP";
			break;
		case SDLK_DOWN:
			keyResult = prefix + "DOWN";
			break;
		case SDLK_LEFT:
			keyResult = prefix + "LEFT";
			break;
		case SDLK_RIGHT:
			keyResult = prefix + "RIGHT";
			break;
		case SDLK_INSERT:
			keyResult = prefix + "INSERT";
			break;
		case SDLK_DELETE:
			keyResult = prefix + "DELETE";
			break;
		case SDLK_HOME:
			keyResult = prefix + "HOME";
			break;
		case SDLK_END:
			keyResult = prefix + "END";
			break;
		case SDLK_PAGEUP:
			keyResult = prefix + "PAGEUP";
			break;
		case SDLK_PAGEDOWN:
			keyResult = prefix + "PAGEDOWN";
			break;
		case SDLK_LCTRL:
			keyResult = prefix + "LCTRL";
			break;
		case SDLK_LGUI:
			keyResult = prefix + "LGUI";
			break;
		case SDLK_LALT:
			keyResult = prefix + "LALT";
			break;
		case SDLK_LSHIFT:
			keyResult = prefix + "LSHIFT";
			break;
		case SDLK_RCTRL:
			keyResult = prefix + "RCTRL";
			break;
		case SDLK_RGUI:
			keyResult = prefix + "RGUI";
			break;
		case SDLK_RALT:
			keyResult = prefix + "RALT";
			break;
		case SDLK_RSHIFT:
			keyResult = prefix + "RSHIFT";
			break;
		case SDLK_SPACE:
			keyResult = prefix + "SPACE";
			break;
		case SDLK_TAB:
		case SDLK_KP_TAB:
			keyResult = prefix + "TAB";
			break;
		case SDLK_CAPSLOCK:
			keyResult = prefix + "CAPSLOCK";
			break;
		case SDLK_NUMLOCKCLEAR:
			keyResult = prefix + "NUMLOCK";
			break;
		case SDLK_PRINTSCREEN:
			keyResult = prefix + "PRINTSCREEN";
			break;
		case SDLK_SCROLLLOCK:
			keyResult = prefix + "SCROLLLOCK";
			break;
		case SDLK_PAUSE:
			keyResult = prefix + "PAUSE";
			break;
		case SDLK_AUDIOMUTE:
			keyResult = prefix + "AUDIOMUTE";
			break;
		case SDLK_AUDIOPREV:
			keyResult = prefix + "AUDIOPREV";
			break;
		case SDLK_AUDIONEXT:
			keyResult = prefix + "AUDIONEXT";
			break;
		case SDLK_AUDIOPLAY:
			keyResult = prefix + "AUDIOPLAY";
			break;
		case SDLK_AUDIOSTOP:
			keyResult = prefix + "AUDIOSTOP";
			break;
		case SDLK_VOLUMEUP:
			keyResult = prefix + "VOLUMEUP";
			break;
		case SDLK_VOLUMEDOWN:
			keyResult = prefix + "VOLUMEDOWN";
			break;
		case SDLK_BRIGHTNESSUP:
			keyResult = prefix + "BRIGHTNESSUP";
			break;
		case SDLK_BRIGHTNESSDOWN:
			keyResult = prefix + "BRIGHTNESSDOWN";
			break;
		case SDLK_BACKQUOTE:
			keyResult = prefix + "BACKQUOTE";
			break;
		case SDLK_EXCLAIM:
		case SDLK_KP_EXCLAM:
			keyResult = prefix + "EXCLAM";
			break;
		case SDLK_AT:
		case SDLK_KP_AT:
			keyResult = prefix + "AT";
			break;
		case SDLK_HASH:
		case SDLK_KP_HASH:
			keyResult = prefix + "HASH";
			break;
		case SDLK_DOLLAR:
			keyResult = prefix + "DOOLAR";
			break;
		case SDLK_CARET:
		case SDLK_KP_POWER:
			keyResult = prefix + "CARET";
			break;
		case SDLK_AMPERSAND:
		case SDLK_KP_AMPERSAND:
			keyResult = prefix + "AMPERSAND";
			break;
		case SDLK_KP_DBLAMPERSAND:
			keyResult = prefix + "DBLAMPERSAND";
			break;
		case SDLK_ASTERISK:
		case SDLK_KP_MULTIPLY:
			keyResult = prefix + "ASTERISK";
			break;
		case SDLK_LEFTPAREN:
		case SDLK_KP_LEFTPAREN:
			keyResult = prefix + "LEFTPAREN";
			break;
		case SDLK_RIGHTPAREN:
		case SDLK_KP_RIGHTPAREN:
			keyResult = prefix + "RIGHTPAREN";
			break;
		case SDLK_MINUS:
		case SDLK_KP_MINUS:
			keyResult = prefix + "MINUS";
			break;
		case SDLK_UNDERSCORE:
			keyResult = prefix + "UNDERSCORE";
			break;
		case SDLK_PLUS:
		case SDLK_KP_PLUS:
			keyResult = prefix + "PLUS";
			break;
		case SDLK_EQUALS:
		case SDLK_KP_EQUALS:
			keyResult = prefix + "EQUALS";
			break;
		case SDLK_LEFTBRACKET:
			keyResult = prefix + "LEFTBRACKET";
			break;
		case SDLK_RIGHTBRACKET:
			keyResult = prefix + "RIGHTBRACKET";
			break;
		case SDLK_KP_LEFTBRACE:
			keyResult = prefix + "LEFTBRACE";
			break;
		case SDLK_KP_RIGHTBRACE:
			keyResult = prefix + "RIGHTBRACE";
			break;
		case SDLK_COLON:
			keyResult = prefix + "COLON";
			break;
		case SDLK_SEMICOLON:
			keyResult = prefix + "SEMICOLON";
			break;
		case SDLK_BACKSLASH:
			keyResult = prefix + "BACKSLASH";
			break;
		case SDLK_QUOTE:
			keyResult = prefix + "QUOTE";
			break;
		case SDLK_QUOTEDBL:
			keyResult = prefix + "QUOTEDBL";
			break;
		case SDLK_LESS:
			keyResult = prefix + "LESS";
			break;
		case SDLK_GREATER:
			keyResult = prefix + "GREATER";
			break;
		case SDLK_COMMA:
			keyResult = prefix + "COMMA";
			break;
		case SDLK_PERIOD:
			keyResult = prefix + "PERIOD";
			break;
		case SDLK_QUESTION:
			keyResult = prefix + "QUESTION";
			break;
		case SDLK_SLASH:
			keyResult = prefix + "SLASH";
			break;
		case SDLK_KP_VERTICALBAR:
			keyResult = prefix + "VERTICALBAR";
			break;
		case SDLK_KP_DBLVERTICALBAR:
			keyResult = prefix + "DBLVERTICALBAR";
			break;
		case SDLK_WWW:
			keyResult = prefix + "WWW";
			break;
		case SDLK_MAIL:
			keyResult = prefix + "EMAIL";
			break;
		default:
			break;
		}
		lua_pushstring(L, keyResult.c_str());
		break;
	default:
		break;
	}
	return 1;
}

// ��ͣ����
// �޲���
// �޷���ֵ
extern "C" int pause(lua_State * L)
{
	system("pause");
	return 0;
}

// �������ָ��ʱ��
// 1����������ʱ�䣨��λ�����룩
// �޷���ֵ
extern "C" int sleep(lua_State * L)
{
	int delay = lua_tonumber(L, 1);
	SDL_Delay(delay);
	return 0;
}

static luaL_Reg cMethods[] = {
	{ "Init", init },
	{ "Quit", quit },
	{ "GetBasePath", getBasePath },
	{ "SetClipboardText", setClipboardText },
	{ "GetClipboardText", getClipboardText },
	{ "GetPlatform", getPlatform },
	{ "GetSystemRAM", getSystemRAM },
	{ "ShowMessageBox", showMessageBox},
	{ "CreateWindow", createWindow },
	{ "SetWindowTitle", setWindowTitle },
	{ "GetWindowTitle", getWindowTitle },
	{ "SetWindowMode", setWindowMode },
	{ "SetWindowResizable", setWindowResizable },
	{ "SetWindowOpacity", setWindowOpacity },
	{ "SetWindowSize", setWindowSize },
	{ "GetWindowSize", getWindowSize },
	{ "SetWindowMaxSize", setWindowMaxSize },
	{ "GetWindowMaxSize", getWindowMaxSize },
	{ "SetWindowMinSize", setWindowMinSize },
	{ "GetWindowMinSize", getWindowMinSize },
	{ "SetWindowPosition", setWindowPosition },
	{ "GetWindowPosition", getWindowPosition },
	{ "Clear", clear },
	{ "SetCursorShow", setCursorShow },
	{ "GetCursorPosition", getCursorPosition },
	{ "GetScrollValue", getScrollValue },
	{ "LoadImage", loadImage },
	{ "LoadColorKeyImage", loadColorKeyImage },
	{ "UnloadImage", unloadImage },
	{ "SetImageAlpha", setImageAlpha },
	{ "GetImageSize", getImageSize },
	{ "ShowImage", showImage },
	{ "ShowRotateImage", showRotateImage },
	{ "ShowReshapeImage", showReshapeImage },
	{ "ShowRotateReshapeImage", showRotateReshapeImage },
	{ "SetDrawColor", setDrawColor },
	{ "Point", point },
	{ "Line", line },
	{ "Rectangle", rectangle },
	{ "FillRectangle", fillRectangle },
	{ "Update", update },
	{ "LoadMusic", loadMusic },
	{ "UnloadMusic", unloadMusic },
	{ "PlayMusic", playMusic },
	{ "FadeInMusic", fadeInMusic },
	{ "FadeOutMusic", fadeOutMusic },
	{ "SetMusicVolume", setMusicVolume },
	{ "GetMusicVolume", getMusicVolume },
	{ "PauseMusic", pauseMusic },
	{ "ResumeMusic", resumeMusic },
	{ "RewindMusic", rewindMusic },
	{ "GetMusicType", getMusicType },
	{ "LoadSound", loadSound },
	{ "UnloadSound", unloadSound },
	{ "PlaySound", playSound },
	{ "LoadFont", loadFont },
	{ "UnloadFont", unloadFont },
	{ "SetTextColor", setTextColor },
	{ "DrawText", drawText },
	{ "UpdateEvent", updateEvent },
	{ "GetEventType", getEventType },
	{ "Pause", pause },
	{ "Sleep", sleep },
	{ NULL, NULL }
};

extern "C" __declspec(dllexport)
int luaopen_QGame(lua_State * L)
{
	luaL_newlib(L, cMethods);
	return 1;
}
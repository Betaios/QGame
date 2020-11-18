#ifndef _Q_WINDOW_H_
#define _Q_WINDOW_H_

#include <lua.hpp>
#include <SDL.h>

#include <string>
using namespace std;

extern SDL_Window* window;
extern SDL_Renderer* renderer;

// ��ʾģ̬����ʾ��Ϣ����
// 3�������������ͣ����󡢾��桢��ʾ�������⣬��ʾ��Ϣ����
// �޷���ֵ
extern "C" int showMessageBox(lua_State * L);

// �������ڣ������˴�����Ϊͼ�λ��ƵȲ�����������
// 5���������ڱ��⣬����x��y���꣨��ֵΪĬ��λ�ã������ڿ�Ⱥ͸߶ȣ��������Ա�־����Ӧ��ϵ���·�����ע�ͣ�
// �޷���ֵ
extern "C" int createWindow(lua_State * L);

// �رմ���
// �޲���
// �޷���ֵ
extern "C" int closeWindow(lua_State * L);

// ���ô��ڱ���
// 1��������������
// �޷���ֵ
extern "C" int setWindowTitle(lua_State * L);

// ��ȡ���ڱ���
// �޲���
// 1����ֵ����������
extern "C" int getWindowTitle(lua_State * L);

// ���ô���ģʽ
// 1������ȫ����־��0������ģʽ��1����Ƶģʽ�ı����ȫ����2��ʹ�õ�ǰ��Ļ�ֱ��ʵļ�ȫ����
// �޷���ֵ
extern "C" int setWindowMode(lua_State * L);

// ���ô����Ƿ��С�ɱ�
// 1�������Ƿ��С�ɱ�
// �޷���ֵ
extern "C" int setWindowResizable(lua_State * L);

// ���ô���͸����
// 1������͸���ȴ�С��0-1��
// �޷���ֵ
extern "C" int setWindowOpacity(lua_State * L);

// ���ô��ڴ�С
// 2���������ڵĿ�͸�
// �޷���ֵ
extern "C" int setWindowSize(lua_State * L);

// ��ȡ���ڴ�С
// �޲���
// 2����ֵ�����ڵĿ�͸�
extern "C" int getWindowSize(lua_State * L);

// ���ô������ߴ�Ĵ�С
// 2���������ڿ�ȵ����ֵ�͸߶ȵ����ֵ
// �޷���ֵ
extern "C" int setWindowMaxSize(lua_State * L);

// ��ȡ�������ߴ�Ĵ�С
// �޲���
// 2����ֵ�����ڶȵ����ֵ�͸߶ȵ����ֵ
extern "C" int getWindowMaxSize(lua_State * L);

// ���ô�����С�ߴ�Ĵ�С
// 2���������ڿ�ȵ���Сֵ�͸߶ȵ���Сֵ
// �޷���ֵ
extern "C" int setWindowMinSize(lua_State * L);

// ��ȡ������С�ߴ�Ĵ�С
// �޲���
// 2����ֵ�����ڶȵ���Сֵ�͸߶ȵ���Сֵ
extern "C" int getWindowMinSize(lua_State * L);

// ���ô�������
// 2����������λ�õ�x��y����
// �޷���ֵ
extern "C" int setWindowPosition(lua_State * L);

// ��ȡ��������
// �޲���
// 2����ֵ������λ�õ�x��y����
extern "C" int getWindowPosition(lua_State * L);

// ʹ�õ�ǰ��ͼ��ɫ��մ�������
// �޲���
// �޷���ֵ
extern "C" int clear(lua_State * L);

#endif // !_Q_WINDOW_H_

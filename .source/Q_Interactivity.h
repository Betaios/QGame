#ifndef _Q_INTERACTIVITY_H_
#define _Q_INTERACTIVITY_H_

#include <lua.hpp>
#include <SDL.h>

#include <string>
using namespace std;

extern SDL_Event event;

// ��ȡ���λ��
// �޲���
// 2����ֵ��λ�õ�x��y����
extern "C" int getCursorPosition(lua_State * L);

// ��ȡ�����ֹ�������
// �޲���
// 1����ֵ�����ֹ�������
extern "C" int getScrollValue(lua_State * L);

// ��ȡ�������¼�
// �޲���
// 1����ֵ��-1����ǰ�¼������޴�����¼���0����ȡ�����¼�
extern "C" int updateEvent(lua_State * L);

// ��ȡ��ǰ�¼�����
// �޲���
// 1����ֵ���¼����ͣ��ַ�����
// ע�⣺���ڲ����ַ������¼���Ӧ�������ڴ����������ֻ��ͨ����ϼ���ɣ�����Щ�ַ��İ����¼��ڴ���������Ͽ����޷�ͨ������������
extern "C" int getEventType(lua_State * L);

#endif // !_Q_INTERACTIVITY_H_

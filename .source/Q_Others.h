#ifndef _Q_OTHERS_H_
#define _Q_OTHERS_H_

#include <lua.hpp>
#include <SDL.h>

// ��ȡ�������е�Ŀ¼
// �޲���
// 1����ֵ����������Ŀ¼
extern "C" int getBasePath(lua_State * L);

// ���ü��а��ı�
// 1���������а��ı�
// �޷���ֵ
extern "C" int setClipboardText(lua_State * L);

// ��ȡ���а��ı�
// �޲���
// 1����ֵ�����а��ı�
extern "C" int getClipboardText(lua_State * L);

// ��ȡƽ̨����
// �޲���
// 1����ֵ��ƽ̨���ͣ��ַ�����
extern "C" int getPlatform(lua_State * L);

// ��ȡϵͳ���ڴ��С
// �޲���
// 1����ֵ��ϵͳ�ڴ棨MB��
extern "C" int getSystemRAM(lua_State * L);

#endif // !_Q_OTHERS_H_

#ifndef _Q_MEDIA_H_
#define _Q_MEDIA_H_

#include <lua.hpp>
#include <SDL.h>
#include <SDL_mixer.h>

#include <vector>
using namespace std;

extern vector<Mix_Music*> vMusic;
extern vector<Mix_Chunk*> vSound;

// ���������ļ�
// 1�����������ļ�·��
// 1����ֵ��-1�������ļ�ʧ�ܣ��������Ѽ��ص������ļ������ֿ��еı�־
extern "C" int loadMusic(lua_State * L);

// �ͷ��Ѽ��ص�����
// 1�������Ѽ��ص������ļ������ֿ��еı�־
// 1����ֵ��-1���ͷ�����ʧ�ܣ�0���ͷ����ֳɹ�
extern "C" int unloadMusic(lua_State * L);

// �����Ѽ��ص�����
// 2�������Ѽ��ص������ļ������ֿ��еı�־�����ֲ��ŵĴ�����-1Ϊѭ�����ţ�
// 1����ֵ��-1����������ʧ�ܣ�0���������ֳɹ�
extern "C" int playMusic(lua_State * L);

// ���ŵ�������
// 3�������Ѽ��ص������ļ������ֿ��еı�־�����ֲ��ŵĴ�����������Ч����ʱ�䣨ms��
// 1������-1����������ʧ�ܣ�0���������ֳɹ�
extern "C" int fadeInMusic(lua_State * L);

// ������ֹͣ��ǰ���ڲ��ŵ�����
// 1������������Ч����ʱ�䣨ms��
// �޷���ֵ
extern "C" int fadeOutMusic(lua_State * L);

// �������ֲ��ŵ�����
// 1������������С��0-128��
// �޷���ֵ
extern "C" int setMusicVolume(lua_State * L);

// ��ȡ��ǰ��������
// �޲���
// 1����ֵ����ǰ����������С
extern "C" int getMusicVolume(lua_State * L);

// ��ͣ��ǰ���ڲ��ŵ�����
// �޲���
// �޷���ֵ
extern "C" int pauseMusic(lua_State * L);

// �ָ���ǰ��ͣ״̬������
// �޲���
// �޷���ֵ
extern "C" int resumeMusic(lua_State * L);

// �ز���ǰ���ڲ��ŵ�����
// �޲���
// �޷���ֵ
extern "C" int rewindMusic(lua_State * L);

// ��ȡָ�����ֵ�����
// 1�������Ѽ��ص������ļ������ֿ��еı�־��-1��ʾ���ڲ��ŵ����֣�
// 1����ֵ���ɹ��򷵻�ָ�����ֵ����ͣ��ַ�������δ֪�򷵻�"UNKONWN"��ʧ���򷵻�"ERROR"
extern "C" int getMusicType(lua_State * L);

// ������Ч�ļ�
// 1��������Ч�ļ�·��
// 1����ֵ��-1��������Ч�ļ�ʧ�ܣ��������Ѽ��ص���Ч�ļ�����Ч���еı�־
// ��ע����Ч�ļ�֧�����¸�ʽ��WAVE, AIFF, RIFF, OGG, VOC
extern "C" int loadSound(lua_State * L);

// �ͷ��Ѽ��ص���Ч
// 1�������Ѽ��ص���Ч�ļ������ֿ��еı�־
// 1����ֵ��-1���ͷ���Чʧ�ܣ�0���ͷ���Ч�ɹ�
extern "C" int unloadSound(lua_State * L);

// �����Ѽ��ص���Ч
// 2�������Ѽ��ص���Ч�ļ�����Ч���еı�־����Ч���ŵĴ�����-1Ϊѭ�����ţ�
// 1����ֵ��-1����������ʧ�ܣ�0���������ֳɹ�
extern "C" int playSound(lua_State * L);


#endif // !_Q_MEDIA_H_


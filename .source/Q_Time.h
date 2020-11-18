#ifndef _Q_TIME_H_
#define _Q_TIME_H_

#include <lua.hpp>
#include <SDL.h>

#include <string>
#include <vector>
using namespace std;

struct TimerCallbackParam
{
	string name = "";
	lua_State* L;
};

extern vector<TimerCallbackParam*> vParam;

// �������ָ��ʱ��
// 1����������ʱ�䣨��λ�����룩
// �޷���ֵ
extern "C" int sleep(lua_State * L);

// ��ȡ����ӳ�ʼ�����е�����Ϊֹ��ʱ��
// �޲���
// 1����ֵ������ʱ�䣨��λ�����룩
extern "C" int getInitTime(lua_State * L);

// ��ȡ�߷ֱ��ʼ�������ǰֵ
// �޲���
// 1����ֵ����������ǰֵ
extern "C" int getAccurateCount(lua_State * L);

// ��ȡ�߷ֱ��ʼ�����Ƶ��
// �޲���
// 1����ֵ��������Ƶ�ʣ���λ�����ȣ�
extern "C" int getCounterFrequency(lua_State * L);

// ������ʱ��
// 2��������ʱ������ʱ��������λ�����룩���ص�����
// 1����ֵ��ʧ���򷵻�-1������ʱ��ID
extern "C" int addTimer(lua_State * L);

// �Ƴ���ʱ��
// 1��������ʱ��ID
// 1����ֵ��ʧ���򷵻�-1������0
extern "C" int removeTimer(lua_State * L);

#endif // !_Q_TIME_H_

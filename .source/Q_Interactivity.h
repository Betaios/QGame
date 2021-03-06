#ifndef _Q_INTERACTIVITY_H_
#define _Q_INTERACTIVITY_H_

#include <lua.hpp>
#include <SDL.h>

#include <string>
using namespace std;

extern SDL_Event event;

// 获取鼠标位置
// 无参数
// 2返回值：位置的x和y坐标
extern "C" int getCursorPosition(lua_State * L);

// 获取鼠标滚轮滚动距离
// 无参数
// 1返回值：滚轮滚动距离
extern "C" int getScrollValue(lua_State * L);

// 获取并更新事件
// 无参数
// 1返回值：-1，当前事件队列无待解决事件；0，获取到新事件
extern "C" int updateEvent(lua_State * L);

// 获取当前事件类型
// 无参数
// 1返回值：事件类型（字符串）
// 注意：由于部分字符按键事件对应的输入在大多数键盘上只能通过组合键完成，故这些字符的按键事件在大多数键盘上可能无法通过物理按键激活
extern "C" int getEventType(lua_State * L);

#endif // !_Q_INTERACTIVITY_H_

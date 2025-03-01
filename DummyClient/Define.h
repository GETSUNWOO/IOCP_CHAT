#pragma once

/*---------------
	  Singleton
---------------*/

#define DECLARE_SINGLE(classname)			\
private:									\
	classname() { }							\
public:										\
	static classname* GetInstance()			\
	{										\
		static classname s_instance;		\
		return &s_instance;					\
	}

#define GET_SINGLE(classname)	classname::GetInstance()

#define GAME			GET_SINGLE(GameManager)
#define GRAPHICS		GET_SINGLE(GraphicsManager)
#define IMGUI			GET_SINGLE(ImguiManager)
#define SERVER			GET_SINGLE(ServerManager)
#define MESSAGE			GET_SINGLE(MessageManager)
#define DEVICE			GRAPHICS->GetDevice()
#define CONTEXT			GRAPHICS->GetDeviceContext()
#define MAX_MESSAGES    15


enum class STATE
{
	LOGIN = 0,
	CHAT = 1
};

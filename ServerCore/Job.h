#pragma once
#include <functional>
//v1. 일일이 함수를 만들어서 잡을 추가
//v2. 템플릿을 통해 args... 추가
//v3. 템플릿과 람다, function을 통해

// 람다식 사용할때 주의점 1.
// 참조를 통해 인자를 받아 사용시 
// 추후 함수 호출 때 캡쳐한 객체의 생명주기가 꼭 살아있어야한다

// 람다식 사용할때 주의점 2.
// =를 통해 모든 애들을 캡쳐한다고 해도 this포인터를 요구하는 경우
// 생명주기에 따라 어긋날 수 있다.

// 오해 1. (람다에서 shared_ptr을 섞어쓰면 메모리 릭이 일어난다?)
// 본인이 코드를 잘못짠것.. (자기자신의 shared_ptr를 가지고 있는다던가)

/*--------------
		Job
----------------*/

using CallbackType = std::function<void()>;

class Job
{
public:

	Job(CallbackType&& callback) : _callback(std::move(callback))
	{
	}

	template<typename T , typename Ret, typename... Args>
	Job(shared_ptr<T>owner, Ret(T::* memFunc)(Args...), Args&& ...args)
	{
		_callback = [owner, memFunc, args...]()
			{
				(owner.get()->*memFunc)(args...);
			};
	}

	void Execute()
	{
		_callback();
	}

private:
	CallbackType _callback;
};


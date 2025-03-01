#pragma once
#include <functional>
//v1. ������ �Լ��� ���� ���� �߰�
//v2. ���ø��� ���� args... �߰�
//v3. ���ø��� ����, function�� ����

// ���ٽ� ����Ҷ� ������ 1.
// ������ ���� ���ڸ� �޾� ���� 
// ���� �Լ� ȣ�� �� ĸ���� ��ü�� �����ֱⰡ �� ����־���Ѵ�

// ���ٽ� ����Ҷ� ������ 2.
// =�� ���� ��� �ֵ��� ĸ���Ѵٰ� �ص� this�����͸� �䱸�ϴ� ���
// �����ֱ⿡ ���� ��߳� �� �ִ�.

// ���� 1. (���ٿ��� shared_ptr�� ����� �޸� ���� �Ͼ��?)
// ������ �ڵ带 �߸�§��.. (�ڱ��ڽ��� shared_ptr�� ������ �ִ´ٴ���)

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


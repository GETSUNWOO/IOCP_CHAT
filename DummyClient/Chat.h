#pragma once

class Chat : public IExecute
{
public:

	virtual void Init()override;
	virtual void Update() override;
	virtual void Render() override;

//Login
public:
	void Login();
	void Chatting();

public:
	void PopUp(string msg);

private:
	bool _isConnected = false;
};


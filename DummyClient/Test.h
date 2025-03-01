#pragma once
class Test : public IExecute
{
public:

	virtual void Init()override;
	virtual void Update() override;
	virtual void Render() override;

};


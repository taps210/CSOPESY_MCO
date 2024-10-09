#pragma once
class CSOPESYThread
{
public:
	CSOPESYThread();
	~CSOPESYThread();

	void start();
	static void sleep(int ms);
protected: 
	virtual void run() = 0;
};


#pragma once

// �����Ҵ�
// 1. ��Ÿ�ӿ� ���� �� ������ ����
// 2. ���α׷� ���� �� �ݵ�� ��ü �޸� ���� �����־�� �Ѵ�.

// ��������
// 1. ��ü �޸� ������ �Ű澲�� �ʾƵ� ��
// 2. ��Ÿ�� ���� ���������ν� ������ų �� ����

template<typename T>
class CSingleton
{
private:
	static T* g_Inst;

	typedef void(*EXIT)(void);
	
public:
	static T* GetInst()
	{
		if (nullptr == g_Inst)
		{
			g_Inst = new T;
		}

		return g_Inst;
	}

	static void Destroy()
	{
		if (g_Inst)
		{
			delete g_Inst;
			g_Inst = nullptr;
		}
	}

public:
	CSingleton()
	{
		atexit((EXIT)&CSingleton<T>::Destroy);
	}
	CSingleton(const CSingleton& _Other) = delete;
	virtual ~CSingleton() {}
};

template<typename T>
T* CSingleton<T>::g_Inst = nullptr;
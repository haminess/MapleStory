#pragma once


typedef void(*EXIT)(void);

template<typename T>
class singleton
{
private:
	static T* m_Inst;

public:
	static T* GetInst()
	{
		// 싱글톤 1. 지역 정적 변수 사용
		// 장점: 소멸에 신경쓸 필요 없어 간편
		// 단점: 항상 메모리 차지
		//static CEngine engine;
		//return &engine;


		// 싱글톤 2. 클래스 정적 포인터 변수 사용
		// 장점: 필요할 때 할당, 소멸 가능
		// 단점: 직접 소멸해주어야 한다.
		if (nullptr == m_Inst)
		{
			m_Inst = new T;

			// 메인 함수가 종료될 때 순차적으로 호출시킬 함수 등록
			// => 각 싱글톤 패턴의 장점만 활용 가능
			atexit((EXIT)&singleton<T>::Destroy);
		}

		return m_Inst;
	}

	static void Destroy()
	{
		if (nullptr != m_Inst)
		{
			delete m_Inst;
			m_Inst = nullptr;
		}
	}
};

template<typename T>
T* singleton<T>::m_Inst = nullptr;


#define SINGLE(TYPE) private:\
						TYPE();\
						TYPE(const TYPE& _Other) = delete;\
						~TYPE();\
						TYPE& operator =(const TYPE& _Other) = delete;\
						friend class singleton<TYPE>;
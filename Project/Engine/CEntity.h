#pragma once
class CEntity
{
private:
	static UINT g_NextID;

private:
	wstring m_Name;
	UINT m_ID;


public:
	void SetName(const wstring& _Name) { m_Name = _Name; }
	wstring GetName() { return m_Name; }
	UINT GetID() { return m_ID; }


	virtual void SaveToLevel(FILE* _File);
	virtual void LoadFromLevel(FILE* _File);


	// 부모 포인터로 오브젝트를 관리하는 경우가 많다.
	// Clone을 virtual로 다형성의 특징을 살려 구현해야 한다.
	// 클래스를 생성할 때 마다 Clone 함수와 복사 생성자를 신경써야 함
	virtual CEntity* Clone() = 0;

public:
	CEntity();
	CEntity(const CEntity& _Other);
	virtual ~CEntity();
};


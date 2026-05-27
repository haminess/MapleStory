#pragma once

enum class KEY
{
	// key
	Q, W, E, R, T, Y, U, I, O, P,
	A, S, D, F, G, H, J, K, L,
	Z, X, C, V, B, N, M,
	N1, N2, N3, N4, N5, N6, N7, N8, N9, N0, MINUS, EQUAL,

	UP, DOWN,
	LEFT, RIGHT,

	SPACE, ENTER,

	NUM0, NUM1, NUM2, NUM3, NUM4,
	NUM5, NUM6, NUM7, NUM8, NUM9,

	F1, F2, F3, F4,
	F5, F6, F7, F8,
	F9, F10, F11, F12,

	// mouse
	LBTN, RBTN,
	MBTN, WHEEL_UP, WHEEL_DOWN,
	DEL, LSHIFT, RSHIFT, CTRL, ALT,
	ESC, TAB,
	ANY,

	END
};

enum class KEY_STATE
{
	TAP,		// 지금 막 눌림
	PRESSED,	// 계속 눌려있는 상태
	RELEASED,	// 키 뗀 해제 상태
	NONE,
};

struct tKeyInfo
{
	KEY_STATE	State;			// 키 상태
	bool		PrevPressed;	// 이전번에 눌린 적이 있었는지
	bool		NextPressed;	// 다음 프레임에 무조건 트리거 될 지
};

class CCamera;
class CKeyMgr
	: public singleton<CKeyMgr>
{
public:
	static Vector2 GetScreenToWorld(Vector2 _Screen, CCamera* _Cam = nullptr);
	static Vector2 GetWorldToScreen(Vector2 _World, CCamera* _Cam = nullptr);
	static Vector2 GetWorldToResolutionPos(Vector2 _World);

	// 키 동기화 하기 위해 사용
	SINGLE(CKeyMgr);

private:
	vector<tKeyInfo>	m_vecKey;

	Vector2				m_MousePos;
	Vector2				m_MousePrevPos;
	Vector2				m_MouseDir;

	Vector2				m_MouseWorldPos;
	Vector2				m_MouseWorldPrevPos;
	Vector2				m_MouseWorldDir;

public:
	Vector2 GetMousePos() { return m_MousePos; }
	Vector2 GetMouseDir() { return m_MouseDir; }

	Vector2 GetMouseWorldPos() { return m_MouseWorldPos; }
	Vector2 GetMouseWorldDir() { return m_MouseWorldDir; }

	void SetMouseWorldPos(Vector2 _Pos) { m_MouseWorldPos = _Pos; }


	void UpdateKeyState(KEY _Key) { m_vecKey[(int)_Key].NextPressed = true; }

public:
	void Init();
	void Tick();

	KEY_STATE GetKeyState(KEY _Key) { return m_vecKey[(int)_Key].State; }
	bool IsThisKeyPressed(KEY _Key) { return m_vecKey[(int)_Key].PrevPressed; }
};


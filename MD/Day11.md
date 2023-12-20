23.12.19<br>

## CCameraMoveScript
카메라 스피드 멤버 추가
```cpp
float m_camSpeed = 500.f;
```
카메라 조종 관련 코드 추가
- 상하좌우 위치이동 : W,A,S,D
- 스케일 변화 : I,O
```cpp
void CCameraMoveScript::Tick()
{
	if (KEY_TAP(KEY::P))
	{
		if (Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
			Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
		else
		{
			Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
			Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
		}
	}

	// Traslation
	if (Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
	{
		MoveOrthographic();
	}
	else
	{
		MovePerspective();
	}

	// Scale
	if (KEY_PRESSED(KEY::I))
	{
		if (Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
			Camera()->SetScale(Camera()->GetScale() + DT * 0.2f);
		else
			Camera()->SetFOV(Camera()->GetFOV() + DT * 2.f);
	}
	if (KEY_PRESSED(KEY::O))
	{
		if (Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
			Camera()->SetScale(Camera()->GetScale() - DT * 0.2f);
		else
			Camera()->SetFOV(Camera()->GetFOV() - DT * 2.f);
	}
}
// 직교투영(Orth)
void CCameraMoveScript::MoveOrthographic()
{
	Vec3 vPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::W))
	{
		vPos.y += DT * m_camSpeed;
	}

	if (KEY_PRESSED(KEY::S))
	{
		vPos.y -= DT * m_camSpeed;
	}

	if (KEY_PRESSED(KEY::A))
	{
		vPos.x -= DT * m_camSpeed;
	}

	if (KEY_PRESSED(KEY::D))
	{
		vPos.x += DT * m_camSpeed;
	}

	Transform()->SetRelativePos(vPos);
}
// 원근투영(Perspec)
void CCameraMoveScript::MovePerspective()
{
	Vec3 vPos = Transform()->GetRelativePos();

	Vec3 vFront = Transform()->GetDir(DIR_TYPE::FRONT);
	Vec3 vRight = Transform()->GetDir(DIR_TYPE::RIGHT);

	if (KEY_PRESSED(KEY::W))
	{
		vPos += DT * m_camSpeed * vFront;
	}

	if (KEY_PRESSED(KEY::S))
	{
		vPos += DT * m_camSpeed * -vFront;
	}

	if (KEY_PRESSED(KEY::A))
	{
		vPos += DT * m_camSpeed * -vRight;
	}

	if (KEY_PRESSED(KEY::D))
	{
		vPos += DT * m_camSpeed * vRight;
	}

	Transform()->SetRelativePos(vPos);

	if (KEY_PRESSED(KEY::RBTN))
	{
		Vec2 vDrag = CKeyMgr::GetInst()->GetMouseDrag();
		Vec3 vRot = Transform()->GetRelativeRotation();
		vRot.y += vDrag.x * DT * XM_PI * 4.f;
		vRot.x += vDrag.y * DT * XM_PI * 4.f;
		Transform()->SetRelativeRotation(vRot);
	}
}
```

카메라 투영방식 변경 코드 추가
```cpp
if (KEY_TAP(KEY::P))
{
    if (Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
        Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
    else
    {
        Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
        Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
    }
}
```

## CCamera
enum class 추가하여 투영방식 표현
```cpp
enum class PROJ_TYPE
{
	ORTHOGRAPHIC, // 직교 투영
	PERSPECTIVE,  // 원근 투영
};
```
Camera 클래스에 투영방식을 저장하는 멤버변수 추가 및 Getter,Setter 함수 추가
```cpp
PROJ_TYPE	m_projType;

PROJ_TYPE GetProjType() { return m_projType; }
void SetProjType(PROJ_TYPE _type) { m_projType = _type; }

float GetScale() { return m_scale; }
void SetScale(float _scale) { m_scale = _scale; }

float GetFOV() { return m_fov; }
void SetFOV(float _fov) { m_fov = _fov; }
```

## CPlayerScript
플레이어 이동키 WASD -> 방향키 변경
```cpp
if (KEY_PRESSED(KEY::UP))
    vPos.z += DT * m_speed;

if (KEY_PRESSED(KEY::DOWN))
    vPos.z -= DT * m_speed;

if (KEY_PRESSED(KEY::LEFT))
    vPos.x -= DT * m_speed;

if (KEY_PRESSED(KEY::RIGHT))
    vPos.x += DT * m_speed;
```

## CKeyMgr
마우스 드래그 관련 멤버변수,함수 추가
```cpp
Vec2    m_vMousePrevPos;
Vec2    m_vMouseDrag;

Vec2 GetMouseDrag() { return m_vMouseDrag; }
```
23.12.26<br>


## CTaskMgr class_Add

### enum class TASK_TYPE 정의
```cpp
enum class TASK_TYPE
{
	// Param1 : Layer Index, Param2 : Object Adress
	CREATE_OBJECT,

	// Param1 : Object Adress
	DELETE_OBJECT,

	// Param1 : LEVEL_TYPE
	LEVEL_CHANGE,

	// Param1 : Parent Object, Param2 : Child Object
	ADD_CHILD,

	// Param1 : Parent Object, Param2 : Child Object
	DISCONNECT_PARENT,
};
```

### 구조체 FTask 저의
```cpp
struct FTask
{
	TASK_TYPE Type;
	UINT_PTR  Param_1;
	UINT_PTR  Param_2;
};
```

### CLevel 수정


### LevelMgr 수정 
1. 추가
CTaskMgr::Tick() 에서 처리해야하는 Task의 Type 이 CREATE_OBJECT 인 경우 아래와같이 LevelMgr로부터 GetCurrentLevel() 함수 호출을 통해 현재 Level 을 받아와야한다.
```cpp
// CTaskMgr.cpp
case TASK_TYPE::CREATE_OBJECT:
{
    int LayerIdx = (int)m_vecTask[i].Param_1;
    CGameObject* Object = (CGameObject*)m_vecTask[i].Param_2;

    CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    pCurLevel->AddObject(Object, LayerIdx, true);
}
```
```cpp
// CLevelMgr.h
CLevel* GetCurrentLevel() { return m_curLevel; }
```
2. 수정




## func.cpp_Add


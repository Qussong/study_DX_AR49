#pragma once

#define	SINGLE(classType)	private:\
								classType();\
								~classType();\
							friend class CSingleton<classType>;

#define CHECK(hr)	if (FAILED(hr))\
						return E_FAIL;
// Device, DeviceContext
#define DEVICE	CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetContext()
// DeltaTime
#define DT	CTimeMgr::GetInst()->GetDeltaTime()
// Key
#define KEY_CHECK(Key, State)	CKeyMgr::GetInst()->GetKeyState(Key) == State
#define KEY_TAP(Key)			KEY_CHECK(Key, TAP)
#define KEY_PRESSED(Key)		KEY_CHECK(Key, PRESSED)
#define KEY_RELEASED(Key)		KEY_CHECK(Key, RELEASED)
#define KEY_NONE(Key)			KEY_CHECK(Key, NONE)

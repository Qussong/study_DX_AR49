#pragma once

#define	SINGLE(classType)	private:\
								classType();\
								~classType();\
							friend class CSingleton<classType>;

#define CHECK(hr)	if (FAILED(hr))\
						return E_FAIL;

#define DEVICE CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetContext()

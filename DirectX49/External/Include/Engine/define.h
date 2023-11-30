#pragma once

#define	SINGLE(classType)	private:\
								classType();\
								~classType();\
							friend class CSingleton<classType>;
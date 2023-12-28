#pragma once
#include "CEntity.h"

#include "Ptr.h"

class CAsset
	: public CEntity
{
private:
	wstring				m_Key;
	wstring				m_RelativePath;
	int					m_RefCount;
	const ASSET_TYPE	m_Type;

public:
	const wstring& GetKey() { return m_Key; }
	const wstring& GetRelativePath() { return m_RelativePath; }

private:
	void SetKey(const wstring& _key) { m_Key = _key; }
	void SetRelativePath(const wstring& _path) { m_RelativePath = _path; }

	void AddRef()
	{
		++m_RefCount;
	}

	//void SubRef() { --mRefCount; }
	void Release()
	{
		--m_RefCount;
		if (0 >= m_RefCount)
		{
			delete this;
		}
	}

	int GetRefCount() { return m_RefCount; }
	ASSET_TYPE GetType() { return m_Type; }

	virtual int Load(const wstring& _strFilePath) { return E_FAIL; }

public:
	CAsset(ASSET_TYPE _type);
	~CAsset();

	friend class CAssetMgr;

	template<typename T>
	friend class Ptr;
};


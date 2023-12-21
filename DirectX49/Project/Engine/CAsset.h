#pragma once
#include "CEntity.h"

class CAsset
	: public CEntity
{
private:
	wstring				mKey;
	wstring				mRelativePath;
	int					mRefCount;
	const ASSET_TYPE	mType;

public:
	const wstring& GetKey() { return mKey; }
	const wstring& GetRelativePath() { return mRelativePath; }

private:
	void SetKey(const wstring& _key) { mKey = _key; }
	void SetRelativePath(const wstring& _path) { mRelativePath = _path; }

	void AddRef() { ++mRefCount; }
	void SubRef() { --mRefCount; }

	int GetRefCount() { return mRefCount; }
	ASSET_TYPE GetType() { return mType; }

	virtual int Load(const wstring& _strFilePath) { return E_FAIL; }

public:
	CAsset(ASSET_TYPE _type);
	~CAsset();

	friend class CAssetMgr;
};


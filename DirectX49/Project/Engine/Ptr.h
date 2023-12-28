#pragma once

template<typename T>
class Ptr
{
public:
	Ptr()
		: asset(nullptr)
	{}

	Ptr(const Ptr<T>& _other)
		:asset(_other.asset)
	{
		if (nullptr != asset)
			asset->AddRef();
	}

	Ptr(T* _asset)
		: asset(_asset)
	{
		if (nullptr != asset)
			asset->AddRef();
	}

	~Ptr()
	{
		if (asset)
		{
			asset->Release();
		}
	}

private:
	T* asset;

public:
	T*	Get() const { return asset; }
	T** GetAddressOf() const { return &asset; }
	T*	operator->() const { return asset; }

public:
	void operator= (const Ptr& _ptr)
	{
		if (nullptr != asset)
			asset->Release();

		asset = _ptr.asset;

		if (nullptr != asset)
			asset->AddRef();
	}

	void operator= (T* _asset)
	{
		if (nullptr != asset)
			asset->Release();

		asset = _asset;

		if (nullptr != asset)
			asset->AddRef();
	}

	bool operator== (const Ptr<T>& _other)
	{
		if (asset == _other.asset)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool operator!= (const Ptr<T>& _other)
	{
		return !(*this == _other);
	}

	bool operator== (const T* _asset)
	{
		if (asset == _asset)
			return true;
		else
			return false;
	}

	bool operator!= (const T* _asset)
	{
		return !(*this == _asset);
	}
};

template<typename T>
bool operator== (const void* _asset, const Ptr<T>& _ptr)
{
	if (_asset == _ptr.Get())
		return true;
	else
		return false;
}

template<typename T>
bool operator!= (const void* _asset, const Ptr<T>& _ptr)
{
	return !(_asset == _ptr);
}
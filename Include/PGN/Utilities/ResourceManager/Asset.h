#pragma once
namespace pgn {

class AssetFactory;

class Asset
{
public:
	virtual AssetFactory* getFactory() = 0;
	virtual bool cook(void* rawData) = 0;
	virtual bool submit(void* rawData, void* customArg) = 0;
	virtual void unload(void* customArg) = 0; // 加载失败再调这个也没事
	virtual void* core() = 0; // 未加载好或加载失败返回0
};

}

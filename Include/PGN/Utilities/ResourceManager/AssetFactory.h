#pragma once
namespace pgn {

class Asset;

class AssetFactory
{
public:
	virtual Asset* create() = 0;
	virtual void recycle(Asset* asset) = 0;
};

}

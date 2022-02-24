#pragma once
#include "Bindable.h"
#include <array>


class Rasterizer : public Bindable
{
public:
	Rasterizer(Graphics& gfx, bool twoSided);
	void Bind(Graphics& gfx) noexcept override;
protected:
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRasterizer;
	bool twoSided;
};


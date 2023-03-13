#pragma once

class Bindable
{
public:
	virtual void Bind(Graphics& gfx) noexcpt = 0;
	virtual ~Bindable() = default;

protected:
	static ID3D11DeviceContext* GetContext(Graphics& gfx) noexcept;
	static ID3D11Device* GetDevice(Graphics& gfx) noexcept;
	static DxgiInfoManager& GetInfoManager(Graphics& gfx) noexcept(IS_DEBUG);
};
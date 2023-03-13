#pragma once

class Drawable
{
	template <class T>
	friend class DrawableBase;

public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual ~Drawable() = default;

	virtual XMMATRIX GetTransform() const noexcept = 0;
	void Draw(Graphics& gfx) const noexcpt;
	virtual void Update(float dt) noexcept = 0;

protected:
	template <class T>
	T* QueryBindable() noexcept
	{
		for (auto& pb : binds)
		{
			if (auto pt = dynamic_cast<T*>(pb.get()))
			{
				return pt;
			}
		}
		return nullptr;
	}

	void AddBind(std::unique_ptr<Bindable> bind) noexcpt;
	void AddIndexBuffer(std::unique_ptr<class IndexBuffer> indexBuffer) noexcpt;

private:
	virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept = 0;
	const IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> binds;
};

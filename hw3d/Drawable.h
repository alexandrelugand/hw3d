#pragma once
#include "Forwards.h"

namespace Draw
{
	class Drawable
	{
	public:
		Drawable() = default;
		Drawable(const Drawable&) = delete;
		virtual ~Drawable() = default;

		virtual XMMATRIX GetTransform() const noexcept = 0;
		virtual void Draw(Graphics& gfx) const noexcpt;

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

	protected:
		void AddBind(std::shared_ptr<Bind::Bindable> bind) noexcpt;
		CullMode cull = Front;

	private:
		const Bind::IndexBuffer* pIndexBuffer = nullptr;
		std::vector<std::shared_ptr<Bind::Bindable>> binds;
	};
}

#pragma once
#include "Forwards.h"

namespace Draw
{
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

		virtual void Update(float dt) noexcept
		{
		}

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

		void AddBind(std::unique_ptr<Bind::Bindable> bind) noexcpt;
		void AddIndexBuffer(std::unique_ptr<class Bind::IndexBuffer> indexBuffer) noexcpt;

	private:
		virtual const std::vector<std::unique_ptr<Bind::Bindable>>& GetStaticBinds() const noexcept = 0;
		const Bind::IndexBuffer* pIndexBuffer = nullptr;
		std::vector<std::unique_ptr<Bind::Bindable>> binds;
	};
}

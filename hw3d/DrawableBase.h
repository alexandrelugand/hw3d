#pragma once
#include "IndexBuffer.h"

namespace Draw
{
	template <class T>
	class DrawableBase : public Drawable
	{
	public:
		static bool IsStaticInitialized() noexcept
		{
			return !staticBinds.empty();
		}

		static void AddStaticBind(std::unique_ptr<Bind::Bindable> bind) noexcpt
		{
			assert("*Must* use AddStaticIndexBuffer to bind index buffer" && typeid(*bind) != typeid(Bind::IndexBuffer));
			staticBinds.push_back(std::move(bind));
		}

		void AddStaticIndexBuffer(std::unique_ptr<Bind::IndexBuffer> ibuf) noexcpt
		{
			assert(pIndexBuffer == nullptr);
			pIndexBuffer = ibuf.get();
			staticBinds.push_back(std::move(ibuf));
		}

		void SetIndexFromStatic() noexcpt
		{
			assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
			for (const auto& b : staticBinds)
			{
				if (const auto p = dynamic_cast<Bind::IndexBuffer*>(b.get()))
				{
					pIndexBuffer = p;
					return;
				}
			}
			assert("Failed to find index buffer in static binds" && pIndexBuffer != nullptr);
		}

	private:
		const std::vector<std::unique_ptr<Bind::Bindable>>& GetStaticBinds() const noexcept override
		{
			return staticBinds;
		}

		static std::vector<std::unique_ptr<Bind::Bindable>> staticBinds;
	};

	template <class T>
	std::vector<std::unique_ptr<Bind::Bindable>> DrawableBase<T>::staticBinds;
}

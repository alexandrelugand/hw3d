#include "stdafx.h"
#include "Drawable.h"

namespace Draw
{
	void Drawable::Draw(Graphics& gfx) const noexcpt
	{
		bool cullFound = false;
		for (auto& b : binds)
		{
			b->Bind(gfx);
			cullFound |= typeid(*b) == typeid(Bind::Cull);
		}

		if (!cullFound)
		{
			gfx.SetCullMode(cull);
		}

		gfx.DrawIndexed(pIndexBuffer->GetCount());
	}

	void Drawable::AddBind(std::shared_ptr<Bind::Bindable> bind) noexcpt
	{
		// special case for index buffer
		if (typeid(*bind) == typeid(Bind::IndexBuffer))
		{
			assert("Binding multiple index buffers not allowed" && pIndexBuffer == nullptr);
			pIndexBuffer = &static_cast<Bind::IndexBuffer&>(*bind);
		}
		binds.push_back(std::move(bind));
	}
}

#pragma once

namespace Bind
{
	class Codex
	{
	public:
		template <class T, typename... Params>
		static std::shared_ptr<T> Resolve(Graphics& gfx, Params&&... p) noexcpt
		{
			static_assert(std::is_base_of_v<Bindable, T>, "Can only resolve classes derived from Bindable");
			return Get().Resolve_<T>(gfx, std::forward<Params>(p)...);
		}

	private:
		template <class T, typename... Params>
		std::shared_ptr<T> Resolve_(Graphics& gfx, Params&&... p) noexcpt
		{
			const auto key = T::GenerateUID(std::forward<Params>(p)...);
			const auto instance = binds.find(key);
			if (instance == binds.end())
			{
				// not found, instantiate it
				auto bind = std::make_shared<T>(gfx, std::forward<Params>(p)...);
				binds[key] = bind;
				return bind;
			}

			// cast as bindable as shared_ptr<T>
			return std::static_pointer_cast<T>(instance->second);
		}

		// singleton
		static Codex& Get()
		{
			static Codex codex;
			return codex;
		}

		std::unordered_map<std::string, std::shared_ptr<Bindable>> binds;
	};
}

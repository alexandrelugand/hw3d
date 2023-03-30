#pragma once
#include "Forwards.h"

namespace Rgph
{
	class Pass
	{
	public:
		Pass(std::string name_in) noexcept;
		virtual ~Pass() = default;

		virtual void Execute(Graphics& gfx) const noexcpt;
		virtual void Reset() noexcpt;

		const std::string& GetName() const noexcept;
		const std::vector<std::unique_ptr<Sink>>& GetSinks() const;
		Source& GetSource(const std::string& registeredName) const;
		Sink& GetSink(const std::string& registeredName) const;

		void SetSinkLinkage(const std::string& registeredName, const std::string& target) const;
		virtual void Finalize();

	protected:
		void RegisterSink(std::unique_ptr<Sink> sink);
		void RegisterSource(std::unique_ptr<Source> source);

	private:
		std::vector<std::unique_ptr<Sink>> sinks{};
		std::vector<std::unique_ptr<Source>> sources{};
		std::string name;
	};
}

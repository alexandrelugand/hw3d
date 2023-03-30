#include "stdafx.h"
#include "Pass.h"


namespace Rgph
{
	Pass::Pass(std::string name_in) noexcept
		: name(std::move(name_in))
	{
	}

	void Pass::Execute(Graphics& gfx) const noexcpt
	{
	}

	void Pass::Reset() noexcpt
	{
	}

	const std::string& Pass::GetName() const noexcept
	{
		return name;
	}

	const std::vector<std::unique_ptr<Sink>>& Pass::GetSinks() const
	{
		return sinks;
	}

	Source& Pass::GetSource(const std::string& registeredName) const
	{
		for (auto& src : sources)
		{
			if (src->GetName() == registeredName)
			{
				return *src;
			}
		}

		std::ostringstream oss;
		oss << "Output named [" << registeredName << "] not found in pass: " << GetName();
		throw RGC_EXCEPTION(oss.str());
	}

	Sink& Pass::GetSink(const std::string& registeredName) const
	{
		for (auto& si : sinks)
		{
			if (si->GetRegisteredName() == registeredName)
			{
				return *si;
			}
		}

		std::ostringstream oss;
		oss << "Input named [" << registeredName << "] not found in pass: " << GetName();
		throw RGC_EXCEPTION(oss.str());
	}

	void Pass::SetSinkLinkage(const std::string& registeredName, const std::string& target)
	{
		auto& sink = GetSink(registeredName);
		auto targetSplit = String::Split(target, ".");
		if (targetSplit.size() != 2u)
		{
			throw RGC_EXCEPTION("Input target has incorrect format");
		}
		sink.SetTarget(std::move(targetSplit[0]), std::move(targetSplit[1]));
	}

	void Pass::Finalize()
	{
		for (auto& in : sinks)
		{
			in->PostLinkValidate();
		}

		for (auto& out : sources)
		{
			out->PostLinkValidate();
		}
	}

	void Pass::RegisterSink(std::unique_ptr<Sink> sink)
	{
		// check for overlap of input names
		for (auto& si : sinks)
		{
			if (si->GetRegisteredName() == sink->GetRegisteredName())
			{
				throw RGC_EXCEPTION("Registered input overlaps with existing: " + sink->GetRegisteredName());
			}
		}

		sinks.push_back(std::move(sink));
	}

	void Pass::RegisterSource(std::unique_ptr<Source> source)
	{
		// check for overlap of output names
		for (auto& src : sources)
		{
			if (src->GetName() == source->GetName())
			{
				throw RGC_EXCEPTION("Registered output overlaps with existing: " + source->GetName());
			}
		}

		sources.push_back(std::move(source));
	}
}

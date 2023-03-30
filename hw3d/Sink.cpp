#include "stdafx.h"
#include "Sink.h"

namespace Rgph
{
	const std::string& Sink::GetRegisteredName() const noexcept
	{
		return registeredName;
	}

	const std::string& Sink::GetPassName() const noexcept
	{
		return passName;
	}

	const std::string& Sink::GetOutputName() const noexcept
	{
		return outputName;
	}

	void Sink::SetTarget(std::string passName, std::string outputName)
	{
		{
			if (passName.empty())
				throw RGC_EXCEPTION("Empty pass name");

			const bool nameCharsValid = std::all_of(passName.begin(), passName.end(), [](char c)
			{
				return std::isalnum(c) || c == '_';
			});

			if (passName != "$" && (!nameCharsValid || std::isdigit(passName.front())))
				throw RGC_EXCEPTION("Invalid pass name: " + passName);

			this->passName = passName;
		}
		{
			if (outputName.empty())
				throw RGC_EXCEPTION("Empty output name");

			const bool nameCharsValid = std::all_of(registeredName.begin(), registeredName.end(), [](char c)
			{
				return std::isalnum(c) || c == '_';
			});

			if (!nameCharsValid || std::isdigit(outputName.front()))
				throw RGC_EXCEPTION("Invalid output name: " + outputName);

			this->outputName = outputName;
		}
	}

	Sink::Sink(std::string registeredName_in)
		: registeredName(std::move(registeredName_in))
	{
		if (registeredName.empty())
			throw RGC_EXCEPTION("Empty output name");

		const bool nameCharsValid = std::all_of(registeredName.begin(), registeredName.end(), [](char c)
		{
			return std::isalnum(c) || c == '_';
		});

		if (!nameCharsValid || std::isdigit(registeredName.front()))
			throw RGC_EXCEPTION("Invalid output name: " + registeredName);
	}
}

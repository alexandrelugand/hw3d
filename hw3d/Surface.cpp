// ReSharper disable CppUnsignedZeroComparison
#include "stdafx.h"
#include "Surface.h"

namespace Bind
{
	Surface::Surface(unsigned int width, unsigned int height)
	{
		HRESULT hr = scratch.Initialize2D(format, width, height, 1u, 1u);
		if (FAILED(hr))
			throw Exception(__LINE__, __FILE__, "Failed to initialize ScratchImage", hr);
	}

	void Surface::Clear(const Color& fillValue) const noexcept
	{
		const auto height = GetHeight();
		auto& imgData = *scratch.GetImage(0, 0, 0);
		for (size_t y = 0u; y < height; y++)
		{
			const auto rowStart = reinterpret_cast<Color*>(imgData.pixels + imgData.rowPitch * y);
			std::fill_n(rowStart, imgData.width, fillValue);
		}
	}

	void Surface::PutPixel(unsigned int x, unsigned int y, Color c) const noexcpt
	{
		assert(x >= 0);
		assert(y >= 0);
		assert(x < GetWidth());
		assert(y < GetHeight());
		auto& imgData = *scratch.GetImage(0, 0, 0);
		reinterpret_cast<Color*>(&imgData.pixels[y * imgData.rowPitch])[x] = c;
	}

	Surface::Color Surface::GetPixel(unsigned int x, unsigned int y) const noexcpt
	{
		assert(x >= 0);
		assert(y >= 0);
		assert(x < GetWidth());
		assert(y < GetHeight());
		auto& imgData = *scratch.GetImage(0, 0, 0);
		return reinterpret_cast<Color*>(&imgData.pixels[y * imgData.rowPitch])[x];
	}

	unsigned int Surface::GetWidth() const noexcept
	{
		return static_cast<unsigned int>(scratch.GetMetadata().width);
	}

	unsigned int Surface::GetHeight() const noexcept
	{
		return static_cast<unsigned int>(scratch.GetMetadata().height);
	}

	Surface::Color* Surface::GetBufferPtr() noexcept
	{
		return reinterpret_cast<Color*>(scratch.GetPixels());
	}

	const Surface::Color* Surface::GetBufferPtr() const noexcept
	{
		return const_cast<Surface*>(this)->GetBufferPtr();
	}

	const Surface::Color* Surface::GetBufferPtrConst() const noexcept
	{
		return const_cast<Surface*>(this)->GetBufferPtr();
	}

	Surface Surface::FromFile(const std::string& name)
	{
		ScratchImage scratch;
		HRESULT hr = LoadFromWICFile(String::ToWide(name).c_str(), WIC_FLAGS_NONE, nullptr, scratch);

		if (FAILED(hr))
			throw Exception(__LINE__, __FILE__, name, "Failed to load image", hr);

		if (scratch.GetImage(0, 0, 0)->format != format)
		{
			ScratchImage converted;
			hr = Convert(
				*scratch.GetImage(0, 0, 0),
				format,
				TEX_FILTER_DEFAULT,
				TEX_THRESHOLD_DEFAULT,
				converted
			);

			if (FAILED(hr))
				throw Exception(__LINE__, __FILE__, name, "Failed to convert image", hr);

			return Surface(std::move(converted));
		}
		return Surface(std::move(scratch));
	}

	void Surface::Save(const std::string& filename) const
	{
		const auto GetCodecID = [](const std::string& filename)
		{
			const std::filesystem::path path = filename;
			const auto ext = path.extension().string();

			if (ext == ".png")
				return WIC_CODEC_PNG;
			if (ext == ".jpg")
				return WIC_CODEC_JPEG;
			if (ext == ".bmp")
				return WIC_CODEC_BMP;

			throw Exception(__LINE__, __FILE__, filename, "Image format not supported");
		};

		HRESULT hr = SaveToWICFile(
			*scratch.GetImage(0, 0, 0),
			WIC_FLAGS_NONE,
			GetWICCodec(GetCodecID(filename)),
			String::ToWide(filename).c_str()
		);
		if (FAILED(hr))
		{
			throw Exception(__LINE__, __FILE__, filename, "Failed to save image", hr);
		}
	}


	bool Surface::AlphaLoaded() const noexcept
	{
		return !scratch.IsAlphaAllOpaque();
	}

	Surface::Surface(ScratchImage scratch) noexcept
		: scratch(std::move(scratch))
	{
	}

	// surface exception stuff
	Surface::Exception::Exception(int line, const char* file, std::string note, std::optional<HRESULT> hr) noexcept
		: Hw3DException(line, file),
		  note("[Note] " + note)
	{
		if (hr)
			note = "[Error String] " + Windowing::Window::Exception::TranslateErrorCode(*hr) + "\n" + note;
	}

	Surface::Exception::Exception(int line, const char* file, std::string filename, std::string note_in, std::optional<HRESULT> hr) noexcept
		: Hw3DException(line, file)
	{
		using namespace std::string_literals;
		note = "[File] "s + filename + "\n"s + "[Note] "s + note_in;

		if (hr)
			note = "[Error String] " + Windowing::Window::Exception::TranslateErrorCode(*hr) + note;
	}

	const char* Surface::Exception::what() const noexcept
	{
		std::ostringstream oss;
		oss << Hw3DException::what() << std::endl << GetNote();
		whatBuffer = oss.str();
		return whatBuffer.c_str();
	}

	const char* Surface::Exception::GetType() const noexcept
	{
		return "HW3D Surface Exception";
	}

	const std::string& Surface::Exception::GetNote() const noexcept
	{
		return note;
	}
}

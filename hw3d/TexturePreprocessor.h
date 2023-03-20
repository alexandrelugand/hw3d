#pragma once
#include "Surface.h"

class TexturePreprocessor
{
public:
	static void FlipYAllNormalMapsInObj(const std::string& objPath);
	static void FlipYNormalMap(const std::string& pathIn, const std::string& pathOut);
	static void ValidateNormalMap(const std::string& pathIn, float thresholdMin, float thresholdMax);
	static void MakeStripes(const std::string& pathOut, int size, int stripeWidth);

private:
	template <typename F>
	static void TransformFile(const std::string& pathIn, const std::string& pathOut, F&& func);
	template <typename F>
	static void TransformSurface(Bind::Surface& surf, F&& func);
	static XMVECTOR ColorToVector(Bind::Surface::Color c) noexcept;
	static Bind::Surface::Color VectorToColor(FXMVECTOR n) noexcept;
};

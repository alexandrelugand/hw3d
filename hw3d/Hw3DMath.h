#pragma once

using namespace DirectX;

constexpr float PI = 3.14159265f;
constexpr double PI_D = 3.1415926535897932;

template <typename T>
constexpr auto sq(const T& x) noexcept
{
	return x * x;
}

template <typename T>
T wrap_angle(T theta) noexcept
{
	constexpr T twoPi = static_cast<T>(2) * static_cast<T>(PI_D);
	const T mod = static_cast<T>(fmod(theta, twoPi));
	if (mod > static_cast<T>(PI_D))
		return mod - twoPi;

	if (mod < static_cast<T>(-PI_D))
		return mod + twoPi;

	return mod;
}

template <typename T>
constexpr T interpolate(const T& src, const T& dst, float alpha) noexcept
{
	return src + (dst - src) * alpha;
}

template <typename T>
constexpr T to_rad(T deg) noexcept
{
	return deg * PI / static_cast<T>(180.0);
}

static XMFLOAT3 ExtractEulerAngles(const XMFLOAT4X4& mat)
{
	XMFLOAT3 euler;

	euler.x = asinf(-mat._32); // Pitch
	if (cosf(euler.x) > 0.0001) // Not at poles
	{
		euler.y = atan2f(mat._31, mat._33); // Yaw
		euler.z = atan2f(mat._12, mat._22); // Roll
	}
	else
	{
		euler.y = 0.0f; // Yaw
		euler.z = atan2f(-mat._21, mat._11); // Roll
	}

	return euler;
}

static XMFLOAT3 ExtractTranslation(const XMFLOAT4X4& matrix)
{
	return {matrix._41, matrix._42, matrix._43};
}

static XMMATRIX ScaleTranslation(XMMATRIX matrix, float scale)
{
	matrix.r[3].m128_f32[0] *= scale;
	matrix.r[3].m128_f32[1] *= scale;
	matrix.r[3].m128_f32[2] *= scale;
	return matrix;
}

template <typename T>
constexpr T gauss(T x, T sigma) noexcept
{
	const auto ss = sq(sigma);
	return (static_cast<T>(1.0) / sqrt(static_cast<T>(2.0) * static_cast<T>(PI_D) * ss)) * exp(-sq(x) / (static_cast<T>(2.0) * ss));
}

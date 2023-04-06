#pragma once

namespace Shaders
{
	namespace CBuf
	{
		inline constexpr unsigned int PointLight = 0u;
		inline constexpr unsigned int Transform = 1u;
		inline constexpr unsigned int Object = 2u;
		inline constexpr unsigned int Shadow = 3u;
		inline constexpr unsigned int Scaling = 4u;
		inline constexpr unsigned int Skybox = 4u;
	};

	namespace Texture
	{
		inline constexpr unsigned int Diffuse = 0u;
		inline constexpr unsigned int Specular = 1u;
		inline constexpr unsigned int Normal = 2u;
		inline constexpr unsigned int ShadowMap = 3u;
		inline constexpr unsigned int Skybox = 4u;
	};

	namespace Sampler
	{
		inline constexpr unsigned int Diffuse = 0u;
		inline constexpr unsigned int ShadowMap = 1u;
	};
}

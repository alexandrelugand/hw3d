#pragma once

namespace Draw
{
	template <class T>
	class TestObject : public DrawableBase<T>
	{
	public:
		TestObject(Graphics& gfx, std::mt19937& rng,
		           std::uniform_real_distribution<float>& adist,
		           std::uniform_real_distribution<float>& ddist,
		           std::uniform_real_distribution<float>& odist,
		           std::uniform_real_distribution<float>& rdist)
			:
			r(rdist(rng)),
			theta(adist(rng)),
			phi(adist(rng)),
			chi(adist(rng)),
			droll(ddist(rng)),
			dpitch(ddist(rng)),
			dyaw(ddist(rng)),
			dtheta(odist(rng)),
			dphi(odist(rng)),
			dchi(odist(rng))
		{
		}

		void Update(float dt) noexcept override
		{
			roll = wrap_angle(roll + droll * dt);
			pitch = wrap_angle(pitch + dpitch * dt);
			yaw = wrap_angle(yaw + dyaw * dt);
			theta = wrap_angle(theta + dtheta * dt);
			phi = wrap_angle(phi + dphi * dt);
			chi = wrap_angle(chi + dchi * dt);
		}

		XMMATRIX GetTransform() const noexcept override
		{
			return XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
				XMMatrixTranslation(r, 0.0f, 0.0f) *
				XMMatrixRotationRollPitchYaw(theta, phi, chi);
		}

	protected:
		// positional
		float r;
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float theta;
		float phi;
		float chi;

		// speed (delta/s)
		float droll;
		float dpitch;
		float dyaw;
		float dtheta;
		float dphi;
		float dchi;
	};
}

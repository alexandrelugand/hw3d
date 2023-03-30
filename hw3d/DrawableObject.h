#pragma once

namespace Draw
{
	class DrawableObject : public Drawable
	{
	public:
		DrawableObject(Graphics& gfx, const Material& mat, const aiMesh& mesh, float scale)
			: Drawable(gfx, mat, mesh, scale), gfx(gfx), id(GetNextId()), name(typeid(*this).name()), pos({0.0f, 0.0f, 0.0f}), scale(scale)
		{
		}

		DrawableObject(Graphics& gfx, float scale = 1.0f, const XMFLOAT3& position = {0.0f, 0.0f, 0.0f})
			: gfx(gfx), id(GetNextId()), name("Draw"), pos(position), scale(scale)
		{
		}

		void Update(float dt) noexcept
		{
			roll = wrap_angle(roll + dt);
			pitch = wrap_angle(pitch + dt);
			yaw = wrap_angle(yaw + dt);
			theta = wrap_angle(theta + dt);
			phi = wrap_angle(phi + dt);
			chi = wrap_angle(chi + dt);
		}

		void Reset() noexcept
		{
			pos = XMFLOAT3{0.0f, 0.0f, 0.0f};
			roll = 0.0f;
			pitch = 0.0f;
			yaw = 0.0f;
			theta = 0.0f;
			phi = 0.0f;
			chi = 0.0f;
		}

		int GetId() const noexcept;
		const std::string& GetName() const noexcept;
		void SetName(const std::string& name_in) noexcept;
		std::string GetSysName() const noexcept;

		const XMFLOAT3& GetPosition() const noexcept;
		void SetPosition(XMFLOAT3 position) noexcept;

		XMFLOAT3 GetRotation() const noexcept;
		void SetRotation(XMFLOAT3 rotation) noexcept;

		XMMATRIX GetTransform() const noexcept override;

	protected:
		Graphics& gfx;

		int id;
		std::string name;
		XMFLOAT3 pos;
		float scale;

		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float theta = 0.0f;
		float phi = 0.0f;
		float chi = 0.0f;

		static int GetNextId()
		{
			static int next = 1;
			return next++;
		}
	};
}

#pragma once

class Gun;
class Bullet;

class Player
{
public:
	Player();
	~Player();

	void Update();
	void Render();

	void Move(const Vector2& pos);
	void Fire(const Vector2& mousePos);
	shared_ptr<Transform> GetTransform() { return _quad->GetTransform(); }
	shared_ptr<Gun> GetGun() { return _gun; }

private:
	shared_ptr<Quad> _quad;
	shared_ptr<Collider> _rectCol;
	shared_ptr<Gun> _gun;
};

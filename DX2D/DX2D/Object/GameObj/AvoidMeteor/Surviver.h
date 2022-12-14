#pragma once

class Meteor;

class Surviver
{
public:
	Surviver();
	~Surviver();

	UINT& GetHP() { return _hp; }

	void Update();
	void Render();

	shared_ptr<Collider> GetCollider() { return _rectCol; }

	bool _isAlive = true;
private:
	void Input();

	UINT _hp = 10;
	shared_ptr<Quad> _quad;
	shared_ptr<Collider> _rectCol;

	float _speed = 300.0f;
};


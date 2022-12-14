#include "framework.h"

#include "CH_Bullet.h"

#include "CH_Player.h"

CH_Player::CH_Player()
{
	_transform = make_shared<Transform>();
	_firePos = make_shared<Transform>();
	_firePos->SetParent(_transform);
	_firePos->GetPos().x += 50;
	_collider = make_shared<CircleCollider>(30);
	_collider->GetTransform()->SetParent(_transform);
	CreateAction("Idle");
	CreateAction("Run");
	CreateAction("AimStraightShot");
	CreateAction("Jump");

	_actions[State::IDLE]->SetSpeed(0.1f);
	_actions[State::RUN]->SetSpeed(0.07f);
	_actions[State::SHOT]->SetRepeatType(Action::Type::END);
	_actions[State::SHOT]->SetEndEvent(std::bind(&CH_Player::SetIdle, this));
	_actions[State::JUMP]->SetRepeatType(Action::Type::END);
	_actions[State::JUMP]->SetEndEvent(std::bind(&CH_Player::SetIdle, this));
	_actions[State::JUMP]->SetSpeed(0.07f);

	_transform->GetPos() = { CENTER_X, CENTER_Y - 200 };

	for (int i = 0; i < 10; i++)
	{
		shared_ptr<CH_Bullet> bullet = make_shared<CH_Bullet>();
		_bullets.push_back(bullet);
	}
}

CH_Player::~CH_Player()
{
}

void CH_Player::Input()
{
	if (_state == State::SHOT || _state == State::JUMP)
		return;

	_state = State::IDLE;

	if (KEY_PRESS('A'))
	{
		_transform->GetPos().x -= DELTA_TIME * _speed;
		SetLeft();
		_state = State::RUN;
	}
	if (KEY_PRESS('D'))
	{
		_transform->GetPos().x += DELTA_TIME * _speed;
		SetRight();
		_state = State::RUN;
	}
}

void CH_Player::Update()
{
	Shot();
	Jump();
	Input();

	_transform->Update();
	_firePos->Update();
	_collider->Update();
	_actions[_state]->Update();
	_sprites[_state]->Update();

	for (auto bullet : _bullets)
		bullet->Update();
}

void CH_Player::Render()
{
	_sprites[_state]->SetSpriteAction(_actions[_state]->GetCurClip());
	_sprites[_state]->Render();
	_collider->Render();

	for (auto bullet : _bullets)
		bullet->Render();
}

void CH_Player::PostRender()
{
}

void CH_Player::CreateAction(string state)
{
	wstring srvPath;
	srvPath.assign(state.begin(), state.end());
	srvPath = L"CupHead/" + srvPath + L".png";
	shared_ptr<SRV> srv = SRVManager::GetInstance()->AddSRV(srvPath);
	vector<Action::Clip> clips;

	shared_ptr<tinyxml2::XMLDocument> document = make_shared<tinyxml2::XMLDocument>();
	string xmlPath = "Resource/Texture/CupHead/" + state + ".xml";
	document->LoadFile(xmlPath.c_str());

	tinyxml2::XMLElement* textureAtlas = document->FirstChildElement();
	tinyxml2::XMLElement* row = textureAtlas->FirstChildElement();

	int averageW = 0;
	int averageH = 0;
	int count = 0;

	while (true)
	{
		if (row == nullptr)
			break;
		int x = row->FindAttribute("x")->IntValue();
		int y = row->FindAttribute("y")->IntValue();
		int w = row->FindAttribute("w")->IntValue();
		averageW += w;
		int h = row->FindAttribute("h")->IntValue();
		averageH += h;

		count++;

		Action::Clip clip = Action::Clip(x, y, w, h, srv);
		clips.push_back(clip);

		row = row->NextSiblingElement();
	}

	shared_ptr<Sprite> sprite;
	averageW /= count * 1.5f;
	averageH /= count * 1.5f;

	sprite = make_shared<Sprite>(srvPath, Vector2(averageW, averageH));
	sprite->GetTransform()->SetParent(_transform);

	_sprites.push_back(sprite);
	shared_ptr<Action> action = make_shared<Action>(clips, state, Action::LOOP, 0.1f);
	action->Play();
	_actions.push_back(action);
}

void CH_Player::Shot()
{
	if (KEY_DOWN(VK_LBUTTON))
	{
		_state = State::SHOT;
		_actions[_state]->Play();

		for (auto bullet : _bullets)
		{
			if (bullet->isActive == false)
			{
				bullet->isActive = true;
				bullet->SetDirection(_firePos->GetPos().Normal());
				bullet->GetTransform()->GetPos() = _firePos->GetWorldPos();
				bullet->GetTransform()->Update();
				break;
			}
		}
	}
}

void CH_Player::Jump()
{
	if (KEY_DOWN(VK_SPACE))
	{
		_transform->GetPos().y += _jump;
		_state = State::JUMP;
		_actions[_state]->Play();
	}
}

void CH_Player::SetIdle()
{
	_state = State::IDLE;
	_transform->GetPos().y = CENTER_Y - 200.0f;
}

void CH_Player::SetLeft()
{
	_firePos->GetPos().x = -30;
	for (auto sprite : _sprites)
	{
		sprite->SetLeft();
	}
}

void CH_Player::SetRight()
{
	_firePos->GetPos().x = 30;
	for (auto sprite : _sprites)
	{
		sprite->SetRight();
	}
}

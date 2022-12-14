#pragma once

class Planet;

class SolarScene : public Scene
{
public:
	SolarScene();
	virtual ~SolarScene();

	virtual void Update() override;
	virtual void Render() override;
private:
	shared_ptr<Planet> _sun;
	shared_ptr<Planet> _earth;
	shared_ptr<Planet> _moon;

	shared_ptr<Transform> _sunTrans;
	shared_ptr<Transform> _earthTrans;
};

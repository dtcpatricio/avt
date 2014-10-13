#pragma once
class Frog
{
public:
	Frog();
	~Frog();

	float getX();
	float getY();
	float getZ();

	void setX(float x);
	void setY(float y);
	void setZ(float z);

private:
	float xPos;
	float yPos;
	float zPos;
};


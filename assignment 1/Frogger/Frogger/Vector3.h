#pragma once
class Vector3
{
public:
	Vector3();
	Vector3(float x, float y, float z);
	~Vector3();

	float getX();
	float getY();
	float getZ();

	void set(float x, float y, float z);
	bool operator=(Vector3 *vec);
	Vector3* operator*(float scalar);
	Vector3* operator+(Vector3 *vec);
	Vector3* operator-(Vector3 *vec);

private:
	float _x;
	float _y;
	float _z;
};


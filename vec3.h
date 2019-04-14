/* minimal 3 dimensional vector class */
class Vec3
{
public:
	float f[3]; // float container 

	// constructors
	Vec3() {}
	Vec3(float x, float y, float z) { f[0] = x; f[1] = y; f[2] = z; }

	// basic arithmetic operators
	Vec3 operator+ (const Vec3 &v) { return Vec3(f[0] + v.f[0], f[1] + v.f[1], f[2] + v.f[2]); }
	Vec3 operator- (const Vec3 &v) { return Vec3(f[0] - v.f[0], f[1] - v.f[1], f[2] - v.f[2]); }
	Vec3 operator/ (const float &a) { return Vec3(f[0] / a, f[1] / a, f[2] / a); }
	Vec3 operator* (const float &a){ return Vec3(f[0] * a, f[1] * a, f[2] * a); }

	// returns the negative of the vector
	Vec3 operator- () { return Vec3(-f[0], -f[1], -f[2]); }

	// concatenated addition
	void operator+= (const Vec3 &v)
	{
		f[0] += v.f[0];
		f[1] += v.f[1];
		f[2] += v.f[2];
	}

	// concatenated subtraction
	void operator-= (const Vec3 &v)
	{
		f[0] -= v.f[0];
		f[1] -= v.f[1];
		f[2] -= v.f[2];
	}

	// returns the length of the vector
	float Length() { return sqrt(f[0] * f[0] + f[1] * f[1] + f[2] * f[2]); }

	// normalizes the vector
	Vec3 Normalized()
	{
		float l = Length();
		return Vec3(f[0] / l, f[1] / l, f[2] / l);
	}

	// returns the dot product of two vectors
	float Dot(const Vec3 &v)
	{
		return f[0] * v.f[0] + f[1] * v.f[1] + f[2] * v.f[2];
	}

	// returns the cross product of two vectors
	Vec3 Cross(const Vec3 &v)
	{
		return Vec3(f[1] * v.f[2] - f[2] * v.f[1], f[2] * v.f[0] - f[0] * v.f[2], f[0] * v.f[1] - f[1] * v.f[0]);
	}
};
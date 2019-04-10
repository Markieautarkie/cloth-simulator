/* represents a particle with mass */
class Particle
{
private:
	bool fixed;        // is the particle movable or not
	bool broken;       // is this particle part of a broken constraint or not

	float mass;        // particle mass
	Vec3 acceleration; // current acceleration of the particle

	Vec3 currPos;      // current particle position
	Vec3 prevPos;      // previous particle position
	Vec3 nonNormal;    // non-normalized normal (used for shading)

public:
	// constructors
	Particle() {}
	Particle(Vec3 pos) : fixed(false), broken(false), mass(1), acceleration(Vec3(0, 0, 0)), currPos(pos), prevPos(pos), nonNormal(Vec3(0, 0, 0)) {}

	// adds a force to the particle
	void AddForce(Vec3 force) { acceleration += force / mass; }

	// updates the position of the particle using verlet integration
	void Update()
	{
		// return if the particle is unmovable
		if (fixed)
			return;

		// verlet integration
		Vec3 temp = currPos;
		currPos = currPos + (currPos - prevPos) * (1.0 - DAMPING) + acceleration * TIMESTEP2;
		prevPos = temp;

		// reset acceleration
		acceleration = Vec3(0, 0, 0);			
	}

	// position functions
	Vec3& GetPos() { return currPos; }
	void OffsetPos(const Vec3 v) { if (!fixed) currPos += v; }

	// normal functions, normal is not unit length
	Vec3& GetNormal() { return nonNormal; }
	void ResetNormal() { nonNormal = Vec3(0, 0, 0); }
	void AddToNormal(Vec3 normal) { nonNormal += normal.Normalized(); }

	// resets acceleration
	void ResetAcceleration() { acceleration = Vec3(0, 0, 0); }

	// make the particle movable/unmovable
	bool GetMoveState() { return fixed; }
	void MakeMovable() { fixed = false; }
	void MakeUnmovable() { fixed = true; }

	// flag the particle as being part of a broken constraint
	void SetToBroken() { broken = true; }
	bool IsBroken() { return broken; }
};
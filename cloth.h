/* class for the cloth, made with a mass spring system */
class Cloth
{
private:
	Vec3 worldPos;                       // the position of the cloth in world
	float width, height;                 // the width and height of the cloth
	int particlesWidth, particlesHeight; // number of particles in the cloth

	bool showTears;     			     // visualize the tears or not
	bool tearable;                       // is the cloth tearable or not
	float stretch;                       // the factor with which the particles can stretch the constraint before it breaks

	Pattern pattern;     // the cloth pattern
	Vec3 color1, color2; // the color(s) of the cloth

	// the particles in the cloth and the constraints between these particles
	std::vector<Particle> particles; 
	std::vector<Constraint> constraints, backupConstraints;

	// the amount of constraint solving iterations of the cloth (less is softer, more is rigid)
	int constIter;

	// method to get a certain particle and to set constraints between particles
	Particle* GetParticle(int x, int y) { return &particles[x + y * particlesWidth]; }
	void SetConstraint(Particle *p1, Particle *p2) { constraints.push_back(Constraint(p1, p2)); }

	// set the pattern of the cloth
	Vec3 ClothPattern(int x, int y);

	// calculates the normal of a triangle, defined by 3 particles
	Vec3 CalcTriangleNormal(Particle *p1, Particle *p2, Particle *p3);

	// color and draw a triangle
	void DrawTriangle(Particle *p1, Particle *p2, Particle *p3, const Vec3 color);

	// method to simulate forces on the triangle
	void AddForcesToTriangle(Particle *p1, Particle *p2, Particle *p3, const Vec3 direction);

public:
	// constructor
	Cloth(Vec3 worldPos, float width, float height, int particlesWidth, int particlesHeight, 
		Pattern pattern = Pattern::Vertical, Vec3 color1 = Vec3(0.6f, 0.2f, 0.2f), Vec3 color2 = Vec3(1.0f, 1.0f, 1.0f), 
		int constIter = 15, float stretchFactor = 1.0f)
		: worldPos(worldPos), width(width), height(height), particlesWidth(particlesWidth), particlesHeight(particlesHeight), 
		pattern(pattern), color1(color1), color2(color2), constIter(constIter)
	{
		// set the initial tearing state of a cloth to false
		tearable = false;

		// the stretch depends on the stretchFactor and the amount of particles in the cloth
		float particleAmount = ((float)particlesWidth * (float)particlesHeight) / 1000;
		float particleDensity = (particlesWidth > particlesHeight) ? 
			(float)particlesWidth / (float)particlesHeight : (float)particlesHeight / (float)particlesWidth;
		stretch = stretchFactor * particleDensity * particleAmount;

		// resize the vector to house all the particles
		particles.resize(particlesWidth*particlesHeight);

		// initialize all the particles in the grid
		for (int x = 0; x < particlesWidth; x++)
			for (int y = 0; y < particlesHeight; y++)
			{
				Vec3 pos = Vec3(width * (x / (float)particlesWidth), -height * (y / (float)particlesHeight), 0);
				particles[x + y * particlesWidth] = Particle(pos + worldPos);
			}
		
		// for each particle, connect it to its neighbors
		for (int x = 0; x < particlesWidth; x++)
			for (int y = 0; y < particlesHeight; y++)
			{
				if (x < particlesWidth - 1) SetConstraint(GetParticle(x, y), GetParticle(x + 1, y));
				if (y < particlesHeight - 1) SetConstraint(GetParticle(x, y), GetParticle(x, y + 1));
				if (x < particlesWidth - 1 && y < particlesHeight - 1) SetConstraint(GetParticle(x, y), GetParticle(x + 1, y + 1));
				if (x < particlesWidth - 1 && y < particlesHeight - 1) SetConstraint(GetParticle(x + 1, y), GetParticle(x, y + 1));
			}
		
		// do the same for the secondary neighbors
		for (int x = 0; x < particlesWidth; x++)
			for (int y = 0; y < particlesHeight; y++)
			{
				if (x < particlesWidth - 2) SetConstraint(GetParticle(x, y), GetParticle(x + 2, y));
				if (y < particlesHeight - 2) SetConstraint(GetParticle(x, y), GetParticle(x, y + 2));
				if (x < particlesWidth - 2 && y < particlesHeight - 2) SetConstraint(GetParticle(x, y), GetParticle(x + 2, y + 2));
				if (x < particlesWidth - 2 && y < particlesHeight - 2) SetConstraint(GetParticle(x + 2, y), GetParticle(x, y + 2));
			}
		
		// Fix the top 2 corners so that the cloth hangs
		SwitchCorner(1); SwitchCorner(2);
	}

	// draw the triangles in a smooth shaded format
	// four particles in a grid are connected via 2 triangles, diagonal from bottom left to top right
	void DrawShaded();

	// updates the cloth by satisfying the constraints and updating the particle positions
	void Update();

	// adds a force to all the particles in the cloth
	void AddForce(const Vec3 direction);

	// add the wind force to all the particles, 
	// seperately added since the final force is proportional to the triangle area from the wind direction
	void AddWindForce(const Vec3 direction);

	// switches a specific corner state
	void SwitchCorner(int corner);

	// reset the position of the cloth and cloth state
	void ResetCloth();

	// resolves collision with a sphere
	void SphereCollision(const Vec3 center, const float radius);

	// show the tears in the cloth or not
	void SwitchShowTears() { showTears = !showTears; }
	// make the cloth tearable or not
	void SwitchTearable() { tearable = !tearable; }
};
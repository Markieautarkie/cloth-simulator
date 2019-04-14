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
	Vec3 ClothPattern(int x, int y)
	{
		switch (pattern)
		{
		case Pattern::Vertical:
			if (x % 2)
				return color1;
			else
				return color2;
			break;
		case Pattern::Horizontal:
			if (y % 2)
				return color1;
			else
				return color2;
			break;
		case Pattern::Checkerboard:
			if (x % 2 == y % 2)
				return color1;
			else
				return color2;
			break;
		case Pattern::Random:
			if (rand() % 2 == 0)
				return color1;
			else
				return color2;
			break;
		}
	}

	// calculates the normal of a triangle, defined by 3 particles
	Vec3 CalcTriangleNormal(Particle *p1, Particle *p2, Particle *p3)
	{
		// get the vertices and calculate the edges 
		Vec3 v1 = p1->GetPos(), v2 = p2->GetPos(), v3 = p3->GetPos();
		Vec3 e1 = v2 - v1, e2 = v3 - v1;

		// return the normal
		return e1.Cross(e2);
	}

	// color and draw a triangle
	void DrawTriangle(Particle *p1, Particle *p2, Particle *p3, const Vec3 color)
	{
		// set the color
		glColor3fv((GLfloat*) &color);

		// set the vertices and normals for the vertices 
		glNormal3fv((GLfloat *) &(p1->GetNormal().Normalized()));
		glVertex3fv((GLfloat *) &(p1->GetPos()));

		glNormal3fv((GLfloat *) &(p2->GetNormal().Normalized()));
		glVertex3fv((GLfloat *) &(p2->GetPos()));

		glNormal3fv((GLfloat *) &(p3->GetNormal().Normalized()));
		glVertex3fv((GLfloat *) &(p3->GetPos()));
	}

	// method to simulate forces on the triangle
	void AddForcesToTriangle(Particle *p1, Particle *p2, Particle *p3, const Vec3 direction)
	{
		// calculate the normal and normalize it
		Vec3 normal = CalcTriangleNormal(p1, p2, p3);
		Vec3 d = normal.Normalized();

		// calculate the force and add the force to the vertices
		Vec3 force = normal * (d.Dot(direction));
		p1->AddForce(force);
		p2->AddForce(force);
		p3->AddForce(force);
	}

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
	void DrawShaded()
	{
		// reset normals
		std::vector<Particle>::iterator p;
		for (p = particles.begin(); p != particles.end(); p++)
			(*p).ResetNormal();

		// create smooth normals by adding up all the normals from every vertex (connected vertices are added twice)
		for (int x = 0; x < particlesWidth - 1; x++)
			for (int y = 0; y < particlesHeight - 1; y++)
			{
				Vec3 normal = CalcTriangleNormal(GetParticle(x + 1, y), GetParticle(x, y), GetParticle(x, y + 1));
				GetParticle(x + 1, y)->AddToNormal(normal);
				GetParticle(x, y)->AddToNormal(normal);
				GetParticle(x, y + 1)->AddToNormal(normal);

				normal = CalcTriangleNormal(GetParticle(x + 1, y + 1), GetParticle(x + 1, y), GetParticle(x, y + 1));
				GetParticle(x + 1, y + 1)->AddToNormal(normal);
				GetParticle(x + 1, y)->AddToNormal(normal);
				GetParticle(x, y + 1)->AddToNormal(normal);
			}
		

		// draw the triangles
		glBegin(GL_TRIANGLES);
		for (int x = 0; x < particlesWidth - 1; x++)
			for (int y = 0; y < particlesHeight - 1; y++)
			{
				// this sets the color of the cloth
				Vec3 color = ClothPattern(x, y);

				// get the particles that need to be drawn
				Particle *p1 = GetParticle(x, y);
				Particle *p2 = GetParticle(x, y + 1);
				Particle *p3 = GetParticle(x + 1, y);
				Particle *p4 = GetParticle(x + 1, y + 1);

				// make sure the particles aren't part of a broken constraint before drawing the triangles
				if (showTears || (!p3->IsBroken() || !p1->IsBroken() || !p2->IsBroken()))
					DrawTriangle(p3, p1, p2, color);
				if (showTears || (!p4->IsBroken() || !p3->IsBroken() || !p2->IsBroken()))
					DrawTriangle(p4, p3, p2, color);
			}
		glEnd();
	}

	// updates the cloth by satisfying the constraints and updating the particle positions
	void Update()
	{
		// iterate over the constraints several times and satisfy them
		// if the constraint stretched too far, break it
		std::vector<Constraint>::iterator constraint;
		for (int i = 0; i < constIter; i++)
			for (constraint = constraints.begin(); constraint != constraints.end(); constraint++)
				if ((*constraint).SatisfyConstraint(tearable, stretch))
				{
					// save a copy of the broken constraint
					backupConstraints.push_back(*constraint);
					constraints.erase(constraint--);
				}
			
		// update the particles
		std::vector<Particle>::iterator particle;
		for (particle = particles.begin(); particle != particles.end(); particle++)
			(*particle).Update();
	}

	// adds a force to all the particles in the cloth
	void AddForce(const Vec3 direction)
	{
		std::vector<Particle>::iterator particle;
		for (particle = particles.begin(); particle != particles.end(); particle++)
			(*particle).AddForce(direction);	
	}

	// add the wind force to all the particles, seperately added since the final force is proportional to the triangle area from the wind direction
	void AddWindForce(const Vec3 direction)
	{
		for (int x = 0; x < particlesWidth - 1; x++)
			for (int y = 0; y < particlesHeight - 1; y++)
			{
				// get the particles that we need to set impulses to
				Particle *p1 = GetParticle(x, y);
				Particle *p2 = GetParticle(x, y + 1);
				Particle *p3 = GetParticle(x + 1, y);
				Particle *p4 = GetParticle(x + 1, y + 1);

				// make sure the particles aren't part of a broken constraint before applying the impulses
				if (showTears || (!p3->IsBroken() || !p1->IsBroken() || !p2->IsBroken()))
					AddForcesToTriangle(p3, p1, p2, direction);
				if (showTears || (!p4->IsBroken() || !p3->IsBroken() || !p2->IsBroken()))
					AddForcesToTriangle(p4, p3, p2, direction);
			}
	}

	// switches a specific corner state
	void SwitchCorner(int corner)
	{
		// do this for 3 pixels at the time
		for (int i = 0; i < 3; i++)
		{
			// get a reference to the current particle depending on the corner
			Particle *p;
			switch (corner)
			{
			case 1:
				p = GetParticle(i, 0);
				break;
			case 2:
				p = GetParticle(particlesWidth - 1 - i, 0);
				break;
			case 3:
				p = GetParticle(particlesWidth - 1 - i, particlesHeight - 1);
				break;
			case 4:
				p = GetParticle(i, particlesHeight - 1);
				break;
			default:
				p = NULL;
				break;
			}

			// make the points movable or unmovable depending on the state
			if (p->GetMoveState() == false && !p->IsBroken())
				p->MakeUnmovable();
			else
				p->MakeMovable();
		}
	}

	// reset the position of the cloth and cloth state
	void ResetCloth()
	{
		// reset all particles
		for (int x = 0; x < particlesWidth; x++)
			for (int y = 0; y < particlesHeight; y++)
			{
				// reset the positions
				Vec3 pos = Vec3(width * (x / (float)particlesWidth), -height * (y / (float)particlesHeight), 0);
				particles[x + y * particlesWidth] = Particle(pos + worldPos);

				// reset the state of the particles
				GetParticle(x, y)->SetToFixed();
			}

		// repair the constraints between all particles
		constraints.insert(constraints.end(), backupConstraints.begin(), backupConstraints.end());

		// set the top 2 corners so that the cloth doesn't immediately fall again
		SwitchCorner(1); SwitchCorner(2);
	}

	// show the tears in the cloth or not
	void SwitchShowTears() { showTears = !showTears; }

	// make the cloth tearable or not
	void SwitchTearable() { tearable = !tearable; }

	// resolves collision with a sphere
	void SphereCollision(const Vec3 center, const float radius)
	{
		// loop over all the particles
		std::vector<Particle>::iterator particle;
		for (particle = particles.begin(); particle != particles.end(); particle++)
		{
			// check how far the particle is away from the sphere center
			Vec3 v = (*particle).GetPos() - center;
			float l = v.Length();

			// if the particle is inside the sphere, project the particle on the surface of the sphere
			if (v.Length() < radius) 
				(*particle).OffsetPos(v.Normalized()*(radius - l)); 			
		}
	}
};
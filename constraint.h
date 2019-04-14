/* simple constraint class for the cloth simulation */
class Constraint
{
private:
	float restDist; // the rest length between two particles

public:
	Particle *p1, *p2; // two connected particles

	// constructor
	Constraint(Particle *p1, Particle *p2) : p1(p1), p2(p2)
	{
		Vec3 spring = p1->GetPos() - p2->GetPos();
		restDist = spring.Length();
	}

	// satisfy the constraint between two particles
	// if the constraint stretches too much, it should break
	bool SatisfyConstraint(bool tearable, float stretchFactor)
	{
		// get the spring and the current length of the spring
		Vec3 spring = p2->GetPos() - p1->GetPos();
		float currDist = spring.Length(); 

		// check if the constraint should break
		// if so, flag the particles as part of a broken constraint
		if (tearable && currDist > restDist * stretchFactor)
		{
			// randomly break one of the particles
			bool randTear = rand() % 2;		
			if (randTear) { p1->SetToBroken(); }
			else { p2->SetToBroken(); }

			return true;
		}

		// calculate the correction to move back to the rest length
		Vec3 correction = (spring * (1 - restDist / currDist)) * 0.5;

		// apply the correction to both particles
		p1->OffsetPos(correction); 
		p2->OffsetPos(-correction);

		// the constraint isn't broken so return false
		return false;
	}
};
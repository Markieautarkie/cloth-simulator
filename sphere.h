/* class to generate sphere objects */
class Sphere 
{
private:
	// position and radius of the sphere
	Vec3 position;
	float radius;

	// color of the sphere
	Vec3 color;

	// setup for making the sphere out of quads
	int sectorCount;
	int stackCount;
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<unsigned int> indices;

	//interleaved
	std::vector<float> interleavedVertices;
	int interleavedStride;

	// sets the sectors and stacks
	void Set(float radius, int sectors, int stacks);

	// clears the arrays
	void ClearArrays();

	// build the vertices of the sphere
	void BuildVertices();

	// interleave all the vertices of the sphere
	void BuildInterleavedVertices();

	// adds a vertex
	void AddVertex(float x, float y, float z);

	// adds a normal
	void AddNormal(float nx, float ny, float nz);

	// adds indicess
	void AddIndices(unsigned int i1, unsigned int i2, unsigned int i3);

public:
	// constructor
	Sphere(Vec3 position, Vec3 color, float radius = 1.0f, int sectorCount = 36, int stackCount = 18) 
		: interleavedStride(24), position(position), color(color)
    {
        Set(radius, sectorCount, stackCount);
    }

	// draws the sphere
	void Draw() const;

    // update position - only y for now (depth)
	void UpdatePosition(float y);

	// returns the current sphere position
	Vec3 GetPosition() { return position; }
};
#pragma once

class Sphere 
{
public:
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

	// constructor
	Sphere(Vec3 position, Vec3 color, float radius = 1.0f, int sectorCount = 36, int stackCount = 18) : interleavedStride(24), position(position), color(color)
    {
        Set(radius, sectorCount, stackCount);
    }

	// sets the sectors and stacks
    void Set(float radius, int sectors, int stacks)
    {
        this->radius = radius;

        if(sectors >= 3)
            this->sectorCount = sectors;
        if(stacks >= 2)
            this->stackCount = stacks;

        BuildVertices();
    }

	// clears the arrays
    void ClearArrays()
    {
        std::vector<float>().swap(vertices);
        std::vector<float>().swap(normals);
        std::vector<unsigned int>().swap(indices);
    }

	// build the vertices of the sphere
    void BuildVertices()
    {
        // clear previous data
        ClearArrays();

		// vertex position
        float x, y, z, tx, ty, tz, xy;      
		// normal
        float nx, ny, nz, lengthInv = 1.0f / radius;    
        
        float sectorStep = 2 * PI / sectorCount;
        float stackStep = PI / stackCount;
        float sectorAngle, stackAngle;

        for (int i = 0; i <= stackCount; ++i)
        {
            stackAngle = PI / 2 - i * stackStep;
            xy = radius * cosf(stackAngle);
            tz = radius * sinf(stackAngle);
            z = position.f[2] + tz;

            for (int j = 0; j <= sectorCount; ++j)
            {
                sectorAngle = j * sectorStep;

                // vertex position
                tx = xy * cosf(sectorAngle);
                ty = xy * sinf(sectorAngle);
                x = position.f[0] + tx;
                y = position.f[1] + ty;
                AddVertex(x, y, z);

                // normalized vertex normal
                nx = tx * lengthInv;
                ny = ty * lengthInv;
                nz = tz * lengthInv;
                AddNormal(nx, ny, nz);
            }
        }

        // indices
        unsigned int k1, k2;
        for (int i = 0; i < stackCount; ++i)
        {
            k1 = i * (sectorCount + 1);
            k2 = k1 + sectorCount + 1;

            for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) 
            {
                if(i != 0)
                    AddIndices(k1, k2, k1 + 1);
                if (i != (stackCount - 1))
                    AddIndices(k1 + 1, k2, k2 + 1);
            }
        }

        BuildInterleavedVertices();
    }

	// interleave all the vertices of the sphere
    void BuildInterleavedVertices()
    {
        std::vector<float>().swap(interleavedVertices);

        std::size_t i, j;
        std::size_t count = vertices.size();
        for (int i = 0; i < count; i += 3)
        {
            interleavedVertices.push_back(vertices[i]);
            interleavedVertices.push_back(vertices[i+1]);
            interleavedVertices.push_back(vertices[i+2]);

            interleavedVertices.push_back(normals[i]);
            interleavedVertices.push_back(normals[i+1]);
            interleavedVertices.push_back(normals[i+2]);
        }
    }

	// adds a vertex
    void AddVertex(float x, float y, float z)
    {
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
    }

	// adds a normal
    void AddNormal(float nx, float ny, float nz)
    {
        normals.push_back(nx);
        normals.push_back(ny);
        normals.push_back(nz);
    }

	// adds indicess
    void AddIndices(unsigned int i1, unsigned int i2, unsigned int i3)
    {
        indices.push_back(i1);
        indices.push_back(i2);
        indices.push_back(i3);
    }

	// draws the sphere
    void Draw() const
    {
        glColor3f(color.f[0], color.f[1], color.f[2]);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glVertexPointer(3, GL_FLOAT, interleavedStride, &interleavedVertices[0]);
        glNormalPointer(GL_FLOAT, interleavedStride, &interleavedVertices[3]);
        
        glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, indices.data());

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
    }

    // update position - only y for now (depth)
    void UpdatePosition(float y)
    {
        position.f[1] = y;
        BuildVertices();
    }
};
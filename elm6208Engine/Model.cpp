#include "Model.h"
using namespace std;



Model::Model()
{
	vertArr = 0;
	vertCount = 0;
	objFile = "models/quad.obj";
}

// pass in object file name
Model::Model(std::string obFil)
{
	vertArr = 0;
	vertCount = 0;
	objFile = obFil;
}




Model::~Model()
{
}

bool Model::buffer()
{
	std::vector<glm::vec3> locs;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> norms;
	std::vector<VertInd> vertInds;

	std::string line;

	std::ifstream outFile;
	outFile.open(objFile);
	if (outFile.is_open())
	{
		while(getline(outFile, line))
		{
			std::istringstream ss(line);
			std::string label;
			ss >> label;

			if (label == "v")
			{
				float x, y, z;
				ss >> x >> y >> z;
				locs.push_back({ x,y,z });
				//locs.push_back(glm::vec3());
				//ss >> locs[locs.size - 1].x >> locs[locs.size - 1].y >> locs[locs.size - 1].z;
			}

			if (label == "vt")
			{
				float x2, y2;
				ss >> x2 >> y2;
				uvs.push_back({ x2,y2 });
				//uvs.push_back(glm::vec2());
				//ss >> uvs[uvs.size - 1].x >> uvs[uvs.size - 1].y;
			}

			if (label == "vn")
			{
				float x3, y3, z3;
				ss >> x3 >> y3 >> z3;
				norms.push_back({ x3,y3,z3 });
				//norms.push_back(glm::vec3());
				//ss >> norms[norms.size - 1].x >> norms[norms.size - 1].y >> norms[norms.size - 1].z;
			}

			if (label == "f")
			{
				char slash;
				unsigned int i1, i2, i3;
				ss >> i1 >> slash >> i2 >> slash >> i3;
				vertInds.push_back({i1 - 1, i2 - 1, i3 - 1});

				unsigned int i4, i5, i6;
				ss >> i4 >> slash >> i5 >> slash >> i6;
				vertInds.push_back({ i4 - 1, i5 - 1, i6 - 1 });

				unsigned int i7, i8, i9;
				ss >> i7 >> slash >> i8 >> slash >> i9;
				vertInds.push_back({ i7 - 1, i8 - 1, i9 - 1 });
				//VertInds.push_back(VertInd());
				// locInd - 1 uvInd - 1 normInd - 1
				//ss >> VertInds[VertInds.size - 1].locInd >> slash >> VertInds[VertInds.size - 1].uvInd >> slash >> VertInds[VertInds.size - 1].normInd;
			}

		}
		if (outFile.is_open())
		{
			outFile.close();
		}
	}


	// Draw buffered models
	// Define model vertex locations
	/*std::vector<Vertex> locs =
	{
		Vertex({ 1, 1, 0 },{ 1, 1 }),
		Vertex({ -1, 1, 0 },{ 0, 1 }),
		Vertex({ -1,-1, 0 },{ 0, 0 }),
		Vertex({ 1,-1, 0 },{ 1, 0 }) };*/
	//std::vector <unsigned int>
	//	locInds =
	//{ 0, 1, 2,
	//	0, 2, 3 };





	vertCount = vertInds.size();
	std::vector<Vertex> vertBufData(vertCount);
	for (unsigned int i = 0; i < vertCount; i++)
	{
		vertBufData[i] = { locs[vertInds[i].locInd],
			uvs[vertInds[i].uvInd],
			norms[vertInds[i].normInd] };
	}
		
	GLuint vertBuf;

	glGenVertexArrays(1, &vertArr);
	glGenBuffers(1, &vertBuf);

	glBindVertexArray(vertArr);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuf);

	glBufferData(GL_ARRAY_BUFFER,	// Where to copy to
		sizeof(Vertex) * vertCount,// # bytes to copy
		&vertBufData[0],			// Where to copy from
		GL_STATIC_DRAW);			// "hint" to OpenGL

	glEnableVertexAttribArray(0); // Attribute index - 0
	glVertexAttribPointer(
		0,				// Attribute index - 0 in this case
		3,				// Number of components (x, y, z)
		GL_FLOAT,		// Type of data
		GL_FALSE,		// Should we normalize the data?
		sizeof(Vertex),	// Stride (Bytes per vertex)
		0);		// Offset to this attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)sizeof(glm::vec3));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
	glBindVertexArray(0); // unbind when finished editing
	return true;
}

void Model::render()
{
	// Render game objects.
	

		glBindVertexArray(vertArr);
		
		glDrawArrays(GL_TRIANGLES, 0, vertCount);
	
}

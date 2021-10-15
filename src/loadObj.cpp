#include "loadObj.h"

bool loadFile(const std::string& filename, Mesh *mesh) {
	// peek at the extension
	std::string::size_type idx;
	idx = filename.rfind('.');

	if (idx != std::string::npos) {
		std::string extension = filename.substr(idx + 1);
		if (extension == "obj") {
			return loadObj(filename, mesh);
		}
		else {
			std::cerr << "ERROR: unable to load file " << filename
				<< "  -- unknown extension." << std::endl;
			std::cerr << "Input only (.obj) files" << std::endl;
		}
	}
	// No filename extension found, or none matching {.obj}
	return false;
}

bool loadObj(const std::string& filename, Mesh *mesh) {
	std::ifstream ifs(filename.c_str());

	if (ifs.is_open()) {
		int f0, f1, f2;
		float v0, v1, v2;
		int c0;
		std::string line;
		char temp[200];

		while (std::getline(ifs, line)) {
			
			// Load Vertices
			if (line[0] == 'v') { // vertices
				sscanf(line.c_str(), "%s %f %f %f", temp, &v0, &v1, &v2);

				Vertex *v = new Vertex(Vector3f(v0, v1, v2));
				mesh->vertices.push_back(v);

				v->crease = 0;

			}
			
			//Load Faces
			else if (line[0] == 'f') {

				sscanf(line.c_str(), "%s %d %d %d", temp, &f0, &f1, &f2);

				std::vector<int> indices;
				indices.push_back(f0);
				indices.push_back(f1);
				indices.push_back(f2);
				
				mesh->vertexIndices.push_back(indices);

			}

			//Add constraints
			else if (line[0] == 'c') {

				sscanf(line.c_str(), "%s %d", temp, &c0);

				mesh->vertices[c0]->isConstrained = true;

			}
		}
	}
	return true;
}

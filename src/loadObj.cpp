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
		std::string line;
		char temp[200];

		while (std::getline(ifs, line)) {
			
			// Load Vertices
			if (line[0] == 'v') { // vertices
				sscanf(line.c_str(), "%s %f %f %f", temp, &v0, &v1, &v2);

				Vertex *v = new Vertex(Vector3f(v0, v1, v2));
				mesh->vertices.push_back(v);

				// Vector3f vector(v0, v1, v2);
				// temp_vertices.push_back(vector);
				// Vertex *v = new Vertex(vector);
				// v->crease = 0;
				// glvertices.push_back(v);

			}
			
			//Load Faces
			else if (line[0] == 'f') {

				sscanf(line.c_str(), "%s %d %d %d", temp, &f0, &f1, &f2);

				std::vector<int> indices;
				indices.push_back(f0);
				indices.push_back(f1);
				indices.push_back(f2);
				
				mesh->vertexIndices.push_back(indices);

				// std::vector<int> face;
				// face.push_back(t0[0] - 1);
				// face.push_back(t1[0] - 1);
				// face.push_back(t2[0] - 1);


				// if (t3[0] != -1) {
				// 	quad = true;
				// 	face.push_back(t3[0] - 1);
				// }
				// vertex_indices.push_back(face);

			}
		}
	}
	return true;
}

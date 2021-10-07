#include <fstream>
#include <sstream>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "halfEdge.h"

bool loadFile(const std::string& filename, std::vector<Vertex> &vertices, std::vector<MeshFace> &faces);
bool loadObj(const std::string& filename, std::vector<Vertex> &vertices, std::vector<MeshFace> &faces);

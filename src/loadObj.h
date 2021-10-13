#include <fstream>
#include <sstream>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "halfEdge.h"

bool loadFile(const std::string& filename, Mesh *mesh);
bool loadObj(const std::string& filename, Mesh *mesh);

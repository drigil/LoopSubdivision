#ifndef LOOP
#define LOOP

#include "halfEdge.h"
#include <vector>

void computedOddPoints(Mesh* mesh, Mesh* prevMesh);
void computeEvenPoints(Mesh *mesh, Mesh *prevMesh);
void computeNewMesh(Mesh *mesh, Mesh* prevMesh);

#endif
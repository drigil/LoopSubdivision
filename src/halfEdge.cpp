#include "halfEdge.h"

HalfEdge* most_clockwise(HalfEdge* edge) {
	if (edge == NULL) { return NULL; }
	HalfEdge *e = edge->prev;
	while (e && e->pair) {
		e = e->pair->prev;
		if (e == edge->prev) { break; }
	}
	return e;
}

HalfEdge* most_counter_clockwise(HalfEdge* edge) {
	if (edge == NULL) { return NULL; }
	HalfEdge *e = edge;
	while (e && e->pair) {
		e = e->pair->next;
		if (e == edge) { break; }
	}
	return e;
}

void makeFace(Mesh *mesh, std::vector<Vertex*> &fVertices){
	// Construct a half edge mesh face 
	Face *face = new Face();

	// push new edges
	std::vector<HalfEdge*> edges;

	for (int i = 0; i < 3; i++) { // 3 vertices for each triangle face
		edges.push_back(new HalfEdge());
	}

	// Fill the half edges
	for(int i = 0; i<3; i++){
		
		// Vertex vertex(vertices[meshFace.indices.getCoord(i)]);
		edges[i]->start = fVertices[i];
		edges[i]->face = face;
		
		mesh->halfEdges.push_back(edges[i]);

		edges[i]->next = edges[(i + 1) % 3]; // 0->1, 1->2, 2-> 0
		edges[i]->prev = edges[(i + 2) % 3]; // 0->2, 1->0. 2->1
		
		if(fVertices[i]->edge == NULL){
			fVertices[i]->edge = edges[i];
		}
	}

	face->edge = edges[0]; // Face points to the first half edge
	mesh->faces.push_back(face);

	// Connect with corresponding half edge pairs 
	for (int i = 0; i < 3; ++i) {
		HalfEdge* e = most_clockwise(fVertices[i]->edge);
		if (e && e->start->pos == edges[i]->next->start->pos && e->next->start->pos == edges[i]->start->pos) {
			edges[i]->pair = e;
			e->pair = edges[i];
			
		}
	}

}	


void constructMesh(Mesh* mesh, std::vector<Vector3f> &faceVertices){
	// Iterate over each face and produce corresponding half edges and half edeg faces
	for(int i = 0; i<mesh->vertexIndices.size(); i++){
		
		std::vector<Vertex*> fVertices; //Store the vertices of the current face

		for (int j = 0; j < 3; j++) { //Number of vertices associated with a face = 3
			faceVertices.push_back(mesh->vertices[mesh->vertexIndices[i][j]]->pos);
			fVertices.push_back(mesh->vertices[mesh->vertexIndices[i][j]]);
		}

		makeFace(mesh, fVertices);
	}
}


void facesToVertices(std::vector<Face> inputFaces, std::vector<Vector3f> outputVertices){
	for(int i = 0; i<inputFaces.size(); i++){
		Face face = inputFaces[i];
		HalfEdge *halfEdge = face.edge;

		while(halfEdge != face.edge){
			outputVertices.push_back(halfEdge->start->pos);
			halfEdge = halfEdge->next;
		}
	}
}
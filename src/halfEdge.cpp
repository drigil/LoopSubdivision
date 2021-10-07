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

void makeFace(MeshFace &meshFace, std::vector<Vertex> &vertices, std::vector<HalfEdge> &halfEdges, std::vector<Face> &faces){
	// Construct a half edge mesh face 
	Face face;

	// push new edges
	std::vector<HalfEdge> edges;

	for (int i = 0; i < 3; i++) { // 3 vertices for each triangle face
		edges.push_back(HalfEdge());
	}

	// Fill the half edges
	for(int i = 0; i<3; i++){
		
		// Vertex vertex(vertices[meshFace.indices.getCoord(i)]);
		edges[i].start = &(vertices[meshFace.indices.getCoord(i)]);
		edges[i].face = &face;
		
		halfEdges.push_back(edges[i]);

		edges[i].next = &edges[(i + 1)%3]; // 0->1, 1->2, 2-> 0
		edges[i].prev = &edges[(i + 2) % 3]; // 0->2, 1->0. 2->1
		
		if(vertices[meshFace.indices.getCoord(i)].edge == NULL){
			vertices[meshFace.indices.getCoord(i)].edge = &edges[i];
		}
	}

	face.edge = &edges[0]; // Face points to the first half edge
	faces.push_back(face);

	// Connect with corresponding half edge pairs 
	for (int i = 0; i < 3; ++i) {
		HalfEdge* e = most_clockwise(vertices[meshFace.indices.getCoord(i)].edge);
		if (e && e->start->pos == edges[i].next->start->pos
			&& e->next->start->pos == edges[i].start->pos) {
			edges[i].pair = e;
			e->pair = &edges[i];
			
		}
	}

}	


void constructMesh( std::vector<Vertex> &inputVertices, std::vector<MeshFace> &inputFaces, std::vector<HalfEdge> &halfEdges, std::vector<Face> &faces){
	// Iterate over each face and produce corresponding half edges and half edeg faces
	for(int i = 0; i<inputFaces.size(); i++){
		makeFace(inputFaces[i], inputVertices, halfEdges, faces);
	}
}

/*
void make_face(std::vector<Vertex*> &fvertices, Mesh *mesh) {
	// make new face
	Face *face = new Face();

	// push new edges
	std::vector<HalfEdge*> edges;
	for (int i = 0; i < (int)fvertices.size(); i++) { // n half edges
		edges.push_back(new HalfEdge());
	}

	// connect half edges
	int num = (int)fvertices.size();
	for (int i = 0; i < num; i++) {
		edges[i]->start = fvertices[i]; // points to the next vertex!
		edges[i]->face = face; // face
		mesh->glhalfedges.push_back(edges[i]);
		edges[i]->next = edges[(i + 1) % num];
		edges[i]->prev = edges[((i + (num - 1)) % num)]; // useful
		if (fvertices[i]->edge == NULL) {
			fvertices[i]->edge = edges[i];
		}
	}
	face->edge = edges[0]; // face points to any half edge
	mesh->glfaces.push_back(face);

	// connect pairs after making the face // not my idea

	for (size_t i = 0; i < num; ++i) {
		HalfEdge* e = most_clockwise(fvertices[i]->edge);
		if (e && e->start->pos == edges[i]->next->start->pos
			&& e->next->start->pos == edges[i]->start->pos) {
			edges[i]->pair = e;
			e->pair = edges[i];
			//printf("pairs\n");
			//e->start->pos.print();
			//edges[i]->start->pos.print();
			//printf("\n");
		}
	}
}

*/

// // FIRST TIME
// void init_mesh(Mesh *mesh, std::vector<Vector3f> &draw) {
// 	std::vector<std::vector<int>>::iterator face;
// 	for (face = mesh->vertex_indices.begin(); face != mesh->vertex_indices.end(); face++) {
// 		std::vector<Vertex *> fvertices;
// 		for (int i = 0; i < face->size(); i++) {
// 			draw.push_back(mesh->glvertices[(*face)[i]]->pos);
// 			fvertices.push_back(mesh->glvertices[(*face)[i]]);
// 		}
// 		make_face(fvertices, mesh);
// 	}
// }
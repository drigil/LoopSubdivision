#include "loop.h"

void computedOddPoints(Mesh* mesh, Mesh* prevMesh){

	for(int i = 0; i<prevMesh->halfEdges.size(); i++){
		
		HalfEdge *halfEdge = &(*(prevMesh->halfEdges[i])); // CHANGED

		if(halfEdge -> edgePoint){
			continue;
		}

		// Make a new edge point
		Vertex *edgePoint = new Vertex();

		// Boundary or constraint case
		if (halfEdge->pair == NULL){ // || (halfEdge->start->crease == 1 || halfEdge->pair->start->crease == 1) ) {
			// printf("halfEdge next start pos  %f %f %f \n", halfEdge->next->start->pos.x, halfEdge->next->start->pos.y, halfEdge->next->start->pos.z);
			edgePoint->pos = ((halfEdge->start->pos) + (halfEdge->next->start->pos)) / 2.0;
			// edgePoint.color = ((*edge)->start->color + (*edge)->next->start->color) / 2.0;
			// edgePoint.crease = 1;
			halfEdge->edgePoint = edgePoint;
			
		}

		// regular case
		else if (halfEdge->pair) {
			Vector3f edge1 = halfEdge->start->pos;
			Vector3f edge2 = halfEdge->next->start->pos;
			Vector3f edge3 = halfEdge->next->next->start->pos;
			Vector3f edge4 = halfEdge->pair->next->next->start->pos;
			// Vector3f edge5 = halfEdge->start->color;
			// Vector3f edge6 = halfEdge->next->start->color;
			// Vector3f edge7 = halfEdge->next->next->start->color;
			// Vector3f edge8 = halfEdge->pair->next->next->start->color;
			edgePoint->pos = (edge3 + edge4 + edge1 * 3.0 + edge2 * 3.0) / 8.0;
			// edgePoint->color = (edge7 + edge8 + edge5 * 3.0 + edge6 * 3.0) / 8.0;
			// edgePoint->crease = 0;

			// assign to both
			halfEdge->edgePoint = edgePoint;
			halfEdge->pair->edgePoint = halfEdge->edgePoint;
		}

		mesh->vertices.push_back(halfEdge->edgePoint); // append new vertex to new mesh
	}
}


void computeEvenPoints(Mesh *mesh, Mesh *prevMesh) {
	for(int i = 0; i<prevMesh->vertices.size(); i++){

		Vertex *v = &(*prevMesh->vertices[i]); // CHANGED

		Vertex *newPoint = new Vertex();
		v->newPoint = newPoint;
		int valence = 0;
		HalfEdge* e = v->edge;

		// loop around the vertex // flip code
		std::vector<Vertex*> neighbors;

		do{
			valence++;
			neighbors.push_back(e->edgePoint);
			// WE NEED TO GO BACKWARD
			if (e->pair == NULL) {
				HalfEdge *p = (v)->edge->prev;
				while (p) {
					valence++;
					neighbors.push_back(p->edgePoint);
					if (p->pair == NULL) { break; }
					p = p->pair->prev;
				}
				break;
			}
			e = e->pair->next;
		} while (e != (v)->edge);
 
		if (valence >= 3){//  && (e->start->crease == 0 || (*v)->crease == 0)) 
			float beta;
			
			//Test this alternative as well 
			//float term = 3.0 + 2.0*cos((2.0*M_PI)/valence);
			//beta = (1.0/(8.0*valence)) * (40.0 - (term*term));
			
			if (valence == 3) {
				beta = 3.0 / 16.0;
			}
			else {
				beta = 3.0 / (valence * 8.0);
			}

			newPoint->pos = ((v)->pos * (1.0 - valence * beta));
			// newPoint->color = ((*v)->color * (1.0 - valence * beta));
			for (std::vector<Vertex*>::iterator t = neighbors.begin(); t != neighbors.end(); t++) {
				newPoint->pos = newPoint->pos + ((*t)->pos * beta);
				// newPoint->color = newPoint->color + ((*t)->color * beta);
			}
			// newPoint->crease = 0;
		}
		else { // BOUNDARY VERTEX RULE 1/8 3/4 1/8
			HalfEdge *ccw_edge = most_counter_clockwise(v->edge);
			HalfEdge *cw_edge = most_clockwise(v->edge);
			newPoint->pos = ccw_edge->edgePoint->pos*(1.0 / 8.0) + cw_edge->edgePoint->pos*(1.0 / 8.0) + (v)->pos*0.75;
			// newPoint->color = ccw_edge->edgePoint->color*(1.0 / 8.0) + cw_edge->edgePoint->color*(1.0 / 8.0) + (*v)->color*0.75;
			// newPoint->crease = 1;
			//for (std::vector<Vertex*>::iterator t = neighbors.begin();
			//     t != neighbors.end(); t++) {
			//    (*v)->newPoint->pos = (*v)->newPoint->pos + ((*t)->pos * (1.0/8.0));
			//}
			//newPoint->pos = newPoint->pos + (*v)->pos*(3.0/4.0);
		}

		v->newPoint = newPoint;
		mesh->vertices.push_back(v->newPoint);
	}
}

void computeNewMesh(Mesh *mesh, Mesh* prevMesh){
	for(int i = 0; i< prevMesh->faces.size(); i++){
		Face *face = prevMesh->faces[i];
		HalfEdge *edge = face->edge;

		// first special face
		std::vector<Vertex*> fVertices;
		Vertex *s1 = &(*(edge->next->edgePoint)); //CHANGED
		Vertex *s2 = &(*(edge->next->next->edgePoint));
		Vertex *s3 = &(*(edge->edgePoint));
		fVertices.push_back(s1);
		fVertices.push_back(s2);
		fVertices.push_back(s3);
		makeFace(mesh, fVertices);

		// Other three faces
		do{
			std::vector<Vertex*> fVertices;
			Vertex *v1 = &(*(edge->next->edgePoint));
			Vertex *v2 = &(*(edge->edgePoint));
			Vertex *v3 = &(*(edge->next->start->newPoint));
			fVertices.push_back(v1);
			fVertices.push_back(v2);
			fVertices.push_back(v3);
			makeFace(mesh, fVertices);
			edge = edge->next;
		} while(edge!=(face)->edge);
	}
}
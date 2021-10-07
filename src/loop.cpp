#include "loop.h"

void computedOddPoints(std::vector<HalfEdge> &halfEdges, std::vector<Vertex> &newVertices){
	for(int i = 0; i<halfEdges.size(); i++){
		HalfEdge *halfEdge = &halfEdges[i];

		if(halfEdge -> edgePoint){
			continue;
		}

		// Make a new edge point
		Vertex edgePoint;

		// Boundary or constraint case
		if (halfEdge->pair == NULL){ // || (halfEdge->start->crease == 1 || halfEdge->pair->start->crease == 1) ) {
			edgePoint.pos = (halfEdge->start->pos + halfEdge->next->start->pos) / 2.0;
			// edgePoint.color = ((*edge)->start->color + (*edge)->next->start->color) / 2.0;
			// edgePoint.crease = 1;
			halfEdge->edgePoint = &edgePoint;
			
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
			edgePoint.pos = (edge3 + edge4 + edge1 * 3.0 + edge2 * 3.0) / 8.0;
			// edgePoint->color = (edge7 + edge8 + edge5 * 3.0 + edge6 * 3.0) / 8.0;
			// edgePoint->crease = 0;

			// assign to both
			halfEdge->edgePoint = &edgePoint;
			halfEdge->pair->edgePoint = halfEdge->edgePoint;
		}

		newVertices.push_back(*(halfEdge->edgePoint)); // append new vertex to new list of vertices
	}
}


void computeEvenPoints(std::vector<Vertex> &vertices, std::vector<Vertex> &newVertices) {
	for(int i = 0; i<vertices.size(); i++){

		Vertex *v = &vertices[i];
		Vertex newPoint;
		v->newPoint = &newPoint;
		int valence = 0;
		HalfEdge* e = v->edge;

		// loop around the vertex // flip code
		std::vector<Vertex*> neighbors;
		while (e != (v)->edge){
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
		} 
 
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

			newPoint.pos = ((v)->pos * (1.0 - valence * beta));
			// newPoint->color = ((*v)->color * (1.0 - valence * beta));
			for (std::vector<Vertex*>::iterator t = neighbors.begin(); t != neighbors.end(); t++) {
				newPoint.pos = newPoint.pos + ((*t)->pos * beta);
				// newPoint->color = newPoint->color + ((*t)->color * beta);
			}
			// newPoint->crease = 0;
		}
		else { // BOUNDARY VERTEX RULE 1/8 3/4 1/8
			HalfEdge *ccw_edge = most_counter_clockwise(v->edge);
			HalfEdge *cw_edge = most_clockwise(v->edge);
			newPoint.pos = ccw_edge->edgePoint->pos*(1.0 / 8.0) + cw_edge->edgePoint->pos*(1.0 / 8.0) + (v)->pos*0.75;
			// newPoint->color = ccw_edge->edgePoint->color*(1.0 / 8.0) + cw_edge->edgePoint->color*(1.0 / 8.0) + (*v)->color*0.75;
			// newPoint->crease = 1;
			//for (std::vector<Vertex*>::iterator t = neighbors.begin();
			//     t != neighbors.end(); t++) {
			//    (*v)->newPoint->pos = (*v)->newPoint->pos + ((*t)->pos * (1.0/8.0));
			//}
			//newPoint->pos = newPoint->pos + (*v)->pos*(3.0/4.0);
		}

		v->newPoint = &newPoint;
		newVertices.push_back(*(v->newPoint));
	}
}
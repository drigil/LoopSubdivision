#ifndef HALFEDGE_H
#define HALFEDGE_H

#include <stdio.h>
#include <math.h>
#include <vector>

class Vertex;
class Face;
class HalfEdge;

// Define a Vector3f
struct Vector3f {
	float x, y, z;
	Vector3f() { x = 0.;  y = 0.; z = 0.; }
	Vector3f(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }
	Vector3f operator*(float s) { return Vector3f(x*s, y*s, z*s); }
	Vector3f operator/(float s) { return Vector3f(x / s, y / s, z / s); }
	Vector3f operator-() { return Vector3f(-x, -y, -z); }
	Vector3f operator-(Vector3f const o) { return Vector3f(x - o.x, y - o.y, z - o.z); }
	Vector3f operator+(Vector3f const o) { return Vector3f(x + o.x, y + o.y, z + o.z); }
	float length() { return sqrtf(x*x + y * y + z * z); }
	void normalize() {
		float l = length();
		x = x / l;
		y = y / l;
		z = z / l;
	}
	bool operator == (const Vector3f& v) {
		return (x == v.x)
			&& (y == v.y)
			&& (z == v.z);
	}
	void print() {
		printf("%.4f %.4f %.4f\n", x, y, z);
	}
	float getx()
	{
		return x;
	}
	float gety()
	{
		return y;
	}
	float getz()
	{
		return z;
	}
};

// Define a Vector3d
struct Vector3d {
	int x, y, z;
	Vector3d() { x = 0;  y = 0; z = 0; }
	Vector3d(int _x, int _y, int _z) { x = _x; y = _y; z = _z; }
	Vector3d operator*(int s) { return Vector3d(x*s, y*s, z*s); }
	Vector3d operator/(int s) { return Vector3d(x / s, y / s, z / s); }
	Vector3d operator-() { return Vector3d(-x, -y, -z); }
	Vector3d operator-(Vector3d const o) { return Vector3d(x - o.x, y - o.y, z - o.z); }
	Vector3d operator+(Vector3d const o) { return Vector3d(x + o.x, y + o.y, z + o.z); }
	int length() { return sqrtf(x*x + y * y + z * z); }
	void normalize() {
		int l = length();
		x = x / l;
		y = y / l;
		z = z / l;
	}
	bool operator == (const Vector3d& v) {
		return (x == v.x)
			&& (y == v.y)
			&& (z == v.z);
	}
	void print() {
		printf("%.4f %.4f %.4f\n", x, y, z);
	}
	int getx()
	{
		return x;
	}
	int gety()
	{
		return y;
	}
	int getz()
	{
		return z;
	}
	int getCoord(int i)
	{
		if(i==0){
			return x;
		}
		else if(i==1){
			return y;
		}
		else { // (i==2)
			return z;
		}
			
	}
};


struct MeshFace{
	Vector3d indices;
	MeshFace(Vector3d i){indices = i;}
};

struct Vertex {
	Vector3f pos;
	Vector3f color;
	Vector3f normal;
	Vector3f texture;
	HalfEdge *edge;
	
	Vertex() { edge = NULL; }
	Vertex(Vector3f p) { edge = NULL; pos = p; }
	Vertex(Vertex &v) { this->pos = v.pos; }
	Vertex(Vertex const &v) { this->pos = v.pos; this->edge = v.edge; }
	Vertex *newPoint; // future newPoint
};

struct Face {
	// Each face references one of the HalfEdges the belongs to it.
	HalfEdge *edge;
	Vertex *facePoint;
	Face() {
		edge = NULL;
		facePoint = NULL;
	}
};

// Define the half edge in the mesh
struct HalfEdge {
	Vertex *start; // the Vertex it points to
	HalfEdge *next; // the next HalfEdge inside the face
	HalfEdge *prev; // the prev HalfEdge
	HalfEdge *pair; // the pair HalfEdge
	Face *face; // the face it belongs to
	Vertex* edgePoint; // its future edge point
				   //bool boundary;
	HalfEdge() {
		start = NULL;
		next = NULL;
		prev = NULL;
		pair = NULL;
		face = NULL;
		edgePoint = NULL;
	}
};

HalfEdge* most_clockwise(HalfEdge* edge);
HalfEdge* most_counter_clockwise(HalfEdge* edge);
void makeFace(MeshFace &meshFace, std::vector<Vertex> &vertices, std::vector<HalfEdge> &halfEdges, std::vector<Face> &faces);
void constructMesh( std::vector<Vertex> &inputVertices, std::vector<MeshFace> &inputFaces, std::vector<HalfEdge> &halfEdges, std::vector<Face> &faces);

#endif

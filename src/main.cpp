//Assignment 02: Trandformations, viewing and projection

#include "utils.h"
#include "loadObj.h"
#include "halfEdge.h"

int width = 640, height=640;

int main(int, char**)
{	

    // Setup window
    GLFWwindow *window = setupWindow(width, height);
    ImGuiIO& io = ImGui::GetIO(); // Create IO object

    //Phong Shading
    //unsigned int shaderProgram = createProgram("./shaders/vshader2.vs", "./shaders/fshader2.fs");
    
    //Gouraud Shading
    unsigned int shaderProgram = createProgram("./shaders/vshader1.vs", "./shaders/fshader.fs");
    glUseProgram(shaderProgram);

    setupModelTransformation(shaderProgram);
    // setupViewTransformation(shaderProgram);
    // setupProjectionTransformation(shaderProgram, width , height);
    
    // int vVertex_attrib = glGetAttribLocation(shaderProgram, "vVertex");
    // if(vVertex_attrib == -1) {
    //     std::cout << "Could not bind location: vVertex\n" ;
    //     exit(0);
    // }else{
    //     std::cout << "vVertex found at location " << vVertex_attrib << std::endl;
    // }

	std::vector<Vertex> vertices;
	std::vector<MeshFace> meshFaces;
    std::vector<HalfEdge> halfEdges;
    std::vector<Face> faces;


    bool isFileOpen = loadFile("./models/triangle.obj", vertices, meshFaces);

    if(isFileOpen == false){
    	return -1; // Invalid File
    }

    // Vertices obtained from the file
    for(Vertex v: vertices){
    	printf("Vertex %f %f %f \n", v.pos.x, v.pos.y, v.pos.z);
    }

    // Mesh Faces obtained from the file
    for(MeshFace f:meshFaces){
    	printf("Face %d %d %d \n", f.indices.x, f.indices.y, f.indices.z);
    }

    // Convert the mesh to half edge data structure
    constructMesh(vertices, meshFaces, halfEdges, faces);

    for(HalfEdge halfEdge: halfEdges){
        printf("Half Edge start vertex %f %f %f\n", halfEdge.start->pos.x, halfEdge.start->pos.y, halfEdge.start->pos.z);
    }

    for(Face face: faces){
        printf("Face Edge start vertex %f %f %f\n", face.edge->start->pos.x, face.edge->start->pos.y, face.edge->start->pos.z);
    }



}

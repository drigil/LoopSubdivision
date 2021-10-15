//Assignment 02: Trandformations, viewing and projection

#include "utils.h"
#include "loadObj.h"
#include "halfEdge.h"
#include "loop.h"

#define NUM_LOOPS 3

int width = 640, height=640; // Window dimensions

std::vector<Mesh*> meshArray; // Keep track of all meshes
std::vector<Vector3f> faceVertices; // Keep track of vertices per face to draw the mesh using opengl
int currMeshIndex = 0; // Keeps track of the index of the current mesh

// Fill face vertices to draw the mesh
void constructFaceVertices(int meshIndex){
    faceVertices.clear();
    for(int i = 0; i<meshArray[meshIndex]->faces.size(); i++){
        HalfEdge *halfEdge = meshArray[meshIndex]->faces[i]->edge;
        do{
            faceVertices.push_back(halfEdge->start->pos);
            halfEdge = halfEdge->next;
        }
        while(halfEdge!=meshArray[meshIndex]->faces[i]->edge);
    }
}

void loopSubdivide(){
    Mesh* mesh = new Mesh();
    meshArray.push_back(mesh);
    currMeshIndex++;

    computedOddPoints(meshArray[currMeshIndex], meshArray[currMeshIndex - 1]);
    computeEvenPoints(meshArray[currMeshIndex], meshArray[currMeshIndex - 1]);
    computeNewMesh(meshArray[currMeshIndex], meshArray[currMeshIndex - 1]);

    constructFaceVertices(currMeshIndex);
}

int main(int, char**)
{	

    // Setup window
    GLFWwindow *window = setupWindow(width, height);
    ImGuiIO& io = ImGui::GetIO(); // Create IO object

    ImVec4 clearColor = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);

    //Phong Shading
    //unsigned int shaderProgram = createProgram("./shaders/vshader2.vs", "./shaders/fshader2.fs");
    
    //Gouraud Shading
    unsigned int shaderProgram = createProgram("shaders/vshader1.vs", "shaders/fshader.fs");
    glUseProgram(shaderProgram);

    setupModelTransformation(shaderProgram);
    setupViewTransformation(shaderProgram);
    setupProjectionTransformation(shaderProgram, width , height);
    
    int vVertex_attrib = glGetAttribLocation(shaderProgram, "vVertex");
    if(vVertex_attrib == -1) {
        std::cout << "Could not bind location: vVertex\n" ;
        exit(0);
    }else{
        std::cout << "vVertex found at location " << vVertex_attrib << std::endl;
    }

	Mesh *mesh = new Mesh();
    bool isFileOpen = loadFile("./models/triangle.obj", mesh); // primitive_triangle, triangle, concentric_triangle, square, sample2

    if(isFileOpen == false){
    	return -1; // Invalid File
    }

    // Vertices obtained from the file
    // for(Vertex *v: mesh->vertices){
    // 	printf("Vertex %f %f %f \n", v->pos.x, v->pos.y, v->pos.z);
    //     printf("Constraint %d\n", v->isConstrained);
    // }

    // Mesh Faces obtained from the file
    // for(std::vector<int> face: mesh->vertexIndices){
    // 	printf("Face %d %d %d \n", face[0], face[1], face[2]);
    // }

    // Push current mesh into the array
    meshArray.push_back(mesh);

    // Convert the mesh to half edge data structure
    constructMesh(mesh, faceVertices);

    // for(HalfEdge *halfEdge: mesh->halfEdges){
    //     printf("Half Edge start vertex %f %f %f\n", halfEdge->start->pos.x, halfEdge->start->pos.y, halfEdge->start->pos.z);
    //     printf("Half Edge next start vertex %f %f %f\n", halfEdge->next->start->pos.x, halfEdge->next->start->pos.y, halfEdge->next->start->pos.z);
    // }

    // for(Face *face: mesh->faces){
    //     printf("Face Edge start vertex %f %f %f\n", face->edge->start->pos.x, face->edge->start->pos.y, face->edge->start->pos.z);
    // }

    for(int i = 0; i<NUM_LOOPS; i++){
        loopSubdivide();    
    }
    

    // Construct 1D vertex array to display
    int numVertices = faceVertices.size() * 3; // 3 coordinates per vertex
    GLfloat glVertices[numVertices];

    int counter = 0;
    for(int i = 0; i<faceVertices.size(); i++){
        
        glVertices[counter] = faceVertices[i].x;
        glVertices[counter + 1] = faceVertices[i].y;
        glVertices[counter + 2] = faceVertices[i].z;
        counter = counter + 3;
    }

    glUseProgram(shaderProgram);

    GLuint vertex_VBO;
    glGenBuffers(1, &vertex_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glVertices), glVertices, GL_STATIC_DRAW); //else tri_points
      
    
    GLuint obj_VAO;
    glGenVertexArrays(1, &obj_VAO);
    glBindVertexArray(obj_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
    glVertexAttribPointer(static_cast<uint>(vVertex_attrib), 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(static_cast<uint>(vVertex_attrib));


    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glUseProgram(shaderProgram);
        // showOptionsDialog(shaderProgram, angle, matrix);

        {
            // static float f = 0.0f;
            // static int counter = 0;

            ImGui::Begin("Information");                          
            ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glBindVertexArray(obj_VAO); 

        glDrawArrays(GL_POINTS, 0, numVertices);//else tri_points   GL_TRIANGLES
        glDrawArrays(GL_TRIANGLES, 0, numVertices);//else tri_points   GL_TRIANGLES

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

    }

    // Cleanup
    cleanup(window);

    return 0;

}

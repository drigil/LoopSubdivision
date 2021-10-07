//Assignment 02: Trandformations, viewing and projection

#include "utils.h"
#include "loadObj.h"
#include "halfEdge.h"
#include "loop.h"

int width = 640, height=640;

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

    // Construct 1D vertex array to display
    int numVertices = meshFaces.size() * 3 * 3; // 3 vertices per face, 3 coordinates per vertex
    GLfloat glVertices[numVertices];

    int counter = 0;
    for(int i = 0; i<meshFaces.size(); i++){
        for(int j = 0; j<3; j++){
            glVertices[counter] = vertices[meshFaces[i].indices.getCoord(j)].pos.x;
            glVertices[counter + 1] = vertices[meshFaces[i].indices.getCoord(j)].pos.y;
            glVertices[counter + 2] = vertices[meshFaces[i].indices.getCoord(j)].pos.z;
            counter = counter + 3;
        }
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

        glDrawArrays(GL_TRIANGLES, 0, numVertices);//else tri_points   

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

    }

    // Cleanup
    cleanup(window);

    return 0;

}

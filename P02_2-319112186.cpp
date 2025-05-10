//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";

static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crearían acá
static const char* vShaderA = "shaders/shader_azul.vert";
static const char* vShaderC = "shaders/shader_cafe.vert";
static const char* vShaderR = "shaders/shader_rojo.vert";
static const char* vShaderV = "shaders/shader_verde.vert";
static const char* vShaderVF = "shaders/shader_verdeF.vert";
float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular regular [0]
void CreaPiramide()
{
	unsigned int indices[] = {
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3

	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}
//Pirámide cuadrangular regular azul [1]
void CreaPiramideCA()
{
	unsigned int pic_indices[] = {
	   0, 1, 2,  // Cara frontal
	   0, 2, 3,  // Cara derecha
	   0, 3, 4,  // Cara trasera 
	   0, 4, 1,  // Cara izquierda 
	   //Base
	   1, 2, 3,
	   1, 3, 4

	};
	GLfloat pic_vertices[] = {
		 0.0f,  0.3f,  0.0f,  // 0: Punta
		-0.6f, -0.3f, -0.6f,  // 1: Base, esquina inferior izquierda
		 0.6f, -0.3f, -0.6f,  // 2: Base, esquina inferior derecha
		 0.6f, -0.3f,  0.6f,  // 3: Base, esquina superior derecha
		-0.6f, -0.3f,  0.6f   // 4: Base, esquina superior izquierda

	};
	Mesh* pic = new Mesh();
	pic->CreateMesh(pic_vertices, pic_indices, 15, 18);
	meshList.push_back(pic);
}

//Pirámide Cuadrangular regular Verde F [2]
void CreaPiramideCV()
{
	unsigned int picv_indices[] = {
	   0, 1, 2,  // Cara frontal
	   0, 2, 3,  // Cara derecha
	   0, 3, 4,  // Cara trasera 
	   0, 4, 1,  // Cara izquierda 
	   //Base
	   1, 2, 3,
	   1, 3, 4

	};
	GLfloat picv_vertices[] = {
		 0.0f,  0.25f,  0.0f,  // 0: Punta
		-0.2f, -0.25f, -0.2f,  // 1: Base, esquina inferior izquierda
		 0.2f, -0.25f, -0.2f,  // 2: Base, esquina inferior derecha
		 0.2f, -0.25f,  0.2f,  // 3: Base, esquina superior derecha
		-0.2f, -0.25f,  0.2f   // 4: Base, esquina superior izquierda

	};
	Mesh* picv = new Mesh();
	picv->CreateMesh(picv_vertices, picv_indices, 15, 18);
	meshList.push_back(picv);
}

//Vértices de un cubo Verde [3]
void CrearCuboR()
{
	unsigned int cuboR_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cuboR_vertices[] = {
		// front
		-0.15f, -0.15f,  0.015f,
		0.15f, -0.15f,  0.015f,
		0.15f,  0.15f,  0.015f,
		-0.15f,  0.15f,  0.015f,
		// back
		-0.15f, -0.15f, -0.015f,
		0.15f, -0.15f, -0.015f,
		0.15f,  0.15f, -0.015f
		-0.15f,  0.15f, -0.015f
	};
	Mesh* cuboR = new Mesh();
	cuboR->CreateMesh(cuboR_vertices, cuboR_indices, 24, 36);
	meshList.push_back(cuboR);
}

//Vértices de un cubo Rojo [4]
void CrearCuboV()
{
	unsigned int cuboV_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cuboV_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cuboV = new Mesh();
	cuboV->CreateMesh(cuboV_vertices, cuboV_indices, 24, 36);
	meshList.push_back(cuboV);
}

//Vértices de un cubo cafe [5]
void CrearCuboC()
{
	unsigned int cuboC_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cuboC_vertices[] = {
		// front
		-0.1f, -0.1f,  0.1f,
		0.1f, -0.1f,  0.1f,
		0.1f,  0.1f,  0.1f,
		-0.1f,  0.1f,  0.1f,
		// back
		-0.1f, -0.1f, -0.1f,
		0.1f, -0.1f, -0.1f,
		0.1f,  0.1f, -0.1f,
		-0.1f,  0.1f, -0.1f
	};
	Mesh* cuboC = new Mesh();
	cuboC->CreateMesh(cuboC_vertices, cuboC_indices, 24, 36);
	meshList.push_back(cuboC);
}

void CrearLetrasyFiguras()
{
	//Letra M [1]
	GLfloat vertices_M[] = {
		//X			Y			Z			R		G		B
		//Primer Triangulo
		-0.3f,	0.8f,		0.0f,		0.6706f, 0.3059f, 0.4196f,
		-0.2f,	0.8f,		0.0f,		0.6706f, 0.3059f, 0.4196f,
		-0.3,	-0.8f,		0.0f,		0.6706f, 0.3059f, 0.4196f,
	};
		MeshColor* letra_M = new MeshColor();
		letra_M->CreateMeshColor(vertices_M, 18);
		meshColorList.push_back(letra_M);
}


void CreateShaders()
{

	Shader* shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);

	//Shader color Azul [2]
	Shader* shader3 = new Shader(); 
	shader3->CreateFromFiles(vShaderA, fShader);
	shaderList.push_back(*shader3);

	//Shader color Cafe [3]
	Shader* shader4 = new Shader();
	shader4->CreateFromFiles(vShaderC, fShader);
	shaderList.push_back(*shader4);

	//Shader color rojo [4]
	Shader* shader5 = new Shader();
	shader5->CreateFromFiles(vShaderR, fShader);
	shaderList.push_back(*shader5);

	//Shader color verde [5]
	Shader* shader6 = new Shader(); 
	shader6->CreateFromFiles(vShaderV, fShader);
	shaderList.push_back(*shader6);

	//Shader color Verde Fuerte [6]
	Shader* shader7 = new Shader();
	shader7->CreateFromFiles(vShaderVF, fShader);
	shaderList.push_back(*shader7);
}


int main()
{
	mainWindow = Window(800, 800);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CreaPiramideCA(); //índice 1 en MeshList
	CreaPiramideCV(); //índice 2 en MeshList
	CrearCuboR();//índice 3 en MeshList
	CrearCuboV();//índice 4 en MeshList
	CrearCuboC();//índice 5 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	//glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 view = glm::mat4(1.0);
	//view = glm::lookAt(glm::vec3(0.0f, 3.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad

	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad


		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		//Pino Derecho (PCV)
		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.8f, -0.55f, -2.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[2]->RenderMesh();
		//-------------------------

		//Pino Izquierdo (PCV)
		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f, -0.55f, -2.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[2]->RenderMesh();
		//-------------------------

		//Techo (PCA)
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.3f, -2.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();
		//-------------------------

		//Tronco Derecho (CC)
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.8f, -0.9f, -2.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[5]->RenderMesh();
		//-------------------------

		//Tronco Izquierdo (CC)
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f, -0.9f, -2.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[5]->RenderMesh();
		//-------------------------

		//Cubo Rojo
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, -2.5f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[4]->RenderMesh();
		//-------------------------

		//Ventana Izquierdo (CV)
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.25f, -0.25f, -2.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[3]->RenderMesh();
		//-------------------------

		//Ventana Derecho (CV)
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.25f, -0.25f, -2.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[3]->RenderMesh();
		//-------------------------

		//Puerta
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.85f, -2.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[3]->RenderMesh();
		//-------------------------


		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/
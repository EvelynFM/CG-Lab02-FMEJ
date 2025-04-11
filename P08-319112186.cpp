/*
Práctica 7: Iluminación 1 
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture dadoTexture;
Texture ladrilloTexture;
Texture AgaveTexture;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;
//Carro
Model Carro_cuerpo;
Model Carro_cofre;
Model Carro_LlantaTD;
Model Carro_LlantaTI;
Model Carro_LlantaDD;
Model Carro_LlantaDI;
Model Carro_Espejo;
Model Carro_Parrilla;
//lampara
Model Lampara;
//Edificio
Model edificio;
//Skybox
Skybox skybox;
//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

//Liz puntual
bool luz_lamp = false;
bool luz_edif = false;
bool luz_carro = false;

// luz direccional
DirectionalLight mainLight;
//PointLight
PointLight pointLights[MAX_POINT_LIGHTS];//4 luces en MAX_PINT
PointLight pointLights2[MAX_POINT_LIGHTS];
//Spotlight
SpotLight spotLights[MAX_SPOT_LIGHTS];
SpotLight spotLights2[MAX_SPOT_LIGHTS];
SpotLight spotLights3[MAX_SPOT_LIGHTS];


// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//normal sirve para que se vea la luz
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, -1.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, -1.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, -1.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, -1.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, -1.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, -1.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, -1.0f,


	};
	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearOctaedro()
{
	unsigned int octaedro_indices[] = {
		//Arriba
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11,

		// Abajo
		12, 13, 14,
		15, 16, 17,
		18, 19, 20,
		21, 22, 23,

	};

	GLfloat octaedro_vertices[] = {
		//Arriba
		// Frente D - Dwaekki 
		//x		y		z		S		T			NX		NY		NZ
		0.0f, 0.5f,  0.0f,		0.142f,	0.66f,		-1.0f,	1.0f,	-1.0f,
		0.0f, 0.0f,  0.5f,		0.01f,	0.34f,		-1.0f,	1.0f,	-1.0f,
		0.5f,  0.0f,  0.0f,		0.284f,	0.34f,		-1.0f,	1.0f,	-1.0f,
		// Frente I - Quokka 
		//x		y		z		S		T			NX		NY		NZ
		0.0f, 0.5f,  0.0f,		0.43f,	0.66f,		1.0f,	1.0f,	-1.0f,
		0.0f, 0.0f,  0.5f,		0.568f,	0.34f,		1.0f,	1.0f,	-1.0f,
		-0.5f, 0.0f,  0.0f,		0.29f,	0.34f,		1.0f,	1.0f,	-1.0f,
		// Atras D - Leebit
		//x		y		z		S		T			NX		NY		NZ
		0.0f, 0.5f,  0.0f,		0.7108f,0.66f,		-1.0f,	1.0f,	1.0f,
		0.5f,  0.0f,  0.0f,		0.5775f,0.34f,		-1.0f,	1.0f,	1.0f,
		0.0f, 0.0f,  -0.5f,		0.852f,	0.34f,		-1.0f,	1.0f,	1.0f,
		// Atras I - Wolfchan
		//x		y		z		S		T			NX		NY		NZ
		0.0f, 0.5f,  0.0f,		0.568f,	0.99f,		1.0f,	1.0f,	1.0f,
		-0.5f, 0.0f,  0.0f,		0.7108f,0.67f,		1.0f,	1.0f,	1.0f,
		0.0f, 0.0f,  -0.5f,		0.436f,	0.67f,		1.0f,	1.0f,	1.0f,
		// Abajo
		//Frente D - Jiniret
		//x		y		z		S		T			NX		NY		NZ
		0.0f, -0.5f,  0.0f,		0.284f,	0.34f,		-1.0f,	-1.0f,	-1.0f,
		0.0f, 0.0f,  0.5f,		0.149f,	0.66f,		-1.0f,	-1.0f,	-1.0f,
		0.5f,  0.0f,  0.0f,		0.426f,	0.66f,		-1.0f,	-1.0f,	-1.0f,
		// Frente I - Foxiny
		//x		y		z		S		T			NX		NY		NZ
		0.0f, -0.5f,  0.0f,		0.575f,	0.34f,		1.0f,	-1.0f,	-1.0f,
		0.0f, 0.0f,  0.5f,		0.71f,0.66f,		1.0f,	-1.0f,	-1.0f,
		-0.5f, 0.0f,  0.0f,		0.435f,	0.66f,		1.0f,	-1.0f,	-1.0f,
		// Atras D - Bbokari 
		//x		y		z		S		T			NX		NY		NZ
		0.0f, -0.5f,  0.0f,		0.855f,	0.34f,		-1.0f,	-1.0f,	1.0f,
		0.5f,  0.0f,  0.0f,		0.721f,0.66f,		-1.0f,	-1.0f,	1.0f,
		0.0f, 0.0f,  -0.5f,		0.99f,	0.66f,		-1.0f,	-1.0f,	1.0f,
		// Atras I - PuppyM
		//x		y		z		S		T			NX		NY		NZ
		0.0f, -0.5f,  0.0f,		0.426f,	0.01f,		1.0f,	-1.0f,	1.0f,
		-0.5f, 0.0f,  0.0f,		0.568f,	0.33f,		1.0f,	-1.0f,	1.0f,
		0.0f, 0.0f,  -0.5f,		0.29f,	0.33f,		1.0f,	-1.0f,	1.0f,
	};
	Mesh* octaedro = new Mesh();
	octaedro->CreateMesh(octaedro_vertices, octaedro_indices, 232, 24);
	meshList.push_back(octaedro);
}

void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,

		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};

	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.26f,  0.34f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.49f,	0.34f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.49f,	0.66f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.26f,	0.66f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.0f,  0.0f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	1.0f,	0.0f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	1.0f,	1.0f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.0f,	1.0f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		1.0f,	0.0f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		1.0f,	1.0f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,	1.0f,		0.0f,	0.0f,	1.0f,
		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.0f,  0.0f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	1.0f,	0.0f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	1.0f,	1.0f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.0f,	1.0f,		1.0f,	0.0f,	0.0f,
		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	1.0f,	0.0f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	1.0f,	1.0f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.0f,	1.0f,		0.0f,	1.0f,	0.0f,
		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.0f,  0.0f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f,	0.0f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	1.0f,	1.0f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.0f,	1.0f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);

}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	CrearOctaedro();
	CrearCubo();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();

	//Dado
	dadoTexture = Texture("Textures/dado_octa.png");
	dadoTexture.LoadTextureA();
	//Pared
	ladrilloTexture = Texture("Textures/ladrillos2.jpg");
	ladrilloTexture.LoadTextureA();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");

	//------------------Carro-------------------
	// Cuerpo
	Carro_cuerpo = Model();
	Carro_cuerpo.LoadModel("Models/Carro_P6/cuerpo.obj");
	//Cofre
	Carro_cofre = Model();
	Carro_cofre.LoadModel("Models/Carro_P6/cofre.obj");
	//Llanta Trasera Dererecha
	Carro_LlantaTD = Model();
	Carro_LlantaTD.LoadModel("Models/Carro_P6/LlantaTD.obj");
	//Llanta Trasera Izquierda
	Carro_LlantaTI = Model();
	Carro_LlantaTI.LoadModel("Models/Carro_P6/LlantaTI.obj");
	//Llanta Delantera Derecha
	Carro_LlantaDD = Model();
	Carro_LlantaDD.LoadModel("Models/Carro_P6/LlantaDD.obj");
	//Llanta Delantera Izquierda
	Carro_LlantaDI = Model();
	Carro_LlantaDI.LoadModel("Models/Carro_P6/LlantaDI.obj");
	//Espejo
	Carro_Espejo = Model();
	Carro_Espejo.LoadModel("Models/Carro_P6/espejo.obj");
	//Parrilla
	Carro_Parrilla = Model();
	Carro_Parrilla.LoadModel("Models/Carro_P6/parrilla.obj");
	//------------------Lampara-------------------
	Lampara = Model();
	Lampara.LoadModel("Models/Lampara1/lampara.obj");
	//------------------Edificio-------------------
	edificio = Model();
	edificio.LoadModel("Models/edificio/edificiot.obj");


	

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		//tener todo iluminado y: -1
		0.0f, -1.0f, 0.0f);

	//contador de luces puntuales
	unsigned int pointLightCount = 0;

	//Declaración de luces puntuales
	//Luz de la lampara
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
	  //radiacion | intensisdad
		1.0f, 1.0f, 
		//posicion
		0.0f, 5.5f, -20.0f,
	  //con | lin | exp
		0.5f, 0.2f, 0.1f);//intensisidad ambiental
	pointLightCount++;

	//Luz edificio
	pointLights[1] = PointLight(1.0f, 1.0f, 0.0f,
		//radiacion | intensisdad
		1.0f, 1.0f,
		//posicion
		0.0f, 20.0f, 20.0f,
		//con | lin | exp
		0.5f, 0.2f, 0.1f);//intensisidad ambiental
	pointLightCount++;

	pointLights2[0] = pointLights[1];
	pointLights2[1] = pointLights[0];


	//SpotLight
	unsigned int spotLightCount = 0;
	//Arreglo - 1
	//linterna - ligada a la camara
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f, 
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
	  //con | lin | exp
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;
	//luz cofre
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
		0.3f, 1.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);//se amplia el rango del cono (angulo)
	spotLightCount++;
	
	//Arreglo - 2
	//Luz faro del carro - adelante
	spotLights2[0] = spotLights[0];
	spotLights2[1] = spotLights[1];
	spotLights2[2] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		//posicion
		1.0f, 1.0f, 0.0f,
		//direccion
		-5.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	//Arreglo - 3
	//Luz faro del carro - atras
	spotLights3[0] = spotLights[0];
	spotLights3[1] = spotLights[1];
	spotLights3[2] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		//posicion
		1.0f, 1.0f, 0.0f,
		//direccion
		5.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;




	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
			glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		//luz direccional
		shaderList[0].SetDirectionalLight(&mainLight);

		//luz puntual
		//luz de la lampara y edificio
		luz_lamp = mainWindow.getarticulacion5(); //X
		luz_edif = mainWindow.getarticulacion4(); //Z
		if (luz_lamp == 1.0f) {
			if (luz_edif == 1.0f) {
				shaderList[0].SetPointLights(pointLights, pointLightCount);
			}
			else {
				shaderList[0].SetPointLights(pointLights, pointLightCount - 1);
			}
		}
		else {
			if (luz_edif == 1.0f) {
				shaderList[0].SetPointLights(pointLights2, pointLightCount - 1);
			}
			else {
				shaderList[0].SetPointLights(pointLights2, pointLightCount - 2);
			}
		}

		//luz spotlight
		//luz del carro
		luz_carro = mainWindow.getarticulacion6(); // J - K
		if (luz_carro == 1.0f) {
			shaderList[0].SetSpotLights(spotLights2, spotLightCount);
		}
		else {
			shaderList[0].SetSpotLights(spotLights3, spotLightCount);
		}

		//luz cofre
		//shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		//Dado de Opengl
		/*model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 15.0f, 15.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		meshList[4]->RenderMesh();*/


		//Pared
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, 16.0f, -1.0f));
		model = glm::scale(model, glm::vec3(0.1f, 35.0f, 15.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ladrilloTexture.UseTexture();
		meshList[5]->RenderMesh();


		//------------*INICIA DIBUJO DE NUESTROS DEMÁS OBJETOS-------------------*
		//cuerpo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 8.5f, -1.0f));
		model = glm::translate(model, glm::vec3(mainWindow.getarticulacion3(), 0.0f, 0.0f));
		color = glm::vec3(0.91f, 0.48f, 0.69f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carro_cuerpo.RenderModel();

		//Cofre
		modelaux = model;
		model = glm::translate(model, glm::vec3(-8.5f, 0.45f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, -1.0f));
		spotLights[1].SetFlash(glm::vec3(model * glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f)), glm::normalize(glm::vec3(model * glm::vec4(-5.0f, 0.0f, 0.0f, 0.0f))));
		spotLights2[1].SetFlash(glm::vec3(model * glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f)), glm::normalize(glm::vec3(model * glm::vec4(-5.0f, 0.0f, 0.0f, 0.0f))));
		spotLights3[1].SetFlash(glm::vec3(model * glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f)), glm::normalize(glm::vec3(model * glm::vec4(-5.0f, 0.0f, 0.0f, 0.0f))));
		color = glm::vec3(0.91f, 0.48f, 0.69f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carro_cofre.RenderModel();
		model = modelaux;

		//Espejo
		modelaux = model;
		model = glm::translate(model, glm::vec3(-7.0f, 3.5f, 0.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carro_Espejo.RenderModel();
		model = modelaux;

		//Parrilla
		modelaux = model;
		model = glm::translate(model, glm::vec3(-19.65f, -3.75f, 0.0f));
		//--Luz		
		spotLights2[2].SetPos(glm::vec3(mainWindow.getarticulacion3(), 5.0f, 0.0f));
		spotLights3[2].SetPos(glm::vec3(mainWindow.getarticulacion3(), 5.0f, 0.0f));
		//--
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carro_Parrilla.RenderModel();
		model = modelaux;

		//Llanta delantera derecha
		modelaux = model;
		model = glm::translate(model, glm::vec3(-14.0f, -6.0f, -6.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carro_LlantaDD.RenderModel();
		model = modelaux;

		//Llanta delantera izquierda
		modelaux = model;
		model = glm::translate(model, glm::vec3(-14.0f, -6.0f, 6.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carro_LlantaDI.RenderModel();
		model = modelaux;

		//Llanta trasera derecha
		modelaux = model;
		model = glm::translate(model, glm::vec3(11.5f, -6.0f, -6.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carro_LlantaTD.RenderModel();
		model = modelaux;

		//Llanta trasera izquierda
		modelaux = model;
		model = glm::translate(model, glm::vec3(11.5f, -6.0f, 6.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carro_LlantaTI.RenderModel();
		model = modelaux;

		//Helicoptero
		/*model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 25.0f, 6.0));
		model = glm::translate(model, glm::vec3(mainWindow.getarticulacion4(), 0.0f, 0.0f));
		spotLights[3].SetFlash(glm::vec3(model* glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f)), glm::normalize(glm::vec3(model* glm::vec4(0.0f, -0.5f, 0.0f, 0.0f))));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();*/

		//Lampara
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 3.8f, -20.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara.RenderModel();

		//edificio
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 40.0f, 40.0));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		edificio.RenderModel();


		//Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
		/*model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -15.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);*/

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}

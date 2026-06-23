#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

int mostrar(vector<vector<int>> matrix) {
	cout << endl;
	for (const auto& fila : matrix) {
		for (const auto& valor : fila) {
			cout << valor << " ";
		}
		cout << endl;
	}
	cout << endl;
	return 0;
}

pair<vector<vector<int>>, vector<pair<int,int>>> crear_lista(int f, int c) {
	vector<vector<int>> lista(f, vector<int>(c, 1));
	if (f > 1 && c > 1) lista[1][1] = 0;
	vector<pair<int,int>> queque = {
		{0,0}, {0,1}, {0,2},
		{1,0}, {1,2},
		{2,0}, {2,1}, {2,2}
	};
	return {lista, queque};
}

pair<vector<vector<int>>, vector<pair<int,int>>> expand(vector<vector<int>> matrix, vector<pair<int,int>> queque) {
	int fl=matrix.size();
	int cl=matrix[0].size();
	vector<vector<int>> nueva_matrix(fl*3, vector<int>(cl*3, 0));
	for(int i=0; i<fl; i++) {
		for(int j=0; j<cl; j++) {
			if(matrix[i][j]==1) {
				if(find(queque.begin(), queque.end(), make_pair(i,j))== queque.end()) {
					queque.push_back({i,j});
				}
				for (int row = i*3; row < (i+1)*3; row++) {
					for (int col = j*3; col < (j+1)*3; col++) {
						nueva_matrix[row][col] = 1;
					}
				}
			}
		}
	}
	return {nueva_matrix, queque};
}

vector<vector<int>> remove(vector<vector<int>> matrix, vector<pair<int,int>> queque) {
	for (const auto& cell : queque) {
		int row = cell.first * 3 + 1;
		int col = cell.second * 3 + 1;
		matrix[row][col] = 0;
	}
	return matrix;
}

vector<vector<int>> computar2d(vector<vector<int>> matrix, int ciclos, vector<pair<int,int>> queque) {
	vector<vector<int>> lista_final = matrix;
	pair<vector<vector<int>>, vector<pair<int,int>>> result;

	for (int i=0; i<ciclos; i++) {
		result = expand(lista_final, queque);
		lista_final = result.first;
		queque = result.second;
		lista_final = remove(lista_final, queque);
		mostrar(lista_final);
	}
	return lista_final;
}

vector<vector<vector<int>>> computar3d(vector<vector<int>> matrix2d) {
	int tm = matrix2d.size();
	vector<vector<vector<int>>> matrix3d(tm, vector<vector<int>>(tm, vector<int>(tm, 1)));
	for (int i=0; i<tm; i++) {
		for (int j=0; j<tm; j++) {
			for (int k=0; k<tm; k++) {
				if (matrix2d[i][j] == 0) {
					matrix3d[i][j][k] = 0;
					matrix3d[i][k][j] = 0;
					matrix3d[k][i][j] = 0;
				}
			}
		}
	}
	return matrix3d;
}

// Graficar

const char* vertexShaderSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aNormal;
    layout (location = 2) in vec3 aOffset;

    out vec3 Normal;
    uniform mat4 view;
    uniform mat4 projection;

    void main() {
        gl_Position = projection * view * vec4(aPos + aOffset, 1.0);
        Normal = aNormal;
    }
)glsl";

const char* fragmentShaderSource = R"glsl(
    #version 330 core
    out vec4 FragColor;
    in vec3 Normal;

    void main() {
        vec3 lightDir = normalize(vec3(0.5, 1.0, 0.3));
        float diff = max(dot(Normal, lightDir), 0.4);
        vec3 cubeColor = vec3(0.2, 0.8, 0.4);
        FragColor = vec4(cubeColor * diff, 1.0);
    }
)glsl";

// Fin de Graficar

int openGL(vector<glm::vec3> voxelPositions, int tm) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Voxeles 3D - Esponja", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

    glEnable(GL_DEPTH_TEST);

    // === PARTE D: COMPILAR SHADERS ===
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // === PARTE E: BUFFER DE DATOS A LA GPU ===
    float cubeVertices[] = {
        // Posiciones          // Normales
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    unsigned int VAO, cubeVBO, instanceVBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &instanceVBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    // Instancias (Las posiciones filtradas de tu matriz)
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, voxelPositions.size() * sizeof(glm::vec3), &voxelPositions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glVertexAttribDivisor(2, 1);

    // === PARTE F: RENDER LOOP ===
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Cámara giratoria adaptada al tamaño de tu matriz dinámica
        float time = glfwGetTime();
        float radius = tm * 1.5f; 
        glm::mat4 view = glm::lookAt(
            glm::vec3(sin(time * 0.5f) * radius, tm * 0.8f, cos(time * 0.5f) * radius), 
            glm::vec3(tm / 2.0f, tm / 2.0f, tm / 2.0f), // Apuntar al centro de la matriz
            glm::vec3(0.0f, 1.0f, 0.0f)
        );
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 1000.0f);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // Dibujar
        glBindVertexArray(VAO);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 36, voxelPositions.size());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


int main() {

	int f = 3, c = 3;
	int ciclos = 3;
	vector<vector<int>> matrix2d;
	vector<vector<vector<int>>> matrix3d;
	vector<pair<int,int>> queque;
	
	tie(matrix2d, queque) = crear_lista(f, c);
	
	mostrar(matrix2d);
	
	matrix2d = computar2d(matrix2d, ciclos, queque);
	
	matrix3d = computar3d(matrix2d);
	int tm = matrix3d.size();

	vector<glm::vec3> voxelPositions;
	for(int i=0; i<tm; i++) {
		for(int j=0; j<tm; j++) {
			for(int k=0; k<tm; k++) {
				if(matrix3d[i][j][k] == 1) {
					voxelPositions.push_back(glm::vec3(i, j, k));
				}
			}
		}
	}

	//OPENGL
	return openGL(voxelPositions, tm);
}


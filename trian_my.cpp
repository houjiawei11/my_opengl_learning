#include <iostream>

// GLEW
//#define GLEW_STATIC
#include <glew.h>

// GLFW
#include <GLFW/glfw3.h>

const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";

const GLchar* fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"	void main()\n"
"	{\n"
"	color = vec4(1.0f, 0.5f, 0.5f, 1.0f);\n"
"	}\0";


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// The MAIN function, from here we start the application and run the game loop
int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//����OpenGL 3.3�汾�����Ҫ�����汾��(Major)�ʹΰ汾��(Minor)����Ϊ3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//��ȷ����GLFW����ʹ�õ��Ǻ���ģʽ(Core-profile)
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);//�������û��������ڵĴ�С

	// ����һ�����ڣ����� GLFWwindow ָ�����ᱻ GLFW����������ʹ��
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	//��GLEW�ڹ���OpenGL�ĺ���ָ��ʱ�����ʹ���ִ����ļ���
	//�����������ΪGL_FALSE�Ļ����ܻ���ʹ��OpenGL�ĺ���ģʽʱ����һЩ����
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}


	//OpenGL��һ��3Dͼ�ο⣬����������OpenGL��ָ�����������궼��3D���꣨x��y��z����
	//OpenGL���Ǽ򵥵ذ����е�3D����任Ϊ��Ļ�ϵ�2D���أ�
	//OpenGL����3D������3���ᣨx��y��z���϶�Ϊ-1.0��1.0�ķ�Χ��ʱ�Ŵ�������
	//z��������Ϊ0.0��������ÿһ�����ȶ���һ���ģ�ʹ������ȥ����2D��
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};
	//��׼���豸������Ż�任Ϊ��Ļ�ռ�����(Screen-space Coordinates)��
	//����ʹ����ͨ��glViewport�����ṩ�����ݣ������ӿڱ任(Viewport Transform)��ɵġ�

	//����glViewport���������ô��ڵ�ά��
	//glViewport����ǰ�����������ƴ������½ǵ�λ�ã��������ĸ�����������Ⱦ���ڵĿ�Ⱥ͸߶ȣ����أ�
	//����������ֱ�Ӵ�GLFW�л�ȡ�ӿڵ�ά�ȶ�������Ϊ800*600��Ϊ�������ڸ�DPI����Ļ��Ҳ������������
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	//ʹ��glGenBuffers������һ������ID����һ��VBO����
	GLuint VBO;
	glGenBuffers(1, &VBO);
	//���㻺�����Ļ���������GL_ARRAY_BUFFER��ʹ��glBindBuffer�������´����Ļ���󶨵�GL_ARRAY_BUFFERĿ����
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//��֮ǰ����Ķ������ݸ��Ƶ�������ڴ���
	//��һ��������Ŀ�껺������ͣ����㻺�����ǰ�󶨵�GL_ARRAY_BUFFERĿ���ϡ�
	//�ڶ�������ָ���������ݵĴ�С(���ֽ�Ϊ��λ)
	//����������������ϣ�����͵�ʵ�����ݡ�
	//���ĸ�����ָ��������ϣ���Կ���ι�����������ݣ�GL_STATIC_DRAW/GL_DYNAMIC_DRAW/GL_STREAM_DRAW 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//����һ����ɫ�����󣬰���Ҫ��������ɫ�������Բ�����ʽ�ṩ��glCreateShader
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//�������ɫ��Դ�븽�ӵ���ɫ�������ϣ�Ȼ�������
	//��ɫ��Դ�����vertexShaderSource����ַ�������
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//��glGetShaderiv����Ƿ����ɹ�
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		//	 ��glGetShaderInfoLog��ȡ������Ϣ
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	GLint fragmentsuccess;
	GLchar info_fra[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentsuccess);
	if (!fragmentsuccess)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, info_fra);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_fra << std::endl;
	}

	//glCreateProgram��������һ�����򣬲������´�����������ID����
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	//��֮ǰ�������ɫ�����ӵ���������ϣ�Ȼ����glLinkProgram��������
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::LINK::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//�õ��Ľ����һ��������󣬵���glUseProgram������������������
	glUseProgram(shaderProgram);
	//�ڰ���ɫ���������ӵ���������Ժ󣬼ǵ�ɾ����ɫ������
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//ʹ��glVertexAttribPointer��������OpenGL����ν����������ݣ�Ӧ�õ�������������ϣ�
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	// ..:: ��ʼ�����루ֻ����һ�� (�����������Ƶ���ı�)�� :: ..
	// 1. ��VAO
	glBindVertexArray(VAO);
	// 2. �Ѷ������鸴�Ƶ������й�OpenGLʹ��
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. ���ö�������ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//4. ���VAO:�Ժ���ò������VAO�ˣ�0����ԭ��
	glBindVertexArray(0);



	// Game loop����������GLFW�˳�ǰһֱ��������
	while (!glfwWindowShouldClose(window))
	{
		// glfwPollEvents�����û�д���ʲô�¼���Ȼ����ö�Ӧ�Ļص�����
		//����һ������Ϸѭ���Ŀ�ʼ�����¼���������
		glfwPollEvents();

		// Render����Ⱦ
		// Clear the colorbuffer����glClearColor�����������Ļ���õ���ɫ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// 5. ��������
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glBindVertexArray(0);


		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	//��main������������glfwTerminate�������ͷ�GLFW������ڴ�
	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
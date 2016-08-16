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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//基于OpenGL 3.3版本，因此要将主版本号(Major)和次版本号(Minor)都设为3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//明确告诉GLFW我们使用的是核心模式(Core-profile)
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);//不允许用户调整窗口的大小

	// 创建一个窗口，它的 GLFWwindow 指针对象会被 GLFW的其他函数使用
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

	//让GLEW在管理OpenGL的函数指针时更多地使用现代化的技术
	//如果把它设置为GL_FALSE的话可能会在使用OpenGL的核心模式时出现一些问题
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}


	//OpenGL是一个3D图形库，所以我们在OpenGL中指定的所有坐标都是3D坐标（x、y和z）。
	//OpenGL不是简单地把所有的3D坐标变换为屏幕上的2D像素；
	//OpenGL仅当3D坐标在3个轴（x、y和z）上都为-1.0到1.0的范围内时才处理它。
	//z坐标设置为0.0，三角形每一点的深度都是一样的，使它看上去像是2D的
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};
	//标准化设备坐标接着会变换为屏幕空间坐标(Screen-space Coordinates)，
	//这是使用你通过glViewport函数提供的数据，进行视口变换(Viewport Transform)完成的。

	//调用glViewport函数来设置窗口的维度
	//glViewport函数前两个参数控制窗口左下角的位置，第三，四个参数控制渲染窗口的宽度和高度（像素）
	//这里我们是直接从GLFW中获取视口的维度而不设置为800*600是为了让它在高DPI的屏幕上也能正常工作。
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	//使用glGenBuffers函数和一个缓冲ID生成一个VBO对象
	GLuint VBO;
	glGenBuffers(1, &VBO);
	//顶点缓冲对象的缓冲类型是GL_ARRAY_BUFFER，使用glBindBuffer函数把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//把之前定义的顶点数据复制到缓冲的内存中
	//第一个参数是目标缓冲的类型：顶点缓冲对象当前绑定到GL_ARRAY_BUFFER目标上。
	//第二个参数指定传输数据的大小(以字节为单位)
	//第三个参数是我们希望发送的实际数据。
	//第四个参数指定了我们希望显卡如何管理给定的数据：GL_STATIC_DRAW/GL_DYNAMIC_DRAW/GL_STREAM_DRAW 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//创建一个着色器对象，把需要创建的着色器类型以参数形式提供给glCreateShader
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//把这个着色器源码附加到着色器对象上，然后编译它
	//着色器源码存在vertexShaderSource这个字符数组里
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//用glGetShaderiv检查是否编译成功
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		//	 用glGetShaderInfoLog获取错误消息
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

	//glCreateProgram函数创建一个程序，并返回新创建程序对象的ID引用
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	//把之前编译的着色器附加到程序对象上，然后用glLinkProgram链接它们
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::LINK::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//得到的结果是一个程序对象，调用glUseProgram函数激活这个程序对象
	glUseProgram(shaderProgram);
	//在把着色器对象链接到程序对象以后，记得删除着色器对象
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//使用glVertexAttribPointer函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上）
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	// ..:: 初始化代码（只运行一次 (除非你的物体频繁改变)） :: ..
	// 1. 绑定VAO
	glBindVertexArray(VAO);
	// 2. 把顶点数组复制到缓冲中供OpenGL使用
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. 设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//4. 解绑VAO:以后就用不到这个VAO了（0代表还原）
	glBindVertexArray(0);



	// Game loop：在我们让GLFW退出前一直保持运行
	while (!glfwWindowShouldClose(window))
	{
		// glfwPollEvents检查有没有触发什么事件，然后调用对应的回调函数
		//我们一般在游戏循环的开始调用事件处理函数。
		glfwPollEvents();

		// Render：渲染
		// Clear the colorbuffer，用glClearColor来设置清空屏幕所用的颜色
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// 5. 绘制物体
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glBindVertexArray(0);


		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	//在main函数的最后调用glfwTerminate函数来释放GLFW分配的内存
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
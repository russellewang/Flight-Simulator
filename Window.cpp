#include "Window.h"

const char* window_title = "GLFW Starter Project";
GLint shaderProgram;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "shader.vert"
#define FRAGMENT_SHADER_PATH "shader.frag"

int Window::width;
int Window::height;

// Timed Events
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

glm::vec2 mousePos;
bool keys[1024];
bool Window::leftClick;
glm::vec2 camEuler = glm::vec2(0, glm::pi<float>());

Camera* camera;
City* city;
Geometry* plane;
std::vector<Transform*> objects;

float planeSpeed = 3.0f;
float planePitchSpeed = 1.5f;
float planeYawSpeed = 1.5;
float planeRollSpeed = 1.5f;

void Window::initialize_objects()
{
    camera = new Camera();
    camera->SetFov(45.0f);
    camera->SetAspect((float)Window::width / Window::height);
    camera->SetPosition(glm::vec3(0.03f, 3.0f, -2.0f));
    camera->SetRotation(glm::quat(glm::vec3(camEuler, 0.0f)));
    
	plane = new Geometry("plane.obj", 1.0f, true);
	//plane = new Geometry("limb_s.obj", 0.1f, false);
    plane->addChild((Transform*)camera);
    objects.push_back((Transform*)plane);

	// Create city / Load buildings into objects vector
	city = new City(&objects);
    
	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
}

void Window::clean_up()
{
	glDeleteProgram(shaderProgram);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);
    if (camera) camera->SetAspect((float)width / height);
}

void Window::idle_callback()
{
	// Set frame time
	GLfloat currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
    
    glm::vec3 p = plane->GetPosition();
    glm::quat r = plane->GetWorldRotation();
    glm::vec3 fwd = glm::vec3(r * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    plane->SetPosition(p + fwd * planeSpeed * deltaTime);
    
    glm::quat lr = plane->GetRotation();
    
    if (keys[GLFW_KEY_W])
        lr *= glm::quat(glm::vec3(planePitchSpeed * deltaTime, 0.0f, 0.0f));
    else if (keys[GLFW_KEY_S])
        lr *= glm::quat(glm::vec3(-planePitchSpeed * deltaTime, 0.0f, 0.0f));

    if (keys[GLFW_KEY_Q])
        lr *= glm::quat(glm::vec3(0.0f, planeYawSpeed * deltaTime, 0.0f));
    else if (keys[GLFW_KEY_E])
        lr *= glm::quat(glm::vec3(0.0f, -planeYawSpeed * deltaTime, 0.0f));
    
    if (keys[GLFW_KEY_A])
        lr *= glm::quat(glm::vec3(0.0f, 0.0f, -planeRollSpeed * deltaTime));
    else if (keys[GLFW_KEY_D])
        lr *= glm::quat(glm::vec3(0.0f, 0.0f, planeRollSpeed * deltaTime));
    
    plane->SetRotation(lr);
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(0,0,0,1);

	// Use the shader of programID
	glUseProgram(shaderProgram);
    
    for(int i = 0; i < objects.size(); i++)
        objects[i]->draw(camera, shaderProgram);

	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (GLFW_PRESS == action)
			Window::leftClick = true;
		else if (GLFW_RELEASE == action)
			Window::leftClick = false;
    }
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	if (Window::leftClick) {
        float dx = (xpos - mousePos.x) * .01f;
        float dy = (ypos - mousePos.y) * .01f;
        
        camEuler += glm::vec2(-dy, -dx);
        camera->SetRotation(glm::quat(glm::vec3(camEuler, 0.0f)));
        
	}
    mousePos.x = xpos;
    mousePos.y = ypos;
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
        switch(key){
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            case GLFW_KEY_I:
                camera->SetFov(camera->GetFov() - 5);
                break;
            case GLFW_KEY_O:
                camera->SetFov(camera->GetFov() + 5);
                break;
        }
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

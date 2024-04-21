#include <cstdint>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>

class	HelloTriangleApplication
{
	public:
		const uint32_t	WIDTH = 800;
		const uint32_t	HEIGHT = 600;

		void	run()
		{
			initWindow();
			initVulkan();
			mainLoop();
			cleanup();
		}

	private:
		GLFWwindow*	window;

		void	initWindow(void)
		{
			glfwInit();

			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			glfwWindowHint(GLFW_RED_BITS, GLFW_FALSE);

			window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
		}

		void	initVulkan(void)
		{
		}

		void	mainLoop(void)
		{
			while (!glfwWindowShouldClose(window))
			{
				glfwPollEvents();
			}
		}

		void	cleanup(void)
		{
			glfwDestroyWindow(window);

			glfwTerminate();
		}
};

int	main(void)
{
	HelloTriangleApplication	app;

	try
	{
		app.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

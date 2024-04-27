#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <vector>

class	HelloTriangleApplication
{
	public:
		const uint32_t	WIDTH = 800;
		const uint32_t	HEIGHT = 600;

		const std::vector<const char*>	validationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

#ifdef NDEBUG
		const bool enableValidationLayers = false;
#else
		const bool enableValidationLayers = true;
#endif // NDEBUG

		void	run(void)
		{
			initWindow();
			initVulkan();
			mainLoop();
			cleanup();
		}

	private:
		VkInstance	instance;
		GLFWwindow*	window;

		std::vector<const char*>	getRequiredExtensions(void) {
			uint32_t					glfwExtensionCount = 0;
			const char**				glfwExtensions;

			glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

			std::vector<const char *>	extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

			if (enableValidationLayers) {
				extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			}

			return extensions;
		}

		bool	checkValidationLayerSupport(void) {
			uint32_t						layerCount;

			vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

			std::vector<VkLayerProperties>	availableLayers(layerCount);

			vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

			for (const char* layerName : validationLayers) {
				bool	layerFound = false;

				for (const auto& layerProperties : availableLayers) {
					if (strcmp(layerName, layerProperties.layerName) == 0) {
						layerFound = true;
						break;
					}
				}

				if (!layerFound) {
					return false;
				}
			}

			return true;
		}

		std::vector<VkExtensionProperties>	getSupportedExtensions(void)
		{
			uint32_t	extensionCount = 0;
			
			vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

			std::vector<VkExtensionProperties>	extensions(extensionCount);

			vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

			return (extensions);
		}

		void	initWindow(void)
		{
			glfwInit();

			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			glfwWindowHint(GLFW_RED_BITS, GLFW_FALSE);

			window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
			std::cout << "Window created!" << std::endl;
		}

		void	createInstance(void)
		{
			VkInstanceCreateInfo	createInfo{};
			VkApplicationInfo		appInfo{};

			uint32_t				glfwExtensionCount = 0;
			const char**			glfwExtensions;

			if (enableValidationLayers && !checkValidationLayerSupport()) {
				throw std::runtime_error("requested validation layers are not available");
			}
			glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

			// We can avoid determining an intersection between available
			// and required extensions, like the tutorial suggests, by
			// simply checking if the call returns null.
			if (glfwExtensions == nullptr) {
				throw std::runtime_error("missing required Vulkan extensions for GLFW");
			}

			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName = "Hello Triangle";
			appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.pEngineName	= "No Engine";
			appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.apiVersion = VK_API_VERSION_1_0;

			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo = &appInfo;
			createInfo.enabledLayerCount = glfwExtensionCount;
			createInfo.ppEnabledExtensionNames = glfwExtensions;
			createInfo.enabledLayerCount = 0;

			if (enableValidationLayers) {
				createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
				createInfo.ppEnabledLayerNames = validationLayers.data();
			} else {
				createInfo.enabledLayerCount = 0;
			}

			if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
				throw std::runtime_error("failed to create instance");
			}

			std::vector<VkExtensionProperties>	supportedExtensions = getSupportedExtensions();

			std::cout << "Available extensions:\n";
			for (const auto& extension : supportedExtensions) {
				std::cout << '\t' << extension.extensionName << '\n';
			}
		}

		void	initVulkan(void)
		{
			createInstance();
			std::cout << "Vulkan instance created!" << std::endl;
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
			vkDestroyInstance(instance, nullptr);

			glfwDestroyWindow(window);

			glfwTerminate();
			std::cout << "Cleanup..." << std::endl;
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

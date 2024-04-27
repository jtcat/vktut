#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <map>
#include <optional>

struct	QueueFamilyIndices {
	std::optional<uint32_t>	graphicsFamily;

	bool	isComplete()
	{
		return graphicsFamily.has_value();
	}
};

VkResult	CreateDebugUtilsMessengerEXT(
		VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
		const VkAllocationCallbacks *pAllocator,
		VkDebugUtilsMessengerEXT *pMessenger)
{
	auto	func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance,
			"vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr)
		return func(instance, pCreateInfo, pAllocator, pMessenger);
	else
	 	return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void	DestroyDebugUtilsMessengerEXT(VkInstance instance,
		VkDebugUtilsMessengerEXT messenger,
		const VkAllocationCallbacks *pAllocator)
{
	auto	func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr)
		return func(instance, messenger, pAllocator);
}

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
		#endif

		void	run(void)
		{
			initWindow();
			initVulkan();
			mainLoop();
			cleanup();
		}

	private:
		VkInstance					instance;
		VkDebugUtilsMessengerEXT	debugMessenger;
		VkPhysicalDevice			physicalDevice = VK_NULL_HANDLE;
		GLFWwindow*					window;

		static VKAPI_ATTR VkBool32 VKAPI_CALL	debugCallback(
				VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
				VkDebugUtilsMessageTypeFlagsEXT messageType,
				const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
				void* pUserData)
		{
			std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

			return VK_FALSE;
		};

		std::vector<const char*>	getRequiredExtensions(void) {
			uint32_t					glfwExtensionCount = 0;
			const char**				glfwExtensions;

			glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

			if (glfwExtensions == nullptr)
			{
				throw std::runtime_error("missing required Vulkan extensions for GLFW");
			}

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

		void	populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
		{
			createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			createInfo.messageSeverity =
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			createInfo.messageType =
				VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			createInfo.pfnUserCallback = debugCallback;
			createInfo.pUserData = nullptr;
		}

		void	setupDebugMessenger(void)
		{
			VkDebugUtilsMessengerCreateInfoEXT	createInfo{};

			if (!enableValidationLayers) return;

			populateDebugMessengerCreateInfo(createInfo);

			if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
				throw std::runtime_error("failed to setup debug messenger!");
			std::cout << "Debug messenger has been setup" << std::endl;
		}

		QueueFamilyIndices	findQueueFamilies(VkPhysicalDevice device)
		{
			QueueFamilyIndices	indices;
			uint32_t			queueFamilyCount = 0;

			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
			std::vector<VkQueueFamilyProperties>	queueFamilies(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

			int	i = 0;
			for (const auto& queueFamily : queueFamilies) {
				if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
					indices.graphicsFamily = i;
				}

				if (indices.isComplete()) {
					break;
				}

				i++;
			}

			return indices;
		}

		int	rateDeviceSuitability(VkPhysicalDevice device)
		{
			VkPhysicalDeviceProperties	deviceProperties;
			VkPhysicalDeviceFeatures	deviceFeatures;
			QueueFamilyIndices			indices;
			int							score = 0;

			vkGetPhysicalDeviceProperties(device, &deviceProperties);
			vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
			indices = findQueueFamilies(device);

			if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
				score += 1000;
			}

			score += deviceProperties.limits.maxImageDimension2D;

			if (!indices.isComplete() || !deviceFeatures.geometryShader) {
				return 0;
			}

			return score;
		}

		void	pickPhysicalDevice(void)
		{
			uint32_t	deviceCount = 0;
			std::multimap<int, VkPhysicalDevice>	candidates;

			vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

			if (deviceCount == 0)
			{
				throw std::runtime_error("failed to find GPUs with Vulkan support");
			}

			std::vector<VkPhysicalDevice>	devices(deviceCount);
			vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

			for (const auto& device : devices)
			{
				int	score = rateDeviceSuitability(device);
				candidates.insert(std::make_pair(score,device));
			}

			if (candidates.rbegin()->first > 0) {
				physicalDevice = candidates.rbegin()->second;
			}
			else {
				throw std::runtime_error("failed to find a suitable GPU");
			}
		}

		std::string	getPhysicalDeviceName(VkPhysicalDevice&	device)
		{
			VkPhysicalDeviceProperties	deviceProperties;

			vkGetPhysicalDeviceProperties(device, &deviceProperties);
			return std::string(deviceProperties.deviceName);
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
			VkInstanceCreateInfo				createInfo{};
			VkApplicationInfo					appInfo{};
			VkDebugUtilsMessengerCreateInfoEXT	debugCreateInfo{};

			uint32_t					glfwExtensionCount = 0;
			std::vector<const char *>	extensions;

			if (enableValidationLayers && !checkValidationLayerSupport()) {
				throw std::runtime_error("requested validation layers are not available");
			}

			extensions = getRequiredExtensions();

			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName = "Hello Triangle";
			appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.pEngineName	= "No Engine";
			appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.apiVersion = VK_API_VERSION_1_0;

			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo = &appInfo;
			createInfo.enabledLayerCount = glfwExtensionCount;
			createInfo.ppEnabledExtensionNames = extensions.data();
			createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());

			if (enableValidationLayers) {
				createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
				createInfo.ppEnabledLayerNames = validationLayers.data();

				populateDebugMessengerCreateInfo(debugCreateInfo);
				createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
			} else {
				createInfo.enabledLayerCount = 0;
				createInfo.pNext = nullptr;
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
			setupDebugMessenger();
			pickPhysicalDevice();
			std::cout << "Selected GPU: " << getPhysicalDeviceName(physicalDevice) << std::endl;
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
			if (enableValidationLayers) {
				DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
			}

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

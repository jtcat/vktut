#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <string>
#include <cstring>
#include <vector>
#include <optional>

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const uint32_t	WIDTH = 800;
const uint32_t	HEIGHT = 600;

const int		MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<const char*>		validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char *>		deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

const std::vector<VkDynamicState>	dynamicStates = {
	VK_DYNAMIC_STATE_VIEWPORT,
	VK_DYNAMIC_STATE_SCISSOR
};

struct	SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR		capabilities;
	std::vector<VkSurfaceFormatKHR>	formats;
	std::vector<VkPresentModeKHR>	presentModes;
};

struct	QueueFamilyIndices {
	std::optional<uint32_t>	graphicsFamily;
	std::optional<uint32_t>	presentFamily;

	bool	isComplete()
	{
		return graphicsFamily.has_value()
			&& presentFamily.has_value();
	}
};

VkResult	CreateDebugUtilsMessengerEXT(
		VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
		const VkAllocationCallbacks *pAllocator,
		VkDebugUtilsMessengerEXT *pMessenger);

void	DestroyDebugUtilsMessengerEXT(VkInstance instance,
		VkDebugUtilsMessengerEXT messenger,
		const VkAllocationCallbacks *pAllocator);

class	HelloTriApp
{
	public:
		bool	framebufferResized = false;

		void	run(void);

	private:
		VkInstance					instance;
		VkPhysicalDevice			physicalDevice = VK_NULL_HANDLE;
		VkDevice					device;

		VkDebugUtilsMessengerEXT	debugMessenger;

		VkQueue						graphicsQueue;
		VkQueue						presentQueue;

		VkSurfaceKHR				surface;

		VkSwapchainKHR				swapChain;
		std::vector<VkImage>		swapChainImages;
		std::vector<VkImageView>	swapChainImageViews;
		VkFormat					swapChainImageFormat;
		VkExtent2D					swapChainExtent;

		VkRenderPass				renderPass;
		VkPipelineLayout			pipelineLayout;
		VkPipeline					graphicsPipeline;
		std::vector<VkFramebuffer>	swapChainFramebuffers;

		VkCommandPool				commandPool;

		std::vector<VkCommandBuffer>	commandBuffers;
		std::vector<VkSemaphore>		imageAvailableSemaphores;
		std::vector<VkSemaphore>		renderFinishedSemaphores;
		std::vector<VkFence>			inFlightFences;

		GLFWwindow*					window;

		uint32_t					currentFrame = 0;

		static VKAPI_ATTR VkBool32 VKAPI_CALL	debugCallback(
				VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
				VkDebugUtilsMessageTypeFlagsEXT messageType,
				const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
				void* pUserData)
		;

		std::vector<const char*>	getRequiredExtensions(void);

		bool	checkValidationLayerSupport(void);

		std::vector<VkExtensionProperties>	getSupportedInstanceExtensions(void);

		void	populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

		void	setupDebugMessenger(void);

		SwapChainSupportDetails	querySwapChainSupport(VkPhysicalDevice device);

		QueueFamilyIndices	findQueueFamilies(VkPhysicalDevice device);

		bool	checkDeviceExtensionSupport(VkPhysicalDevice device);

		int	rateDeviceSuitability(VkPhysicalDevice device);

		VkExtent2D	chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		VkPresentModeKHR	chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

		VkSurfaceFormatKHR	chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

		void	pickPhysicalDevice(void);

		std::string	getPhysicalDeviceName(VkPhysicalDevice&	device);

		void	initWindow(void);

		void	createInstance(void);

		void	createLogicalDevice(void);

		void	createSurface(void);

		void	cleanupSwapChain(void);

		void	recreateSwapChain(void);

		void	createSwapChain(void);

		void	createImageViews(void);

		VkShaderModule	createShaderModule(const std::vector<char>& code);

		void	createRenderPass(void);

		void	createGraphicsPipeline(void);

		void	createFramebuffers(void);

		void	createCommandPool(void);

		void	createCommandBuffers(void);

		void	recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t	imageIndex);

		void	createSyncObjects(void);

		void	drawFrame(void);

		void	initVulkan(void);

		void	mainLoop(void);

		void	cleanup(void);
};

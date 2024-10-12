#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "readfile.h"
#include <array>
#include <cstdlib>
#include <string>
#include <cstring>
#include <vector>
#include <optional>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

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
	std::optional<uint32_t>	transferFamily;

	bool	isComplete()
	{
		return graphicsFamily.has_value()
			&& presentFamily.has_value()
			&& transferFamily.has_value();
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

struct	Vertex {
	glm::vec2	pos;
	glm::vec3	color;
	glm::vec2	texCoord;

	static VkVertexInputBindingDescription	getBindingDescription() {
		VkVertexInputBindingDescription		bindingDescription{};

		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static	std::array<VkVertexInputAttributeDescription, 3>	getAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 3>	attributeDescriptions{};

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

		return attributeDescriptions;
	}
};

struct	UniformBufferObject {
	glm::mat4	model;
	glm::mat4	view;
	glm::mat4	proj;
};

const	std::vector<Vertex>	vertices = {
	{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
	{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
	{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
	{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
};

const	std::vector<uint16_t> indices = {
	0, 1, 2, 2, 3, 0
};

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
		VkQueue						transferQueue;

		VkSurfaceKHR				surface;

		VkSwapchainKHR				swapChain;
		std::vector<VkImage>		swapChainImages;
		std::vector<VkImageView>	swapChainImageViews;
		VkFormat					swapChainImageFormat;
		VkExtent2D					swapChainExtent;

		VkRenderPass				renderPass;
		VkDescriptorSetLayout		descriptorSetLayout;
		VkPipelineLayout			pipelineLayout;
		VkPipeline					graphicsPipeline;
		std::vector<VkFramebuffer>	swapChainFramebuffers;

		VkDescriptorPool				descriptorPool;
		std::vector<VkDescriptorSet>	descriptorSets;

		VkCommandPool				graphicsCommandPool;
		VkCommandPool				transferCommandPool;

		VkBuffer					vertexBuffer;
		VkDeviceMemory				vertexBufferMemory;

		std::vector<VkBuffer>		uniformBuffers;
		std::vector<VkDeviceMemory>	uniformBuffersMemory;
		std::vector<void *>			uniformBuffersMapped;

		VkBuffer					indexBuffer;
		VkDeviceMemory				indexBufferMemory;

		VkImage						textureImage;
		VkDeviceMemory				textureImageMemory;
		VkImageView					textureImageView;
		VkSampler					textureSampler;

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

		std::vector<uint32_t>	getUniqueQueueFamilies(VkPhysicalDevice device);

		bool	checkDeviceExtensionSupport(VkPhysicalDevice device);

		int	rateDeviceSuitability(VkPhysicalDevice device);

		VkExtent2D	chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		VkPresentModeKHR	chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

		VkSurfaceFormatKHR	chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

		void	pickPhysicalDevice(void);

		std::string	getPhysicalDeviceName(VkPhysicalDevice&	device);

		uint32_t	findMemoryType(uint32_t	typeFilter, VkMemoryPropertyFlags properties);

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

		void	createDescriptorSetLayout(void);

		void	createGraphicsPipeline(void);

		void	createFramebuffers(void);

		void	createCommandPools(void);

		void	createCommandBuffers(void);

		void	setupCommandBuffer(VkCommandBuffer commandBuffer);

		void	flushCommandBuffer(VkCommandBuffer commandBuffer);

		void	createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

		VkCommandBuffer	beginSingleTimeCommands(const VkCommandPool& commandPool);

		void	endSingleTimeCommands(VkCommandPool& commandPool, VkCommandBuffer commandBuffer, VkQueue& queue);

		void	transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

		void	copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

		void	copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

		void	createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory);

		void	createTextureImage(void);

		void	createTextureImageView(void);

		void	createTextureSampler(void);

		void	createVertexBuffer(void);

		void	createIndexBuffer(void);

		void	createUniformBuffers(void);

		void	createDescriptorPool(void);

		void	createDescriptorSets(void);

		void	recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t	imageIndex);

		void	createSyncObjects(void);

		void	updateUniformBuffer(uint32_t currentFrame);

		void	drawFrame(void);

		void	initVulkan(void);

		void	mainLoop(void);

		void	cleanup(void);
};

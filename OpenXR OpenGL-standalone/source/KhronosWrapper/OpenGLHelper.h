#pragma once

// OpenXR Headers
#define XR_USE_GRAPHICS_API_OPENGL
#define XR_USE_PLATFORM_WIN32
#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_NATIVE_INCLUDE_NONE
#include <GLFW/glfw3native.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/quaternion.hpp>

#include <iostream>
#include <unordered_map>

enum class SwapchainType : uint8_t {
	COLOR,
	DEPTH
};
struct ImageViewCreateInfo {
	void* image;
	enum class Type : uint8_t {
		RTV,
		DSV,
		SRV,
		UAV
	} type;
	enum class View : uint8_t {
		TYPE_1D,
		TYPE_2D,
		TYPE_3D,
		TYPE_CUBE,
		TYPE_1D_ARRAY,
		TYPE_2D_ARRAY,
		TYPE_CUBE_ARRAY,
	} view;
	int64_t format;
	enum class Aspect : uint8_t {
		COLOR_BIT = 0x01,
		DEPTH_BIT = 0x02,
		STENCIL_BIT = 0x04
	} aspect;
	uint32_t baseMipLevel;
	uint32_t levelCount;
	uint32_t baseArrayLayer;
	uint32_t layerCount;
};
struct Viewport {
	float x;
	float y;
	float width;
	float height;
	float minDepth;
	float maxDepth;
};
struct Offset2D {
	int32_t x;
	int32_t y;
};
struct Extent2D {
	uint32_t width;
	uint32_t height;
};
struct Rect2D {
	Offset2D offset;
	Extent2D extent;
};

class OpenGLHelper {
public:
	std::unordered_map<XrSwapchain, std::pair<SwapchainType, std::vector<XrSwapchainImageOpenGLKHR>>> swapchainImagesMap{};
	std::unordered_map<GLuint, ImageViewCreateInfo> imageViews{};

	GLuint setFramebuffer = 0;
public:
	XrSwapchainImageBaseHeader* AllocateSwapchainImageData(XrSwapchain swapchain, SwapchainType type, uint32_t count);
	void FreeSwapchainImageData(XrSwapchain swapchain);

	void* GetSwapchainImage(XrSwapchain swapchain, uint32_t index);

	void* CreateImageView(const ImageViewCreateInfo& imageViewCI);
	void DestroyImageView(void*& imageView);

	void SetRenderAttachments(void** colorViews, size_t colorViewCount, void* depthStencilView, uint32_t width, uint32_t height);

};

GLFWwindow* CreateOpenGLContext();

void CreateProjection(glm::mat4* result, const float tanAngleLeft,
	const float tanAngleRight, const float tanAngleUp, float const tanAngleDown,
	const float nearZ, const float farZ);

// Creates a projection matrix based on the specified FOV.
void CreateProjectionFov(glm::mat4* result, const XrFovf fov, const float nearZ, const float farZ);
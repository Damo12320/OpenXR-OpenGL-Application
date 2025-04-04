#include "OpenGLHelper.h"

XrSwapchainImageBaseHeader* OpenGLHelper::AllocateSwapchainImageData(XrSwapchain swapchain, SwapchainType type, uint32_t count) {
	swapchainImagesMap[swapchain].first = type;
	swapchainImagesMap[swapchain].second.resize(count, { XR_TYPE_SWAPCHAIN_IMAGE_OPENGL_KHR });
	return reinterpret_cast<XrSwapchainImageBaseHeader*>(swapchainImagesMap[swapchain].second.data());
}
void OpenGLHelper::FreeSwapchainImageData(XrSwapchain swapchain) {
	swapchainImagesMap[swapchain].second.clear();
	swapchainImagesMap.erase(swapchain);
}

void* OpenGLHelper::GetSwapchainImage(XrSwapchain swapchain, uint32_t index) {
	//return (XrSwapchainImageBaseHeader*)&swapchainImagesMap[swapchain].second[index];
	return (void*)(uint64_t)swapchainImagesMap[swapchain].second[index].image;
}

void* OpenGLHelper::CreateImageView(const ImageViewCreateInfo& imageViewCI) {
	GLuint framebuffer = 0;
	glGenFramebuffers(1, &framebuffer);

	GLenum attachment = imageViewCI.aspect == ImageViewCreateInfo::Aspect::COLOR_BIT ? GL_COLOR_ATTACHMENT0 : GL_DEPTH_ATTACHMENT;

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	/*if (imageViewCI.view == ImageViewCreateInfo::View::TYPE_2D_ARRAY) {
		glFramebufferTextureMultiviewOVR(GL_DRAW_FRAMEBUFFER, attachment, (GLuint)(uint64_t)imageViewCI.image, imageViewCI.baseMipLevel, imageViewCI.baseArrayLayer, imageViewCI.layerCount);
	}
	else */
	if (imageViewCI.view == ImageViewCreateInfo::View::TYPE_2D) {
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, attachment, GL_TEXTURE_2D, (GLuint)(uint64_t)imageViewCI.image, imageViewCI.baseMipLevel);
	}
	else {
		//DEBUG_BREAK;
		std::cout << "ERROR: OPENGL: Unknown ImageView View type." << std::endl;
	}

	GLenum result = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	if (result != GL_FRAMEBUFFER_COMPLETE) {
		//DEBUG_BREAK;
		std::cout << "ERROR: OPENGL: Framebuffer is not complete. " << result << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	imageViews[framebuffer] = imageViewCI;
	return (void*)(uint64_t)framebuffer;
}
void OpenGLHelper::DestroyImageView(void*& imageView) {
	GLuint framebuffer = (GLuint)(uint64_t)imageView;
	imageViews.erase(framebuffer);
	glDeleteFramebuffers(1, &framebuffer);
	imageView = nullptr;
}

void OpenGLHelper::SetRenderAttachments(void** colorViews, size_t colorViewCount, void* depthStencilView, uint32_t width, uint32_t height) {
	// Reset Framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &this->setFramebuffer);
	this->setFramebuffer = 0;

	glGenFramebuffers(1, &this->setFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, this->setFramebuffer);

	// Color
	for (size_t i = 0; i < colorViewCount; i++) {
		GLenum attachment = GL_COLOR_ATTACHMENT0;

		GLuint glColorView = (GLuint)(uint64_t)colorViews[i];
		const ImageViewCreateInfo& imageViewCI = this->imageViews[glColorView];

		/*if (imageViewCI.view == ImageViewCreateInfo::View::TYPE_2D_ARRAY) {
			glFramebufferTextureMultiviewOVR(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, (GLuint)(uint64_t)imageViewCI.image, imageViewCI.baseMipLevel, imageViewCI.baseArrayLayer, imageViewCI.layerCount);
		}*/
		if (imageViewCI.view == ImageViewCreateInfo::View::TYPE_2D) {
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, (GLuint)(uint64_t)imageViewCI.image, imageViewCI.baseMipLevel);
		}
		else {
			std::cout << "ERROR: OPENGL: Unknown ImageView View type." << std::endl;
		}
	}
	// DepthStencil
	if (depthStencilView) {
		GLuint glDepthView = (GLuint)(uint64_t)depthStencilView;
		const ImageViewCreateInfo& imageViewCI = this->imageViews[glDepthView];

		/*if (imageViewCI.view == ImageViewCreateInfo::View::TYPE_2D_ARRAY) {
			glFramebufferTextureMultiviewOVR(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, (GLuint)(uint64_t)imageViewCI.image, imageViewCI.baseMipLevel, imageViewCI.baseArrayLayer, imageViewCI.layerCount);
		}*/
		if (imageViewCI.view == ImageViewCreateInfo::View::TYPE_2D) {
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, (GLuint)(uint64_t)imageViewCI.image, imageViewCI.baseMipLevel);
		}
		else {
			std::cout << "ERROR: OPENGL: Unknown ImageView View type." << std::endl;
		}
	}

	GLenum result = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	if (result != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR: OPENGL: Framebuffer is not complete." << std::endl;
	}
}



GLFWwindow* CreateOpenGLContext() {
	glfwInit();

	//Set OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//Create Window
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);

	//Load GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return nullptr;
	}

	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	return window;
}

void CreateProjection(glm::mat4* result, const float tanAngleLeft,
	const float tanAngleRight, const float tanAngleUp, float const tanAngleDown,
	const float nearZ, const float farZ) {

	const float tanAngleWidth = tanAngleRight - tanAngleLeft;

	// Set to tanAngleDown - tanAngleUp for a clip space with positive Y down (Vulkan).
	// Set to tanAngleUp - tanAngleDown for a clip space with positive Y up (OpenGL / D3D / Metal).
	const float tanAngleHeight = (tanAngleUp - tanAngleDown);

	// Set to nearZ for a [-1,1] Z clip space (OpenGL / OpenGL ES).
	// Set to zero for a [0,1] Z clip space (Vulkan / D3D / Metal).
	const float offsetZ = nearZ;

	if (farZ <= nearZ) {
		// place the far plane at infinity

		glm::vec4 column = glm::vec4(
			2.0f / tanAngleWidth,
			0.0f,
			(tanAngleRight + tanAngleLeft) / tanAngleWidth,
			0.0f);
		*result = glm::row(*result, 0, column);

		column = glm::vec4(
			0.0f,
			2.0f / tanAngleHeight,
			(tanAngleUp + tanAngleDown) / tanAngleHeight,
			0.0f);
		*result = glm::row(*result, 1, column);

		column = glm::vec4(
			0.0f,
			0.0f,
			-1.0f,
			-(nearZ + offsetZ));
		*result = glm::row(*result, 2, column);

		column = glm::vec4(
			0.0f,
			0.0f,
			-1.0f,
			0.0f);
		*result = glm::row(*result, 3, column);
	}
	else {
		// normal projection

		glm::vec4 column = glm::vec4(
			2.0f / tanAngleWidth,
			0.0f,
			(tanAngleRight + tanAngleLeft) / tanAngleWidth,
			0.0f);
		*result = glm::row(*result, 0, column);

		column = glm::vec4(
			0.0f,
			2.0f / tanAngleHeight,
			(tanAngleUp + tanAngleDown) / tanAngleHeight,
			0.0f);
		*result = glm::row(*result, 1, column);

		column = glm::vec4(
			0.0f,
			0.0f,
			-(farZ + offsetZ) / (farZ - nearZ),
			-(farZ * (nearZ + offsetZ)) / (farZ - nearZ));
		*result = glm::row(*result, 2, column);

		column = glm::vec4(
			0.0f,
			0.0f,
			-1.0f,
			0.0f);
		*result = glm::row(*result, 3, column);
	}
}

// Creates a projection matrix based on the specified FOV.
void CreateProjectionFov(glm::mat4* result, const XrFovf fov, const float nearZ, const float farZ) {
	const float tanLeft = tanf(fov.angleLeft);
	const float tanRight = tanf(fov.angleRight);

	const float tanDown = tanf(fov.angleDown);
	const float tanUp = tanf(fov.angleUp);

	CreateProjection(result, tanLeft, tanRight, tanUp, tanDown, nearZ, farZ);
}
#include "OpenXrContext.h"

void OpenXrContext::CreateInstance() {
	XrApplicationInfo AI;
	strncpy_s(AI.applicationName, "OpenXR Tutorial Chapter 2", XR_MAX_APPLICATION_NAME_SIZE);
	AI.applicationVersion = 1;
	strncpy_s(AI.engineName, "OpenXR Engine", XR_MAX_ENGINE_NAME_SIZE);
	AI.engineVersion = 1;
	AI.apiVersion = XR_CURRENT_API_VERSION;

	// Add debug utils extension to the instance extensions
	//m_instanceExtensions.push_back(XR_EXT_DEBUG_UTILS_EXTENSION_NAME);
	// Ensure m_apiType is already defined when we call this line.
	m_instanceExtensions.push_back("XR_KHR_opengl_enable");

	// Get all the API Layers from the OpenXR runtime.
	uint32_t apiLayerCount = 0;
	std::vector<XrApiLayerProperties> apiLayerProperties;

	XrResult result = xrEnumerateApiLayerProperties(0, &apiLayerCount, nullptr);
	if (result != XR_SUCCESS) {
		std::cout << "Failed to enumerate ApiLayerProperties. \n";
	}

	apiLayerProperties.resize(apiLayerCount, { XR_TYPE_API_LAYER_PROPERTIES });
	result = xrEnumerateApiLayerProperties(apiLayerCount, &apiLayerCount, apiLayerProperties.data());
	if (result != XR_SUCCESS) {
		std::cout << "Failed to enumerate ApiLayerProperties. \n";
	}

	// Check the requested API layers against the ones from the OpenXR. If found add it to the Active API Layers.
	for (auto& requestLayer : m_apiLayers) {
		for (auto& layerProperty : apiLayerProperties) {
			// strcmp returns 0 if the strings match.
			if (strcmp(requestLayer.c_str(), layerProperty.layerName) != 0) {
				continue;
			}
			else {
				m_activeAPILayers.push_back(requestLayer.c_str());
				break;
			}
		}
	}

	// Get all the Instance Extensions from the OpenXR instance.
	uint32_t extensionCount = 0;
	std::vector<XrExtensionProperties> extensionProperties;
	result = xrEnumerateInstanceExtensionProperties(nullptr, 0, &extensionCount, nullptr);
	if (result != XR_SUCCESS) {
		std::cout << "Failed to enumerate InstanceExtensionProperties. \n";
	}
	extensionProperties.resize(extensionCount, { XR_TYPE_EXTENSION_PROPERTIES });
	result = xrEnumerateInstanceExtensionProperties(nullptr, extensionCount, &extensionCount, extensionProperties.data());
	if (result != XR_SUCCESS) {
		std::cout << "Failed to enumerate InstanceExtensionProperties. \n";
	}

	// Check the requested Instance Extensions against the ones from the OpenXR runtime.
	// If an extension is found add it to Active Instance Extensions.
	// Log error if the Instance Extension is not found.
	for (auto& requestedInstanceExtension : m_instanceExtensions) {
		bool found = false;
		for (auto& extensionProperty : extensionProperties) {
			// strcmp returns 0 if the strings match.
			if (strcmp(requestedInstanceExtension.c_str(), extensionProperty.extensionName) != 0) {
				continue;
			}
			else {
				m_activeInstanceExtensions.push_back(requestedInstanceExtension.c_str());
				found = true;
				break;
			}
		}
		if (!found) {
			std::cout << "Failed to find OpenXR instance extension: " << requestedInstanceExtension;
		}
	}

	XrInstanceCreateInfo instanceCI{ XR_TYPE_INSTANCE_CREATE_INFO };
	instanceCI.createFlags = 0;
	instanceCI.applicationInfo = AI;
	instanceCI.enabledApiLayerCount = static_cast<uint32_t>(m_activeAPILayers.size());
	instanceCI.enabledApiLayerNames = m_activeAPILayers.data();
	instanceCI.enabledExtensionCount = static_cast<uint32_t>(m_activeInstanceExtensions.size());
	instanceCI.enabledExtensionNames = m_activeInstanceExtensions.data();

	result = xrCreateInstance(&instanceCI, &m_xrInstance);
	if (result != XR_SUCCESS) {
		std::cout << "Failed to create Instance: " << result << "\n";
	}
}
void OpenXrContext::DestroyInstance() {
	XrResult result = xrDestroyInstance(m_xrInstance);
	if (result != XR_SUCCESS) {
		std::cout << "Failed to destroy Instance: " << result << "\n";
	}
}

void OpenXrContext::GetInstanceProperties(){
	XrInstanceProperties instanceProperties{ XR_TYPE_INSTANCE_PROPERTIES };
	XrResult result = xrGetInstanceProperties(m_xrInstance, &instanceProperties);
	if (result != XR_SUCCESS) {
		std::cout << "Failed to get InstanceProperties: " << result << "\n";
	}

	std::cout << "OpenXR Runtime: " << instanceProperties.runtimeName << " - "
		<< XR_VERSION_MAJOR(instanceProperties.runtimeVersion) << "."
		<< XR_VERSION_MINOR(instanceProperties.runtimeVersion) << "."
		<< XR_VERSION_PATCH(instanceProperties.runtimeVersion);
}
void OpenXrContext::GetSystemID() {
	XrResult result;

	// Get the XrSystemId from the instance and the supplied XrFormFactor.
	XrSystemGetInfo systemGI{ XR_TYPE_SYSTEM_GET_INFO };
	systemGI.formFactor = m_formFactor;
	result = xrGetSystem(m_xrInstance, &systemGI, &m_systemID);
	if (result != XR_SUCCESS) {
		std::cout << "Failed to get SystemID: " << result << "\n";
	}

	// Get the System's properties for some general information about the hardware and the vendor.
	result = xrGetSystemProperties(m_xrInstance, m_systemID, &m_systemProperties);
	if (result != XR_SUCCESS) {
		std::cout << "Failed to get SystemProperties: " << result << "\n";
	}

	if (m_formFactor == XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY) {
		std::cout << "FormFactor: Head Mounted Display" << "\n";
	}
	else {
		std::cout << "FormFactor: Unknown" << "\n";
	}
}

int OpenXrContext::CreateSession() {
	XrResult result;
	XrSessionCreateInfo sessionCI{ XR_TYPE_SESSION_CREATE_INFO };

	result = xrGetInstanceProcAddr(m_xrInstance, "xrGetOpenGLGraphicsRequirementsKHR", (PFN_xrVoidFunction*)&xrGetOpenGLGraphicsRequirementsKHR);
	if (result != XR_SUCCESS) {
		std::cout << "Failed to get InstanceProcAddr for xrGetOpenGLGraphicsRequirementsKHR: " << result << std::endl;
	}

	//not needed. it should be checked, if this engine supports the required opengl version
	XrGraphicsRequirementsOpenGLKHR graphicsRequirements{ XR_TYPE_GRAPHICS_REQUIREMENTS_OPENGL_KHR };

	result = xrGetOpenGLGraphicsRequirementsKHR(m_xrInstance, m_systemID, &graphicsRequirements);
	if (result != XR_SUCCESS) {
		std::cout << "Failed to get Graphics Requirements for OpenGL: " << result << std::endl;
	}
	//---

	//m_graphicsAPI = std::make_unique<GraphicsAPI_OpenGL>(m_xrInstance, m_systemID);
	this->window = CreateOpenGLContext();
	if (this->window == nullptr) {
		std::cout << "Failed to create OpenGL Context." << std::endl;
		return -1;
	}

	//which opengl context is used by this application (the session needs this)
	graphicsBinding = { XR_TYPE_GRAPHICS_BINDING_OPENGL_WIN32_KHR };
	graphicsBinding.hDC = GetDC(glfwGetWin32Window(window));
	graphicsBinding.hGLRC = glfwGetWGLContext(window);

	sessionCI.next = &graphicsBinding;
	sessionCI.createFlags = 0;
	sessionCI.systemId = m_systemID;

	result = xrCreateSession(m_xrInstance, &sessionCI, &m_session);
	if (result != XR_SUCCESS) {
		std::cout << "Failed to create Session: " << result << std::endl;
	}

	return 0;
}
void OpenXrContext::DestroySession() {
	XrResult result = xrDestroySession(m_session);
	if (result != XR_SUCCESS) {
		std::cout << "Failed to destroy Session: " << result << "\n";
	}
}

void OpenXrContext::PollEvents() {
	XrResult result;

	// Poll OpenXR for a new event.
	XrEventDataBuffer eventData{ XR_TYPE_EVENT_DATA_BUFFER };
	auto XrPollEvents = [&]() -> bool {
		eventData = { XR_TYPE_EVENT_DATA_BUFFER };
		return xrPollEvent(m_xrInstance, &eventData) == XR_SUCCESS;
		};

	while (XrPollEvents()) {
		switch (eventData.type) {
			// Log the number of lost events from the runtime.
		case XR_TYPE_EVENT_DATA_EVENTS_LOST: {
			XrEventDataEventsLost* eventsLost = reinterpret_cast<XrEventDataEventsLost*>(&eventData);
			std::cout << "OPENXR: Events Lost: " << eventsLost->lostEventCount;
			break;
		}
										   // Log that an instance loss is pending and shutdown the application.
		case XR_TYPE_EVENT_DATA_INSTANCE_LOSS_PENDING: {
			XrEventDataInstanceLossPending* instanceLossPending = reinterpret_cast<XrEventDataInstanceLossPending*>(&eventData);
			std::cout << "OPENXR: Instance Loss Pending at: " << instanceLossPending->lossTime;
			m_sessionRunning = false;
			m_applicationRunning = false;
			break;
		}
													 // Log that the interaction profile has changed.
		case XR_TYPE_EVENT_DATA_INTERACTION_PROFILE_CHANGED: {
			XrEventDataInteractionProfileChanged* interactionProfileChanged = reinterpret_cast<XrEventDataInteractionProfileChanged*>(&eventData);
			std::cout << "OPENXR: Interaction Profile changed for Session: " << interactionProfileChanged->session;
			if (interactionProfileChanged->session != m_session) {
				std::cout << "XrEventDataInteractionProfileChanged for unknown Session";
				break;
			}
			break;
		}
														   // Log that there's a reference space change pending.
		case XR_TYPE_EVENT_DATA_REFERENCE_SPACE_CHANGE_PENDING: {
			XrEventDataReferenceSpaceChangePending* referenceSpaceChangePending = reinterpret_cast<XrEventDataReferenceSpaceChangePending*>(&eventData);
			std::cout << "OPENXR: Reference Space Change pending for Session: " << referenceSpaceChangePending->session;
			if (referenceSpaceChangePending->session != m_session) {
				std::cout << "XrEventDataReferenceSpaceChangePending for unknown Session";
				break;
			}
			break;
		}
															  // Session State changes:
		case XR_TYPE_EVENT_DATA_SESSION_STATE_CHANGED: {
			XrEventDataSessionStateChanged* sessionStateChanged = reinterpret_cast<XrEventDataSessionStateChanged*>(&eventData);
			if (sessionStateChanged->session != m_session) {
				std::cout << "XrEventDataSessionStateChanged for unknown Session";
				break;
			}

			if (sessionStateChanged->state == XR_SESSION_STATE_READY) {
				// SessionState is ready. Begin the XrSession using the XrViewConfigurationType.
				XrSessionBeginInfo sessionBeginInfo{ XR_TYPE_SESSION_BEGIN_INFO };
				sessionBeginInfo.primaryViewConfigurationType = m_viewConfiguration;
				result = xrBeginSession(m_session, &sessionBeginInfo);
				if (result != XR_SUCCESS) {
					std::cout << "Failed to begin Session." << "\n";
				}
				m_sessionRunning = true;
			}
			if (sessionStateChanged->state == XR_SESSION_STATE_STOPPING) {
				// SessionState is stopping. End the XrSession.
				result = xrEndSession(m_session);
				if (result != XR_SUCCESS) {
					std::cout << "Failed to end Session." << "\n";
				}
				m_sessionRunning = false;
			}
			if (sessionStateChanged->state == XR_SESSION_STATE_EXITING) {
				// SessionState is exiting. Exit the application.
				m_sessionRunning = false;
				m_applicationRunning = false;
			}
			if (sessionStateChanged->state == XR_SESSION_STATE_LOSS_PENDING) {
				// SessionState is loss pending. Exit the application.
				// It's possible to try a reestablish an XrInstance and XrSession, but we will simply exit here.
				m_sessionRunning = false;
				m_applicationRunning = false;
			}
			// Store state for reference across the application.
			m_sessionState = sessionStateChanged->state;
			break;
		}
		default: {
			break;
		}
		}
	}
}

void OpenXrContext::GetViewConfigurationViews() {
	XrResult result;

	// Gets the View Configuration Types. The first call gets the count of the array that will be returned. The next call fills out the array.
	uint32_t viewConfigurationCount = 0;
	result = xrEnumerateViewConfigurations(m_xrInstance, m_systemID, 0, &viewConfigurationCount, nullptr);
	if (result != XR_SUCCESS) {
		std::cout << "Failed to enumerate View Configurations: " << result << "\n";
	}
	m_viewConfigurations.resize(viewConfigurationCount);
	result = xrEnumerateViewConfigurations(m_xrInstance, m_systemID, viewConfigurationCount, &viewConfigurationCount, m_viewConfigurations.data());
	if (result != XR_SUCCESS) {
		std::cout << "Failed to enumerate View Configurations: " << result << "\n";
	}

	// Pick the first application supported View Configuration Type con supported by the hardware.
	for (const XrViewConfigurationType& viewConfiguration : m_applicationViewConfigurations) {
		if (std::find(m_viewConfigurations.begin(), m_viewConfigurations.end(), viewConfiguration) != m_viewConfigurations.end()) {
			m_viewConfiguration = viewConfiguration;
			break;
		}
	}
	if (m_viewConfiguration == XR_VIEW_CONFIGURATION_TYPE_MAX_ENUM) {
		std::cerr << "Failed to find a view configuration type. Defaulting to XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO." << std::endl;
		m_viewConfiguration = XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO;
	}

	// Gets the View Configuration Views. The first call gets the count of the array that will be returned. The next call fills out the array.
	uint32_t viewConfigurationViewCount = 0;
	result = xrEnumerateViewConfigurationViews(m_xrInstance, m_systemID, m_viewConfiguration, 0, &viewConfigurationViewCount, nullptr);
	if (result != XR_SUCCESS) {
		std::cout << "Failed to enumerate ViewConfiguration Views: " << result << "\n";
	}
	m_viewConfigurationViews.resize(viewConfigurationViewCount, { XR_TYPE_VIEW_CONFIGURATION_VIEW });
	result = xrEnumerateViewConfigurationViews(m_xrInstance, m_systemID, m_viewConfiguration, viewConfigurationViewCount
		, &viewConfigurationViewCount, m_viewConfigurationViews.data());
	if (result != XR_SUCCESS) {
		std::cout << "Failed to enumerate ViewConfiguration Views: " << result << "\n";
	}
}
void OpenXrContext::GetEnvironmentBlendModes() {
	XrResult result;

	// Retrieves the available blend modes. The first call gets the count of the array that will be returned. The next call fills out the array.
	uint32_t environmentBlendModeCount = 0;
	result = xrEnumerateEnvironmentBlendModes(m_xrInstance, m_systemID, m_viewConfiguration, 0, &environmentBlendModeCount, nullptr);
	if (result != XR_SUCCESS) {
		std::cout << "Failed to enumerate EnvironmentBlend Modes: " << result << "\n";
	}
	m_environmentBlendModes.resize(environmentBlendModeCount);
	result = xrEnumerateEnvironmentBlendModes(m_xrInstance, m_systemID, m_viewConfiguration, environmentBlendModeCount, &environmentBlendModeCount, m_environmentBlendModes.data());
	if (result != XR_SUCCESS) {
		std::cout << "Failed to enumerate EnvironmentBlend Modes: " << result << "\n";
	}

	// Pick the first application supported blend mode supported by the hardware.
	for (const XrEnvironmentBlendMode& environmentBlendMode : m_applicationEnvironmentBlendModes) {
		if (std::find(m_environmentBlendModes.begin(), m_environmentBlendModes.end(), environmentBlendMode) != m_environmentBlendModes.end()) {
			m_environmentBlendMode = environmentBlendMode;
			break;
		}
	}
	if (m_environmentBlendMode == XR_ENVIRONMENT_BLEND_MODE_MAX_ENUM) {
		std::cout << "Failed to find a compatible blend mode. Defaulting to XR_ENVIRONMENT_BLEND_MODE_OPAQUE.";
		m_environmentBlendMode = XR_ENVIRONMENT_BLEND_MODE_OPAQUE;
	}
}

void OpenXrContext::CreateReferenceSpace() {
	// Fill out an XrReferenceSpaceCreateInfo structure and create a reference XrSpace, specifying a Local space with an identity pose as the origin.
	XrReferenceSpaceCreateInfo referenceSpaceCI{ XR_TYPE_REFERENCE_SPACE_CREATE_INFO };
	referenceSpaceCI.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_STAGE;
	referenceSpaceCI.poseInReferenceSpace = { {0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f} };
	XrResult result = xrCreateReferenceSpace(m_session, &referenceSpaceCI, &m_localSpace);
	if (result != XR_SUCCESS) {
		std::cout << "Failed to create ReferenceSpace: " << result << "\n";
	}
}
void OpenXrContext::DestroyReferenceSpace() {
	// Destroy the reference XrSpace.
	XrResult result = xrDestroySpace(m_localSpace);
	if (result != XR_SUCCESS) {
		std::cout << "Failed to destroy Space: " << result << "\n";
	}
}

void OpenXrContext::CreateSwapchains() {
	XrResult result;
	// Get the supported swapchain formats as an array of int64_t and ordered by runtime preference.
	uint32_t formatCount = 0;
	result = xrEnumerateSwapchainFormats(m_session, 0, &formatCount, nullptr);
	if (result != XR_SUCCESS) {
		std::cout << "Failed to enumerate Swapchain Formats: " << result << "\n";
	}
	std::vector<int64_t> formats(formatCount);
	result = xrEnumerateSwapchainFormats(m_session, formatCount, &formatCount, formats.data());
	if (result != XR_SUCCESS) {
		std::cout << "Failed to enumerate Swapchain Formats: " << result << "\n";
	}
	/*if (m_graphicsAPI->SelectDepthSwapchainFormat(formats) == 0) {
		std::cerr << "Failed to find depth format for Swapchain." << std::endl;
		DEBUG_BREAK;
	}*/

	//Resize the SwapchainInfo to match the number of view in the View Configuration.
	m_colorSwapchainInfos.resize(m_viewConfigurationViews.size());
	m_depthSwapchainInfos.resize(m_viewConfigurationViews.size());

	for (size_t i = 0; i < m_viewConfigurationViews.size(); i++) {
		SwapchainInfo& colorSwapchainInfo = m_colorSwapchainInfos[i];
		SwapchainInfo& depthSwapchainInfo = m_depthSwapchainInfos[i];

		// Fill out an XrSwapchainCreateInfo structure and create an XrSwapchain.
		// Color.
		XrSwapchainCreateInfo swapchainCI{ XR_TYPE_SWAPCHAIN_CREATE_INFO };
		swapchainCI.createFlags = 0;
		swapchainCI.usageFlags = XR_SWAPCHAIN_USAGE_SAMPLED_BIT | XR_SWAPCHAIN_USAGE_COLOR_ATTACHMENT_BIT;
		swapchainCI.format = GL_RGBA8;                // Use GraphicsAPI to select the first compatible format. GL_RGB10_A2, GL_RGBA16F | GL_RGBA8, GL_RGBA8_SNORM // Oculus: GL_RGBA8, SteamVR: GL_RGBA16
		swapchainCI.sampleCount = m_viewConfigurationViews[i].recommendedSwapchainSampleCount;  // Use the recommended values from the XrViewConfigurationView.
		swapchainCI.width = m_viewConfigurationViews[i].recommendedImageRectWidth;
		swapchainCI.height = m_viewConfigurationViews[i].recommendedImageRectHeight;
		swapchainCI.faceCount = 1;
		swapchainCI.arraySize = 1;
		swapchainCI.mipCount = 1;
		result = xrCreateSwapchain(m_session, &swapchainCI, &colorSwapchainInfo.swapchain);
		if (result != XR_SUCCESS) {
			std::cout << "Failed to create Color Swapchain: " << result << "\n";
		}
		colorSwapchainInfo.swapchainFormat = swapchainCI.format;  // Save the swapchain format for later use.

		// Depth.
		swapchainCI.createFlags = 0;
		swapchainCI.usageFlags = XR_SWAPCHAIN_USAGE_SAMPLED_BIT | XR_SWAPCHAIN_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		swapchainCI.format = GL_DEPTH_COMPONENT32F;                // Use GraphicsAPI to select the first compatible format. GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT32
		swapchainCI.sampleCount = m_viewConfigurationViews[i].recommendedSwapchainSampleCount;  // Use the recommended values from the XrViewConfigurationView.
		swapchainCI.width = m_viewConfigurationViews[i].recommendedImageRectWidth;
		swapchainCI.height = m_viewConfigurationViews[i].recommendedImageRectHeight;
		swapchainCI.faceCount = 1;
		swapchainCI.arraySize = 1;
		swapchainCI.mipCount = 1;
		result = xrCreateSwapchain(m_session, &swapchainCI, &depthSwapchainInfo.swapchain);
		if (result != XR_SUCCESS) {
			std::cout << "Failed to create Depth Swapchain: " << result << "\n";
		}
		depthSwapchainInfo.swapchainFormat = swapchainCI.format;  // Save the swapchain format for later use.


		// Get the number of images in the color/depth swapchain and allocate Swapchain image data via GraphicsAPI to store the returned array.
		uint32_t colorSwapchainImageCount = 0;
		result = xrEnumerateSwapchainImages(colorSwapchainInfo.swapchain, 0, &colorSwapchainImageCount, nullptr);
		if (result != XR_SUCCESS) {
			std::cout << "Failed to enumerate Color Swapchain Images: " << result << "\n";
		}
		
		XrSwapchainImageBaseHeader* colorSwapchainImages = this->openGLHelper.AllocateSwapchainImageData(colorSwapchainInfo.swapchain, SwapchainType::COLOR, colorSwapchainImageCount);
		//swapchainImagesMap[colorSwapchainInfo.swapchain].first = SwapchainType::COLOR;
		//swapchainImagesMap[colorSwapchainInfo.swapchain].second.resize(colorSwapchainImageCount, { XR_TYPE_SWAPCHAIN_IMAGE_OPENGL_KHR });

		//XrSwapchainImageBaseHeader* colorSwapchainImages = m_graphicsAPI->AllocateSwapchainImageData(colorSwapchainInfo.swapchain, GraphicsAPI::SwapchainType::COLOR, colorSwapchainImageCount);
		result = xrEnumerateSwapchainImages(colorSwapchainInfo.swapchain, colorSwapchainImageCount, &colorSwapchainImageCount, colorSwapchainImages);
		if (result != XR_SUCCESS) {
			std::cout << "Failed to enumerate Color Swapchain Images: " << result << "\n";
		}

		uint32_t depthSwapchainImageCount = 0;
		result = xrEnumerateSwapchainImages(depthSwapchainInfo.swapchain, 0, &depthSwapchainImageCount, nullptr);
		if (result != XR_SUCCESS) {
			std::cout << "Failed to enumerate Depth Swapchain Images: " << result << "\n";
		}

		XrSwapchainImageBaseHeader* depthSwapchainImages = this->openGLHelper.AllocateSwapchainImageData(depthSwapchainInfo.swapchain, SwapchainType::DEPTH, depthSwapchainImageCount);
		//swapchainImagesMap[depthSwapchainInfo.swapchain].first = SwapchainType::DEPTH;
		//swapchainImagesMap[depthSwapchainInfo.swapchain].second.resize(depthSwapchainImageCount, { XR_TYPE_SWAPCHAIN_IMAGE_OPENGL_KHR });

		//XrSwapchainImageBaseHeader* depthSwapchainImages = m_graphicsAPI->AllocateSwapchainImageData(depthSwapchainInfo.swapchain, GraphicsAPI::SwapchainType::DEPTH, depthSwapchainImageCount);
		result = xrEnumerateSwapchainImages(depthSwapchainInfo.swapchain, depthSwapchainImageCount, &depthSwapchainImageCount, depthSwapchainImages);
		if (result != XR_SUCCESS) {
			std::cout << "Failed to enumerate Depth Swapchain Images: " << result << "\n";
		}


		// Per image in the swapchains, fill out a GraphicsAPI::ImageViewCreateInfo structure and create a color/depth image view.
		for (uint32_t j = 0; j < colorSwapchainImageCount; j++) {
			ImageViewCreateInfo imageViewCI;
			imageViewCI.image = this->openGLHelper.GetSwapchainImage(colorSwapchainInfo.swapchain, j);
			//imageViewCI.image = (void*)(uint64_t)this->openGLHelper.swapchainImagesMap[colorSwapchainInfo.swapchain].second[j].image;
			imageViewCI.type = ImageViewCreateInfo::Type::RTV;
			imageViewCI.view = ImageViewCreateInfo::View::TYPE_2D;
			imageViewCI.format = colorSwapchainInfo.swapchainFormat;
			imageViewCI.aspect = ImageViewCreateInfo::Aspect::COLOR_BIT;
			imageViewCI.baseMipLevel = 0;
			imageViewCI.levelCount = 1;
			imageViewCI.baseArrayLayer = 0;
			imageViewCI.layerCount = 1;
			colorSwapchainInfo.imageViews.push_back(this->openGLHelper.CreateImageView(imageViewCI));
		}
		for (uint32_t j = 0; j < depthSwapchainImageCount; j++) {
			ImageViewCreateInfo imageViewCI;
			imageViewCI.image = this->openGLHelper.GetSwapchainImage(depthSwapchainInfo.swapchain, j);
			//imageViewCI.image = (void*)(uint64_t)this->openGLHelper.swapchainImagesMap[depthSwapchainInfo.swapchain].second[j].image;
			imageViewCI.type = ImageViewCreateInfo::Type::DSV;
			imageViewCI.view = ImageViewCreateInfo::View::TYPE_2D;
			imageViewCI.format = depthSwapchainInfo.swapchainFormat;
			imageViewCI.aspect = ImageViewCreateInfo::Aspect::DEPTH_BIT;
			imageViewCI.baseMipLevel = 0;
			imageViewCI.levelCount = 1;
			imageViewCI.baseArrayLayer = 0;
			imageViewCI.layerCount = 1;
			depthSwapchainInfo.imageViews.push_back(this->openGLHelper.CreateImageView(imageViewCI));
		}
	}
}
void OpenXrContext::DestroySwapchains() {
	// Per view in the view configuration:
	for (size_t i = 0; i < m_viewConfigurationViews.size(); i++) {
		SwapchainInfo& colorSwapchainInfo = m_colorSwapchainInfos[i];
		SwapchainInfo& depthSwapchainInfo = m_depthSwapchainInfos[i];

		// Destroy the color and depth image views from GraphicsAPI.
		for (void*& imageView : colorSwapchainInfo.imageViews) {
			this->openGLHelper.DestroyImageView(imageView);
			/*GLuint framebuffer = (GLuint)(uint64_t)imageView;
			imageViews.erase(framebuffer);
			glDeleteFramebuffers(1, &framebuffer);
			imageView = nullptr;*/
		}
		for (void*& imageView : depthSwapchainInfo.imageViews) {
			this->openGLHelper.DestroyImageView(imageView);
			/*GLuint framebuffer = (GLuint)(uint64_t)imageView;
			imageViews.erase(framebuffer);
			glDeleteFramebuffers(1, &framebuffer);
			imageView = nullptr;*/
		}

		// Free the Swapchain Image Data.
		this->openGLHelper.FreeSwapchainImageData(colorSwapchainInfo.swapchain);
		this->openGLHelper.FreeSwapchainImageData(depthSwapchainInfo.swapchain);
		/*swapchainImagesMap[colorSwapchainInfo.swapchain].second.clear();
		swapchainImagesMap.erase(colorSwapchainInfo.swapchain);
		swapchainImagesMap[depthSwapchainInfo.swapchain].second.clear();
		swapchainImagesMap.erase(depthSwapchainInfo.swapchain);*/

		// Destroy the swapchains.
		XrResult result;
		result = xrDestroySwapchain(colorSwapchainInfo.swapchain);
		if (result != XR_SUCCESS) {
			std::cout << "Failed to destroy Color Swapchain: " << result << "\n";
		}
		result = xrDestroySwapchain(depthSwapchainInfo.swapchain);
		if (result != XR_SUCCESS) {
			std::cout << "Failed to destroy Depth Swapchain: " << result << "\n";
		}
	}
}

//Getter
XrInstance* OpenXrContext::GetInstance() {
	return &m_xrInstance;
}

XrSession* OpenXrContext::GetSession() {
	return &m_session;
}
XrSpace* OpenXrContext::GetLocalSpace() {
	return &m_localSpace;
}
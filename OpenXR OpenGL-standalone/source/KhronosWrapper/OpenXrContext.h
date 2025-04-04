#pragma once

#include <string>
#include <vector>

#include "OpenGLHelper.h"

struct SwapchainInfo {
	XrSwapchain swapchain = XR_NULL_HANDLE;
	int64_t swapchainFormat = 0;
	std::vector<void*> imageViews;
};

struct RenderLayerInfo {
	XrTime predictedDisplayTime;
	std::vector<XrCompositionLayerBaseHeader*> layers;
	XrCompositionLayerProjection layerProjection = { XR_TYPE_COMPOSITION_LAYER_PROJECTION };
	std::vector<XrCompositionLayerProjectionView> layerProjectionViews;
};

class OpenXrContext {
private:
	XrInstance m_xrInstance = {};
	std::vector<const char*> m_activeAPILayers = {};
	std::vector<const char*> m_activeInstanceExtensions = {};
	std::vector<std::string> m_apiLayers = {};
	std::vector<std::string> m_instanceExtensions = {};

	XrFormFactor m_formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;
	XrSystemId m_systemID = {};
	XrSystemProperties m_systemProperties = { XR_TYPE_SYSTEM_PROPERTIES };

	XrSession m_session = XR_NULL_HANDLE;
	XrSessionState m_sessionState = XR_SESSION_STATE_UNKNOWN;
	bool m_applicationRunning = true;
	bool m_sessionRunning = false;


	std::vector<XrViewConfigurationType> m_applicationViewConfigurations = { XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO, XR_VIEW_CONFIGURATION_TYPE_PRIMARY_MONO };
	std::vector<XrViewConfigurationType> m_viewConfigurations;
	XrViewConfigurationType m_viewConfiguration = XR_VIEW_CONFIGURATION_TYPE_MAX_ENUM;
	std::vector<XrViewConfigurationView> m_viewConfigurationViews;

	std::vector<SwapchainInfo> m_colorSwapchainInfos = {};
	std::vector<SwapchainInfo> m_depthSwapchainInfos = {};

	std::vector<XrEnvironmentBlendMode> m_applicationEnvironmentBlendModes = { XR_ENVIRONMENT_BLEND_MODE_OPAQUE, XR_ENVIRONMENT_BLEND_MODE_ADDITIVE };
	std::vector<XrEnvironmentBlendMode> m_environmentBlendModes = {};
	XrEnvironmentBlendMode m_environmentBlendMode = XR_ENVIRONMENT_BLEND_MODE_MAX_ENUM;

	XrSpace m_localSpace = XR_NULL_HANDLE;


	PFN_xrGetOpenGLGraphicsRequirementsKHR xrGetOpenGLGraphicsRequirementsKHR = nullptr;
	XrGraphicsBindingOpenGLWin32KHR graphicsBinding{};

	OpenGLHelper openGLHelper{};

	GLFWwindow* window;
public:
	void CreateInstance();
	void DestroyInstance();

	void GetInstanceProperties();
	void GetSystemID();

	int CreateSession();
	void DestroySession();

	void PollEvents();

	void GetViewConfigurationViews();
	void GetEnvironmentBlendModes();

	void CreateReferenceSpace();
	void DestroyReferenceSpace();

	void CreateSwapchains();
	void DestroySwapchains();

	bool IsApplicationRunning() const { return m_applicationRunning; }
	bool IsSessionRunning() const { return m_sessionRunning; }

	//Getter
	XrInstance* GetInstance();
	XrSession* GetSession();
	XrSpace* GetLocalSpace();
	XrSessionState GetSessionState() const { return m_sessionState; }
	XrEnvironmentBlendMode GetEnvironmentBlendMode() const { return m_environmentBlendMode; }
	OpenGLHelper GetOpenGLHelper() const { return openGLHelper; }
	XrViewConfigurationType GetViewConfiguration() const { return m_viewConfiguration; }
	std::vector<XrViewConfigurationView> GetViewConfigurationViewsVariable() const { return m_viewConfigurationViews; }
	XrViewConfigurationView GetViewConfigurationView(uint32_t index) const { return m_viewConfigurationViews[index]; }
	//std::vector<SwapchainInfo> GetColorSwapchainInfos() const { return &m_colorSwapchainInfos; }
	SwapchainInfo* GetColorSwapchainInfo(int index) { return &m_colorSwapchainInfos[index]; }
	SwapchainInfo* GetDepthSwapchainInfo(int index) { return &m_depthSwapchainInfos[index]; }
};
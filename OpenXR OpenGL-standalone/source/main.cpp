#include "KhronosWrapper/OpenXrContext.h"
#include "KhronosWrapper/OpenXrActionSetManager.h"

#include "View.h"
#include "Model.h"
#include "Time.h"
#include "OpenGLComponents/Framebuffer.h"
#include "Engine/VRInput.h"
#include "Engine/Physics/Physics.h"
#include "Engine/VRCamera.h"

#include <cmath>

class OpenXrRuntime {
private:
	OpenXrContext openXrContext{};
	OpenXrActionSetManager actionSetManager{};
	VRCamera camera{};

	std::unique_ptr<View> view;
	Model model{};

	std::unique_ptr<ShaderProgramm> finalShader;
	bool firstFrame = true;

public:
	OpenXrRuntime() {

	}

	void Run() {
		//connects to runtime and sets the layers/extensions
		this->openXrContext.CreateInstance();
		XrInstance* instance = this->openXrContext.GetInstance();

		//infos about the runtime
		this->openXrContext.GetInstanceProperties();
		//infos about the headset
		this->openXrContext.GetSystemID();

		this->actionSetManager.CreateActionSet(instance);
		this->actionSetManager.SuggestBindings(instance);

		//Get infos about the displays. Stereo/Mono - Height/Width
		this->openXrContext.GetViewConfigurationViews();
		//sets blendmode to whatever is supported by the runtime. VR vs AR
		this->openXrContext.GetEnvironmentBlendModes();

		//create session and openGL Context(glfw)
		//the session handles the application states. The runtime now controlls this application over states
		this->openXrContext.CreateSession();
		XrSession* session = this->openXrContext.GetSession();
		this->actionSetManager.CreateActionPoses(instance, session);
		this->actionSetManager.AttachActionSetToSession(session);

		//where is the origin
		this->openXrContext.CreateReferenceSpace();
		this->openXrContext.CreateSwapchains();

		this->finalShader = std::make_unique<ShaderProgramm>("assets/shader/drawFramebuffer.vert", "assets/shader/drawFramebuffer.frag");

		this->model.Start();

		std::cout << "Starting Main Loop" << std::endl;
		while (this->openXrContext.IsApplicationRunning()) {
			Time::UpdateTime(glfwGetTime());
			this->openXrContext.PollEvents();
			if (this->openXrContext.IsSessionRunning()) {
				this->RenderFrame();
			}
		}

		this->openXrContext.DestroySwapchains();
		this->openXrContext.DestroyReferenceSpace();
		this->openXrContext.DestroySession();

		this->openXrContext.DestroyInstance();
	}

private:
	void RenderFrame() {
		XrResult result;
		XrSession session = *this->openXrContext.GetSession();

		// Get the XrFrameState for timing and rendering info.
		XrFrameState frameState{ XR_TYPE_FRAME_STATE };
		XrFrameWaitInfo frameWaitInfo{ XR_TYPE_FRAME_WAIT_INFO };
		result = xrWaitFrame(session, &frameWaitInfo, &frameState);
		if (result != XR_SUCCESS) {
			std::cout << "Failed to wait for XR Frame: " << result << "\n";
		}

		// Tell the OpenXR compositor that the application is beginning the frame.
		XrFrameBeginInfo frameBeginInfo{ XR_TYPE_FRAME_BEGIN_INFO };
		result = xrBeginFrame(session, &frameBeginInfo);
		if (result != XR_SUCCESS) {
			std::cout << "Failed to begin the XR Frame: " << result << "\n";
		}

		// Variables for rendering and layer composition.
		bool rendered = false;
		RenderLayerInfo renderLayerInfo;
		renderLayerInfo.predictedDisplayTime = frameState.predictedDisplayTime;

		// Check that the session is active and that we should render.
		XrSessionState sessionState = this->openXrContext.GetSessionState();
		bool sessionActive = (sessionState == XR_SESSION_STATE_SYNCHRONIZED || sessionState == XR_SESSION_STATE_VISIBLE || sessionState == XR_SESSION_STATE_FOCUSED);
		if (sessionActive && frameState.shouldRender) {
			//Maybe the physics and interactions should be outside  the if  statement "frameState.shouldRender"
			//The simulation should maybe continue, even if it isn't rendered

			//Poll Controller Actions and Update VRInput
			this->actionSetManager.PollActions(this->openXrContext.GetSession(), this->openXrContext.GetLocalSpace(), frameState.predictedDisplayTime);
			VRInput::GetInstance()->Update(&this->actionSetManager);

			//Update Physiscs
			Physics::GetInstance()->Update(Scene::GetCurrentScene(), Time::DeltaTime());
			//Update Playerposition
			VRInput::GetInstance()->UpdatePlayerPosition();
			this->camera.CameraWorldOffset = VRInput::GetInstance()->GetPlayerPosition();
			//Update Model and Debug
			this->model.Update(Time::DeltaTime());
			Debug::Update();

			// Render the stereo image and associate one of swapchain images with the XrCompositionLayerProjection structure.
			rendered = RenderLayer(renderLayerInfo);
			if (rendered) {
				renderLayerInfo.layers.push_back(reinterpret_cast<XrCompositionLayerBaseHeader*>(&renderLayerInfo.layerProjection));
			}

			Scene::GetCurrentScene()->DestroyGameObjects();
		}

		// Tell OpenXR that we are finished with this frame; specifying its display time, environment blending and layers.
		XrFrameEndInfo frameEndInfo{ XR_TYPE_FRAME_END_INFO };
		frameEndInfo.displayTime = frameState.predictedDisplayTime;
		frameEndInfo.environmentBlendMode = this->openXrContext.GetEnvironmentBlendMode();
		frameEndInfo.layerCount = static_cast<uint32_t>(renderLayerInfo.layers.size());
		frameEndInfo.layers = renderLayerInfo.layers.data();
		result = xrEndFrame(session, &frameEndInfo);
		if (result != XR_SUCCESS) {
			std::cout << "Failed to end the XR Frame: " << result << "\n";
		}
	}

	bool RenderLayer(RenderLayerInfo& renderLayerInfo) {
		XrResult result;

		// Locate the views from the view configuration within the (reference) space at the display time.
		std::vector<XrView> views(this->openXrContext.GetViewConfigurationViewsVariable().size(), { XR_TYPE_VIEW });

		XrViewState viewState{ XR_TYPE_VIEW_STATE };  // Will contain information on whether the position and/or orientation is valid and/or tracked.
		XrViewLocateInfo viewLocateInfo{ XR_TYPE_VIEW_LOCATE_INFO };
		viewLocateInfo.viewConfigurationType = this->openXrContext.GetViewConfiguration();
		viewLocateInfo.displayTime = renderLayerInfo.predictedDisplayTime;
		viewLocateInfo.space = *this->openXrContext.GetLocalSpace();
		uint32_t viewCount = 0;
		result = xrLocateViews(*this->openXrContext.GetSession(), &viewLocateInfo, &viewState, static_cast<uint32_t>(views.size()), &viewCount, views.data());
		if (result != XR_SUCCESS) {
			std::cout << "Failed to locate Views. \n";
			return false;
		}

		// Resize the layer projection views to match the view count. The layer projection views are used in the layer projection.
		renderLayerInfo.layerProjectionViews.resize(viewCount, { XR_TYPE_COMPOSITION_LAYER_PROJECTION_VIEW });

		//Update the HeadTransform with the views
		VRInput::GetInstance()->UpdateHeadTransform(&views, viewCount);

		// Per view in the view configuration:
		for (uint32_t i = 0; i < viewCount; i++) {
			SwapchainInfo* colorSwapchainInfo = this->openXrContext.GetColorSwapchainInfo(i);
			SwapchainInfo* depthSwapchainInfo = this->openXrContext.GetDepthSwapchainInfo(i);

			// Acquire and wait for an image from the swapchains.
			// Get the image index of an image in the swapchains.
			// The timeout is infinite.
			uint32_t colorImageIndex = 0;
			uint32_t depthImageIndex = 0;
			XrSwapchainImageAcquireInfo acquireInfo{ XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO };
			result = xrAcquireSwapchainImage(colorSwapchainInfo->swapchain, &acquireInfo, &colorImageIndex);
			if (result != XR_SUCCESS) {
				std::cout << "Failed to acquire Image from the Color Swapchian: " << result << "\n";
			}
			result = xrAcquireSwapchainImage(depthSwapchainInfo->swapchain, &acquireInfo, &depthImageIndex);
			if (result != XR_SUCCESS) {
				std::cout << "Failed to acquire Image from the Depth Swapchian: " << result << "\n";
			}

			//wait for the image so that I can write to it savely
			XrSwapchainImageWaitInfo waitInfo = { XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO };
			waitInfo.timeout = XR_INFINITE_DURATION;
			result = xrWaitSwapchainImage(colorSwapchainInfo->swapchain, &waitInfo);
			if (result != XR_SUCCESS) {
				std::cout << "Failed to wait for Image from the Color Swapchain: " << result << "\n";
			}
			result = xrWaitSwapchainImage(depthSwapchainInfo->swapchain, &waitInfo);
			if (result != XR_SUCCESS) {
				std::cout << "Failed to wait for Image from the Depth Swapchain: " << result << "\n";
			}

			// Get the width and height and construct the viewport and scissors.
			const uint32_t& width = this->openXrContext.GetViewConfigurationView(i).recommendedImageRectWidth;
			const uint32_t& height = this->openXrContext.GetViewConfigurationView(i).recommendedImageRectHeight;
			Viewport viewport = { 0.0f, 0.0f, (float)width, (float)height, 0.0f, 1.0f };
			Rect2D scissor = { {(int32_t)0, (int32_t)0}, {width, height} };

			// Fill out the XrCompositionLayerProjectionView structure specifying the pose and fov from the view.
			// This also associates the swapchain image with this layer projection view.
			renderLayerInfo.layerProjectionViews[i] = { XR_TYPE_COMPOSITION_LAYER_PROJECTION_VIEW };
			renderLayerInfo.layerProjectionViews[i].pose = views[i].pose;
			renderLayerInfo.layerProjectionViews[i].fov = views[i].fov;
			renderLayerInfo.layerProjectionViews[i].subImage.swapchain = colorSwapchainInfo->swapchain;
			renderLayerInfo.layerProjectionViews[i].subImage.imageRect.offset.x = 0;
			renderLayerInfo.layerProjectionViews[i].subImage.imageRect.offset.y = 0;
			renderLayerInfo.layerProjectionViews[i].subImage.imageRect.extent.width = static_cast<int32_t>(width);
			renderLayerInfo.layerProjectionViews[i].subImage.imageRect.extent.height = static_cast<int32_t>(height);
			renderLayerInfo.layerProjectionViews[i].subImage.imageArrayIndex = 0;  // Useful for multiview rendering.

			/*glBindFramebuffer(GL_FRAMEBUFFER, (GLuint)(uint64_t)colorSwapchainInfo.imageViews[colorImageIndex]);
			glClearColor(0.17f, 0.17f, 0.17f, 1.00f);
			glClear(GL_COLOR_BUFFER_BIT);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);*/

			if (this->openXrContext.GetEnvironmentBlendMode() == XR_ENVIRONMENT_BLEND_MODE_OPAQUE) {
				// VR mode use a background color.
				glBindFramebuffer(GL_FRAMEBUFFER, (GLuint)(uint64_t)colorSwapchainInfo->imageViews[colorImageIndex]);//colorSwapchainInfo.imageViews[colorImageIndex] is handle to the framebuffer
				glClearColor(0.17f, 0.17f, 0.17f, 1.00f);
				glClear(GL_COLOR_BUFFER_BIT);
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				//m_graphicsAPI->ClearColor(colorSwapchainInfo.imageViews[colorImageIndex], 0.17f, 0.17f, 0.17f, 1.00f);
			}
			else {
				// In AR mode make the background color black.
				glClearColor(0.00f, 0.00f, 0.00f, 1.00f);
				glClear(GL_COLOR_BUFFER_BIT);
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				//m_graphicsAPI->ClearColor(colorSwapchainInfo.imageViews[colorImageIndex], 0.00f, 0.00f, 0.00f, 1.00f);
			}
			//Clear Depth
			glBindFramebuffer(GL_FRAMEBUFFER, (GLuint)(uint64_t)depthSwapchainInfo->imageViews[depthImageIndex]);
			glClearDepth(1.0f);
			glClear(GL_DEPTH_BUFFER_BIT);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);


			//SetViewport
			glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
			glDepthRange((GLdouble)viewport.minDepth, (GLdouble)viewport.maxDepth);
			//SetSissors
			glScissor((GLint)scissor.offset.x, (GLint)scissor.offset.y, (GLsizei)scissor.extent.width, (GLsizei)scissor.extent.height);


			//Render Info creation
			RenderInfo renderInfo;
			renderInfo.ViewMatrix = this->camera.CreateViewMatrix(views[i]);
			renderInfo.ProjectionMatrix = this->camera.CreateProjectionMatrix(views[i]);
			renderInfo.CameraPosition = this->camera.GetCameraPosition(views[i]);


			//GBuffer Rendering here

			if (this->firstFrame) {
				std::cout << "First Frame" << std::endl;

				this->view = std::make_unique<View>(width, height);
				this->firstFrame = false;
			}

			this->view->DrawToGBuffer(Scene::GetCurrentScene(), Time::DeltaTime(), renderInfo);

			this->view->CombineGBuffer(Scene::GetCurrentScene(), renderInfo);

			//Set Framebuffer to Swapchain
			this->openXrContext.GetOpenGLHelper().SetRenderAttachments(&colorSwapchainInfo->imageViews[colorImageIndex], 1, depthSwapchainInfo->imageViews[depthImageIndex], width, height);

			//Final Rendering

			GLTexture2D* colorTexture = this->view->GetFramebufferTexture();
			if (colorTexture == nullptr) {
				std::cout << "ERROR::FRAMEBUFFER::ATTACHMENT:: Color Texture is nullptr!" << std::endl;
			}

			this->finalShader->Bind();
			colorTexture->BindToUnit(3);
			this->finalShader->SetTextureLocation("frameBufferImage", 3);

			glDisable(GL_DEPTH_TEST);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			//End Rendering

			// Give the swapchain image back to OpenXR, allowing the compositor to use the image.
			XrSwapchainImageReleaseInfo releaseInfo{ XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO };
			result = xrReleaseSwapchainImage(colorSwapchainInfo->swapchain, &releaseInfo);
			if (result != XR_SUCCESS) {
				std::cout << "Failed to release Image back to the Color Swapchain: " << result << "\n";
			}
			result = xrReleaseSwapchainImage(depthSwapchainInfo->swapchain, &releaseInfo);
			if (result != XR_SUCCESS) {
				std::cout << "Failed to release Image back to the Depth Swapchain: " << result << "\n";
			}
		}

		// Fill out the XrCompositionLayerProjection structure for usage with xrEndFrame().
		renderLayerInfo.layerProjection.layerFlags = XR_COMPOSITION_LAYER_BLEND_TEXTURE_SOURCE_ALPHA_BIT | XR_COMPOSITION_LAYER_CORRECT_CHROMATIC_ABERRATION_BIT;
		renderLayerInfo.layerProjection.space = *this->openXrContext.GetLocalSpace();
		renderLayerInfo.layerProjection.viewCount = static_cast<uint32_t>(renderLayerInfo.layerProjectionViews.size());
		renderLayerInfo.layerProjection.views = renderLayerInfo.layerProjectionViews.data();

		return true;
	}
};

int main() {
	std::cout << "Starting \n";

	OpenXrRuntime app;
	app.Run();

	std::cout << "Ending \n";
	return 0;
}
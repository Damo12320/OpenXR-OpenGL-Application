#include "VRInput.h"

//static
VRInput* VRInput::instance = nullptr;
//static
VRInput* VRInput::GetInstance() {
	if (instance == nullptr) {
		instance = new VRInput();
	}
	return instance;
}


void VRInput::Update(OpenXrActionSetManager* actionsetManager) {
	//Hand Transforms
	XrPosef leftHandPose = actionsetManager->GetHandPose(0);
	VRInputTransform leftHandTransform = {
		glm::vec3(leftHandPose.position.x, leftHandPose.position.y, leftHandPose.position.z),
		glm::quat(leftHandPose.orientation.w, leftHandPose.orientation.x, leftHandPose.orientation.y, leftHandPose.orientation.z)
	};
	this->handTransform[Hand::LEFT] = leftHandTransform;

	XrPosef rightHandPose = actionsetManager->GetHandPose(1);
	VRInputTransform rightHandTransform = {
		glm::vec3(rightHandPose.position.x, rightHandPose.position.y, rightHandPose.position.z),
		glm::quat(rightHandPose.orientation.w, rightHandPose.orientation.x, rightHandPose.orientation.y, rightHandPose.orientation.z)
	};
	this->handTransform[Hand::RIGHT] = rightHandTransform;

	XrActionStateFloat floatState;
	XrActionStateVector2f vector2State;

	//Hand Grab Values
	floatState = actionsetManager->GetGrabState(0);
	this->handGrabValue[Hand::LEFT] = floatState.currentState;
	floatState = actionsetManager->GetGrabState(1);
	this->handGrabValue[Hand::RIGHT] = floatState.currentState;

	//Hand Select Values
	floatState = actionsetManager->GetSelectState(0);
	this->handSelectValue[Hand::LEFT] = floatState.currentState;
	floatState = actionsetManager->GetSelectState(1);
	this->handSelectValue[Hand::RIGHT] = floatState.currentState;

	//Hand Thumbstick Values
	vector2State = actionsetManager->GetThumbstickState(0);
	this->handThumbstickValue[Hand::LEFT] = glm::vec2(vector2State.currentState.x, vector2State.currentState.y);
	vector2State = actionsetManager->GetThumbstickState(1);
	this->handThumbstickValue[Hand::RIGHT] = glm::vec2(vector2State.currentState.x, vector2State.currentState.y);
}
void VRInput::UpdatePlayerPosition() {
	glm::vec2 thumbstick = this->GetHandThumbstickValue(Hand::LEFT);
	glm::vec3 headForward = this->headTransform.Forward();
	headForward.y = 0;
	headForward = glm::normalize(headForward);
	glm::vec3 headRight = this->headTransform.Right();
	headRight.y = 0;
	headRight = glm::normalize(headRight);
	glm::vec3 movement = glm::vec3(0.0);
	movement += headForward * -thumbstick.y;
	movement += headRight * thumbstick.x;

	this->playerPosition += movement * Time::DeltaTime();
}
void VRInput::UpdateHeadTransform(std::vector<XrView>* views, int viewCount) {
	VRInputTransform transform;
	transform.position = glm::vec3(0);
	for (int i = 0; i < viewCount; i++) {
		XrView view = views->at(i);
		transform.position += glm::vec3(view.pose.position.x, view.pose.position.y, view.pose.position.z);
	}
	transform.position /= viewCount;
	XrView view = views->at(0);
	transform.rotation = glm::quat(view.pose.orientation.w, view.pose.orientation.x, view.pose.orientation.y, view.pose.orientation.z);

	this->headTransform = transform;
}


VRInputTransform VRInput::GetHandTransform(Hand hand) {
	VRInputTransform transform = this->handTransform[hand];
	transform.position += this->playerPosition;

	return transform;
}
VRInputTransform VRInput::GetHeadTransform() {
	VRInputTransform transform = this->headTransform;
	transform.position += this->playerPosition;
	return transform;
}

glm::vec3 VRInput::GetPlayerPosition() {
	return this->playerPosition;
}

float VRInput::GetHandGrabValue(Hand hand) {
	return this->handGrabValue[hand];
}
float VRInput::GetHandSelectValue(Hand hand) {
	return this->handSelectValue[hand];
}
glm::vec2 VRInput::GetHandThumbstickValue(Hand hand) {
	return this->handThumbstickValue[hand];
}


bool VRInput::IsHandGrabbing(Hand hand) {
	return this->handGrabValue[hand];
}
bool VRInput::IsHandSelecting(Hand hand) {
	return this->handSelectValue[hand];
}
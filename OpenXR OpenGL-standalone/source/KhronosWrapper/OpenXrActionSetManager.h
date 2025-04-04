#pragma once

#include "OpenXrContext.h"

class OpenXrActionSetManager {
private:
	XrActionSet actionSet;

	XrAction grabAction;
	XrAction selectAction;

	XrAction thumbstickAction;

	XrActionStateFloat grabState[2] = { {XR_TYPE_ACTION_STATE_FLOAT}, {XR_TYPE_ACTION_STATE_FLOAT} };
	XrActionStateFloat selectState[2] = { {XR_TYPE_ACTION_STATE_FLOAT}, {XR_TYPE_ACTION_STATE_FLOAT} };

	XrActionStateVector2f thumbstickState[2] = {{XR_TYPE_ACTION_STATE_VECTOR2F}, {XR_TYPE_ACTION_STATE_VECTOR2F}};

	XrAction vibrationAction;
	float vibration[2] = { 0, 0 };

	XrAction palmPoseAction;

	XrPath handPaths[2] = { 0, 0 };
	XrSpace handPoseSpaces[2];

	XrActionStatePose handPoseState[2] = { {XR_TYPE_ACTION_STATE_POSE}, {XR_TYPE_ACTION_STATE_POSE} };

	XrPosef handPose[2] = {
		{{1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
		{{1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}} };

private:
	XrPath CreateXrPath(XrInstance* instance, const char* path_string);
	std::string FromXrPath(XrInstance* instance, XrPath path);

public:
	void CreateActionSet(XrInstance* instance);
	void SuggestBindings(XrInstance* instance);
	void RecordCurrentBindings(XrInstance* instance, XrSession* session);
	void CreateActionPoses(XrInstance* instance, XrSession* session);
	void AttachActionSetToSession(XrSession* session);

	void PollActions(XrSession* session, XrSpace* localSpace, XrTime predictedTime);

	XrPosef GetHandPose(int hand);
	XrActionStateFloat GetGrabState(int hand);
	XrActionStateFloat GetSelectState(int hand);
	XrActionStateVector2f GetThumbstickState(int hand);
};
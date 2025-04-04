#include "OpenXrActionSetManager.h"

XrPath OpenXrActionSetManager::CreateXrPath(XrInstance* instance, const char* path_string) {
	XrPath path;
	XrResult result = xrStringToPath(*instance, path_string, &path);
	if (XR_FAILED(result)) {
		//throw std::runtime_error("Failed to create path from string: " + std::string(path_string));
        std::cout << "Failed to create path from string: " << std::string(path_string) << "\n";
	}
	return path;
}

std::string OpenXrActionSetManager::FromXrPath(XrInstance* instance, XrPath path) {
    uint32_t strl;
    char text[XR_MAX_PATH_LENGTH];
    XrResult result;
    result = xrPathToString(*instance, path, XR_MAX_PATH_LENGTH, &strl, text);
    std::string str;
    if (result == XR_SUCCESS) {
        str = text;
    }
    else {
        if (result != XR_SUCCESS) {
            std::cout << "Failed to retrieve path." << "\n";
        }
    }
    return str;
}

void OpenXrActionSetManager::CreateActionSet(XrInstance* instance) {
    XrResult result;

    XrActionSetCreateInfo actionSetCI{ XR_TYPE_ACTION_SET_CREATE_INFO };
    // The internal name the runtime uses for this Action Set.
    strncpy_s(actionSetCI.actionSetName, "openxr-tutorial-actionset", XR_MAX_ACTION_SET_NAME_SIZE);
    // Localized names are required so there is a human-readable action name to show the user if they are rebinding Actions in an options screen.
    strncpy_s(actionSetCI.localizedActionSetName, "OpenXR Tutorial ActionSet", XR_MAX_LOCALIZED_ACTION_SET_NAME_SIZE);

    result = xrCreateActionSet(*instance, &actionSetCI, &this->actionSet);
    if (result != XR_SUCCESS) {
        std::cout << "Failed to create ActionSet." << "\n";
    }
    // Set a priority: this comes into play when we have multiple Action Sets, and determines which Action takes priority in binding to a specific input.
    actionSetCI.priority = 0;

    //Function to create an action
    auto CreateAction = [this](XrInstance* instance, XrAction& xrAction, const char* name, XrActionType xrActionType, std::vector<const char*> subaction_paths = {}) -> void {
        XrActionCreateInfo actionCI{ XR_TYPE_ACTION_CREATE_INFO };
        // The type of action: float input, pose, haptic output etc.
        actionCI.actionType = xrActionType;
        // Subaction paths, e.g. left and right hand. To distinguish the same action performed on different devices.
        std::vector<XrPath> subaction_xrpaths;
        for (auto p : subaction_paths) {
            subaction_xrpaths.push_back(CreateXrPath(instance, p));
        }
        actionCI.countSubactionPaths = (uint32_t)subaction_xrpaths.size();
        actionCI.subactionPaths = subaction_xrpaths.data();
        // The internal name the runtime uses for this Action.
        strncpy_s(actionCI.actionName, name, XR_MAX_ACTION_NAME_SIZE);
        // Localized names are required so there is a human-readable action name to show the user if they are rebinding the Action in an options screen.
        strncpy_s(actionCI.localizedActionName, name, XR_MAX_LOCALIZED_ACTION_NAME_SIZE);

        XrResult result = xrCreateAction(this->actionSet, &actionCI, &xrAction);
    };

    // An Action for grabbing
    CreateAction(instance, this->grabAction, "grab", XR_ACTION_TYPE_FLOAT_INPUT, { "/user/hand/left", "/user/hand/right" });
    CreateAction(instance, this->selectAction, "select", XR_ACTION_TYPE_FLOAT_INPUT, { "/user/hand/left", "/user/hand/right" });
    // An Action for the thumbsticks
    CreateAction(instance, this->thumbstickAction, "thumbstick", XR_ACTION_TYPE_VECTOR2F_INPUT, { "/user/hand/left", "/user/hand/right" });
    // An Action for the position of the palm of the user's hand - appropriate for the location of a grabbing Actions.
    CreateAction(instance,  this->palmPoseAction, "palm-pose", XR_ACTION_TYPE_POSE_INPUT, { "/user/hand/left", "/user/hand/right" });
    // An Action for a vibration output on one or other hand.
    CreateAction(instance, this->vibrationAction, "buzz", XR_ACTION_TYPE_VIBRATION_OUTPUT, { "/user/hand/left", "/user/hand/right" });
    // For later convenience we create the XrPaths for the subaction path names.
    this->handPaths[0] = CreateXrPath(instance, "/user/hand/left");
    this->handPaths[1] = CreateXrPath(instance, "/user/hand/right");
}

void OpenXrActionSetManager::SuggestBindings(XrInstance* instance) {
    auto SuggestBindings = [this](XrInstance* instance, const char* profile_path, std::vector<XrActionSuggestedBinding> bindings) -> bool {
        // The application can call xrSuggestInteractionProfileBindings once per interaction profile that it supports.
        XrInteractionProfileSuggestedBinding interactionProfileSuggestedBinding{ XR_TYPE_INTERACTION_PROFILE_SUGGESTED_BINDING };
        interactionProfileSuggestedBinding.interactionProfile = CreateXrPath(instance, profile_path);
        interactionProfileSuggestedBinding.suggestedBindings = bindings.data();
        interactionProfileSuggestedBinding.countSuggestedBindings = (uint32_t)bindings.size();
        if (xrSuggestInteractionProfileBindings(*instance, &interactionProfileSuggestedBinding) == XrResult::XR_SUCCESS) {
            return true;
        }
        else {
            std::cout << "Failed to suggest bindings with " << profile_path << "\n";
            return false;
        }
    };

    XrPath grabPathLeft = CreateXrPath(instance, "/user/hand/left/input/squeeze/value");
    XrPath grabPathRight = CreateXrPath(instance, "/user/hand/right/input/squeeze/value");

    XrPath selectPathLeft = CreateXrPath(instance, "/user/hand/left/input/trigger/value");
    XrPath selectPathRight = CreateXrPath(instance, "/user/hand/right/input/trigger/value");

    XrPath thumbstickPathLeft = CreateXrPath(instance, "/user/hand/left/input/thumbstick");
    XrPath thumbstickPathRight = CreateXrPath(instance, "/user/hand/right/input/thumbstick");

    XrPath palmPosePathLeft = CreateXrPath(instance, "/user/hand/left/input/grip/pose");
    XrPath palmPosePathRight = CreateXrPath(instance, "/user/hand/right/input/grip/pose");

    XrPath vibrationPathLeft = CreateXrPath(instance, "/user/hand/left/output/haptic");
    XrPath vibrationPathRight = CreateXrPath(instance, "/user/hand/right/output/haptic");

    bool any_ok = false;
    // Each Action here has two paths, one for each SubAction path.
    any_ok |= SuggestBindings(instance, "/interaction_profiles/oculus/touch_controller", { {this->grabAction, grabPathLeft},
                                                                            {this->grabAction, grabPathRight},

                                                                            {this->selectAction, selectPathLeft},
                                                                            {this->selectAction, selectPathRight},

                                                                            {this->thumbstickAction, thumbstickPathLeft},
                                                                            {this->thumbstickAction, thumbstickPathRight},

                                                                            {this->palmPoseAction, palmPosePathLeft},
                                                                            {this->palmPoseAction, palmPosePathRight},

                                                                            {this->vibrationAction, vibrationPathLeft},
                                                                            {this->vibrationAction, vibrationPathRight} });

    if (!any_ok) {
        std::cout << "Failed to suggest any bindings." << "\n";
        return;
    }


}

void OpenXrActionSetManager::RecordCurrentBindings(XrInstance *instance, XrSession *session) {
    if (*session) {
        // now we are ready to:
        XrInteractionProfileState interactionProfile = { XR_TYPE_INTERACTION_PROFILE_STATE, 0, 0 };
        // for each action, what is the binding?
        XrResult result;
        result = xrGetCurrentInteractionProfile(*session, this->handPaths[0], &interactionProfile);
        if (result != XR_SUCCESS) {
			std::cout << "Failed to get profile." << "\n";
		}
        if (interactionProfile.interactionProfile) {
            std::cout << "user/hand/left ActiveProfile " << FromXrPath(instance, interactionProfile.interactionProfile).c_str() << "\n";
        }
        result = xrGetCurrentInteractionProfile(*session, this->handPaths[1], &interactionProfile);
        if (result != XR_SUCCESS) {
            std::cout << "Failed to get profile." << "\n";
        }
        if (interactionProfile.interactionProfile) {
            std::cout << "user/hand/right ActiveProfile " << FromXrPath(instance, interactionProfile.interactionProfile).c_str() << "\n";
        }
    }
}

void OpenXrActionSetManager::CreateActionPoses(XrInstance* instance, XrSession* session) {
    // Create an xrSpace for a pose action.
    auto CreateActionPoseSpace = [this](XrInstance *instance, XrSession *session, XrAction xrAction, const char* subaction_path = nullptr) -> XrSpace {
        XrSpace xrSpace;
        const XrPosef xrPoseIdentity = { {0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f} };
        // Create frame of reference for a pose action
        XrActionSpaceCreateInfo actionSpaceCI{ XR_TYPE_ACTION_SPACE_CREATE_INFO };
        actionSpaceCI.action = xrAction;
        actionSpaceCI.poseInActionSpace = xrPoseIdentity;
        if (subaction_path)
            actionSpaceCI.subactionPath = CreateXrPath(instance, subaction_path);

        XrResult result = xrCreateActionSpace(*session, &actionSpaceCI, &xrSpace);
        if (result != XR_SUCCESS) {
			std::cout << "Failed to create ActionSpace." << "\n";
		}
        return xrSpace;
        };
    this->handPoseSpaces[0] = CreateActionPoseSpace(instance, session, this->palmPoseAction, "/user/hand/left");
    this->handPoseSpaces[1] = CreateActionPoseSpace(instance, session, this->palmPoseAction, "/user/hand/right");
}

void OpenXrActionSetManager::AttachActionSetToSession(XrSession* session) {
    // Attach the action set we just made to the session. We could attach multiple action sets!
    XrSessionActionSetsAttachInfo actionSetAttachInfo{ XR_TYPE_SESSION_ACTION_SETS_ATTACH_INFO };
    actionSetAttachInfo.countActionSets = 1;
    actionSetAttachInfo.actionSets = &this->actionSet;
    XrResult result = xrAttachSessionActionSets(*session, &actionSetAttachInfo);
    if (result != XR_SUCCESS) {
		std::cout << "Failed to attach ActionSet to Session." << "\n";
	}
}

void OpenXrActionSetManager::PollActions(XrSession *session, XrSpace *localSpace, XrTime predictedTime) {
    XrResult result;

    // Update our action set with up-to-date input data.
    // First, we specify the actionSet we are polling.
    XrActiveActionSet activeActionSet{};
    activeActionSet.actionSet = this->actionSet;
    activeActionSet.subactionPath = XR_NULL_PATH;
    // Now we sync the Actions to make sure they have current data.
    XrActionsSyncInfo actionsSyncInfo{ XR_TYPE_ACTIONS_SYNC_INFO };
    actionsSyncInfo.countActiveActionSets = 1;
    actionsSyncInfo.activeActionSets = &activeActionSet;
    result = xrSyncActions(*session, &actionsSyncInfo);
    if (result != XR_SUCCESS) {
        std::cout << "Failed to sync Actions." << "\n";
    }

    //HandPoseAction
    XrActionStateGetInfo actionStateGetInfo{ XR_TYPE_ACTION_STATE_GET_INFO };
    // We pose a single Action, twice - once for each subAction Path.
    actionStateGetInfo.action = this->palmPoseAction;
    // For each hand, get the pose state if possible.
    for (int i = 0; i < 2; i++) {
        // Specify the subAction Path.
        actionStateGetInfo.subactionPath = this->handPaths[i];
        result = xrGetActionStatePose(*session, &actionStateGetInfo, &this->handPoseState[i]);
        if (result != XR_SUCCESS) {
            std::cout << "Failed to get Pose State." << "\n";
        }
        if (this->handPoseState[i].isActive) {
            XrSpaceLocation spaceLocation{ XR_TYPE_SPACE_LOCATION };
            XrResult res = xrLocateSpace(this->handPoseSpaces[i], *localSpace, predictedTime, &spaceLocation);
            if (XR_UNQUALIFIED_SUCCESS(res) &&
                (spaceLocation.locationFlags & XR_SPACE_LOCATION_POSITION_VALID_BIT) != 0 &&
                (spaceLocation.locationFlags & XR_SPACE_LOCATION_ORIENTATION_VALID_BIT) != 0) {
                this->handPose[i] = spaceLocation.pose;
            }
            else {
                this->handPoseState[i].isActive = false;
            }
        }
    }

    //GrabAction
    for (int i = 0; i < 2; i++) {
        actionStateGetInfo.action = this->grabAction;
        actionStateGetInfo.subactionPath = this->handPaths[i];
        result = xrGetActionStateFloat(*session, &actionStateGetInfo, &this->grabState[i]);
        if (result != XR_SUCCESS) {
            std::cout << "Failed to get Float State of Grab action." << "\n";
        }
    }

    //SelectAction
    for (int i = 0; i < 2; i++) {
        actionStateGetInfo.action = this->selectAction;
        actionStateGetInfo.subactionPath = this->handPaths[i];
        result = xrGetActionStateFloat(*session, &actionStateGetInfo, &this->selectState[i]);
        if (result != XR_SUCCESS) {
            std::cout << "Failed to get Float State of Select action." << "\n";
        }
    }

    //ThumbstickAction
    for (int i = 0; i < 2; i++) {
        actionStateGetInfo.action = this->thumbstickAction;
        actionStateGetInfo.subactionPath = this->handPaths[i];
        result = xrGetActionStateVector2f(*session, &actionStateGetInfo, &this->thumbstickState[i]);
        if (result != XR_SUCCESS) {
            std::cout << "Failed to get Vector2f State of Thumbstick action." << "\n";
        }
    }

    //VibrationAction
    for (int i = 0; i < 2; i++) {
        this->vibration[i] *= 0.5f;
        if (this->vibration[i] < 0.01f)
            this->vibration[i] = 0.0f;
        XrHapticVibration vibration{ XR_TYPE_HAPTIC_VIBRATION };
        vibration.amplitude = this->vibration[i];
        vibration.duration = XR_MIN_HAPTIC_DURATION;
        vibration.frequency = XR_FREQUENCY_UNSPECIFIED;

        XrHapticActionInfo hapticActionInfo{ XR_TYPE_HAPTIC_ACTION_INFO };
        hapticActionInfo.action = this->vibrationAction;
        hapticActionInfo.subactionPath = this->handPaths[i];
        result = xrApplyHapticFeedback(*session, &hapticActionInfo, (XrHapticBaseHeader*)&vibration);
        if (result != XR_SUCCESS) {
            std::cout << "Failed to apply haptic feedback." << "\n";
        }
    }
}


XrPosef OpenXrActionSetManager::GetHandPose(int hand) {
	return this->handPose[hand];
}
XrActionStateFloat OpenXrActionSetManager::GetGrabState(int hand) {
	return this->grabState[hand];
}
XrActionStateFloat OpenXrActionSetManager::GetSelectState(int hand) {
	return this->selectState[hand];
}
XrActionStateVector2f OpenXrActionSetManager::GetThumbstickState(int hand) {
    return this->thumbstickState[hand];
}
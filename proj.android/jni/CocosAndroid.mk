LOCAL_CPPFLAGS := -DSDKBOX_ENABLED \
-DSDKBOX_COCOS_CREATOR
LOCAL_WHOLE_STATIC_LIBRARIES := PluginReview \
sdkbox
LOCAL_WHOLE_STATIC_LIBRARIES := 
$(call import-add-path,$(LOCAL_PATH))
$(call import-module, ./sdkbox)
$(call import-module, ./PluginReview)
LOCAL_LDLIBS := -landroid \
-llog

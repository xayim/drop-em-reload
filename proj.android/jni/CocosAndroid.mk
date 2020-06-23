LOCAL_CPPFLAGS := -DSDKBOX_ENABLED \
-DSDKBOX_COCOS_CREATOR
LOCAL_WHOLE_STATIC_LIBRARIES := PluginReview \
sdkbox \
PluginSdkboxAds
LOCAL_WHOLE_STATIC_LIBRARIES := 
$(call import-add-path,$(LOCAL_PATH))
$(call import-module, ./sdkbox)
$(call import-module, ./PluginReview)
$(call import-module, ./PluginSdkboxAds)
LOCAL_LDLIBS := -landroid \
-llog
LOCAL_WHOLE_STATIC_LIBRARIES := 
LOCAL_WHOLE_STATIC_LIBRARIES := 
LOCAL_WHOLE_STATIC_LIBRARIES := 
LOCAL_WHOLE_STATIC_LIBRARIES := 

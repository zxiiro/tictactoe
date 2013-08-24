LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main
LOCAL_CFLAGS := -DANDROID

SDL_PATH := ../android/jni/SDL2
SDL_IMAGE_PATH := ../android/jni/SDL2_image

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include \
	$(LOCAL_PATH)/$(SDL_IMAGE_PATH)

# Add your application source files here...
LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c \
	main.cpp \
	engine.cpp \
	board.cpp \
	digits.cpp \
	painter.cpp \
	scoreboard.cpp \
	tile.cpp \
	unit.cpp

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_image

LOCAL_LDLIBS := -lGLESv1_CM -llog

include $(BUILD_SHARED_LIBRARY)

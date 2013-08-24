Android Build
=============

Requirements
------------

Software:
* Android SDK (ADT not required, only SDK tools)
* Android NDK
* Apache Ant
* Java JDK

Libraries:
* SDL2 source
* SDL2_image source


Build instructions
------------------

Ensure <android-sdk>/tools and <android-ndk> is in your PATH.

    export PATH=/opt/android-sdk-linux/tools:/opt/android-ndk-r9:$PATH

Ensure your JAVA_HOME is set to your correct JDK location. ($JAVA_HOME/bin/javac should exist)

    ls $JAVA_HOME/bin/javac

Run 'android' and install android API 10.

    android

Symlink to SDL2 and SDL2_image source directories in android/jni

    cd <tictactoe>/android/jni
    ln -s /path/to/SDL2_src SDL2
    ln -s /path/to/SDL2_image_src SDL2_image

* Note: You should modify SDL2_image/Android.mk and set support for WebP
        to false otherwise you may run into build failure.


Build TicTacToe Android Edition

    cd <tictactoe>/android
    android update project --path .
    ndk-build
    ant debug


Testing with Android Emulator
-----------------------------

You will need to create a AVD for a device to emulate.

Open 'android' and click **Tools** > **Manage AVDs**

Settings:
<pre>
Name: nexus4
Set Device: Nexus 4
Target: API Level 10
CPU/ABI: ARM
</pre>

    emulator -avd nexus4

* Note: Press CTRL+F11 to switch to landscape mode.

Install TicTacToe in emulator:

    cd <tictactoe>/android
    ant debug install

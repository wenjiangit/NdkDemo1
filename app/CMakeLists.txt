# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.


#设置动态库位置为libs目录
set(my_lib_path ${CMAKE_SOURCE_DIR}/libs)

#讲第三方库作为动态库引用
add_library( native-lib
             SHARED
             IMPORTED )
#指名第三方库的绝对路径
set_target_properties( native-lib
                       PROPERTIES IMPORTED_LOCATION
                       ${my_lib_path}/${ANDROID_ABI}/libnative-lib.so )

add_library( # Sets the name of the library.
             libTest

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/native-lib.c )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.


#将第三方库连接到我们自己的库下
target_link_libraries( # Specifies the target library.
                       libTest
                       native-lib

                       # Links the target library to the log library
                       # included in the NDK. Android系统的log库
                       ${log-lib} )
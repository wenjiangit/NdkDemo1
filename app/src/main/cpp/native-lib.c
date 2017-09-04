#include <jni.h>
#include "native.h"
#include <android/log.h>
#include <stdlib.h>
#include <stdio.h>

#define TAG "WJ"
#define LOGI(args...) __android_log_print(ANDROID_LOG_INFO, TAG,args)


JNIEXPORT jstring JNICALL
Java_com_douliu_ndkdemo1_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject jobj) {

    return (*env)->NewStringUTF(env, "hahahhaa");
}

JNIEXPORT jint JNICALL
Java_com_douliu_ndkdemo1_MainActivity_add(JNIEnv *env, jobject instance, jint a, jint b) {

    return add(a, b);
}

int get_file_size(const char *path) {
    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        LOGI("文件打开失败");
        return 0;
    }
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fclose(fp);
    return size;
}

JNIEXPORT void JNICALL
Java_com_douliu_ndkdemo1_FileUtils_diff(JNIEnv *env, jclass type, jstring path_,
                                        jstring pattern_path_, jint num) {

    LOGI("JNI diff begin");

    const char *path = (*env)->GetStringUTFChars(env, path_, 0);
    const char *pattern_path = (*env)->GetStringUTFChars(env, pattern_path_, 0);

    char **patches = malloc(sizeof(char *) * num);
    if (patches == NULL) {
        LOGI("内存不足");
        return;
    }

    for (int i = 0; i < num; i++) {
        patches[i] = malloc(sizeof(char) * 100);
        if (patches[i] == NULL) {
            LOGI("内存不足");
            return;
        }

        //格式化子文件名
        sprintf(patches[i], pattern_path, i);
        LOGI(pattern_path, i);
    }

    //获取文件大小
    int fileSize = get_file_size(path);
    if (fileSize == 0) {
        LOGI("文件大小为0");
        return;
    }

    LOGI("取得文件大小%d", fileSize);

    //以二进制形式打开文件
    FILE *frp = fopen(path, "rb");
    if (frp == NULL) {
        LOGI("源文件打开失败");
        return;
    }

    LOGI("打开文件...");

    if (fileSize % num == 0) {
        //获取每个文件的字节数大小
        int part = fileSize / num;
        for (int i = 0; i < num; i++) {
            FILE *fwp = fopen(patches[i], "wb");//以写的方式打开二进制文件,不存在则创建
            if (fwp == NULL) {
                LOGI("子文件打开失败");
                return;
            }
            for (int j = 0; j < part; j++) {
                fputc(fgetc(frp), fwp);
            }
            fclose(fwp);
        }
    } else {
        int part = fileSize / (num - 1);
        for (int i = 0; i < num - 1; ++i) {
            FILE *fwp = fopen(patches[i], "wb");//以写的方式打开二进制文件,不存在则创建
            if (fwp == NULL) {
                LOGI("子文件打开失败");
                return;
            }
            for (int j = 0; j < part; ++j) {
                fputc(fgetc(frp), fwp);
            }
            fclose(fwp);
        }

        int remain = fileSize % (num - 1);
        FILE *fwp = fopen(patches[num - 1], "wb");//以写的方式打开二进制文件,不存在则创建
        if (fwp == NULL) {
            LOGI("子文件打开失败");
            return;
        }
        for (int j = 0; j < remain; ++j) {
            fputc(fgetc(frp), fwp);
        }
        fclose(fwp);
    }

    //关闭文件
    fclose(frp);
    //释放内存
    for (int i = 0; i < num; ++i) {
        free(patches[i]);
    }

    free(patches);

    (*env)->ReleaseStringUTFChars(env, path_, path);

    (*env)->ReleaseStringUTFChars(env, pattern_path_, pattern_path);

    LOGI("JNI diff end");
}


JNIEXPORT void JNICALL
Java_com_douliu_ndkdemo1_FileUtils_patch(JNIEnv *env, jclass type, jstring merge_path_,
                                         jstring pattern_path_, jint num) {
    LOGI("patch jni begin");

    const char *merge_path = (*env)->GetStringUTFChars(env, merge_path_, 0);
    const char *pattern_path = (*env)->GetStringUTFChars(env, pattern_path_, 0);

    char **patches = malloc(sizeof(char *) * num);
    if (patches == NULL) {
        LOGI("内存不足");
        return;
    }

    for (int i = 0; i < num; ++i) {
        patches[i] = malloc(sizeof(char) * 100);

        sprintf(patches[i], pattern_path, i);
    }

    FILE *fwp = fopen(merge_path, "wb");

    for (int j = 0; j < num; j++) {
        FILE *frp = fopen(patches[j], "rb");
        if (frp == NULL) {
            return;
        }
        int ch;
        while ((ch = fgetc(frp)) != EOF) {
            fputc(ch, fwp);
        }

        fclose(frp);
    }

    fclose(fwp);

    for (int i = 0; i < num; ++i) {
        free(patches[i]);
    }

    free(patches);

    (*env)->ReleaseStringUTFChars(env, merge_path_, merge_path);

    (*env)->ReleaseStringUTFChars(env, pattern_path_, pattern_path);

    LOGI("patch jni end");
}
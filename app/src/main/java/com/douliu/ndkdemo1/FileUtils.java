package com.douliu.ndkdemo1;

/**
 *
 *
 * Created by douliu on 2017/9/4.
 */

public class FileUtils {

    public static native void diff(String path, String pattern_path, int num);
    public static native void patch(String merge_path, String pattern_path, int num);






    static {
        System.loadLibrary("libTest");
    }

}

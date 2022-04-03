package cn.kuncb;

import java.util.regex.Pattern;

public class JinExternal {

    static {
//        String osName = System.getProperty("os.name");
//        if (Pattern.matches("Linux.*", osName)) {
//            System.loadLibrary("ossp-uuid");
//        } else if (Pattern.matches("Mac.*", osName)) {
//
//        }
        //名称要和dll或so文件名称一至
        System.loadLibrary("kmblack");

    }

    public native static void kcInit();
    public native static void kcUninit();

    public native static String kcUuid();
}

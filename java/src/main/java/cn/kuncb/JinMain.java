package cn.kuncb;

public class JinMain {
    /**
     * 保留指定的小数位数.其它直接舍去
     *
     * @param val 小数值
     * @param pow 要保留的小数位
     * @return
     */
    public static final double round(double val, int pow) {
        return ((long) (val * pow)) / ((double) pow);
    }

    public static void main(String[] args) {

        try {
            JinExternal.kcInit();
            long begintime = System.nanoTime();
            for (int i = 0; i < 1000; ++i) {
                String uuid = JinExternal.kcUuid();
                //System.out.println(uuid);
            }
            double costTime = (System.nanoTime() - begintime) / 1000000f;
            System.out.println(String.format("time:%.3fms", round(costTime, 1000)));
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }finally {
            JinExternal.kcUninit();

        }
    }
}

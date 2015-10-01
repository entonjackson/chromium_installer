package bz.jako.chromium_installer;

import android.widget.Toast;
import android.view.Gravity;

public class MyToast {
    public static void toast(final String message){
        new Thread()
        {
            public void run()
            {
                org.qtproject.qt5.android.QtNative.activity().runOnUiThread(new Runnable()
                    {
                        public void run() {
                            Toast t = Toast.makeText( org.qtproject.qt5.android.QtNative.activity(), message, Toast.LENGTH_LONG);
                            t.setGravity(Gravity.BOTTOM|Gravity.CENTER_HORIZONTAL, 0, 100);
                            t.show();
                        }
                    });
            }
        }.start();
    }
}


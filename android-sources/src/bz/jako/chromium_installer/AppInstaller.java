package bz.jako.chromium_installer;

// import org.apache.commons.lang.StringUtils;
import java.io.File;
import android.content.Intent;
import android.net.Uri;

public class AppInstaller {
    public static void installAPK(String apkPath){
        Intent intent = new Intent(Intent.ACTION_VIEW);
        intent.setDataAndType(Uri.fromFile(new File(apkPath)), "application/vnd.android.package-archive");
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        org.qtproject.qt5.android.QtNative.activity().startActivity(intent);
      }
}


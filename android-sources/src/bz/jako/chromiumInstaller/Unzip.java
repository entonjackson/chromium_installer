package bz.jako.chromiumInstaller;

import java.io.IOException;
import java.io.File;
import java.util.zip.ZipInputStream;
import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.util.zip.ZipEntry;
import java.io.FileNotFoundException;
import android.os.Environment;

public class Unzip {

    private static final int  BUFFER_SIZE = 4096;

    private static void extractFile(ZipInputStream in, File outdir, String name) throws IOException
    {
      byte[] buffer = new byte[BUFFER_SIZE];
      BufferedOutputStream out = new BufferedOutputStream(new FileOutputStream(new File(outdir,name)));
      int count = -1;
      while ((count = in.read(buffer)) != -1)
        out.write(buffer, 0, count);
      out.close();
    }

    private static void mkdirs(File outdir,String path)
    {
      File d = new File(outdir, path);
      if( !d.exists() )
        d.mkdirs();
    }

    private static String dirpart(String name)
    {
      int s = name.lastIndexOf( File.separatorChar );
      return s == -1 ? null : name.substring( 0, s );
    }

    public static void extract(String zipFileName, String extractDir)
    {
        File zipfile = new File(Environment.getExternalStorageDirectory() + "/" + zipFileName);
        File outdir = new File(Environment.getExternalStorageDirectory() + "/" + extractDir);

      try
      {
        ZipInputStream zin = new ZipInputStream(new FileInputStream(zipfile));
        ZipEntry entry;
        String name, dir;
        while ((entry = zin.getNextEntry()) != null)
        {
          name = entry.getName();
          if( entry.isDirectory() )
          {
            mkdirs(outdir,name);
            continue;
          }

          dir = dirpart(name);
          if( dir != null )
            mkdirs(outdir,dir);

          extractFile(zin, outdir, name);
        }
        zin.close();
      }
      catch (IOException e)
      {
        e.printStackTrace();
      }
    }
}


package kodowanie_8;

import java.io.File;
import java.io.IOException;

/**
 *
 * @author kgb
 */
public class Kodowanie_8 {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) throws IOException {
        File file = new File("/Users/kgb/mg.tga");
        File file2 = new File("/Users/kgb/mg332.tga"); 
        File file3 = new File("/Users/kgb/test.tga"); 
        new Kwantyzacja().obraz(file, file3, 3, 3, 2);   
    }
}

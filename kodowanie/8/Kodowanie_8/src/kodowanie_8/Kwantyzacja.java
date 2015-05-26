package kodowanie_8;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import static java.lang.Math.log10;

/**
 *
 * @author kgb
 */
public class Kwantyzacja {
    private int height,width,predykcja;
    private byte[] naglowek,stopka;
    private Color[][] bitmapa;
    private OutputStream output;
    
    public void obraz(File in,File out,int r,int g,int b) throws FileNotFoundException, IOException{
        output = new BufferedOutputStream(new FileOutputStream(out));
        double blad=0;
        int j=0;
        int sum_sq = 0;
        double mse=0.0;
        byte by[]=b_to_arr(in);
        for(int i=0;i<by.length;i++){           
            if((i>=18)&&(i<by.length-26)){
                int r_v = Byte.toUnsignedInt(by[i]);            
                int g_v = Byte.toUnsignedInt(by[i+1]);
                int b_v = Byte.toUnsignedInt(by[i+2]);
                output.write((int)(r_v/(256/Math.pow(2, r)))*(int)(256/Math.pow(2, r))+(int)((256/Math.pow(2, r))/2));
                output.write((int)(g_v/(256/Math.pow(2, g)))*(int)(256/Math.pow(2, g))+(int)((256/Math.pow(2, g))/2));
                output.write((int)(b_v/(256/Math.pow(2, b)))*(int)(256/Math.pow(2, b))+(int)((256/Math.pow(2, b))/2));
                i+=2;
            }
            else{
                output.write(by[i]);
            }
        }
        output.close();
        byte[]b2 = b_to_arr(out);
        for(int i=0;i<4;i++)
            mse(by,b2,i);
    }
  
    private void mse(byte[]b1,byte[]b2,int x) throws IOException{
        double  peak, signal, noise, mse;
        String color;
        signal = noise = peak = 0;
        for(int i=0;i<b1.length;i++){ 
            if((i>=18)&&(i<b1.length-26)){
                int r1 = Byte.toUnsignedInt(b1[i+x]);            
                int r2 = Byte.toUnsignedInt(b2[i+x]);  
                signal+=r1*r2;
                noise += (r1-r2)*(r1-r2);
                if(peak<r1){
                    peak=r1;
                }
                if(x!=3)
                    i+=2;
            }
        }
        if(x==0)
            color ="r";
        else if(x==1)
            color ="g";
        else
            color="b";
        if(x!=3){
            mse = noise/((b1.length-44)/3); // Mean square error
            System.out.println("MSE("+color+")= " + mse);
            System.out.println("SNR("+color+")= " +(signal/noise)+ " ("+10*log10(signal/noise)+"dB)");
        }
        else{
            mse = noise/((b1.length-44));
            System.out.println("MSE = " + mse);
            System.out.println("SNR = " +(signal/noise)+ "("+10*log10(signal/noise)+"dB)");
        }
      
    }
  
    private  byte[] b_to_arr(File file) throws FileNotFoundException, IOException{
        byte [] arr = new byte[(int)file.length()];
        try(FileInputStream fis = new FileInputStream(file)){
            fis.read(arr);
            fis.close();
        }
        return arr;
    }
    
}

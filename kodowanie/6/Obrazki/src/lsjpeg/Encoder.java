/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package lsjpeg;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import static lsjpeg.LSJPEG.input;

public class Encoder {
    FileInputStream in; 
    FileOutputStream out;
    private short[] headline = new short[18];
    private short[] footer = new short[26];
    private int width;
    private int height;
    private ArrayList<Short> rest = new ArrayList<Short>();
    
	
	public Encoder(FileInputStream in, FileOutputStream out){
		this.in = in;
		this.out = out;		
	}
        
        public void compress() throws FileNotFoundException, IOException
        {
            int quantityOfChars = 0;
            FileInputStream fis = new FileInputStream(input);
		short readByte;
		short lastByte = 0;
		while((readByte = (short) fis.read()) != -1){
                        if(quantityOfChars < 18)
                            headline[quantityOfChars] = readByte;    
                        else if(quantityOfChars >= 18)
                            rest.add(readByte);
			quantityOfChars++;		
            lastByte = readByte;
            }
		fis.close();
                //display_footer();
                for(int i = 0; i < 26; i++)
                {
                    footer[i] = rest.get(rest.size()-1);
                    rest.remove(rest.size()-1);
                }
                //display_footer();
                //display_headline();
                //display_rest();
                if(headline[12] != 0 && headline[14] != 0){
                    height = headline[12];
                    width = headline[14];
                }
                else{
                    height = 256;
                    width = 256;
                }
                //System.out.println("height " + height);
                //System.out.println("width " + width);
                pixel[][] map = new pixel[height][width];
                
                int checker = 0;
                int checker1 = 0;
                int red;
                int green;
                int blue;
                
                //ułożenie pixeli w ramke tak jak na ekranie
                int pixCounter = 0;
                for(int i = 0; i < rest.size(); i=i+3)
                {
                    red  = rest.get(i);
                    green = rest.get(i+1);
                    blue = rest.get(i+2);
                    pixel pix = new pixel(red, green, blue);
                    pixCounter++;
                    map[checker1][checker] = pix;
                    checker++;
                    //System.out.println("checker " + checker + " checker1 " + checker1);
                    if(checker == width)
                    {
                        checker = 0;
                        checker1++;
                    }
                    //System.out.println("PixCounter " + pixCounter);
                }
                System.out.println("0 prediction"); 
                writeToFile(map);          
                countEntropy("/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/out");
                writeRed(map);
                System.out.print("Red entropy " );
                countEntropy("/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/out");
                writeGreen(map);
                System.out.print("Green entropy " );
                countEntropy("/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/out");
                writeBlue(map);
                System.out.print("Blue entropy " );
                countEntropy("/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/out");
                System.out.println();
                
                
                //tablice potrzebne do 8 przypadków
                pixel[][] temp1 = new pixel[height][width];
                pixel[][] temp2 = new pixel[height][width];
                pixel[][] temp3 = new pixel[height][width];
                pixel[][] temp4 = new pixel[height][width];
                pixel[][] temp5 = new pixel[height][width];
                pixel[][] temp6 = new pixel[height][width];
                pixel[][] temp7 = new pixel[height][width];
                pixel[][] temp8 = new pixel[height][width];
                pixel[][] temp9 = new pixel[height][width];
                

//                int counter = 0;
//                for(int i = 0; i < height; i++)
//                {
//                    for(int j = 0; j < width; j++)
//                    {
//                        red = map[i][j].getRed();
//                        green = map[i][j].getGreen();
//                        blue = map[i][j].getBlue();
//                        System.out.println("Red " + red + " Green " + green + " Blue " + blue);
//                        System.out.println("Num of pix " + counter);
//                        counter++;
//                    }
//                }
                
                //pixel defaultowy- murzyni w koło
                pixel def = new pixel(0,0,0);
                
                //przepisanie tablicy do nowych
                for(int i = 0; i < height; i++)
                    for(int j = 0; j < width; j++)
                    {
                        temp1[i][j] = map[i][j];
                        temp2[i][j] = map[i][j];
                        temp3[i][j] = map[i][j];
                        temp4[i][j] = map[i][j];
                        temp5[i][j] = map[i][j];
                        temp6[i][j] = map[i][j];
                        temp7[i][j] = map[i][j];
                        temp8[i][j] = map[i][j];
                    }
                
                              
                //prediction 1a - biore tego z lewej
                int red1;
                int green1;
                int blue1;
                for(int i = 0; i < height; i++)
                    for(int j = 1; j < width; j++)
                    {
                        red = map[i][j].getRed();
                        green = map[i][j].getGreen();
                        blue = map[i][j].getBlue();
                        red1 = map[i][j-1].getRed();
                        green1 = map[i][j-1].getGreen(); 
                        blue1 = map[i][j-1].getBlue();
                        pixel pix = new pixel(red-red1, green-green1, blue-blue1);
                        temp1[i][j] = pix;
                    }
                System.out.println("1 prediction"); 
                writeToFile(temp1);          
                countEntropy("/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/out");
                writeRed(temp1);
                System.out.print("Red entropy " );
                countEntropy("/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/out");
                writeGreen(temp1);
                System.out.print("Green entropy " );
                countEntropy("/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/out");
                writeBlue(temp1);
                System.out.print("Blue entropy " );
                countEntropy("/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/out");
                System.out.println();
                
                //prediction 2b - biore tego z góry
                for(int i = 1; i < height; i++)
                    for(int j = 0; j < width; j++)
                    {
                        red = map[i][j].getRed();
                        green = map[i][j].getGreen();
                        blue = map[i][j].getBlue();
                        red1 = map[i-1][j].getRed();
                        green1 = map[i-1][j].getGreen(); 
                        blue1 = map[i-1][j].getBlue();
                        pixel pix = new pixel(red - red1, green - green1, blue - blue1);
                        temp2[i][j] = pix;
                    }
                System.out.println("2 prediction"); 
                writeToFile(temp2);          
                countEntropy("/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/out");
                writeRed(temp2);
                System.out.print("Red entropy " );
                countEntropy("/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/out");
                writeGreen(temp2);
                System.out.print("Green entropy " );
                countEntropy("/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/out");
                writeBlue(temp2);
                System.out.print("Blue entropy " );
                countEntropy("/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/out");
                System.out.println();
                
                //prediction 3c - biore tego po skosie  z lewej
                for(int i = 1; i < height; i++)
                    for(int j = 1; j < width; j++)
                    {
                        red = map[i][j].getRed();
                        green = map[i][j].getGreen();
                        blue = map[i][j].getBlue();
                        red1 = map[i][j-1].getRed();
                        green1 = map[i][j-1].getGreen(); 
                        blue1 = map[i][j-1].getBlue();
                        pixel pix = new pixel(red - red1, green - green1, blue - blue1);
                        temp3[i][j] = pix;
                    }
                System.out.println("3 prediction"); 
                writeToFile(temp3);          
                countEntropy("/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/out");
                writeRed(temp3);
                System.out.print("Red entropy " );
                countEntropy("/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/out");
                writeGreen(temp3);
                System.out.print("Green entropy " );
                countEntropy("/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/out");
                writeBlue(temp3);
                System.out.print("Blue entropy " );
                countEntropy("/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/out");
                System.out.println();
                
                //prediction 4 a+b-c - biore z lewej + góra - skos
                prediction4(temp4, map);
                
                
                //prediction 5 a+(b-c)/2
                prediction5(temp5, map);
                
                //prediction 6 b+(a-c)/2
                prediction6(temp6, map);
                
                //prediction 7 (a+b)/2
                prediction7(temp7, map);
                
                //prediction8 wtf???

                
                System.out.println("pix " + height*width + " arr siz " + rest.size()/3);
                //display_map(map);
        }
        
        private void prediction6(pixel temp6[][], pixel [][]map) throws IOException{
            //najpierw te z góry czyli z pierwszej lini
            for(int i = 1; i < width; i++)
            {
                int red = map[0][i].getRed();
                int green = map[0][i].getGreen();
                int blue = map[0][i].getBlue();
                int red1 = map[0][i-1].getRed();
                int green1 = map[0][i-1].getGreen(); 
                int blue1 = map[0][i-1].getBlue();
                pixel pix = new pixel(red-red1, green-green1, blue-blue1);
                temp6[0][i] = pix;
            }
            
            //teraz te z lewej strony pierwsza kolumna od lewej
            for(int i = 1; i < height; i++)
            {
                int red = map[i][0].getRed();
                int green = map[i][0].getGreen();
                int blue = map[i][0].getBlue();
                int red1 = map[i-1][0].getRed();
                int green1 = map[i-1][0].getGreen(); 
                int blue1 = map[i-1][0].getBlue();
                pixel pix = new pixel(red-red1, green-green1, blue-blue1);
                temp6[0][i] = pix;
            }
            
            //cala reszta
            for(int i = 1; i < height; i++)
                for(int j = 1; j < width; j++)
                {
                    int red = map[i][j].getRed();
                    int green = map[i][j].getGreen();
                    int blue = map[i][j].getBlue();
                    
                    //te z lewej
                    int redA = map[i][j-1].getRed();
                    int greenA = map[i][j-1].getGreen();
                    int blueA = map[i][j-1].getBlue();
                    
                    //te z gory
                    int redB = map[i-1][j].getRed();
                    int greenB = map[i-1][j].getGreen();
                    int blueB = map[i-1][j].getBlue();
                    
                    //te z ukosa
                    int redC = map[i-1][j-1].getRed();
                    int greenC = map[i-1][j-1].getGreen();
                    int blueC = map[i-1][j-1].getBlue();
                    
                    pixel pix = new pixel(red-redB+(redA-redC)/2, green-greenB+(greenA-greenC)/2, blue-blueB+(blueA-blueC)/2);
                    temp6[i][j] = pix;
                }
            System.out.println("6 prediction"); 
            writeToFile(temp6);          
            countEntropy("/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/out");
            writeRed(temp6);
            System.out.print("Red entropy " );
            countEntropy("/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/out");
            writeGreen(temp6);
            System.out.print("Green entropy " );
            countEntropy("/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/out");
            writeBlue(temp6);
            System.out.print("Blue entropy " );
            countEntropy("/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/out");
            System.out.println();
        }
        
        private void prediction5(pixel temp5[][], pixel[][] map) throws IOException
        {
            //najpierw te z góry czyli z pierwszej lini
            for(int i = 1; i < width; i++)
            {
                int red = map[0][i].getRed();
                int green = map[0][i].getGreen();
                int blue = map[0][i].getBlue();
                int red1 = map[0][i-1].getRed();
                int green1 = map[0][i-1].getGreen(); 
                int blue1 = map[0][i-1].getBlue();
                pixel pix = new pixel(red-red1, green-green1, blue-blue1);
                temp5[0][i] = pix;
            }
            
            //teraz te z lewej strony pierwsza kolumna od lewej
            for(int i = 1; i < height; i++)
            {
                int red = map[i][0].getRed();
                int green = map[i][0].getGreen();
                int blue = map[i][0].getBlue();
                int red1 = map[i-1][0].getRed();
                int green1 = map[i-1][0].getGreen(); 
                int blue1 = map[i-1][0].getBlue();
                pixel pix = new pixel(red-red1, green-green1, blue-blue1);
                temp5[0][i] = pix;
            }
            
            //cala reszta
            for(int i = 1; i < height; i++)
                for(int j = 1; j < width; j++)
                {
                    int red = map[i][j].getRed();
                    int green = map[i][j].getGreen();
                    int blue = map[i][j].getBlue();
                    
                    //te z lewej
                    int redA = map[i][j-1].getRed();
                    int greenA = map[i][j-1].getGreen();
                    int blueA = map[i][j-1].getBlue();
                    
                    //te z gory
                    int redB = map[i-1][j].getRed();
                    int greenB = map[i-1][j].getGreen();
                    int blueB = map[i-1][j].getBlue();
                    
                    //te z ukosa
                    int redC = map[i-1][j-1].getRed();
                    int greenC = map[i-1][j-1].getGreen();
                    int blueC = map[i-1][j-1].getBlue();
                    
                    pixel pix = new pixel(red-redA+(redB-redC)/2, green-greenA+(greenB-greenC)/2, blue-blueA+(blueB-blueC)/2);
                    temp5[i][j] = pix;
                }
            System.out.println("5 prediction"); 
            writeToFile(temp5);          
            countEntropy("/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/out");
            writeRed(temp5);
            System.out.print("Red entropy " );
            countEntropy("/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/out");
            writeGreen(temp5);
            System.out.print("Green entropy " );
            countEntropy("/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/out");
            writeBlue(temp5);
            System.out.print("Blue entropy " );
            countEntropy("/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/out");
            System.out.println();
        }
        
        private void prediction7(pixel temp7[][], pixel[][] map) throws IOException{
            //najpierw te z góry czyli z pierwszej lini
            for(int i = 1; i < width; i++)
            {
                int red = map[0][i].getRed();
                int green = map[0][i].getGreen();
                int blue = map[0][i].getBlue();
                int red1 = map[0][i-1].getRed();
                int green1 = map[0][i-1].getGreen(); 
                int blue1 = map[0][i-1].getBlue();
                pixel pix = new pixel(red-red1, green-green1, blue-blue1);
                temp7[0][i] = pix;
            }
            
            //teraz te z lewej strony pierwsza kolumna od lewej
            for(int i = 1; i < height; i++)
            {
                int red = map[i][0].getRed();
                int green = map[i][0].getGreen();
                int blue = map[i][0].getBlue();
                int red1 = map[i-1][0].getRed();
                int green1 = map[i-1][0].getGreen(); 
                int blue1 = map[i-1][0].getBlue();
                pixel pix = new pixel(red-red1, green-green1, blue-blue1);
                temp7[0][i] = pix;
            }
            
            //cala reszta
            for(int i = 1; i < height; i++)
                for(int j = 1; j < width; j++)
                {
                    int red = map[i][j].getRed();
                    int green = map[i][j].getGreen();
                    int blue = map[i][j].getBlue();
                    
                    //te z lewej
                    int redA = map[i][j-1].getRed();
                    int greenA = map[i][j-1].getGreen();
                    int blueA = map[i][j-1].getBlue();
                    
                    //te z gory
                    int redB = map[i-1][j].getRed();
                    int greenB = map[i-1][j].getGreen();
                    int blueB = map[i-1][j].getBlue();
                    
                    pixel pix = new pixel(red-(redA+redB)/2, green-(greenA+greenB)/2, blue-(blueA+blueB)/2);
                    temp7[i][j] = pix;
                }
            System.out.println("7 prediction"); 
            writeToFile(temp7);          
            countEntropy("/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/out");
            writeRed(temp7);
            System.out.print("Red entropy " );
            countEntropy("/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/out");
            writeGreen(temp7);
            System.out.print("Green entropy " );
            countEntropy("/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/out");
            writeBlue(temp7);
            System.out.print("Blue entropy " );
            countEntropy("/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/out");
            System.out.println();
        }
        
        private void prediction4(pixel temp4[][], pixel [][] map) throws IOException{
            //najpierw te z góry czyli z pierwszej lini
            for(int i = 1; i < width; i++)
            {
                int red = map[0][i].getRed();
                int green = map[0][i].getGreen();
                int blue = map[0][i].getBlue();
                int red1 = map[0][i-1].getRed();
                int green1 = map[0][i-1].getGreen(); 
                int blue1 = map[0][i-1].getBlue();
                pixel pix = new pixel(red-red1, green-green1, blue-blue1);
                temp4[0][i] = pix;
            }
            //teraz te z lewej strony pierwsza kolumna od lewej
            for(int i = 1; i < height; i++)
            {
                int red = map[i][0].getRed();
                int green = map[i][0].getGreen();
                int blue = map[i][0].getBlue();
                int red1 = map[i-1][0].getRed();
                int green1 = map[i-1][0].getGreen(); 
                int blue1 = map[i-1][0].getBlue();
                pixel pix = new pixel(red-red1, green-green1, blue-blue1);
                temp4[0][i] = pix;
            }
            //cala reszta
            for(int i = 1; i < height; i++)
                for(int j = 1; j < width; j++)
                {
                    int red = map[i][j].getRed();
                    int green = map[i][j].getGreen();
                    int blue = map[i][j].getBlue();
                    
                    //te z lewej
                    int redA = map[i][j-1].getRed();
                    int greenA = map[i][j-1].getGreen();
                    int blueA = map[i][j-1].getBlue();
                    
                    //te z gory
                    int redB = map[i-1][j].getRed();
                    int greenB = map[i-1][j].getGreen();
                    int blueB = map[i-1][j].getBlue();
                    
                    //te z ukosa
                    int redC = map[i-1][j-1].getRed();
                    int greenC = map[i-1][j-1].getGreen();
                    int blueC = map[i-1][j-1].getBlue();
                    
                    pixel pix = new pixel(red-redA+redB-redC, green-greenA+greenB-greenC, blue-blueA+blueB-blueC);
                    temp4[i][j] = pix;
                }
            System.out.println("4 prediction"); 
            writeToFile(temp4);          
            countEntropy("/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/out");
            writeRed(temp4);
            System.out.print("Red entropy " );
            countEntropy("/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/out");
            writeGreen(temp4);
            System.out.print("Green entropy " );
            countEntropy("/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/out");
            writeBlue(temp4);
            System.out.print("Blue entropy " );
            countEntropy("/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/out");
            System.out.println();
        }
        
        private void writeRed(pixel[][] temp) throws FileNotFoundException, IOException
        {
            FileOutputStream fos = new FileOutputStream(new File("/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/out"));
            try{
                for(int i = 0; i < height; i++)
                    for(int j = 0; j < width; j=j+3)
                    {
                        fos.write(temp[i][j].getRed());
                        fos.write(temp[i][j].getGreen());
                        fos.write(temp[i][j].getBlue());
                    }
            } catch (Exception e1) {
			e1.printStackTrace();
		}finally{
                fos.close();
            }
        }
        
        private void writeGreen(pixel[][] temp) throws FileNotFoundException, IOException
        {
            FileOutputStream fos = new FileOutputStream(new File("/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/out"));
            try{
                for(int i = 0; i < height; i++)
                    for(int j = 1; j < width; j=j+3)
                    {
                        fos.write(temp[i][j].getRed());
                        fos.write(temp[i][j].getGreen());
                        fos.write(temp[i][j].getBlue());
                    }
            } catch (Exception e1) {
			e1.printStackTrace();
		}finally{
                fos.close();
            }
        }
        
        private void writeBlue(pixel[][] temp) throws FileNotFoundException, IOException
        {
            FileOutputStream fos = new FileOutputStream(new File("/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/out"));
            try{
                for(int i = 0; i < height; i++)
                    for(int j = 2; j < width; j=j+3)
                    {
                        fos.write(temp[i][j].getRed());
                        fos.write(temp[i][j].getGreen());
                        fos.write(temp[i][j].getBlue());
                    }
            } catch (Exception e1) {
			e1.printStackTrace();
		}finally{
                fos.close();
            }
        }
        
        
        private void writeToFile(pixel[][] temp) throws FileNotFoundException, IOException{
            FileOutputStream fos = new FileOutputStream(new File("/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/out"));
            try{
                for(int i = 0; i < height; i++)
                    for(int j = 0; j < width; j++)
                    {
                        fos.write(temp[i][j].getRed());
                        fos.write(temp[i][j].getGreen());
                        fos.write(temp[i][j].getBlue());
                    }
            } catch (Exception e1) {
			e1.printStackTrace();
		}finally{
                fos.close();
            }
            
            
        }
        
        private void display_map(int[][] map)
        {
            for(int i = 0; i < height; i++)
                for(int j = 0; j < width; j++)
                    System.out.println(map[i][j]);
        }
        
        private void display_headline()
        {
            for(int i = 0; i < 18; i++)
                System.out.println(headline[i]);
        }
        private void display_footer()
        {
            for(int i = 0; i < 26; i++)
                System.out.println(footer[i]);
        }
        private void display_rest()
        {
            for(int i = 0; i < rest.size(); i++)
                System.out.println(rest.get(i));
        }
        
        public static double log2(double n)
	{
	    return (Math.log(n) / Math.log(2.0));
	}

private static void countEntropy(String input) throws FileNotFoundException, IOException {
        Map<Short,Integer> countByte = new HashMap<Short,Integer>();
		Map<Short,Map<Short,Integer>> countByteAfterByte = new HashMap<Short,Map<Short,Integer>>();
		int[] count = new int[256];
		int quantityOfChars = 0;
		double entropia=0;
		double entropiaWar=0;
		for(int i=0;i<256;i++){
			count[i] = 0;
			countByte.put((short)i, 0);
			Map<Short,Integer> branch = new HashMap<Short,Integer>();
			
			for(int j=0;j<256;j++){
				branch.put((short)j, 0);
			}
			countByteAfterByte.put( (short)i, branch);
		}

// Otherwise, compress
File inputFile = new File(input);

FileInputStream fis = new FileInputStream(input);
		short readByte;
		short lastByte = 0;
		while((readByte = (short) fis.read()) != -1){
			quantityOfChars++;
			countByte.put(readByte,countByte.get(readByte)+1);
			count[readByte] = count[readByte]+1;
            Map<Short, Integer> branch = countByteAfterByte.get(lastByte);
            branch.put(readByte, branch.get(readByte) + 1);
            countByteAfterByte.put(lastByte, branch);			
			
            lastByte = readByte;
		}
		fis.close();
                
                for(short i=255;i>=0;i--){
			if(countByte.get(i)==0){
				countByte.remove(i);
			}
			Map<Short,Integer> branch = countByteAfterByte.get(i);
			
			for(short j=255;j>=0;j--){
				if(branch.get((short)j)== 0){
					branch.remove(j);
				}
			}
			countByteAfterByte.put( (short)i, branch);
		}
		for (Integer quantity : countByte.values()) {
			entropia += (((double)quantity/(double)quantityOfChars)*(-log2((double)quantity/(double)quantityOfChars )));
		}
		
		for (Short key : countByte.keySet()) {
			double H = 0;
			for (Integer quantity : countByteAfterByte.get(key).values()) {
				H += (((double)quantity/(double)countByte.get(key))*(-log2((double)quantity/(double)countByte.get(key))));
			}
			int quantity = countByte.get(key);
			entropiaWar += (((double)quantity/(double)quantityOfChars)*(H));
		}
		//System.out.println(countByte);
		System.out.println( entropia);
		//System.out.println("First conditional entropy = " + entropiaWar);
    }

}
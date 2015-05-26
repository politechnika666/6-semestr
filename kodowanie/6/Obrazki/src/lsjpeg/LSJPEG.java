
package lsjpeg;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;


public class LSJPEG {

    public static String input = "/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/example0.tga"; 
    public static String output = "/home/afro/Dokumenty/repo/6-semestr/kodowanie/6/OUTPUT";

    public static void main(String[] args) throws IOException {
        FileInputStream fis;
        FileOutputStream fos;
        
        try {
        	System.out.println("Coding");
			fis = new FileInputStream(new File(input));
			fos = new FileOutputStream(new File(output));
			
			Encoder e = new Encoder(fis, fos);
			e.compress();
			fis.close();
			fos.close();			

		} catch (Exception e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
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
		System.out.println(countByte);
		System.out.println("First entropy: " + entropia);
		System.out.println("First conditional entropy = " + entropiaWar);
    }
    
}
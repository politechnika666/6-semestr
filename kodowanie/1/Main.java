import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

public class Main {

	public static void main(String[] args) throws IOException {
		Map<Short,Integer> countByte = new HashMap<Short,Integer>();
		Map<Short,Map<Short,Integer>> countByteAfterByte = new HashMap<Short,Map<Short,Integer>>();
		int quantityOfChars = 0;
		double entropia=0;
		double entropiaWar=0;
		for(int i=0;i<256;i++){
			countByte.put((short)i, 0);
			Map<Short,Integer> branch = new HashMap<Short,Integer>();
			
			for(int j=0;j<256;j++){
				branch.put((short)j, 0);
			}
			countByteAfterByte.put( (short)i, branch);
		}
		
		
		File file = new File(args[0]);
		FileInputStream fis = new FileInputStream(file);
		short readByte;
		short lastByte = 0;
		while((readByte = (short) fis.read()) != -1){
			quantityOfChars++;
			countByte.put(readByte,countByte.get(readByte)+1);
			
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
		double roznica = entropia-entropiaWar;
		System.out.println();
		System.out.println("Entropia: " + entropia);
		System.out.println("Entropia warunkowa = " + entropiaWar);
		System.out.println("Entropia - entropia warunkowa = " + roznica);
		System.out.println();
	}
	public static double log2(double n)
	{
	    return (Math.log(n) / Math.log(2.0));
	}
}
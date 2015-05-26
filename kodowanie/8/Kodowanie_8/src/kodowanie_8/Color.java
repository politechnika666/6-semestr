package kodowanie_8;
/**
 *
 * @author kgb
 */
public class Color {
    private final int r,g,b;
    public Color (int r,int g,int b) {
        this.b = b;
	this.g = g;
	this.r = r;
    }
    @Override
    public String toString() {return r+":"+g+":"+b;}
    
    public int getBlue(){return b;}
    public int getGreen(){return g;}
    public int getRed() {return r;}

}

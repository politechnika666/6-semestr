package lsjpeg;

class pixel {
    private int red;
    private int green;
    private int blue;
    
    public pixel(int red, int green, int blue)
    {
        this.red = red;
        this.green = green;
        this.blue = blue;
    }
    
    public void displayPixel(pixel Pix)
    {
        int red = Pix.getRed();
        int green = Pix.getGreen();
        int blue = Pix.getBlue();
        System.out.println("Red " + red + " Green " + green + " Blue " + blue);
    }
    
    
    public int getRed()
    {
        return red;
    }
    public int getGreen()
    {
        return green;
    }
    public int getBlue()
    {
        return blue;
    }
}
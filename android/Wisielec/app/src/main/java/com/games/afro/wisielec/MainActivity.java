package com.games.afro.wisielec;

import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.content.*;
import android.widget.TextView;
import android.widget.Toast;

import java.util.Queue;
import java.util.Random;


public class MainActivity extends ActionBarActivity {


    private ImageView gallows;

    private Button[] buttons = new Button[35];
    private String[] words;
    private String word;
    private TextView catchWord;
    private String hiddenWord = "";
    private short gallowsNum = 0;
    private int length = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        gallows = (ImageView)findViewById(R.id.imageView);

        buttons[0] = (Button)findViewById(R.id.Abutton);
        buttons[1] = (Button)findViewById(R.id.A1button);
        buttons[2] = (Button)findViewById(R.id.Bbutton);
        buttons[3] = (Button)findViewById(R.id.Cbutton);
        buttons[4] = (Button)findViewById(R.id.C1button);
        buttons[5] = (Button)findViewById(R.id.Dbutton);
        buttons[6] = (Button)findViewById(R.id.Ebutton);
        buttons[7] = (Button)findViewById(R.id.E1button);
        buttons[8] = (Button)findViewById(R.id.Fbutton);
        buttons[9] = (Button)findViewById(R.id.Gbutton);
        buttons[10] = (Button)findViewById(R.id.Hbutton);
        buttons[11] = (Button)findViewById(R.id.Ibutton);
        buttons[12] = (Button)findViewById(R.id.Jbutton);
        buttons[13] = (Button)findViewById(R.id.Kbutton);
        buttons[14] = (Button)findViewById(R.id.Lbutton);
        buttons[15] = (Button)findViewById(R.id.L1button);
        buttons[16] = (Button)findViewById(R.id.Mbutton);
        buttons[17] = (Button)findViewById(R.id.Nbutton);
        buttons[18] = (Button)findViewById(R.id.N1button);
        buttons[19] = (Button)findViewById(R.id.Obutton);
        buttons[20] = (Button)findViewById(R.id.O1button);
        buttons[21] = (Button)findViewById(R.id.Pbutton);
        buttons[22] = (Button)findViewById(R.id.Qbutton);
        buttons[23] = (Button)findViewById(R.id.Rbutton);
        buttons[24] = (Button)findViewById(R.id.Sbutton);
        buttons[25] = (Button)findViewById(R.id.S1button);
        buttons[26] = (Button)findViewById(R.id.Tbutton);
        buttons[27] = (Button)findViewById(R.id.Ubutton);
        buttons[28] = (Button)findViewById(R.id.Vbutton);
        buttons[29] = (Button)findViewById(R.id.Wbutton);
        buttons[30] = (Button)findViewById(R.id.Xbutton);
        buttons[31] = (Button)findViewById(R.id.Ybutton);
        buttons[32] = (Button)findViewById(R.id.Zbutton);
        buttons[33] = (Button)findViewById(R.id.Z1button);
        buttons[34] = (Button)findViewById(R.id.Z2button);

        words = getResources().getStringArray(R.array.words);
        catchWord = (TextView)findViewById(R.id.word);

        Random rand = new Random();
        int question = rand.nextInt(7);
        word = "";
        length = words[question].length();
        char temp;

        for(int i = 0; i<words[question].length(); i++) {
            if(i < words[question].length()-1) {
                temp = words[question].charAt(i);
                word += String.valueOf(temp);
                word += " ";
            }
            else {
                temp = words[question].charAt(i);
                word += String.valueOf(temp);
            }
        }
        length = word.length();
        for(int i = 0; i<words[question].length(); i++) {
            if(i < word.length()-1) {
                hiddenWord += "_";
                hiddenWord += " ";
            }
            else
                hiddenWord += "_";
        }

        catchWord.setText(hiddenWord);

        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    public void checkAndWrite(char letter) {
        StringBuilder temp = new StringBuilder(hiddenWord);
        int count = 0;
        for(int i = 0; i<length; i++) {
            if(word.charAt(i) == letter) {
                count++;
                temp.setCharAt(i, letter);
                hiddenWord = temp.toString();
                catchWord.setText(hiddenWord);
            }
        }
        int win = 0;
        for(int i = 0; i<length; i++) {
            if(hiddenWord.charAt(i) == word.charAt(i))
                win++;

        }
        if(win == word.length()) {
            Toast.makeText(this, "You won!", Toast.LENGTH_LONG).show();
            disableAll();
        }
        if(count == 0) {
             switch(gallowsNum) {
                 case 0:
                     gallowsNum++;
                     gallows.setImageResource(R.drawable.hangman1);
                     break;
                 case 1:
                     gallowsNum++;
                     gallows.setImageResource(R.drawable.hangman2);
                     break;
                 case 2:
                     gallowsNum++;
                     gallows.setImageResource(R.drawable.hangman3);
                     break;
                 case 3:
                     gallowsNum++;
                     gallows.setImageResource(R.drawable.hangman4);
                     break;
                 case 4:
                     gallowsNum++;
                     gallows.setImageResource(R.drawable.hangman0);
                     catchWord.setText(word);
                     Toast.makeText(this, "You loose!", Toast.LENGTH_LONG).show();
                     disableAll();
                     break;
             }
        }
    }

    public void disableAll() {
        for(int i = 0; i<35; i++)
            buttons[i].setEnabled(false);
    }


    public void clickA(View view) {
        checkAndWrite('a');
        buttons[0].setEnabled(false);
    }
    public void clickA1(View view) {
        checkAndWrite('ą');
        buttons[1].setEnabled(false);
    }
    public void clickB(View view) {
        checkAndWrite('b');
        buttons[2].setEnabled(false);
    }
    public void clickC(View view) {
        checkAndWrite('c');
        buttons[3].setEnabled(false);
    }
    public void clickC1(View view) {
        checkAndWrite('ć');
        buttons[4].setEnabled(false);
    }
    public void clickD(View view) {
        checkAndWrite('d');
        buttons[5].setEnabled(false);
    }
    public void clickE(View view) {
        checkAndWrite('e');
        buttons[6].setEnabled(false);
    }
    public void clickE1(View view) {
        checkAndWrite('ę');
        buttons[7].setEnabled(false);
    }
    public void clickF(View view) {
        checkAndWrite('f');
        buttons[8].setEnabled(false);
    }
    public void clickG(View view) {
        checkAndWrite('g');
        buttons[9].setEnabled(false);
    }
    public void clickH(View view) {
        checkAndWrite('h');
        buttons[10].setEnabled(false);
    }
    public void clickI(View view) {
        checkAndWrite('i');
        buttons[11].setEnabled(false);
    }
    public void clickJ(View view) {
        checkAndWrite('j');
        buttons[12].setEnabled(false);
    }
    public void clickK(View view) {
        checkAndWrite('k');
        buttons[13].setEnabled(false);
    }
    public void clickL(View view) {
        checkAndWrite('l');
        buttons[14].setEnabled(false);
    }
    public void clickL1(View view) {
        checkAndWrite('ł');
        buttons[15].setEnabled(false);
    }
    public void clickM(View view) {
        checkAndWrite('m');
        buttons[16].setEnabled(false);
    }
    public void clickN(View view) {
        checkAndWrite('n');
        buttons[17].setEnabled(false);
    }
    public void clickN1(View view) {
        checkAndWrite('ń');
        buttons[18].setEnabled(false);
    }
    public void clickO(View view) {
        checkAndWrite('o');
        buttons[19].setEnabled(false);
    }
    public void clickO1(View view) {
        checkAndWrite('ó');
        buttons[20].setEnabled(false);
    }
    public void clickP(View view) {
        checkAndWrite('p');
        buttons[21].setEnabled(false);
    }
    public void clickQ(View view) {
        checkAndWrite('q');
        buttons[22].setEnabled(false);
    }
    public void clickR(View view) {
        checkAndWrite('r');
        buttons[23].setEnabled(false);
    }
    public void clickS(View view) {
        checkAndWrite('s');
        buttons[24].setEnabled(false);
    }
    public void clickS1(View view) {
        checkAndWrite('ś');
        buttons[25].setEnabled(false);
    }
    public void clickT(View view) {
        checkAndWrite('t');
        buttons[26].setEnabled(false);
    }
    public void clickU(View view) {
        checkAndWrite('u');
        buttons[27].setEnabled(false);
    }
    public void clickV(View view) {
        checkAndWrite('v');
        buttons[28].setEnabled(false);
    }
    public void clickW(View view) {
        checkAndWrite('w');
        buttons[29].setEnabled(false);
    }
    public void clickX(View view) {
        checkAndWrite('x');
        buttons[30].setEnabled(false);
    }
    public void clickY(View view) {
        checkAndWrite('y');
        buttons[31].setEnabled(false);
    }
    public void clickZ(View view) {
        checkAndWrite('z');
        buttons[32].setEnabled(false);
    }
    public void clickZ1(View view) {
        checkAndWrite('ź');
        buttons[33].setEnabled(false);
    }
    public void clickZ2(View view) {
        checkAndWrite('ż');
        buttons[34].setEnabled(false);
    }
}

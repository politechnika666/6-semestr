package com.codes.afro.gallery;

//import android.app.Activity;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
//import android.support.v7.app.ActionBarActivity;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ImageView;
import android.widget.RatingBar;
import android.widget.Toast;


public class FullSizeActivity extends Activity {

    public String name = "";
    private ImageView fullImage;
    private RatingBar ratingBar;
    static final String STATE_STARS = "numOfStars";


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_full_size);
        fullImage = (ImageView)findViewById(R.id.fullImg);
        Intent intent = getIntent();
        String imgId = intent.getStringExtra(MainActivity.IMG);
        name = intent.getStringExtra(MainActivity.NAME);
        Toast.makeText(getApplicationContext(), name, Toast.LENGTH_SHORT).show();
        fullImage.setImageResource(Integer.valueOf(imgId));

        addListenerOnRatingBar();

        //Toast.makeText(getApplicationContext(), imgId, Toast.LENGTH_SHORT).show();
    }
    /*@Override
    public void onResume() {
        super.onResume();  // Always call the superclass method first
        Toast.makeText(getApplicationContext(), "onResume", Toast.LENGTH_LONG).show();
    }

    @Override
    public void onStop() {
        super.onStop();  // Always call the superclass method first
        Toast.makeText(getApplicationContext(), "onStop", Toast.LENGTH_LONG).show();
    }

    @Override
    public void onStart() {
        super.onStart();  // Always call the superclass method first
        Toast.makeText(getApplicationContext(), "onStart", Toast.LENGTH_LONG).show();
    }

    @Override
    public void onPause() {
        super.onPause();  // Always call the superclass method first
        Toast.makeText(getApplicationContext(), "onPause", Toast.LENGTH_LONG).show();
        //ratingBar.setRating(numOfStars);
    }*/

    /*@Override
    public void onBackPressed() {
        Intent main = new Intent(this, MainActivity.class);
        //Toast.makeText(getApplicationContext(), String.valueOf(numOfStars), Toast.LENGTH_LONG).show();
        main.putExtra("stars",numOfStars);
        startActivity(main);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        //ratingBar.setRating(numOfStars);
        Toast.makeText(getApplicationContext(), "onDestroy", Toast.LENGTH_LONG).show();
    }*/



    public void addListenerOnRatingBar() {
        ratingBar = (RatingBar) findViewById(R.id.ratingBar);
        ratingBar.setOnRatingBarChangeListener(new RatingBar.OnRatingBarChangeListener() {
            public void onRatingChanged(RatingBar ratingBar, float rating, boolean fromUser) {
                int numOfStars = (int)rating;
                Intent intent = new Intent(getApplicationContext(), MainActivity.class);
                intent.putExtra("Name", name);
                intent.putExtra("Rating", numOfStars);
                //Toast.makeText(getApplicationContext(), String.valueOf(numOfStars), Toast.LENGTH_SHORT).show();
                setResult(Activity.RESULT_OK, intent);
                finish();
            }
        });
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_full_size, menu);
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
}
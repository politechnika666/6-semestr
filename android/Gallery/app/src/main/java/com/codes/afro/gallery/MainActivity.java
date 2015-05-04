package com.codes.afro.gallery;

import android.app.Activity;
import android.app.ListActivity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.RatingBar;
import android.widget.Toast;


public class MainActivity extends Activity{
    private ListView list;
    public final static String IMG = "com.codes.afro.MESSAGE";
    public final static String NAME = "com.codes.afro.NAME_MESSAGE";
    public int stars = 0;
    static final int rates = 0;

    String[] itemname ={
            "Image 0",
            "Image 1",
            "Image 2",
            "Image 3",
    };
    Integer[] imgid={
            R.drawable.img0,
            R.drawable.img1,
            R.drawable.img2,
            R.drawable.img3,
    };
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        CustomListAdapter adapter=new CustomListAdapter(this, itemname, imgid);
        list=(ListView)findViewById(R.id.list);
        list.setAdapter(adapter);
        list.setSelection(1);
        list.setOnItemClickListener(new OnItemClickListener() {

            @Override
            public void onItemClick(AdapterView<?> parent, View view,
                                    int position, long id) {
// TODO Auto-generated method stub
                String Slecteditem= itemname[+position];
                //ratingBar = (RatingBar)findViewById(R.id.pop_ratingbar);
                Integer image = imgid[+position];

                //Toast.makeText(getApplicationContext(), rating, Toast.LENGTH_SHORT).show();
                Intent fullSize = new Intent(getApplicationContext(), FullSizeActivity.class);
                fullSize.putExtra(IMG, String.valueOf(image));
                fullSize.putExtra(NAME, String.valueOf(itemname[+position]));
                startActivityForResult(fullSize, 1);
            }
        });
    }

    protected void onActivityResult(int requestCode, int resultCode, Intent data) {

        if (requestCode == 1) {
            if(resultCode == RESULT_OK){
                String name = data.getStringExtra("Name");
                int rating  = data.getIntExtra("Rating", 0);

                //Toast.makeText(MainActivity.this, name, Toast.LENGTH_SHORT).show();
                //Toast.makeText(MainActivity.this, String.valueOf(rating), Toast.LENGTH_SHORT).show();
                //Toast.makeText(MainActivity.this, "position: " + position, Toast.LENGTH_SHORT).show();
                int pos = 0;

                if(name.equals("Image 0"))
                    pos = 0;
                else if(name.equals("Image 1"))
                    pos = 1;
                else if(name.equals("Image 2"))
                    pos = 2;
                else if(name.equals("Image 3"))
                    pos = 3;
                //Toast.makeText(MainActivity.this, "Pos: " + String.valueOf(pos), Toast.LENGTH_SHORT).show();

                int wantedPosition = pos; // Whatever position you're looking for
                int firstPosition = list.getFirstVisiblePosition() - list.getHeaderViewsCount(); // This is the same as child #0
                int wantedChild = wantedPosition - firstPosition;
                // Say, first visible position is 8, you want position 10, wantedChild will now be 2
                // So that means your view is child #2 in the ViewGroup:
                if (wantedChild < 0 || wantedChild >= list.getChildCount()) {
                    Toast.makeText(MainActivity.this, "how did u get there?", Toast.LENGTH_SHORT).show();
                    return;
                }
                // Could also check if wantedPosition is betwePICK_CONTACT_REQUESTen listView.getFirstVisiblePosition() and listView.getLastVisiblePosition() instead.
                View wantedView = list.getChildAt(wantedChild);

                RatingBar ratingBar = (RatingBar) wantedView.findViewById(R.id.pop_ratingbar);

                ratingBar.setRating(rating);


            }
            if (resultCode == RESULT_CANCELED) {
                Toast.makeText(getApplicationContext(), "RESULT_CANCELED", Toast.LENGTH_SHORT).show();
            }
        }
    }


    @Override
    public void onResume() {
        super.onResume();
        //ratingBar.setRating(3);
    }
}
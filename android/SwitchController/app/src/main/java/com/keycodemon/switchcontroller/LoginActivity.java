package com.keycodemon.switchcontroller;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.graphics.Rect;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.text.method.PasswordTransformationMethod;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class LoginActivity extends AppCompatActivity implements View.OnClickListener, View.OnTouchListener {

    EditText edtUsername, edtPassword;
    Button btnLogin;
    boolean showPass = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        init();
    }

    void init(){
        edtUsername = findViewById(R.id.edtUsername);
        edtPassword = findViewById(R.id.edtPassword);
        btnLogin = findViewById(R.id.btnLogin);

        edtPassword.setOnTouchListener(this);
        btnLogin.setOnClickListener(this);
    }

    boolean validInput(String username, String password){

        if(!username.equals("") && !password.equals("")){
            return true;
        }

        Toast.makeText(this, getString(R.string.not_valid_input), Toast.LENGTH_LONG).show();
        return false;
    }

    @Override
    public void onClick(View view) {
        String username = edtUsername.getText().toString();
        String password = edtPassword.getText().toString();

        if(validInput(username, password)){
            if(username.equals("admin") && password.equals("admin123")){
                Intent mainIntent = new Intent(this, MainActivity.class);
                startActivity(mainIntent);
            }else{
                Toast.makeText(this, getString(R.string.login_failed), Toast.LENGTH_LONG).show();
            }
        }
    }

    @Override
    public boolean onTouch(View view, MotionEvent motionEvent) {
        EditText edtPass = (EditText)view;
        Drawable drawable = edtPass.getCompoundDrawables()[2];
        Rect bounds = drawable.getBounds();
        int mouseX = (int) motionEvent.getX();
        int mouseY = (int) motionEvent.getY();
        int tapArea = 24;
        int x = mouseX + tapArea;
        int y = mouseY - tapArea;
        x = view.getWidth() - x;

        if(x <= 0){
            x += tapArea;
        }

        if(y<=0){
            y = mouseY;
        }

        if(motionEvent.getAction() == MotionEvent.ACTION_DOWN){
            if (bounds.contains(x, y)) {
                showPass = !showPass;
                edtPass.setCompoundDrawablesWithIntrinsicBounds(getDrawable(R.drawable.ic_lock_24), null, getDrawable((showPass)?R.drawable.ic_hide_password_24:R.drawable.ic_show_password_24), null);
                edtPass.setTransformationMethod((showPass) ? null : PasswordTransformationMethod.getInstance());
            }
        }

        return false;
    }
}
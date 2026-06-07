#include "raylib.h"
#include <cstring>
#include <queue>
#include <string>

static bool showmenu = false;

static char fulldalog[256] = "";
static char currenttext[256] = "";
static float dialogtimer = 0.0f;
static int currentcharindex = 0;

static bool dialogcomplete = false;
static float cleartimer;

static bool hasplayercontrol = false;
static bool prevcollison = false;
std::queue<std::string> dialogqueue;



enum Gamescene
{
    gamemenu,
    gameplay,
    gameover,

};

enum choicesystem
{
    attack,
    defence,
    run,
    nothing,

};

void resetdialog(const char* newdialog)
{
    dialogqueue.push(newdialog);
   
}

void drawtext()
{
    
    Vector2 dialogpostion = { 20 ,500 };
    Rectangle dialogspace = { 20 , 500 , 700 , 500 };
    DrawTextEx(GetFontDefault(), currenttext, dialogpostion, 20, 2, BLACK);
}

void dialogcontinue()
{
    if (!dialogqueue.empty())
    {
        std::string next = dialogqueue.front();
        dialogqueue.pop();
        currentcharindex = 0;
        dialogtimer = 0.0f;
        dialogcomplete = false;
        memset(currenttext, 0, sizeof(currenttext));
        strncpy_s(fulldalog, sizeof(fulldalog), next.c_str(), _TRUNCATE);
    }
}

bool updatedialog(float delta )
{
    if (!dialogcomplete)
    {
        dialogtimer += delta;
        if (dialogtimer >= 0.05f)
        {

            if (fulldalog[currentcharindex] == '\0')
            {
                dialogcomplete = true;
                cleartimer = 0.0f;
            }
            else
            {
                currenttext[currentcharindex] = fulldalog[currentcharindex];
                currentcharindex += 1;
               
            }
            dialogtimer = 0.00f;
        }

    }

    if (dialogcomplete)
    {
        cleartimer += delta;
        if (cleartimer >= 2.0f)
        {
            if (!dialogqueue.empty())
            {
                dialogcontinue();
            }
            else
            {
                memset(currenttext, 0, sizeof(currenttext));
                dialogcomplete = false;
            }
        }

    }

    if (IsKeyPressed(KEY_SPACE) && !dialogcomplete)
    {
        memset(currenttext, 0, sizeof(currenttext));
        int len = strlen(fulldalog);
        for (int i = 0; i < len; i++)
        {
            currenttext[i] = fulldalog[i];

        }
        currenttext[len] = '\0';
        dialogcomplete = true;

    }
    return dialogcomplete;

}




void choice(choicesystem yourchoice, Vector2& spritepos)
{
   
    switch (yourchoice)
    {
    case attack:

        resetdialog("Aliens uses fast movements");
        spritepos.x += 200;
        break;

    case defence:

        spritepos.x -= 25;
        resetdialog("defence done \n hell yeah \n baby");
        break;

    case run:

        spritepos.y += 23;
        resetdialog("run done");
        break;

    case nothing:
        resetdialog("HI i am GD");
        resetdialog("Hii i am GG");
        resetdialog("how r u doing");
        dialogcontinue();

        break;

    }
    
}

void collisonchangechoice(Rectangle& rec1, Rectangle& rec2, Rectangle& rec3, Rectangle& rec4, choicesystem& change , Vector2& spritepos)                                             
{
    if (CheckCollisionPointRec(GetMousePosition(), rec1) )
    {
        DrawRectangleLinesEx(rec1, 2, RED);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)    )
        { 
            TraceLog(LOG_INFO, "Attack Selected    X");
            change = attack;
            choice(change, spritepos);
            showmenu = !showmenu;
            
        }
    }
    if (CheckCollisionPointRec(GetMousePosition(), rec2))
    {
        DrawRectangleLinesEx(rec2, 2, RED);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)  )
        {
            TraceLog(LOG_INFO, "defence Selected    B");
            change = defence;
            choice(change, spritepos);
            showmenu = !showmenu;
        }
    }
    if (CheckCollisionPointRec(GetMousePosition(), rec3))
    {
        DrawRectangleLinesEx(rec3, 2, RED);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) )
        {
            TraceLog(LOG_INFO, "Run Selected  Y");
            change = run;
            choice(change, spritepos);
            showmenu = !showmenu;
        }
    }

    if (CheckCollisionPointRec(GetMousePosition(), rec4))
    {
        DrawRectangleLinesEx(rec4, 2, RED);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            TraceLog(LOG_INFO, "nothing Selected     A");
            change = nothing;
            choice(change, spritepos);
            showmenu = !showmenu;
        }
    }

}

void updategame(  Vector2& sprite ,   float speed  , float delta , bool iscollided)
{
    if (!iscollided)
    {
        
            if (IsKeyDown(KEY_W))
            {
                sprite.y -= speed * delta;
            }
            if (IsKeyDown(KEY_S))
            {
                sprite.y += speed * delta;
            }
            if (IsKeyDown(KEY_A))
            {
                sprite.x -= speed * delta;
            }
            if (IsKeyDown(KEY_D))
            {
                sprite.x += speed * delta;
            }
         }

 }



void updategame2(Vector2& sprite, float& speed, float& delta, bool& iscollided)
{
    if (iscollided)
    {

        if (IsKeyDown(KEY_W))
        {
            sprite.y -= speed * delta;
        }
        if (IsKeyDown(KEY_S))
        {
            sprite.y += speed * delta;
        }
        if (IsKeyDown(KEY_A))
        {
            sprite.x -= speed * delta;
        }
        if (IsKeyDown(KEY_D))
        {
            sprite.x += speed * delta;
        }
    }

}

  void playerchange( Vector2& sprite2pos, float& newspeed, bool& iscollided, float& delta)
  {
    updategame2(sprite2pos, newspeed, delta  , iscollided);
  }


bool collisonshoot(Rectangle& boundingbox, Rectangle& trap , bool iscollided)
{
    if (CheckCollisionRecs(boundingbox, trap))
    {
       
        return true;
    }
    return iscollided;
}

bool collisondialog(Rectangle& boundingbox, Rectangle& choicebox, bool choiceccollsion , choicesystem& changechoice , Vector2& spritepos)
{
    if (CheckCollisionRecs(boundingbox, choicebox) )
    {
        if (IsKeyPressed(KEY_E))
        {
            showmenu = !showmenu;
        }

        if (showmenu)
        {
            {


                Rectangle choice1 = { 0 , 450 , 800, 25 };
                DrawRectangleRec(choice1, BLACK);
                Rectangle choice2 = { 0 ,480 , 800, 25 };
                DrawRectangleRec(choice2, BLACK);
                Rectangle choice3 = { 0 , 510 , 800 , 25 };
                DrawRectangleRec(choice3, BLACK);
                Rectangle choice4 = { 0 , 540 , 800 , 25 };
                DrawRectangleRec(choice4, BLACK);

                DrawText("ATTACK ", 300, 460, 9, WHITE);
                DrawText("DEFEND", 300, 490, 9, WHITE);
                DrawText("RUN ", 300, 520, 9, WHITE);
                DrawText("INTRODUCE YOURUSELF ", 300, 550, 9, WHITE);

                collisonchangechoice(choice1, choice2, choice3, choice4, changechoice, spritepos);


                return true;

            }

        }
    }

    return false;
 
}

void gamesceneschange(Gamescene scene, Vector2& spritepos,Vector2& sprite2pos  , Texture2D& sprite,Texture2D& sprite2 , bool iscollided, choicesystem& yourchoice, float& speed , float& delta  )
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    switch (scene)
    {

    case gamemenu:
        DrawText("PRESS ENTER TO START", 280, 280, 20, BLACK);
        break;


    case gameplay:

    {
        Rectangle boundingbox = { spritepos.x + 5 , spritepos.y + 15 , (float)sprite.width * 4 , (float)sprite.height * 4 };

        Rectangle rec = { 400 , 300 , 30 , 20 };

        DrawRectangleRec(rec, RED);

        DrawRectangleLinesEx(boundingbox, 2.0f, BLACK);

        DrawTextureEx(sprite, spritepos, 0.0f, 5.0f, WHITE);
        DrawTextureEx(sprite2, sprite2pos, 0.0f, 5.0f, WHITE);

        Rectangle choicedialog = { 400 , 250 ,20 , 30 };
        DrawRectangleRec(choicedialog, BLUE);
        DrawText("E", 400, 200, 25, BLACK);


        bool iscollisonchociebox = false;


        bool collisonoccur = (collisondialog(boundingbox, choicedialog, iscollisonchociebox, yourchoice, spritepos));
        bool done = updatedialog(GetFrameTime());
        drawtext();


       
        Rectangle boundingboxsprite2 = { sprite2pos.x + 5 , sprite2pos.y  , (float)sprite2.width * 4  , (float)sprite2.height * 4 };
        DrawRectangleLinesEx(boundingboxsprite2, 2.0f, BLACK);
        
        float newspeed = 500;  
        
        

        bool spritecollison = CheckCollisionRecs(boundingbox, boundingboxsprite2);

         if(spritecollison&&!prevcollison)
          {

              hasplayercontrol = !hasplayercontrol;
              speed = hasplayercontrol ? 0 : 100;    
          }

         prevcollison = spritecollison;

         if (hasplayercontrol)
         {
             playerchange(sprite2pos, newspeed, hasplayercontrol, delta);
         }
         else
         {

             updategame(spritepos, speed, delta, false);
         }



                
       


          
        break;
    }


    case gameover:

        DrawText("PRESS R TO RESTART ", 200, 300, 3, BLACK);
        hasplayercontrol = false;

        break;  
    }
    EndDrawing();
}

int main()
{
    
    InitWindow(800, 600, "Raylib + VS2022" );
    Gamescene currentscene = gamemenu;
    Texture2D sprite = LoadTexture("seaalien.png");
    Texture2D sprite2 = LoadTexture("seaalien.png");
    static bool iscollided =  false ;
    Vector2 sprite2pos = { 50 , 300 };
    static float speed =  100;
    Vector2 spritepos = { 20  ,150 };
    choicesystem yourchoice{};
    

    while (!WindowShouldClose())
    {
         float delta = GetFrameTime();
       

        switch (currentscene)
        {
        case gamemenu:
            if (IsKeyPressed(KEY_ENTER) )
            {
                currentscene = gameplay;
            }
            break;
        case gameplay:
        {
            Rectangle boundingbox = { spritepos.x + 5 , spritepos.y + 15 , (float)sprite.width * 4 , (float)sprite.height * 4 };
            Rectangle rec = { 400, 300, 30, 20 };
            iscollided = collisonshoot(boundingbox, rec, iscollided);
            updategame(spritepos, speed, delta, iscollided);

            if (iscollided)
            {
                currentscene = gameover;
            }
        }
        break;
        case gameover:
            if (IsKeyPressed(KEY_R))
            {
                currentscene = gameplay;
                spritepos = { 20 , 150 };
                iscollided = false;
                
            }
            break;
        }
        gamesceneschange(currentscene, spritepos,sprite2pos ,sprite, sprite2, iscollided, yourchoice, speed, delta);
    }

    UnloadTexture(sprite);
    CloseWindow();
    return 0;
}

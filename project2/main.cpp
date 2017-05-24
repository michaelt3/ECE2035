#include "misc.h"

DigitalOut led1(LED1);                  // mbed leds
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);
DigitalIn pb_u(p21);                    // push buttons
DigitalIn pb_r(p22);
DigitalIn pb_d(p23);
DigitalIn pb_l(p24);
Serial pc(USBTX, USBRX);                // Serial connection to PC. Useful for debugging!
MMA8452 acc(p28, p27, 100000);          // Accelerometer (SDA, SCL, Baudrate)
uLCD_4DGL uLCD(p9, p10, p11);           // LCD (tx, rx, reset)
SDFileSystem sd(p5, p6, p7, p8, "sd");  // SD (mosi, miso, sck, cs)
AnalogOut DACout(p18);                  // speaker
wave_player player(&DACout);            // wav player
GSYNC game_synchronizer;                // Game_Synchronizer
GSYNC *sync = &game_synchronizer;       //

int winner = -1; //not defined yet
int whose_turn = PLAYER1; 
int testMode = 0;  // multiplayer or singleplayer?
int difficulty = 0; //Easy, Medium, or Hard

void menuBlink(void const *args) { //Blinking Lights for Menu
    while(1) {
        led1 = 1;
        led2 = 0;
        led3 = 1;
        led4 = 0;
        Thread::wait(500);
        led1 = 0;
        led2 = 1;
        led3 = 0;
        led4 = 1;
        Thread::wait(500);
    }
}
void difficultyBlink(void const *args) {    //Blinking Lights for difficulty selection
    while(1) {
        led1 = 1;
        led2 = 0;
        led3 = 0;
        led4 = 0;
        Thread::wait(500);
        led1 = 0;
        led2 = 1;
        led3 = 0;
        led4 = 0;
        Thread::wait(500);
        led1 = 0;
        led2 = 0;
        led3 = 1;
        led4 = 0;
        Thread::wait(500);
        led1 = 0;
        led2 = 0;
        led3 = 0;
        led4 = 1;
        Thread::wait(500);
    }
}
//light effect thread for damage
//rapid blinking...just as the function name implies
void rapidBlink(void const *args) { //Blinking Lights for Game Over!
    while(1) {
        led1 = 1;
        led2 = 1;
        led3 = 1;
        led4 = 1;
        Thread::wait(100);
        led1 = 0.0;
        led2 = 0.0;
        led3 = 0.0;
        led4 = 0.0;
        Thread::wait(100);
    }
}
// Starting game menu. Returns SINGLE_PLAYER or MULTI_PLAYER. Only select 
// MULTI_PLAYER if another mbed is connected via ethernet; otherwise, the
// game will hang.
char *chime = "/sd/wavfiles/chime.wav";         // 4 different sound effects used in Thread
char *fanfare = "/sd/wavfiles/fanfare.wav";
char *floop = "/sd/wavfiles/floop.wav";
char *appluase = "/sd/wavfiles/appluase.wav";

void playChimeSound(void const *args) {
    playSound(chime);
}
void playFanfareSound(void const *args) {
    playSound(fanfare);
}
void playFloopSound(void const *args) {
    playSound(floop);
}
void playAppluaseSound(void const *args) {
    playSound(appluase);
}

int game_menu(void)
{
    uLCD.baudrate(BAUD_3000000);
    Thread thread(menuBlink);
    Thread audio(playChimeSound);
     
    uLCD.locate(3,0);               // Singleplayer or Multiplayer?
    uLCD.puts("BADMINTON");
    uLCD.locate(0,2);
    uLCD.puts("Press 'UP' for");
    uLCD.locate(1,3);
    uLCD.puts("SINGLEPLAYER");
    uLCD.locate(0,5);
    uLCD.puts("Press 'RIGHT' for");
    uLCD.locate(1,6);
    uLCD.puts("MULTIPLAYER");
    uLCD.locate(0,12);
    uLCD.puts("ECE2035 - Michael");
    
    int mode;
    while(pb_u || pb_r)  
    {
        if(!pb_u) 
        {                                   
            Thread thread(difficultyBlink);
            Thread audio(playFloopSound);
            uLCD.cls();
            uLCD.locate(1,3);
            uLCD.puts("SINGLEPLAYER");
            mode = SINGLE_PLAYER;
            testMode = 0;
            thread.wait(1000);
            uLCD.cls();
            uLCD.puts("DIFFICULTY");
            uLCD.locate(0,2);
            uLCD.puts("Press 'UP' for");
            uLCD.locate(1,3);
            uLCD.puts("NORMAL");
            uLCD.locate(0,5);
            uLCD.puts("Press 'RIGHT' for");
            uLCD.locate(1,6);
            uLCD.puts("HARD");
            uLCD.locate(0,8);
            uLCD.puts("Press 'DOWN' for");
            uLCD.locate(1,9);
            uLCD.puts("EASY");
            while(pb_u || pb_r || pb_d)  // Difficulty Level?
            {
                if(!pb_u) 
                {
                    uLCD.cls();
                    uLCD.locate(1,3);
                    uLCD.puts("NORMAL");
                    difficulty = 1;
                    thread.wait(1000);
                    break;
                } 
                else if(!pb_r) 
                {
                    uLCD.cls();
                    uLCD.locate(1,3);
                    uLCD.puts("HARD");
                    difficulty = 2;
                    thread.wait(1000);
                    break;
                }
                else if(!pb_d)
                {
                    uLCD.cls();
                    uLCD.locate(1,3);
                    uLCD.puts("EASY");
                    difficulty = 0;
                    thread.wait(1000);
                    break;
                }    
            }
            break;
        } 
        else if(!pb_r) 
        {
            uLCD.cls();
            uLCD.locate(1,3);
            uLCD.puts("MULTIPLAYER");
            mode = MULTI_PLAYER;
            testMode = 1;
            break;
        }
    }
    return mode;
}

int score1 = 0;         //point system
int score2 = 0;

void draw_racket1(racket *r)    //draw racket 1
{
    int dx = r->width/2;
    int dy = r->height/2;
    GS_filled_rectangle(sync, SCREEN_BOTH, r->x-dx, r->y-dy, r->x+dx, r->y+dy, P1_COL);
}
void draw_racket2(racket *r)    // draw racket 2
{
    int dx = r->width/2;
    int dy = r->height/2;
    GS_filled_rectangle(sync, SCREEN_BOTH, r->x-dx, r->y-dy, r->x+dx, r->y+dy, P2_COL);
}

void draw_shuttle(shuttle *s)   // draw shuttle
{
    GS_pixel(sync, SCREEN_BOTH, s->x, s->y, SHUT_COL);
}

void erase_racket(racket *r)    // erase racket
{
    int dx = r->width/2;
    int dy = r->height/2;
    GS_filled_rectangle(sync, SCREEN_BOTH, r->x-dx, r->y-dy, r->x+dx, r->y+dy, SKY_COL);
}

void erase_shuttle(shuttle *s)  // erase shuttle
{
    GS_pixel(sync, SCREEN_BOTH, s->x, s->y, SKY_COL);
}


int p1bound_x(racket *r)    //Check bounds for racket 1
{
    if (r->x < 7)
    {
        r->x = 7;
    }
    if (r->x > 54)
    {
        r->x = 54;
    }
    return r->x;
}

int p2bound_x(racket *r)    //check bounds for racket 2s
{
    if (r->x < 73)
    {
        r->x = 73;
    }
    if (r->x > 121)
    {
        r->x = 121;
    }
    return r->x;
}

int bound_y(racket *r)      
{
    if (r->y < 11)
    {
        r->y = 11;
        r->time_elapsed = 0;
        r->has_jumped = false;
    }
    if (r->y > 127)
    {
        r->y = 127;
    }
    return r->y;
}

void update_racket(racket *r,shuttle *s, float ax)
{   
    erase_racket(r);
    if(r->has_jumped == false)  //accelerometer
    {
        r->x = r->x + 2*ax;
        if(r->player == 1)
        {
            r->x = p1bound_x(r);
        }
        else
        {
            r->x = p2bound_x(r);
        }
    }
    else if(r->has_jumped == true)
    {
        r->time_elapsed += TIME_STEP;
        r->y = floor(r->y+(r->vy0*r->time_elapsed)-(4.9*r->time_elapsed*r->time_elapsed));
        r->y = bound_y(r);
    }
    if(r->player == 1)
    {
        draw_racket1(r);
    }
    else
    {
        draw_racket2(r);
    }
    
    if (s->is_live == false)        // if shuttle != live, then shuttle follows server
    {   
        erase_shuttle(s);
        s->x = r->x;
        s->y = r->y + 3;
        draw_shuttle(s);
    }
    
    else if(s->is_live == true)     //else, update shuttle
    {
        erase_shuttle(s);
        s->time_elapsed += TIME_STEP;
        if(s->x0 == 0)
        {
            s->x = floor(s->x + (s->vx0 * s->time_elapsed));
        }
        else
        {
            s->x = floor(s->x - (s->vx0 * s->time_elapsed));
        }    
        s->y = floor(s->y+(s->vy0*s->time_elapsed)-(4.9*s->time_elapsed*s->time_elapsed));
        draw_shuttle(s);
    } 
}    

// Initialize Game_Synchronizer. GS functions are unavailable prior to calling
// this function. This should only be called once.
void init_sync(void)
{
    led1 = 0;
    led2 = 0;
    led3 = 0;
    led4 = 0;
    pb_u.mode(PullUp);
    pb_r.mode(PullUp);
    pb_d.mode(PullUp);
    pb_l.mode(PullUp);
    pc.printf("\033[2J\033[0;0H");  // clear serial terminal
    GS_init(sync, &uLCD, &acc, &pb_u, &pb_r, &pb_d, &pb_l, game_menu(), PLAYER1);
    GS_cls(sync, SCREEN_BOTH);
    GS_update(sync);
}

// Initialize game map. This is called to reset the map at the beginning of each
// volley. Feel free to change any of this.
void init_map(racket *r1, racket *r2, shuttle *s)
{
    GS_filled_rectangle(sync, SCREEN_BOTH, 0, 0, 127, 127, SKY_COL);
    GS_filled_rectangle(sync, SCREEN_BOTH, 0, 0, 127, GND_LVL-2, 0x007f00);
    GS_filled_rectangle(sync, SCREEN_BOTH, 63, GND_LVL-2, 64, 45, GND_COL);
    GS_filled_circle(sync, SCREEN_BOTH, 30, 100, 8, 0xe5e500);
    GS_filled_circle(sync, SCREEN_BOTH, 90, 100, 7, 0xffffff);
    GS_filled_circle(sync, SCREEN_BOTH, 95, 100, 5, 0xffffff);
    GS_filled_circle(sync, SCREEN_BOTH, 85, 100, 6.5, 0xffffff);
    int dx = r1->width/2;
    int dy = r1->height/2;
    GS_filled_rectangle(sync, SCREEN_BOTH, r1->x-dx, r1->y-dy, r1->x+dx, r1->y+dy, P1_COL);
    GS_pixel(sync, SCREEN_BOTH, s->last_possession->x, s->last_possession->y+dy+2, SHUT_COL);
    dx = r2->width/2;
    dy = r2->height/2;
    GS_filled_rectangle(sync, SCREEN_BOTH, r2->x-dx, r2->y-dy, r2->x+dx, r2->y+dy, P2_COL);
    GS_locate(sync,SCREEN_BOTH,0,0);
    char p[18];                                         // record score
    sprintf(p,"P1:%d          P2:%d", score1,score2);
    GS_puts(sync,SCREEN_BOTH,p,18);
}

// Game over. Must reset mbed to break loop.
void game_over(int winner)
{
    Thread thread(rapidBlink);
    GS_filled_rectangle(sync, SCREEN_BOTH, 0, 0, 127, 127, 0x2E0854);
    GS_locate(sync,SCREEN_BOTH,1,6);
    char p[30];
    sprintf(p,"PLAYER%d WINS!%d-%d",winner,score1,score2);
    GS_textbackground_color (sync, SCREEN_BOTH, 0x2E0854);
    GS_puts(sync,SCREEN_BOTH,p,30);
    Thread audio(playAppluaseSound);
    GS_update(sync);
    while(1)
    {
        wait(1);
    }
}

// Beginning of execution. 
int main(void)
{
    // Initializations, probably best if these are left alone.
    int *p1_buttons, *p2_buttons;
    float ax1, ay1, az1, ax2, ay2, az2;
    racket *p1 = (racket*)calloc(1, sizeof(racket));
    racket *p2 = (racket*)calloc(1, sizeof(racket));
    shuttle *s = (shuttle*)calloc(1, sizeof(shuttle));
    init_sync();
    int size = 0;           // size used for checking bounds
    if (difficulty == 0)    // Easy mode
    {
        size = 13;
    }
    if (difficulty == 1)    // Normal Mode
    {
        size = 9;
    }
    if (difficulty == 2)    // Hard Mode
    {
        size = 5;
    }
    if (testMode == 0)      // if singleplayer, adjust racket
    {
        init_racket(p1, 1,difficulty);
        init_racket(p2, 2,difficulty);
    }
    else                    // if multiplayer, default to normal
    {
        difficulty = 1;
        init_racket(p1, 1,difficulty);
        init_racket(p2, 2,difficulty);
    }
    init_shuttle(s, p1);    // player 1 always serves first
    whose_turn = PLAYER1;
    GS_textbackground_color(sync, SCREEN_BOTH, SKY_COL);
    init_map(p1, p2, s);
    
    // Game loop. This is where the majority of your time will be spent.
    while(1)
    {   
        // Retrieve inputs from specified source during each iteration.
        p1_buttons = GS_get_p1_buttons(sync);
        p2_buttons = GS_get_p2_buttons(sync);
        GS_get_p1_accel_data(sync, &ax1, &ay1, &az1);
        GS_get_p2_accel_data(sync, &ax2, &ay2, &az2);
        led1 = p1_buttons[U_BUTTON] ^ p2_buttons[U_BUTTON];
        led2 = p1_buttons[R_BUTTON] ^ p2_buttons[R_BUTTON];
        led3 = p1_buttons[D_BUTTON] ^ p2_buttons[D_BUTTON];
        led4 = p1_buttons[L_BUTTON] ^ p2_buttons[L_BUTTON];
   //////////////////////////////////////////////////////////////
        if(testMode == 0)       // SINGLE PLAYER
        { 
            if (whose_turn == PLAYER1)
            { 
                if(p1_buttons[D_BUTTON])    // Update without turning
                {
                    update_racket(p1,s,0);
                }
                if(ax1 > ACC_THRESHOLD) 
                {   
                    update_racket(p1,s,1);       
                } 
                else if(ax1 < -ACC_THRESHOLD)
                {
                    update_racket(p1,s,-1); 
                }
                if(p1_buttons[U_BUTTON]&&s->is_live == false) // SERVE
                {   
                    s->last_possession = p1;
                    s->x0 = 0;                      //x0 corresponds to last possession
                    whose_turn = PLAYER2;
                    s->is_live = true;
                    update_racket(p2,s,0); 
                    wait(0.5); 
                }
                else if(p1_buttons[U_BUTTON]&&s->is_live == true)   //JUMP
                {
                    p1->has_jumped = true;
                    update_racket(p1,s,0);    
                }
                if (s->y < 0)                   // SCORED?
                {
                    Thread audio(playFanfareSound);
                    score2 += 1;   
                    GS_locate(sync,SCREEN_BOTH,9,3);
                    char d[6];
                    sprintf(d,"P2 SCORES!");
                    GS_puts(sync,SCREEN_BOTH,d,10);
                    GS_update(sync);
                    init_racket(p1, 1,difficulty);
                    init_racket(p2, 2,difficulty);
                    init_shuttle(s, p2);
                    s->vx0 = .9;
                    s->vy0 = 8.2; 
                    GS_textbackground_color(sync, SCREEN_BOTH, SKY_COL);
                    init_map(p1, p2, s);
                    whose_turn = PLAYER2;
                    wait(3);
                }
                if(s->x > (p1->x - size) &&s->x <(p1->x + size) && s->y < 13)   // RETURN?
                {
                        s->time_elapsed = 0;
                        whose_turn = PLAYER2;
                        s->last_possession = p1; 
                        s->y = 14;
                        s->x0 = 0;
                        s->vx0 = 2.1;
                        s->vy0 = 7.8;
                        update_racket(p2,s,0);
                        Thread audio(playFloopSound);   
                }            
            }            
    
            if (whose_turn == PLAYER2)
            {   
                if(p1_buttons[D_BUTTON])
                {
                    update_racket(p2,s,0);
                }
                if(ax1 > ACC_THRESHOLD) 
                {   
                    update_racket(p2,s,1);       
                } 
                else if(ax1 < -ACC_THRESHOLD)
                {
                    update_racket(p2,s,-1); 
                }
                if(p1_buttons[U_BUTTON]&&s->is_live == false)
                {
                    s->last_possession = p2;
                    s->x0 = 1;
                    s->is_live = true;
                    update_racket(p1,s,0);
                    whose_turn = PLAYER1;
                    wait(0.5); 
                }
                else if(p1_buttons[U_BUTTON]&&s->is_live == true)
                {
                    p2->has_jumped = true; 
                    update_racket(p2,s,0);   
                }
                if (s->y < 0)
                {
                    Thread audio(playFanfareSound);
                    score1 += 1;   
                    GS_locate(sync,SCREEN_BOTH,0,3);
                    char d[6];
                    sprintf(d,"P1 SCORES!        ");
                    GS_puts(sync,SCREEN_BOTH,d,18);
                    GS_update(sync);
                    init_racket(p1, 1,difficulty);
                    init_racket(p2, 2,difficulty);
                    init_shuttle(s, p1);
                    GS_textbackground_color(sync, SCREEN_BOTH, SKY_COL);
                    init_map(p1, p2, s);
                    whose_turn = PLAYER1; 
                    wait(3);
                }
                if( ((s->x) > ((p2->x)-size)) && ((s->x) < (p2->x+size))) 
                {
                    if(s->y > p2->y-2 && s->y < p2->y+2)
                    {
                        s->time_elapsed = 0;
                        whose_turn = PLAYER1;
                        s->last_possession = p2; 
                        s->y = 14;
                        s->x0 = 1;
                        s->vx0 = .9;
                        s->vy0 = 8.2;
                        update_racket(p1,s,0);  
                        Thread audio(playFloopSound); 
                    }         
                }       
            }     
            GS_update(sync); 
            if (score1 == 3)        // did game end?
            {
                game_over(1); 
            }
            if (score2 == 3)
            {
                game_over(2);
            }
        }
////////////////////////////////////////////////////////////////////////        
        else        // MULTIPLAYER
        {   difficulty = 1;     // ALWAYS DEFAULTS TO NORMAL
            if (whose_turn == PLAYER1)
            { 
                if(p1_buttons[D_BUTTON])
                {
                    update_racket(p1,s,0);
                }
                if(ax1 > ACC_THRESHOLD) 
                {   
                    update_racket(p1,s,1);       
                } 
                else if(ax1 < -ACC_THRESHOLD)
                {
                    update_racket(p1,s,-1); 
                }
                if(p1_buttons[U_BUTTON]&&s->is_live == false)
                {   
                    s->last_possession = p1;
                    s->x0 = 0;                      //x0 corresponds to last possession
                    whose_turn = PLAYER2;
                    s->is_live = true;
                    update_racket(p2,s,0); 
                    wait(0.5); 
                }
                else if(p1_buttons[U_BUTTON]&&s->is_live == true)
                {
                    p1->has_jumped = true;    
                }
                if (s->y < 0)
                {
                    Thread audio(playFanfareSound);
                    score2 += 1;   
                    GS_locate(sync,SCREEN_BOTH,9,3);
                    char d[6];
                    sprintf(d,"P2 SCORES!");
                    GS_puts(sync,SCREEN_BOTH,d,10);
                    GS_update(sync);
                    init_racket(p1, 1,difficulty);
                    init_racket(p2, 2,difficulty);
                    init_shuttle(s, p2);
                    s->vx0 = .9;
                    s->vy0 = 8.2; 
                    GS_textbackground_color(sync, SCREEN_BOTH, SKY_COL);
                    init_map(p1, p2, s);
                    whose_turn = PLAYER2;
                    wait(3);
                }
                if(s->x > (p1->x - size) &&s->x <(p1->x + size) && s->y < 13)
                {
                        Thread audio(playFloopSound);
                        s->time_elapsed = 0;
                        whose_turn = PLAYER2;
                        s->last_possession = p1; 
                        s->y = 14;
                        s->x0 = 0;
                        s->vx0 = 2.1;
                        s->vy0 = 7.8;
                        update_racket(p2,s,0);   
                    //}
                }            
            }            
    
            if (whose_turn == PLAYER2)
            {   
                if(p2_buttons[D_BUTTON])
                {
                    update_racket(p2,s,0);
                }
                if(ax2 > ACC_THRESHOLD) 
                {   
                    update_racket(p2,s,1);       
                } 
                else if(ax2 < -ACC_THRESHOLD)
                {
                    update_racket(p2,s,-1); 
                }
                if(p2_buttons[U_BUTTON]&&s->is_live == false)
                {
                    s->last_possession = p2;
                    s->x0 = 1;
                    s->is_live = true;
                    update_racket(p1,s,0);
                    whose_turn = PLAYER1;
                    wait(0.5); 
                }
                else if(p2_buttons[U_BUTTON]&&s->is_live == true)
                {
                    p2->has_jumped = true;    
                }
                if (s->y < 0)
                {
                    Thread audio(playFanfareSound);
                    score1 += 1;   
                    GS_locate(sync,SCREEN_BOTH,0,3);
                    char d[6];
                    sprintf(d,"P1 SCORES!        ");
                    GS_puts(sync,SCREEN_BOTH,d,18);
                    GS_update(sync);
                    init_racket(p1,1,difficulty);
                    init_racket(p2, 2,difficulty);
                    init_shuttle(s, p1);
                    GS_textbackground_color(sync, SCREEN_BOTH, SKY_COL);
                    init_map(p1, p2, s);
                    whose_turn = PLAYER1; 
                    wait(3);
                }
                if( ((s->x) > ((p2->x)-size)) && ((s->x) < (p2->x+size))) 
                {
                    if(s->y > p2->y-2 && s->y < p2->y+2)
                    {
                        Thread audio(playFloopSound);
                        s->time_elapsed = 0;
                        whose_turn = PLAYER1;
                        s->last_possession = p2; 
                        s->y = 14;
                        s->x0 = 1;
                        s->vx0 = .9;
                        s->vy0 = 8.2;
                        update_racket(p1,s,0);   
                    }         
                }       
            }     
            GS_update(sync); 
            if (score1 == 3)            // did game end?
            {
                game_over(1); 
            }
            if (score2 == 3)
            {
                game_over(2);
            }
        }        
    }
}
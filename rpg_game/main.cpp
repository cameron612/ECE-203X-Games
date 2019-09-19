// Project includes
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "speech.h"

// Functions in this file
int get_action (GameInputs inputs);
int update_game (int action);
void draw_game (int init);
void init_main_map ();
void init_sec_map ();
void init_thrd_map ();
int main ();
void move_up();
void move_down();
void move_right();
void move_left();
void interact(double x, double y);
void open_menu();
void change_mode();
void talk_bud();
void start_quest();
void quest_pt2();
void go_uga();
void start_menu();
void help_screen();
void game_over();
void draw_upper_status();
void draw_lower_status();
void door_();
void jump();
void game_win();
void continue_screen();
void fight_song();


/**
 * The main game state. Must include Player locations and previous locations for
 * drawing to work properly. Other items can be added as needed.
 */
struct {
    int x,y;    // Current locations
    int px, py; // Previous locations
    int has_key;
    int flowers;
    int score;
    int lives;
    int health;
    int jumping;
    int has_T;
    int sec_map;
    bool mode;
    bool init;
    bool cb;
} Player;

/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possbile return values are defined below.
 */
#define NO_ACTION 0
#define ACTION_BUTTON 1
#define MENU_BUTTON 2
#define GO_LEFT 3
#define GO_RIGHT 4
#define GO_UP 5
#define GO_DOWN 6
#define OMNIPOTENT 7
#define JUMP 8
int get_action(GameInputs inputs)
{
    inputs = read_inputs();
    //pc.printf("inputs read \r\n");
    int b1 = inputs.b1;
    int b2 = inputs.b2;
    int b3 = inputs.b3;
    int b4 = inputs.b4;
    double ax = inputs.ax;
    double ay = inputs.ay;
    double az = inputs.az;
    //pc.printf("B1 IS %d, B2 IS %d\n", b1, b2);
    //pc.printf("B3 IS %d, B4 IS %d\n", b3, b4);
        if(b1 == 0){
            return ACTION_BUTTON;
        }
        else if(b2 == 0){
            
            return MENU_BUTTON;
        }
        else if(b3 == 0){
            return OMNIPOTENT;
        }
        if((ax > 0) && (ay < ax)&&(abs(ax)>0.1)){
            
            return GO_DOWN;
        }
        else if((ax < 0) && (abs(ax)>abs(ay))&&(abs(ax)>0.1)){
            //pc.printf("up \r\n");
            return GO_UP;
        }
        else if((ay > 0) && (abs(ay)>abs(ax))&&(abs(ay)>0.1)){
            
            return GO_RIGHT;
        }
        else if((ay < 0) && (ax > ay)&&(abs(ay)>0.1)){
            
            return GO_LEFT;
        }
        else if((az > 0)&&(abs(az)>1.0)){
            if(Player.sec_map == 1){
                if(get_north(Player.x,Player.y)->interaction == 4)
                    return JUMP;
            }
            else
                return NO_ACTION;
        }
        else{
            
            return NO_ACTION;
        }
}

/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the Player position accordingly.
 * 
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the player has not moved.
 */
#define NO_RESULT 0
#define GAME_OVER 1
#define FULL_DRAW 2
int update_game(int action)
{
    // Save player previous location before updating
    Player.px = Player.x;
    Player.py = Player.y;
    
    // Do different things based on the each action.
    // You can define functions like "go_up()" that get called for each case.
    switch(action)
    {
        case GO_UP:
            //pc.printf("up\n");  
            move_up();
            break;
        case GO_LEFT:  
            //pc.printf("left\n");
            move_left(); 
            break;            
        case GO_DOWN: 
            //pc.printf("down\n"); 
            move_down();
            break;
        case GO_RIGHT:
            //pc.printf("right\n");
            move_right();  
            break;
        case ACTION_BUTTON: 
            interact(Player.x, Player.y);
            break;
        case MENU_BUTTON: 
            //int x = Player.x;
            //int y = Player.y;
            open_menu();
            //Player.x = x;
            //Player.y = y;
            //draw_game(true);
            break;
        case OMNIPOTENT:
            change_mode();
            break;
        case JUMP:
            jump();
            break;
        default:    
            return NO_RESULT;
    }
    return NO_RESULT;
}

/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status 
 * bars. Unless init is nonzero, this function will optimize drawing by only 
 * drawing tiles that have changed from the previous frame.
 */
 
 
 
void draw_game(int init)
{
    // Draw game border first
    if(init) draw_border();
    //pc.printf("border drawn \r\n");
    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) // Iterate over columns of tiles
    {
        for (int j = -4; j <= 4; j++) // Iterate over one column of tiles
        {
            // Here, we have a given (i,j)
            
            // Compute the current map (x,y) of this tile
            int x = i + Player.x;
            int y = j + Player.y;
            
            // Compute the previous map (px, py) of this tile
            int px = i + Player.px;
            int py = j + Player.py;
                        
            // Compute u,v coordinates for drawing
            int u = (i+5)*11 + 3;
            int v = (j+4)*11 + 15;
            
            // Figure out what to draw
            DrawFunc draw = NULL;
            if (i == 0 && j == 0){ // Only draw the player on init
                if(get_here(Player.x,Player.y)->type == BUS)
                    draw_bus(u,v);
                else{
                    draw_player(u, v, Player.has_key);
                
                //pc.printf("player drawn\n");
                    continue;
                }
                
            }
            else if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) // Current (i,j) in the map
            {
                //pc.printf("get here entered\r\n");
                MapItem* curr_item = get_here(x, y);
                //pc.printf("get here returned\r\n");
                MapItem* prev_item = get_here(px, py);
                if (init || curr_item != prev_item) // Only draw if they're different
                {
                    if (curr_item) // There's something here! Draw it
                    {
                        draw = curr_item->draw;
                        //pc.printf("found what item to draw\n");
                    }
                    else // There used to be something, but now there isn't
                    {
                        draw = draw_nothing;
                    }
                }
                //pc.printf("end of nested conditionals \r\n");
            }
            
            else if (init) // If doing a full draw, but we're out of bounds, draw the walls.
            {
                draw = draw_wall;
            }
            //pc.printf("about to actually draw tile \r\n");

            // Actually draw the tile
            if (draw != NULL) {
                //pc.printf("about to draw %04x \r\n", draw);
                draw(u, v);
                //pc.printf("done drawing\n");
            }
        }
    }
    //pc.printf("about to draw status bars \r\n");

    // Draw status bars    
    draw_upper_status();
    draw_lower_status();
    //pc.printf("done drawing \r\n");
    Player.init = false;
}


/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion.
 */
void init_main_map()
{
    //plants dont show up
    // "Random" plants
    pc.printf("init main map! \r\n");
    Map* map = set_active_map(0);
    pc.printf("after setting map!\r\n");
    for(int i = map_width() + 3; i < map_area(); i += 39)
    {
        pc.printf("plant not added just yet!\r\n");
        add_plant(i % map_width(), i / map_width(),Player.cb);
        pc.printf("Plant added!\r\n");
    }
    pc.printf("plants\r\n");
         //add roses
    for(int i = map_width() + 3; i < map_area(); i += 39)
    {
        //pc.printf("plant not added just yet!\r\n");
        add_rose(i % map_width()+1, i / map_width()+1,Player.cb);
        //pc.printf("Plant added!\r\n");
    }
    //add violets
     for(int i = map_width() + 3; i < map_area(); i += 39)
    {
        //pc.printf("plant not added just yet!\r\n");
        add_violet(i % map_width()-1, i / map_width()-1,Player.cb);
        //pc.printf("Plant added!\r\n");
    }
    
    //add bud
    add_bud(9,8);
    
    //pc.printf("Adding walls!\r\n");
    add_wall(0,              0,              HORIZONTAL, map_width());
    pc.printf("wall 1 made \r\n");
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    pc.printf("wall 2 made \r\n");
    add_wall(0,              0,              VERTICAL,   map_height());
    pc.printf("wall 3 made \r\n");
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    //add_plant(Player.x+1,Player.y+1);
    pc.printf("wall 4 made \r\n");
   // pc.printf("Walls done!\r\n");

     
    //print_map();
}

/**
 * Program entry point! This is where it all begins.
 * This function orchestrates all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */
int main()
{
    int act,upd;
    GameInputs inputs;
    
    Player.flowers = 0;
    Player.has_key = 0;
    Player.init = true;
    Player.score = 0;
    Player.cb = false;
    Player.lives = 3;
    Player.health = 5;
    Player.jumping = 0;
    Player.has_T = 0;
    Player.sec_map = 0;
    //pc.printf("inputs recieved \r\n");
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
    //pc.printf("hardware initialized \r\n");
    start_menu();
    
    // Initialize the maps
    maps_init();
     //pc.printf("map initialized \r\n");
    init_main_map();
    //pc.printf("main map initialized \r\n");
    // Initialize game state
    set_active_map(0);
    Player.x = Player.y = 9;
    pc.printf("game state initialized \r\n");
    // Initial drawing
    draw_game(Player.init);
    pc.printf("game drawn \r\n");
    // Main game loop
    while(1)
    {
         //pc.printf("GAME LOOP \r\n");
        // Timer to measure game update speed
        Timer t; t.start();
        
        // Actually do the game update:
        // 1. Read inputs     
        inputs = read_inputs();   
        //pc.printf("inputs read \r\n");
        
        // 2. Determine action (get_action)  
        act = get_action(inputs);      
        //pc.printf("action gotten \r\n");
        // 3. Update game (update_game)
        upd = update_game(act);
        //pc.printf("GAME UPDATED \r\n");
        // 3b. Check for game over
        if(upd == 1){
            game_over();
            break;
        }
        // 4. Draw frame (draw_game)
        draw_game(Player.init);
        //pc.printf("frame drawn\n");
        // 5. Frame delay
        t.stop();
        int dt = t.read_ms();
        if (dt < 100) wait_ms(100 - dt);
    }
}
//moverment functions
void move_down()
{
    if(get_south(Player.x,Player.y)){
        if((get_south(Player.x,Player.y)->walkable != 0)||(Player.mode==true)){
            Player.x = Player.px;
            Player.y = Player.py + 1;
        }    
        else{   
            pc.printf("down movement blocked\n");
            speaker.period(1.0/100.0); // 500hz period
            speaker =0.5; //50% duty cycle - max volume
            wait(0.25);
            speaker=0.0;
            Player.x = Player.px;
            Player.y = Player.py;
        }       
    }
    else{
        Player.x = Player.px;
        Player.y = Player.py+1;
    }
}
void move_up()
{
    if(get_north(Player.x,Player.y)){
        if((get_north(Player.x,Player.y)->walkable != 0)||(Player.mode==true)||(Player.jumping == 1)){
            Player.x = Player.px;
            Player.y = Player.py - 1;
            
        }    
        else{
            if(get_north(Player.x,Player.y)->interaction == 4){
                Player.health--;
                uLCD.line(60, 58, 60+5*Player.health, 58, RED);
                if(Player.health == 0){
                    Player.lives--;
                    if(Player.lives == 0)
                        game_over();
                    Player.init = true;
                    //Player.health = 5;
                    pc.printf("player health is %d player has %d lives", Player.health, Player.lives);
                    continue_screen();
                }
                
            }
            pc.printf("up movement blocked\n");  
            speaker.period(1.0/100.0); // 500hz period
            speaker =0.5; //50% duty cycle - max volume
            wait(0.25);
            speaker=0.0; // turn off audio 
            Player.x = Player.px;
            Player.y = Player.py;
        }       
    }
    else{
        Player.x = Player.px;
        Player.y = Player.py-1;
        //Player.sec_map = 0;
    }    
    Player.jumping = 0;
}
void move_left()
{
    if(get_west(Player.x,Player.y)){
        if((get_west(Player.x,Player.y)->walkable != 0)||(Player.mode==true)){
            Player.x = Player.px-1;
            Player.y = Player.py;
        }    
        else{
            pc.printf("left movement blocked\n");
            speaker.period(1.0/100.0); // 500hz period
            speaker =0.5; //50% duty cycle - max volume
            wait(0.25);   
            speaker=0.0;
            Player.x = Player.px;
            Player.y = Player.py;
        }       
    }
    else{
        Player.x = Player.px-1;
        Player.y = Player.py;
    }         
    
}
void move_right()
{
    if(get_east(Player.x,Player.y)){
        if((get_east(Player.x,Player.y)->walkable != 0)||(Player.mode==true)){
            Player.x = Player.px+1;
            Player.y = Player.py;
        }    
        else{
            pc.printf("right movement blocked\n"); 
            speaker.period(1.0/100.0); // 500hz period
            speaker =0.5; //50% duty cycle - max volume
            wait(0.25);  
            speaker=0.0;
            Player.x = Player.px;
            Player.y = Player.py;
        }       
    }
    else{
        Player.x = Player.px+1;
        Player.y = Player.py;
    }    
}
void interact(double x, double y)
{
    if(get_north(x,y)){
        if(get_north(x,y)->interaction == 1){
            //pc.printf("dialogue available \n");
            talk_bud();
            Player.init = true;
        }
        else if(get_north(x,y)->interaction == 2){
            if((get_north(x,y)->type == ROSE)||(get_north(x,y)->type == VIOLET)){
                Player.flowers++;
            }
            pick_up_north(Player.x,Player.y -1);
        }
        else if(get_north(x,y)->interaction == 3){
            //pc.printf("dialogue available \n");
            if((Player.has_key == 1)||(Player.mode == true)){
                Player.init = true;
                door_();
            }
            //Player.init = true;
        }
        else if(get_north(x,y)->interaction == 4){
            //pc.printf("dialogue available \n");
            add_plant(x,y-1,Player.cb);
            speaker.period(1.0/650.0); // 500hz period
            speaker =0.5; //50% duty cycle - max volume
            wait(0.25);
            speaker=0.0;
            //Player.init = true;
        }
        else if(get_north(x,y)->interaction == 5){
            game_win();
        }
        else
            pc.printf("data is null \n");
    }
    else if(get_south(x,y)){
        if(get_south(x,y)->interaction == 1){
            talk_bud();
            Player.init = true;
        }
        else if(get_south(x,y)->interaction == 2){
            if((get_south(x,y)->type == ROSE)||(get_south(x,y)->type == VIOLET))
                Player.flowers++;
                //pc.printf("player has %d flowers\n", Player.flowers);
            pick_up_south(Player.x,Player.y+1);
        }
        else
            pc.printf("data is null \n");
    }
    else if(get_east(x,y)){
        if(get_east(x,y)->interaction == 1){
            talk_bud();
            Player.init = true;
        }
        else if(get_east(x,y)->interaction == 2){
            if((get_east(x,y)->type == ROSE)||(get_east(x,y)->type == VIOLET))
                Player.flowers++;
                //pc.printf("player has %d flowers\n", Player.flowers);
            pick_up_east(Player.x+1,Player.y);
        }
        else
            pc.printf("data is null \n");
    }
    else if(get_west(x,y)){
        if(get_west(x,y)->interaction == 1){
            talk_bud();
            Player.init = true;
        }
        else if(get_west(x,y)->interaction == 2){
            if((get_west(x,y)->type == ROSE)||(get_west(x,y)->type == VIOLET))
                Player.flowers++;
                //pc.printf("player has %d flowers\n", Player.flowers);
            pick_up_west(Player.x-1,Player.y);
        }
        else
            pc.printf("data is null \n");
    }
    
}
void open_menu()
{
    GameInputs inputs;
    //char* str;
    //uLCD.text_string("This is a test of string", 1, 4, FONT_7X8, WHITE);
    while(1){
        inputs = read_inputs();
        uLCD.filled_rectangle(0, 0, 130, 130, BLACK);
        //uLCD.background_color(BLACK);
        /*uLCD.text_string("Score: ", 6, 1, FONT_7X8, WHITE);
        sprintf(str, "%d", Player.score);
        uLCD.text_string(str, 9, 1, FONT_7X8, WHITE);*/
        uLCD.text_string("MENU \n >Press Action \nButton to Return\n >Press B4 \nto Quit \n >Press B3 to\nrestart game in\ncolorblind mode", 1, 4, FONT_7X8, WHITE);
        wait(0.3);
        if(inputs.b4 == 0){
            game_over();
            exit(1);
        }
        if(inputs.b1 == 0){
            draw_game(true);
            return;
        }
        if(inputs.b3 == 0){
            if(Player.cb)
                Player.cb = false;
            else
                Player.cb = true;
            init_main_map();
            draw_game(true);
            return;
        }
        //uLCD.text_string(">Press Menu Button to Return\n", 1, 4, FONT_7X8, WHITE);
        //uLCD.text_string(">Press Action Button to Quit \n", 1, 4, FONT_7X8, WHITE);
        
    }
}
void change_mode(){
    if(Player.mode){
        pc.printf("Omnipotent Mode Off\n");
        Player.mode = false;
    }
    else{
        pc.printf("Omnipotent Mode On\n");
        Player.mode = true;
    }
}
void talk_bud(){
    
    if((Player.has_key == 0)&&(Player.flowers == 0)){
        start_quest();
    }
    else if(Player.has_key == 0){
        quest_pt2();
        if(Player.flowers == 15){
            speech("You collected\nevery flower!","You got an\nextra life!");
            Player.lives++;
        } 
    }
    else{
        go_uga();
    }
}
void start_quest(){
    pc.printf("start quest\n");
    speech("Buzz! It's 4/20!\n Why did nobody\n tell me?","I had no time to \n get ready!");
    speech("Today's my\n anniversary!","Help me grab some \nof the flowers\n laying around");
}
void quest_pt2(){
    Player.has_key = 1;
    Player.score = 50*Player.flowers;
    /*char* scoreMsg;
    sprintf(scoreMsg,"+%d",Player.score);
    pc.printf(scoreMsg);*/
    speech("Thank you so much!\n I think you ","might be the \nperfect");
    speech("fit for a\n special job. ","");
    speech("u(sic)GA stole\n the T... ","and I need you\n to get it back!");
    speech("Take this key,!\n you'll need it. ","Let me know \nwhen you're ready \nto leave!");
    speech("You recieved\na special key! ","");
    speech("You can \nnow jump!","");
    //pc.printf("quest pt 2 \n");
}
void go_uga(){
    Player.init = true;
    if(Player.health != 0)
        speech("Good luck! ","");
    pc.printf("go to uga\n");
    set_active_map(1);
    if(Player.health != 0)
        maps_init();
    //pc.printf("map initialized \r\n");
    init_sec_map();
    //pc.printf("main map initialized \r\n");
    // Initialize game state
    pc.printf("setting player vars\n");
    Player.health = 5;
    Player.x = 10;
    Player.y = 18;
    Player.px = 10;
    Player.py = 18;
    pc.printf("game state initialized \r\n");
    // Initial drawing
    draw_game(Player.init);
    pc.printf("player health is %d player has %d lives", Player.health, Player.lives);
    if(Player.health != 0){
        speech("Oh no!\nThey found out our\nplan! Here","comes the \nfootball team!");
        speech("We can't leave the \nfeild! Get ready to\njump!","");
    }
}
void start_menu(){
    GameInputs inputs;
    while(1){
        inputs = read_inputs();
        uLCD.filled_rectangle(0, 0, 130, 130, BLACK);
        uLCD.filled_rectangle(15, 15, 115, 115, WHITE);

        uLCD.text_string("Between the Hedges \n >Hold Action \nButton to Start\n >Press B4 \nfor Help \n", 0, 4, FONT_7X8, WHITE);
        uLCD.text_string("\n\nMade by Cameron Allotey\nEce 2035 SP2018", 1, 9, FONT_7X8, WHITE);
        //pc.printf("b1 is %d", inputs.b1);
        wait(1);
        if(inputs.b1 == 0){
            
            return;
        }
        if(inputs.b4 == 0){
            help_screen();
            return;
        }
       
    }
}
void help_screen(){
    GameInputs inputs;
    while(1){
        inputs = read_inputs();
        uLCD.filled_rectangle(0, 0, 130, 130, BLACK);
        uLCD.filled_rectangle(15, 15, 115, 115, WHITE);

        uLCD.text_string("HELP \n Buttons (L2R)\n>Misc \n>Omni Mode\n>Menu \n>Action \n", 1, 4, FONT_7X8, WHITE);
        uLCD.text_string("\n\nMade by Cameron Allotey\nEce 2035 SP2018", 1, 9, FONT_7X8, WHITE);
        //pc.printf("b1 is %d", inputs.b1);
        wait(1);
        if(inputs.b1 == 0){
            
            return;
        }
    }
}

void game_over(){
    pc.printf("Game Over\n");
    GameInputs inputs;
    speaker.period(1.0/466.2); // 500hz period
    speaker =0.5; //50% duty cycle - max volume
    wait(0.4);
    speaker=0.0;
    speaker.period(1.0/440.0); // 500hz period
    speaker =0.5; //50% duty cycle - max volume
    wait(0.4);
    speaker=0.0;
    speaker.period(1.0/415.3); // 500hz period
    speaker =0.5; //50% duty cycle - max volume
    wait(0.5);
    speaker=0.0;
    while(1){
        inputs = read_inputs();
        uLCD.filled_rectangle(0, 0, 130, 130, BLACK);
        uLCD.filled_rectangle(15, 15, 115, 115, WHITE);

        uLCD.text_string("GAME OVER\n>Reset Button\n to restart\n>B1/B2 to quit", 1, 4, FONT_7X8, WHITE);
        uLCD.text_string("\n\nMade by Cameron Allotey\nEce 2035 SP2018", 1, 9, FONT_7X8, WHITE);
        //pc.printf("b1 is %d", inputs.b1);
        wait(1);
        if(inputs.b1 == 0){
            exit(1);
            //return;
        }
        if(inputs.b2 == 0){
            exit(1);
            //return;
        }
    }
}

void draw_upper_status()
{
    
    char str[30];
    char str2[30] = "X:   Y:        \n";
    char* str3;
    sprintf(str,"X: %d Y: %d\n",Player.x,Player.y);
    if(Player.mode == true)
        str3 = "Mode: O";
    else
        str3 = "Mode: N";
    //show coords
    // Draw bottom border of status bar
    //pc.printf("drawing upper status \r\n");
    uLCD.line(0, 9, 127, 9, GREEN);
    //uLCD.line(0, 0, 127, 0, GREEN);
    uLCD.text_string(str, 0, 0, FONT_7X8, WHITE);
    uLCD.text_string(str2, 0, 0, FONT_7X8, WHITE);
    uLCD.text_string(str3, 11, 0, FONT_7X8, WHITE);
    
    
    // Add other status info drawing code here
}

void draw_lower_status()
{
    //show score
    char str[30];
    char str3[10];
    //sprintf(str, "wow + %d", 10);
    sprintf(str,"Score:%d",Player.score);
    sprintf(str3,"Lives:%d",Player.lives);
    //pc.printf("drawing lower status \r\n");
    // Draw top border of status bar
    uLCD.line(0, 118, 127, 118, GREEN);
    //uLCD.line(0, 125, 127, 125, GREEN);
    uLCD.text_string(str, 0, 15, FONT_7X8, WHITE);
    uLCD.text_string(str3, 10, 15, FONT_7X8, WHITE);
    // Add other status info drawing code here
}
void init_sec_map()
{
    pc.printf("init secondary map! \r\n");
    Player.sec_map = 1;
    Map* map = set_active_map(1);
    pc.printf("after setting map!\r\n");
    for(int i = 0; i < map_width(); i ++)
    {
        for(int j = 0; j < map_height(); j++){
            if(i==14 || i == 6)
                add_sideline(i,j);
            else
                add_plant(i , j, Player.cb);
            if((i>6 && i<14)&&(j==8))
                add_baller(i,j);
        }
    }
    add_door(10,1);
    add_bus(10,18);
    add_bus(9,18);
    add_wall(0,              0,              HORIZONTAL, map_width());
    pc.printf("wall 1 made \r\n");
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    pc.printf("wall 2 made \r\n");
    add_wall(0,              0,              VERTICAL,   map_height());
    pc.printf("wall 3 made \r\n");
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    //add_plant(Player.x+1,Player.y+1);
    pc.printf("wall 4 made \r\n");
   // pc.printf("Walls done!\r\n");

     
    //print_map();
}
void door_(){
    
    speech("A door...in the \nmiddle of\na football field?","Might as well\ntry the key!");
    Player.has_key = false;
    set_active_map(2);
    maps_init();
    //pc.printf("map initialized \r\n");
    init_thrd_map();
    //pc.printf("main map initialized \r\n");
    // Initialize game state
    
    Player.x = 1;
    Player.y = 2;
    pc.printf("game state initialized \r\n");
    // Initial drawing
    draw_game(Player.init);
}
void init_thrd_map()
{
    Player.sec_map = 0;
    Map* map = set_active_map(2);
    
    add_wall(0,              0,              HORIZONTAL, map_width());
    pc.printf("wall 1 made \r\n");
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    pc.printf("wall 2 made \r\n");
    add_wall(0,              0,              VERTICAL,   map_height());
    pc.printf("wall 3 made \r\n");
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    //add_plant(Player.x+1,Player.y+1);
    pc.printf("wall 4 made \r\n");
   // pc.printf("Walls done!\r\n");
   add_T(2,2);
}
void jump(){
    Player.jumping = 1;
    pc.printf("Jumping\n");
    speaker.period(1.0/550.0); // 500hz period
    speaker =0.5; //50% duty cycle - max volume
    wait(0.25);
    speaker=0.0;
    speaker.period(1.0/850.0); // 500hz period
    speaker =0.5; //50% duty cycle - max volume
    wait(0.25);
    speaker=0.0;
    //wait(1);
    //Player.jumping = 0;
    //move_up();
    //Player.jumping = 0;
}
void game_win(){
    //pc.printf("Game Over\n");
    GameInputs inputs;
    speech("You found\nthe T!","Thanks for \nPlaying!");
    fight_song();
    while(1){
        inputs = read_inputs();
        uLCD.filled_rectangle(0, 0, 130, 130, BLACK);
        uLCD.filled_rectangle(15, 15, 115, 115, WHITE);

        uLCD.text_string("YOU WIN\n>B2 to restart\n>B3 to quit", 1, 4, FONT_7X8, WHITE);
        uLCD.text_string("\n\nMade by Cameron Allotey\nEce 2035 SP2018", 1, 9, FONT_7X8, WHITE);
        draw_lower_status();
        //pc.printf("b1 is %d", inputs.b1);
        wait(1);
        if(inputs.b2 == 0){
            main();
            //return;
        }
        if(inputs.b3 == 0){
            exit(1);
            //return;
        }
    }
}

void continue_screen(){
    
    GameInputs inputs;
    while(1){
        inputs = read_inputs();
        uLCD.filled_rectangle(0, 0, 130, 130, BLACK);
        uLCD.filled_rectangle(15, 15, 115, 115, WHITE);

        uLCD.text_string("Continue?\n>B2 to continue\n>B3 to quit", 1, 4, FONT_7X8, WHITE);
        uLCD.text_string("\n\nMade by Cameron Allotey\nEce 2035 SP2018", 1, 9, FONT_7X8, WHITE);
        draw_lower_status();
        //pc.printf("b1 is %d", inputs.b1);
        wait(1);
        if(inputs.b2 == 0){
            go_uga();
            Player.health = 5;
            return;
        }
        if(inputs.b3 == 0){
            exit(1);
            //return;
        }
    }
}
void fight_song(){
    speaker.period(1.0/698.46); // F
    speaker =0.5; //50% duty cycle - max volume
    wait(0.375);
    speaker=0.0;
    wait(0.125);
    speaker.period(1.0/587.33); // D
    speaker =0.5; //50% duty cycle - max volume
    wait(0.375);
    speaker=0.0;
    wait(0.125);
    speaker.period(1.0/523.25); // C
    speaker =0.5; //50% duty cycle - max volume
    wait(0.375);
    speaker=0.0;
    wait(0.125);
    speaker.period(1.0/440.); // A
    speaker =0.5; //50% duty cycle - max volume
    wait(0.375);
    speaker=0.0;
    wait(0.125);
    speaker.period(1.0/523.25); // C
    speaker =0.5; //50% duty cycle - max volume
    wait(0.167);
    speaker=0.0;
    speaker.period(1.0/587.33); // D
    speaker =0.5; //50% duty cycle - max volume
    wait(0.167);
    speaker=0.0;
    speaker.period(1.0/523.25); // C
    speaker =0.5; //50% duty cycle - max volume
    wait(0.167);
    speaker=0.0;
    speaker.period(1.0/466.16); // Bf
    speaker =0.5; //50% duty cycle - max volume
    wait(0.167);
    speaker=0.0;
    speaker.period(1.0/440.0); // Af
    speaker =0.5; //50% duty cycle - max volume
    wait(0.167);
    speaker=0.0;
    speaker.period(1.0/392.0); // G
    speaker =0.5; //50% duty cycle - max volume
    wait(0.167);
    speaker=0.0;
    speaker.period(1.0/350.0); // G
    speaker =0.5; //50% duty cycle - max volume
    wait(0.25);
    speaker=0.0;
    
}
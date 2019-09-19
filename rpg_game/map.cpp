#include "map.h"

#include "globals.h"
#include "graphics.h"

/**
 * The Map structure. This holds a HashTable for all the MapItems, along with
 * values for the width and height of the Map.
 */
struct Map {
    HashTable* items;
    int w, h;
};

/**
 * Storage area for the maps.
 * This is a global variable, but can only be access from this file because it
 * is static.
 */
static Map map;
static Map uga;
static Map t_room;
static int active_map;

/**
 * The first step in HashTable access for the map is turning the two-dimensional
 * key information (x, y) into a one-dimensional unsigned integer.
 * This function should uniquely map (x,y) onto the space of unsigned integers.
 */
static unsigned XY_KEY(int X, int Y) {
    //cantor pairing function
    X = abs(X);
    Y = abs(Y);
    
    //pc.printf("%d \r\n",(((X+Y)*(X+Y+1))/2 + Y));
    return (((X+Y)*(X+Y+1))/2 + Y);
    //return X | Y;
    //return map_width()*Y+X;
}

/**
 * This is the hash function actually passed into createHashTable. It takes an
 * unsigned key (the output of XY_KEY) and turns it into a hash value (some
 * small non-negative integer).
 */
unsigned map_hash(unsigned key)
{
    return key % map_area();
}

void maps_init()
{
    // TODO: Implement!    
    // Initialize hash table
    // Set width & height
    HashTable* items = createHashTable(map_hash,map_area());
    get_active_map()->items = items;
    get_active_map()->w = map_width();
    get_active_map()->h = map_height();
}
Map* get_map(int m){
    if(m==0)
        return &map;
    else if(m==1)
        return &uga;
    else if(m==2)
        return &t_room;
    else
        return &map;
}
Map* get_active_map()
{
    // There's only one map
    //pc.printf("%d \r\n", map.items);
    Map* active = get_map(active_map);
    return active;
}

Map* set_active_map(int m)
{
    active_map = m;
    return get_map(m);
}

void print_map()
{
    // As you add more types, you'll need to add more items to this array.
    char lookup[] = {'W', 'P'};
    for(int y = 0; y < map_height(); y++)
    {
        for (int x = 0; x < map_width(); x++)
        {
            //pc.printf("Print map!\r\n");
            MapItem* item = get_here(x,y);
            pc.printf("get_here returns %d \r\n", item);
            if (item) {
                pc.printf("%c", lookup[item->type]);
            }
            else {
                pc.printf(" ");
            }
        }
       pc.printf("\r\n");
    }
}

int map_width()
{
    if(active_map == 0)
        return 20;
    else if(active_map == 1)
        return 20;
    else
        return 6;
}

int map_height()
{
    if(active_map == 0)
        return 20;
    else if(active_map == 1)
        return 20;
    else
        return 6;
}

int map_area()
{
    return (map_width())*(map_height());
}

MapItem* get_north(int x, int y)
{
    int key,index;
    key = XY_KEY(x,y-1);
    index = map_hash(key);
    MapItem* val = (MapItem*)getItem(get_active_map()->items,key);
    if(val != NULL){
        
        return val;
        //return NULL;
        
    }
    else
        return NULL;
}

MapItem* get_south(int x, int y)
{
    int key,index;
    key = XY_KEY(x,y+1);
    index = map_hash(key);
    MapItem* val = (MapItem*)getItem(get_active_map()->items,key);
    if(val != NULL){
        return val;
        //return NULL;
        
    }
    else
        return NULL;
}

MapItem* get_east(int x, int y)
{
    int key,index;
    key = XY_KEY(x+1,y);
    index = map_hash(key);
    MapItem* val = (MapItem*)getItem(get_active_map()->items,key);
    if(val != NULL){
        
        return val;
        //return NULL;
        
    }
    else
        return NULL;
}

MapItem* get_west(int x, int y)
{
    int key,index;
    key = XY_KEY(x-1,y);
    index = map_hash(key);
    MapItem* val = (MapItem*)getItem(get_active_map()->items,key);
    if(val != NULL){
        
        return val;
        //return NULL;
        
    }
    else
        return NULL;
}

MapItem* get_here(int x, int y)
{
    //pc.printf("get_here called\r\n");
    int key,index;
    key = XY_KEY(x,y);
    index = map_hash(key);
    MapItem* val = (MapItem*)getItem(get_active_map()->items,key);
    //pc.printf("Hello, world!\r\n");
    if(val != NULL){
        //pc.printf("here object type is %d\n",val->type);
        //pc.printf("here object walkability is %d\n",val->walkable);
        return val;
        //return NULL;
        //TODO write code to find the entry (not value)
        //pc.printf("Hello, world!\r\n");
    }
    else{
        //pc.printf("get_here returned null \r\n");
        return NULL;
    }
}


void map_erase(int x, int y)
{
    int key,index;
    key = XY_KEY(x,y);
    index = map_hash(key);
    deleteItem(get_active_map()->items,key);
}

void add_wall(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++)
    {
        //pc.printf("loop %d \r\n", i);
        MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
        w1->type = WALL;
        w1->draw = draw_wall;
        w1->walkable = false;
        w1->data = NULL;
        w1->interaction = 0;
        //w1->seeable = 0;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        
        //MapItem* val = (MapItem*)insertItem(get_active_map()->items, key, w1);
        void* val = insertItem(get_active_map()->items, key, w1);
       
       
        if (val != NULL) {
           // pc.printf("if statement at end of add wall\r\n");
            free(val); // If something is already there, free it
           // pc.printf("repeat value freed \r\n");
           //deleteItem(get_active_map()->items, key);
        }
        //pc.printf("end of for loop reached \r\n");
    }
}

void add_plant(int x, int y, bool cb)
{
    //pc.printf("add plant function entered!\r\n");
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PLANT;
    if(cb==true){
       // pc.printf("Working?\n");
        w1->draw = draw_plant_cb;
    }
    else{
       // pc.printf("Not Working?\n");
        w1->draw = draw_plant;
    }
    w1->walkable = true;
    w1->data = NULL;
    w1->interaction = 0;
    //w1->seeable = 0;
    //pc.printf("Plant item made!\r\n");
    //MapItem* val = (MapItem*)insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    //pc.printf("Plant item inserted into hash table!\r\n");
    if (val != NULL) {
        free(val);
        //deleteItem(get_active_map()->items, XY_KEY(x, y));
    } // If something is already there, free it
}

void add_rose(int x, int y, bool cb)
{
    //pc.printf("add plant function entered!\r\n");
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = ROSE;
    if(cb==true)
        w1->draw = draw_rose_cb;
    else
        w1->draw = draw_rose;
    w1->walkable = true;
    w1->data = NULL;
    w1->interaction = 2;
    //w1->seeable = 0;
    //pc.printf("Plant item made!\r\n");
    //MapItem* val = (MapItem*)insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    //pc.printf("Plant item inserted into hash table!\r\n");
    if (val != NULL) {
        free(val);
        //deleteItem(get_active_map()->items, XY_KEY(x, y));
    } // If something is already there, free it
}

void add_violet(int x, int y, bool cb)
{
    //pc.printf("add plant function entered!\r\n");
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = VIOLET;
    if(cb==true)
        w1->draw = draw_violet_cb;
    else
        w1->draw = draw_violet;
    w1->walkable = true;
    w1->data = NULL;
    w1->interaction = 2;
    //w1->seeable = 0;
    //pc.printf("Plant item made!\r\n");
    //MapItem* val = (MapItem*)insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    //pc.printf("Plant item inserted into hash table!\r\n");
    if (val != NULL) {
        free(val);
        //deleteItem(get_active_map()->items, XY_KEY(x, y));
    } // If something is already there, free it
}

void add_bud(int x, int y)
{
    //pc.printf("add plant function entered!\r\n");
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = NPC;
    w1->draw = draw_bud;
    w1->walkable = false;
    w1->data = NULL;
    w1->interaction = 1;
    //w1->seeable = 0;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    //pc.printf("Plant item inserted into hash table!\r\n");
    if (val != NULL) {
        free(val);
        //deleteItem(get_active_map()->items, XY_KEY(x, y));
    } // If something is already there, free it
}

void pick_up_north(int x, int y){
    MapItem* item = (MapItem*)removeItem(get_active_map()->items, XY_KEY(x, y));
    speaker.period(1.0/250.0); // 500hz period
    speaker =0.5; //50% duty cycle - max volume
    wait(0.125);  
    speaker=0.0;
    speaker.period(1.0/500.0); // 500hz period
    speaker =0.5; //50% duty cycle - max volume
    wait(0.125);  
    speaker=0.0;
    speaker.period(1.0/300.0); // 500hz period
    speaker =0.5; //50% duty cycle - max volume
    wait(0.125);  
    speaker=0.0;
    if(item -> type == KEY){
        pc.printf("got the key\n");
    }
}
void pick_up_east(int x, int y){
    MapItem* item = (MapItem*)removeItem(get_active_map()->items, XY_KEY(x, y));
    speaker.period(1.0/250.0); // 500hz period
    speaker =0.5; //50% duty cycle - max volume
    wait(0.125);  
    speaker=0.0;
    speaker.period(1.0/500.0); // 500hz period
    speaker =0.5; //50% duty cycle - max volume
    wait(0.125);  
    speaker=0.0;
    speaker.period(1.0/300.0); // 500hz period
    speaker =0.5; //50% duty cycle - max volume
    wait(0.125);  
    speaker=0.0;
    if(item -> type == KEY){
        pc.printf("got the key\n");
    }
}

void pick_up_south(int x, int y){
    MapItem* item = (MapItem*)removeItem(get_active_map()->items, XY_KEY(x, y));
    speaker.period(1.0/250.0); // 500hz period
    speaker =0.5; //50% duty cycle - max volume
    wait(0.125);  
    speaker=0.0;
    speaker.period(1.0/500.0); // 500hz period
    speaker =0.5; //50% duty cycle - max volume
    wait(0.125);  
    speaker=0.0;
    speaker.period(1.0/300.0); // 500hz period
    speaker =0.5; //50% duty cycle - max volume
    wait(0.125);  
    speaker=0.0;
    if(item -> type == KEY){
        pc.printf("got the key\n");
    }
}

void pick_up_west(int x, int y){
    MapItem* item = (MapItem*)removeItem(get_active_map()->items, XY_KEY(x, y));
    speaker.period(1.0/250.0); // 500hz period
    speaker =0.5; //50% duty cycle - max volume
    wait(0.125);  
    speaker=0.0;
    speaker.period(1.0/500.0); // 500hz period
    speaker =0.5; //50% duty cycle - max volume
    wait(0.125);  
    speaker=0.0;
    speaker.period(1.0/300.0); // 500hz period
    speaker =0.5; //50% duty cycle - max volume
    wait(0.125);  
    speaker=0.0;
    if(item -> type == KEY){
        pc.printf("got the key\n");
    }
}


void add_sideline(int x, int y)
{
    //pc.printf("add plant function entered!\r\n");
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = SIDELINE;
    w1->draw = draw_sideline;
    w1->walkable = false;
    w1->data = NULL;
    w1->interaction = 0;
    //w1->seeable = 0;
    //w1->interaction = NULL;
    //pc.printf("Plant item made!\r\n");
    //MapItem* val = (MapItem*)insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    //pc.printf("Plant item inserted into hash table!\r\n");
    if (val != NULL) {
        free(val);
        //deleteItem(get_active_map()->items, XY_KEY(x, y));
    } // If something is already there, free it
}

void add_door(int x, int y)
{
    //pc.printf("add plant function entered!\r\n");
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = DOOR;
    w1->draw = draw_door;
    w1->walkable = false;
    w1->data = NULL;
    w1->interaction = 3;
    //w1->seeable = 0;
    //w1->interaction = NULL;
    //pc.printf("Plant item made!\r\n");
    //MapItem* val = (MapItem*)insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    //pc.printf("Plant item inserted into hash table!\r\n");
    if (val != NULL) {
        free(val);
        //deleteItem(get_active_map()->items, XY_KEY(x, y));
    } // If something is already there, free it
}
void add_baller(int x, int y)
{
    //pc.printf("add plant function entered!\r\n");
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = NPC;
    w1->draw = draw_baller;
    w1->walkable = false;
    w1->data = NULL;
    w1->interaction = 4;
    //w1->seeable = 0;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    //pc.printf("Plant item inserted into hash table!\r\n");
    if (val != NULL) {
        free(val);
        //deleteItem(get_active_map()->items, XY_KEY(x, y));
    } // If something is already there, free it
}

void add_T(int x, int y)
{
    //pc.printf("add plant function entered!\r\n");
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PRIZE;
    w1->draw = draw_T;
    w1->walkable = false;
    w1->data = NULL;
    w1->interaction = 5;
    //w1->seeable = 0;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    //pc.printf("Plant item inserted into hash table!\r\n");
    if (val != NULL) {
        free(val);
        //deleteItem(get_active_map()->items, XY_KEY(x, y));
    } // If something is already there, free it
}

void add_bus(int x, int y)
{
    //pc.printf("add plant function entered!\r\n");
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = BUS;
    w1->draw = draw_bus;
    w1->walkable = true;
    w1->data = NULL;
    w1->interaction = 0;
    //w1->seeable = 1;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    //pc.printf("Plant item inserted into hash table!\r\n");
    if (val != NULL) {
        free(val);
        //deleteItem(get_active_map()->items, XY_KEY(x, y));
    } // If something is already there, free it
}
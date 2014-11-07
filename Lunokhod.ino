//  Display.gfx_RectangleFilled(100,100, 200, 200, BLUE) ; 
//  gfx_Button(state, x, y, buttonColour, txtColour, font, txtWidth txtHeight, text)
//  Display.gfx_Slider(SLIDER_RAISED, 210, 100, 250,10, BLUE, 100, 50) ; // coordinates are different because we are in landscape mode
//  Display.gfx_ScreenMode(LANDSCAPE) ;
//  Display.touch_DetectRegion(100,100, 200, 200) ;
 //Display.gfx_MoveTo(30, 30);

#include <stdio.h>

//#define DEBUG

#define DisplaySerial Serial1 
#define HWLOGGING Serial // Serial port for debugging
#define RESETLINE 4
#define TEXTWIDTH 5
#define TEXTHEIGHT 5
#define BUTTON_H 70
#define BUTTON_W 230
#define BUTTON_SMALL_W 55
#define PADDING 12
#define OK_BUTTON_ROW 6
#define SHOOTPIN1 49
#define SHOOTPIN2 51
#define SHOOTPIN3 53

#define DELAY 10

#include "Picaso_Serial_4DLib.h"
#include "Picaso_Const4D.h"

Picaso_Serial_4DLib Display(&DisplaySerial);

int x;
int y;
char cstr[20];
String str;
int rowadd = 0;
int coladd = 0;
unsigned int num = 0;

unsigned int commands[100];
unsigned int duration[100];

int index;
void setup()
{
  //Missiles
  pinMode(SHOOTPIN1, OUTPUT);
  pinMode(SHOOTPIN2, OUTPUT);
  pinMode(SHOOTPIN3, OUTPUT);
  
  //MOTORS setup
  //Setup Channel A
  pinMode(12, OUTPUT); //Initiates Motor Channel A pin
  pinMode(9, OUTPUT); //Initiates Brake Channel A pin
  
  pinMode(13, OUTPUT); //Initiates Motor Channel A pin
  pinMode(8, OUTPUT); //Initiates Brake Channel A pin
  
//  
//  pinMode(RESETLINE, OUTPUT);  // Set D4 on Arduino to Output (4D Arduino Adaptor V2 - Display Reset)
//  digitalWrite(RESETLINE, 1);  // Reset the Display via D4
//  delay(100);
//  digitalWrite(RESETLINE, 0);  // unReset the Display via D4
//  delay(5000);
  DisplaySerial.begin(9600) ;
  Display.TimeLimit4D   = 5000 ; // 5 second timeout on all commands
  Display.gfx_Cls();
  Display.gfx_ScreenMode(PORTRAIT) ;
  Display.putstr("Lunakhod loading...") ;
//  HWLOGGING.begin(9600);
//  delay(5000); - uncomment in final
}

#define INIT_STATE 0
#define MOVE_STATE 1
#define ROTATE_STATE 2
#define GO_STATE 3
#define ERASE_STATE 4
#define KEYBOARD_STATE 5
#define SHOOT_STATE 6

#define MOVE 1
#define ROTATE_LEFT 2
#define ROTATE_RIGHT 3
#define SHOOT1 4
#define SHOOT2 5
#define SHOOT3 6

int current_command = 0;
int state = INIT_STATE;
void loop()
{
//    //forward @ full speed A
//  digitalWrite(12, HIGH); //Establishes forward direction of Channel A
//  digitalWrite(9, LOW);   //Disengage the Brake for Channel A
//  analogWrite(3, 255);   //Spins the motor on Channel A at full speed
//  
//    //forward @ full speed B
//  digitalWrite(13, LOW); //Establishes forward direction of Channel B
//  digitalWrite(8, LOW);   //Disengage the Brake for Channel B
//  analogWrite(11, 255);   //Spins the motor on Channel B at full speed
//  
//  delay(1000);
  
  Display.touch_Set(TOUCH_ENABLE) ;
  Display.touch_Set(TOUCH_REGIONDEFAULT) ;
  drawInitialScreen();
// Draw a button as a List Box (sunken)
//   Display.gfx_Selection(3, RED, YELLOW); // pre-select "Item3"
//  Display.gfx_Button(DOWN, 30, 30, GREEN, WHITE, FONT3, 1, 1, "Item1\nItem2\nItem3\nItem4");


  while(1)
  {
    do {} while (Display.touch_Get(TOUCH_STATUS) != TOUCH_PRESSED);
    x = Display.touch_Get(TOUCH_GETX);                          // so we can get the first point
    y = Display.touch_Get(TOUCH_GETY);
#if defined DEBUG    
   debugCoordinates();
#endif  
    HWLOGGING.print("X="); 
    HWLOGGING.print(x);  
    HWLOGGING.print("; Y=");
    HWLOGGING.println(y);
    
    switch(state)
    { 
      case INIT_STATE:
        if(insideRegion(0, 1, x, y, BUTTON_W)) // MOVE
        {
          HWLOGGING.print("MOVE");       
          current_command = MOVE;
          state = KEYBOARD_STATE;
          drawKeyboardScreen();
        }else if (insideRegion(0, 2, x, y, BUTTON_W)) // ROTATE
        {
           HWLOGGING.print("ROTATE");       
           state = ROTATE_STATE;
           drawRotateScreen();
        }else if (insideRegion(0, 3, x, y, BUTTON_W)) // SHOOT
        {
          HWLOGGING.print("SHOOT");       
          state = SHOOT_STATE;
          drawShootScreen();
        }else if (insideRegion(0, 5, x, y, BUTTON_W)) // GO
        {
          state = GO_STATE;
          HWLOGGING.print("GO");       
          go();
        }else if (insideRegion(0, 6, x, y, BUTTON_W)) // ERASE
        {
          state = ERASE_STATE;
          HWLOGGING.print("ERASE");       
          drawEraseScreen();
        }
        break;
    
      case KEYBOARD_STATE:
        if(insideRegion(0+coladd, 1+rowadd, x, y, BUTTON_SMALL_W)) // 1
        {
          add(1);
        }else if(insideRegion(1+coladd, 1+rowadd, x, y, BUTTON_SMALL_W)) // 2
        {
          add(2);
        }else if(insideRegion(2+coladd, 1+rowadd, x, y, BUTTON_SMALL_W)) // 3
        {
          add(3);
        }else if(insideRegion(0+coladd, 2+rowadd, x, y, BUTTON_SMALL_W)) // 4
        {
          add(4);
        }else if(insideRegion(1+coladd, 2+rowadd, x, y, BUTTON_SMALL_W)) // 5
        {
          add(5);
        }else if(insideRegion(2+coladd, 2+rowadd, x, y, BUTTON_SMALL_W)) // 6
        {
          add(6);
        }else if(insideRegion(0+coladd, 3+rowadd, x, y, BUTTON_SMALL_W)) // 7
        {
          add(7);
        }else if(insideRegion(1+coladd, 3+rowadd, x, y, BUTTON_SMALL_W)) // 8
        {
          add(8);
        }else if(insideRegion(2+coladd, 3+rowadd, x, y, BUTTON_SMALL_W)) // 9
        {
          add(9);
        }
        else if(insideRegion(1+coladd, 4+rowadd, x, y, BUTTON_SMALL_W)) // 0
        {
          add(0);
        }
        else if(insideRegion(0, OK_BUTTON_ROW, x, y, BUTTON_W)) // OK
        {
          //addCommand(MOVE);
          addCommand(current_command);
          state = INIT_STATE;
          drawInitialScreen();
        }
//        } else if(insideRegion(1, 10, x, y, BUTTON_W)) // OK
//        {
//          //TODO: Save
//          state = INIT_STATE;
//          drawInitialScreen();
//        }
      break;
      
      case ROTATE_STATE:
        if(insideRegion(0, 1, x, y, BUTTON_W)) // LEFT
          {
             current_command = ROTATE_LEFT;
             HWLOGGING.print("Left");       
             state = KEYBOARD_STATE;
             drawKeyboardScreen();
          }else if (insideRegion(0, 2, x, y, BUTTON_W)) // RIGHT
          {
            current_command = ROTATE_RIGHT;
            HWLOGGING.print("Right");       
            state = KEYBOARD_STATE;
            drawKeyboardScreen();
          }else if(insideRegion(0, OK_BUTTON_ROW, x, y, BUTTON_W)) // OK
          {
            state = INIT_STATE;
            drawInitialScreen();
          }
  //        } else if(insideRegion(1, 10, x, y, BUTTON_W)) // OK
  //        {
  //          //TODO: Save
  //          state = INIT_STATE;
  //          drawInitialScreen();
  //        }
     break;
     case GO_STATE:
        if (insideRegion(0, OK_BUTTON_ROW, x, y, BUTTON_W)) // GO
        {
          state = INIT_STATE;
          drawInitialScreen();
        }
     break;
     case ERASE_STATE:
       if (insideRegion(0, 5, x, y, BUTTON_W)) // BACK
        {
          state = INIT_STATE;
          drawInitialScreen();
        }else if (insideRegion(0, 6, x, y, BUTTON_W)) // ERASE
        {
          state = INIT_STATE;
          drawInitialScreen();
          index = 0;
        }
     break;
     case SHOOT_STATE:
        if(insideRegion(0, 1, x, y, BUTTON_W)) // SHOOT1
        {
          current_command = SHOOT1;
          state = KEYBOARD_STATE;
          drawKeyboardScreen();
        }else if (insideRegion(0, 2, x, y, BUTTON_W)) // SHOOT2
        {
          current_command = SHOOT2;
          state = KEYBOARD_STATE;
          drawKeyboardScreen();
        }else if (insideRegion(0, 3, x, y, BUTTON_W)) // SHOOT3
        {
          current_command = SHOOT3;
          state = KEYBOARD_STATE;
          drawKeyboardScreen();
        }
     break;
     }
    
    
    
  }
//    Display.touch_Set(TOUCH_REGIONDEFAULT) ;
}

void addCommand(int type)
{
  commands[index] = type;
  duration[index] = num;
  index++;
  num = 0;
}

void add(int n)
{
  HWLOGGING.print(num);       
  if( ((long)num * 10 + n) > 65535) return;
  num = num * 10 + n;
  drawNumber(); 
}

boolean insideRegion(int position_x, int position_y, int x, int y, int button_width)
{
  int x1 = PADDING * (position_x + 1) + position_x * button_width;
  int y1 = BUTTON_H * position_y;
  int x2 = PADDING * (position_x + 1) + button_width * (position_x + 1);
  int y2 = PADDING * (position_y + 1) + (BUTTON_H - 12) * (position_y + 1);
#if defined DEBUG      
  Display.gfx_Rectangle(x1, y1, x2, y2, RED);
  Display.gfx_Rectangle(x1, y1,x1+5,y1+5, WHITE);
  Display.gfx_PutPixel(x, y, PINK);
#endif

  if(x < x1 || x > x2) return false;
  if(y < y1 || y > y2) return false;
  return true;
}

void drawNumber()
{
  str = String(num);
  str.toCharArray(cstr,16);  
  Display.txt_Width(TEXTWIDTH);
  Display.txt_Height(TEXTHEIGHT);
  Display.txt_FontID(FONT3);
  Display.txt_MoveCursor(0,2); 
  Display.putstr(cstr) ;
}

void printint(int val)
{
  str = String(val);
  str.toCharArray(cstr,16);  
  Display.putstr(cstr) ;
}


void drawInitialScreen()
{
  Display.gfx_Cls();
  drawButton("MOVE",   0, 1, BUTTON_UP);
  drawButton("ROTATE", 0, 2, BUTTON_UP);
  drawButton("SHOOT",  0, 3, BUTTON_UP);
  drawButton("GO",     0, 5, BUTTON_UP);
  drawButton("ERASE",  0, 6, BUTTON_UP);
}

void drawShootScreen()
{
  Display.gfx_Cls();
  drawButton("TYPE 1", 0, 1, BUTTON_UP);
  drawButton("TYPE 2", 0, 2, BUTTON_UP);
  drawButton("TYPE 3", 0, 3, BUTTON_UP);
}

void drawEraseScreen()
{
  Display.gfx_Cls();
  drawButton("BACK",   0, 5, BUTTON_UP);
  drawButton("ERASE",  0, 6, BUTTON_UP);
}

void drawKeyboardScreen()
{
  num =0;
  add(0);
  Display.gfx_Cls();
  drawSmallButton("1", 0+coladd, 1+rowadd, BUTTON_UP);
  drawSmallButton("2", 1+coladd, 1+rowadd, BUTTON_UP);
  drawSmallButton("3", 2+coladd, 1+rowadd, BUTTON_UP);

  drawSmallButton("4", 0+coladd, 2+rowadd, BUTTON_UP);
  drawSmallButton("5", 1+coladd, 2+rowadd, BUTTON_UP);
  drawSmallButton("6", 2+coladd, 2+rowadd, BUTTON_UP);
  
  drawSmallButton("7", 0+coladd, 3+rowadd, BUTTON_UP);
  drawSmallButton("8", 1+coladd, 3+rowadd, BUTTON_UP);
  drawSmallButton("9", 2+coladd, 3+rowadd, BUTTON_UP);

  drawSmallButton("0", 1+coladd, 4+rowadd, BUTTON_UP);  
  okBackButtons();
}

void okBackButtons()
{
//  drawButton("BACK", 0, 6, BUTTON_UP);
  drawButton("OK", 0, OK_BUTTON_ROW, BUTTON_UP);
}
void drawRotateScreen()
{
   Display.gfx_Cls();
  drawButton("LEFT",  0, 1, BUTTON_UP);
  drawButton("RIGHT", 0, 2, BUTTON_UP);
//  drawButton("ROTATE 180 ",  0, 3, BUTTON_UP);
//  drawButton("ERACE",  0, 5, BUTTON_UP);
  okBackButtons();
}


void drawButton(char *text, unsigned int position_x, unsigned int position_y, int state)
{
  int x1 = PADDING * (position_x + 1) + position_x * BUTTON_W;
  int y1 = BUTTON_H * position_y;
  Display.gfx_Button(BUTTON_UP, x1, y1, ORANGE, WHITE, FONT3, TEXTWIDTH, TEXTHEIGHT, text);
}

void drawSmallButton(char *text, unsigned int position_x, unsigned int position_y, int state)
{
  int x1 = PADDING * (position_x + 1) + position_x * BUTTON_SMALL_W;
  int y1 = BUTTON_H * position_y;
  Display.gfx_Button(BUTTON_UP, x1, y1, ORANGE, WHITE, FONT3, TEXTWIDTH, TEXTHEIGHT, text);
}

void displayColors()
{
     //-------------Draw some buttons-------------------------
  Display.gfx_RectangleFilled(0,0,19,19,RED);           //
  Display.gfx_RectangleFilled(20,0,39,19,ORANGE);       //
  Display.gfx_RectangleFilled(40,0,59,19,YELLOW);       //
  Display.gfx_RectangleFilled(60,0,79,19,GREEN);        //
  Display.gfx_RectangleFilled(80,0,99,19,BLUE);         //
  Display.gfx_RectangleFilled(100,0,119,19,INDIGO);     //
  Display.gfx_RectangleFilled(120,0,139,19,PINK);       //
  Display.gfx_RectangleFilled(140,0,159,19,BROWN);      //
  Display.gfx_RectangleFilled(180,0,199,19,BLACK);      //
  Display.gfx_RectangleFilled(200,0,219,19,WHITE);      //
  Display.gfx_RectangleFilled(220,0,239,19,BLUE);    // display area for current color selection
  Display.gfx_Rectangle(260,0,279,19,RED);              //
  Display.gfx_Button(OFF,0,290,WHITE,RED,1,1,1,"Clear");   // I wish I how to use these buttons
}

void debugCoordinates()
{
  Display.txt_Width(1);
  Display.txt_Height(1);
  Display.txt_FontID(FONT1);
  Display.txt_MoveCursor(1,0);
  printint(x);
  Display.txt_MoveCursor(2,0);
  printint(y);
  Display.txt_Width(TEXTWIDTH);
  Display.txt_Height(TEXTHEIGHT);
}

void go()
{
  //TODO:go
  Display.gfx_Cls();
  drawButton("GO", 0, OK_BUTTON_ROW, BUTTON_DOWN);
  for (int i=0; i!= index; i++)
  {
    Display.txt_MoveCursor(i, 0);
    printCommand(i, commands[i], duration[i]);
    
    switch(commands[i])
    {
      case MOVE:
            HWLOGGING.println("move forward");
            move_forward(duration[i]);          

          break;
      case ROTATE_LEFT:
            HWLOGGING.println("rotate left");
            rotate_left(duration[i]);          
          break;
      case ROTATE_RIGHT:
            HWLOGGING.println("rotate right");      
            rotate_right(duration[i]);          
          break;          
      case SHOOT1:
          shoot(SHOOTPIN1, duration[i]);
          break;
      case SHOOT2:
          shoot(SHOOTPIN2, duration[i]);
          break;  
      case SHOOT3:
          shoot(SHOOTPIN3, duration[i]);
          break;        
    }
  }
}

void shoot(int shootPin, unsigned int duration)
{
  digitalWrite(shootPin, HIGH); //Establishes forward direction of Channel A
  delay(duration * DELAY);
  digitalWrite(shootPin, LOW);   //Disengage the Brake for Channel A
}

void printCommand(int i,unsigned int command, unsigned int duration)
{
  Display.txt_Width(2);
  Display.txt_Height(2);
  Display.txt_FontID(FONT3); 
  String c = commandName(command);
  c=String(i)+" "+c+" "+duration;
  c.toCharArray(cstr,16);  
  Display.putstr(cstr);
}

String commandName(unsigned int command)
{
  switch(command)
  {
    case MOVE:
      return "MOVE";
    case ROTATE_LEFT:
      return "LEFT";
    case ROTATE_RIGHT:
      return "RIGHT";
    case SHOOT1:
      return "SHOOT1";
    case SHOOT2:
      return "SHOOT2";
    case SHOOT3:
      return "SHOOT3";      
  }
}

void move_forward(unsigned int duration)
{
  //forward @ full speed A
  digitalWrite(12, HIGH); //Establishes forward direction of Channel A
  digitalWrite(9, LOW);   //Disengage the Brake for Channel A
  analogWrite(3, 255);   //Spins the motor on Channel A at full speed
  
    //forward @ full speed B
  digitalWrite(13, LOW); //Establishes forward direction of Channel B
  digitalWrite(8, LOW);   //Disengage the Brake for Channel B
  analogWrite(11, 255);   //Spins the motor on Channel B at full speed
  
  delay(duration * DELAY);
  stop();
}

void rotate_right(unsigned int duration)
{
  //forward @ full speed B
  digitalWrite(13, LOW); //Establishes forward direction of Channel B
  digitalWrite(8, LOW);   //Disengage the Brake for Channel B
  analogWrite(11, 255);   //Spins the motor on Channel B at full speed
  
  delay(duration * DELAY);
  stop();
}

void rotate_left(unsigned int duration)
{
  digitalWrite(12, HIGH); //Establishes forward direction of Channel A
  digitalWrite(9, LOW);   //Disengage the Brake for Channel A
  analogWrite(3, 255);   //Spins the motor on Channel A at full speed
  delay(duration * DELAY);
  stop();
}

void stop()
{
  digitalWrite(9, HIGH);   //Disengage the Brake for Channel A
  digitalWrite(8, HIGH);   //Disengage the Brake for Channel B
}

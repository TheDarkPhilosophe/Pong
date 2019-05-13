#include <Adafruit_TFTLCD.h> 
#include <Adafruit_GFX.h>    
#include <TouchScreen.h>

#define LCD_CS A3 
#define LCD_CD A2 
#define LCD_WR A1
#define LCD_RD A0 
#define LCD_RESET A4 

#define TS_MINX 122
#define TS_MINY 111
#define TS_MAXX 942
#define TS_MAXY 890

#define YP A3
#define XM A2
#define YM 9
#define XP 8

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 364);

int maxY=240;
int maxX=320;
boolean buttonEnabled = true;
double palletteY1=100;
double palletteX1=0;
double palletteY2=100;
double palletteX2=310;
int palletteWidht=10;
int palletteHeight=40;
double ballX=152.5;
double ballY=117.5;
int ballsize=7;
int oldPalletteY1=palletteY1;
int oldPalletteY2=palletteY2;
int vitessePallette=1;
int vitessePalletteY1=vitessePallette;// c'est juste pour les test il y a une seule vitesse a la fin
int vitessePalletteY2=-vitessePallette;
double oldBallX=ballX;
double oldBallY=ballY;
double vitesseBall=1;
double vitesseBallX=vitesseBall;
double vitesseBallY=0;

int gameStatus = 0;  //Si 0->Menu, 1->Jeu, 2->Termine
int scorePl1=0;     
int scorePl2=0;    

void setup() {
  
  tft.reset();
  uint16_t identifier = tft.readID();
  tft.begin(identifier);
  tft.setRotation(3);
  tft.fillScreen(BLACK);
  //tft.drawRect(0,0,320,240,WHITE);
  
  tft.setCursor(30,40);
  tft.setTextColor(WHITE);
  tft.setTextSize(8);
  tft.print("PONG !");
  
  tft.setCursor(120,120);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("Options");
  
  tft.setCursor(120,150);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("Credits");
  
  tft.fillRect(110,180, 100, 40, RED);
  //tft.drawRect(50,180,210,40,BLACK);
  tft.setCursor(131,190);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("Jouer");

  tft.fillScreen(BLACK);
  //tft.drawRect(maxX/2,0,1,maxY,WHITE);
  tft.fillRect(palletteX1,palletteY1, palletteWidht, palletteHeight, WHITE);//x,y,largeur,hauteur
  tft.fillRect(palletteX2,palletteY2, palletteWidht, palletteHeight, WHITE);//x,y,largeur,hauteur
  tft.fillRect(ballX,ballY, ballsize, ballsize, WHITE);//x,y,largeur,hauteur
  tft.setCursor(50,0);
  tft.setTextColor(WHITE);
  tft.setTextSize(4);
  tft.print(scorePl1);
  tft.setCursor(250,0);
  tft.setTextColor(WHITE);
  tft.setTextSize(4);
  tft.print(scorePl2);
  randomSeed(analogRead(53));

  Serial.begin(9600);

}

void loop() {
    //boolean game=launchGame();
    //Serial.println("\nX = "); Serial.print(game);
    /*TSPoint p = ts.getPoint();
  
    if (p.z > ts.pressureThreshhold) {
      
     p.x = map(p.x, TS_MAXX, TS_MINX, 0, 320);
     p.y = map(p.y, TS_MAXY, TS_MINY, 0, 240);
  
     Serial.println("\nX = "); Serial.print(p.x);
     Serial.println("\nY = "); Serial.print(p.y);
         
     if(p.x>30 && p.x<60 && p.y>120 && p.y<130 && buttonEnabled){//mesure sur l'ecran
        tft.fillScreen(BLACK);
        //tft.drawRect(0,0,319,240,YELLOW);
        tft.fillRect(palletteX1,palletteY1, palletteWidht, palletteHeight, WHITE);//x,y,largeur,hauteur
        tft.fillRect(palletteX2,palletteY2, palletteWidht, palletteHeight, WHITE);//x,y,largeur,hauteur
        tft.fillRect(ballX,ballY, ballsize, ballsize, WHITE);//x,y,largeur,hauteur
        buttonEnabled = false;
        //return true;
        }}*/
  
    //if(game){
      //buttonEnabled = false;
    //}

    buttonEnabled = false;
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    tft.drawRect(maxX/2-0.5,0,1,maxY,WHITE);//trait du milieu vertical
    
    if(buttonEnabled==false){
      palletteY1+=vitessePalletteY1;
      palletteY2+=vitessePalletteY2;
      ballX+=vitesseBallX;
      ballY+=vitesseBallY;
      //test si la balle touche une pallette
      if(ballX+2*ballsize>maxX-palletteWidht&&ballY>palletteY2&&ballY<palletteY2+palletteHeight||ballX<2*palletteWidht&&ballY>palletteY1&&ballY<palletteY1+palletteHeight){
        vitesseBallX*=-1;
        vitesseBallY=random(-1, 1);
        Serial.println("\nVitesse bale Y = "); Serial.print(vitesseBallY);
        //calcul de l'angle de retour
      }
      //test si la balle touche le haut ou le bas de l'ecran
      if(ballY>maxY-ballsize||ballY<0){
        vitesseBallY*=-1;
        Serial.println("\nVitesse bale Y = "); Serial.print(vitesseBallY);
        tft.fillRect(50, 0, 100, 60 , BLACK);//x,y,larg,haut
        tft.setCursor(50,0);
        tft.setTextColor(WHITE);
        tft.setTextSize(4);
        tft.print(scorePl1);
        tft.fillRect(200, 0, 100, 60 , BLACK);
        tft.setCursor(200,0);
        tft.setTextColor(WHITE);
        tft.setTextSize(4);
        tft.print(scorePl2);
        //calcul de l'angle de retour
      }
      //test si la balle dépasse le bord droit ou gauche
      if(ballX<palletteWidht){
        ballX=maxX/2;
        scorePl1+=1;
        tft.fillRect(50, 0, 100, 60 , BLACK);//x,y,larg,haut
        tft.setCursor(50,0);
        tft.setTextColor(WHITE);
        tft.setTextSize(4);
        tft.print(scorePl1);
        //calcul de l'angle de retour
      }
      if(ballX+ballsize>maxX-palletteWidht){
        ballX=maxX/2;
        scorePl2+=1;
        tft.fillRect(200, 0, 100, 60 , BLACK);
        tft.setCursor(200,0);
        tft.setTextColor(WHITE);
        tft.setTextSize(4);
        tft.print(scorePl2);
        //calcul de l'angle de retour
      }
      if(palletteY1+palletteHeight>maxY||palletteY1<0){
        vitessePalletteY1*=-1;
        //vitessePallette=0;
      }
      if(palletteY2+palletteHeight>maxY||palletteY2<0){
        vitessePalletteY2*=-1;
        //vitessePallette=0;
      }
    }
    //tft.drawFastHLine(155,240,1,WHITE);//ligne vertical a corriger  
    //affichage propre sans cliniontement de la balle
    if(ballX!=oldBallX||ballY!=oldBallY){
      
      tft.fillRect(oldBallX,oldBallY, vitesseBallX, vitesseBallY, BLACK);//x,y,largeur,hauteur
      //nettoyage en y
      tft.fillRect(ballX,ballY-ballsize, ballsize, ballsize, BLACK);//x,y,largeur,hauteur
      tft.fillRect(ballX,ballY+ballsize, ballsize, ballsize, BLACK);//x,y,largeur,hauteur
      //tft.fillRect(oldBallX-vitesseBallX,oldBallY-vitesseBallY, ballsize, ballsize, BLACK);//x,y,largeur,hauteur
      //nettoyage en x
      tft.fillRect(ballX-ballsize,ballY, ballsize, ballsize, BLACK);//x,y,largeur,hauteur
      tft.fillRect(ballX+ballsize,ballY, ballsize, ballsize, BLACK);//x,y,largeur,hauteur
      oldBallX=ballX;
      oldBallY=ballY;
      
      //tft.fillRect(ballX,ballY, ballsize+vitesseBallX, ballsize+vitesseBallY, BLACK);//x,y,largeur,hauteur
      tft.fillRect(ballX,ballY, ballsize, ballsize, WHITE);//x,y,largeur,hauteur
    }
    //affichage propre sans cliniontement des pallette
    if(palletteY1!=oldPalletteY1){
      tft.fillRect(palletteX1,oldPalletteY1, palletteWidht, vitessePalletteY1, BLACK);//x,y,largeur,hauteur
      oldPalletteY1=palletteY1;
      tft.fillRect(palletteX1,palletteY1+palletteHeight, palletteWidht, palletteHeight, BLACK);//x,y,largeur,hauteur
      tft.fillRect(palletteX1,palletteY1, palletteWidht, palletteHeight, WHITE);//x,y,largeur,hauteur
      
    }
    if(palletteY2!=oldPalletteY2){
      tft.fillRect(palletteX2,oldPalletteY2, palletteWidht, vitessePalletteY2, BLACK);//x,y,largeur,hauteur
      oldPalletteY2=palletteY2;
      tft.fillRect(palletteX2,palletteY2+palletteHeight, palletteWidht, palletteHeight, BLACK);//x,y,largeur,hauteur
      tft.fillRect(palletteX2,palletteY2, palletteWidht, palletteHeight, WHITE);//x,y,largeur,hauteur
      
    }
    
    //delay(500);
  }

boolean launchGame(){
  TSPoint p = ts.getPoint();
  
  if (p.z > ts.pressureThreshhold) {
    
   p.x = map(p.x, TS_MAXX, TS_MINX, 0, 320);
   p.y = map(p.y, TS_MAXY, TS_MINY, 0, 240);

   Serial.println("\nX = "); Serial.print(p.x);
   Serial.println("\nY = "); Serial.print(p.y);
       
   if(p.x>30 && p.x<60 && p.y>120 && p.y<130 && buttonEnabled){//mesure sur l'ecran
      tft.fillScreen(BLACK);
      //tft.drawRect(0,0,319,240,YELLOW);
      tft.fillRect(palletteX1,palletteY1, palletteWidht, palletteHeight, WHITE);//x,y,largeur,hauteur
      tft.fillRect(palletteX2,palletteY2, palletteWidht, palletteHeight, WHITE);//x,y,largeur,hauteur
      tft.fillRect(ballX,ballY, ballsize, ballsize, WHITE);//x,y,largeur,hauteur
      return true;
      }
}}

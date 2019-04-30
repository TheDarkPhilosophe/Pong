#include <Wire.h>
#include <Adafruit_GFX.h>          //Adrafruit GFX(librairie)
#include <Adafruit_SSD1306.h>      //Librairie pour écran
#define OLED_RESET 4


//gestion joystick


//Définition des entrées/sorties utilisées
int controleur1=2;               //Analogique 2
int controleur2=3;               //Analogique 3
int led=5;                        //Digital 5
int btn=6;                        //Digital 6

//Définition des variables
int gameStatus =0;                //Si 0->Menu, 1->Jeu, 2->Termine
int buttonStatus=0;               //HIGH = Presse

int controleurValue1=0;           //Sauvegarde la valeur venant du potentiometre
int controleurValue2=0;           //Sauvegarde la valeur venant du potentiometre

int positionPlayer1=0;            //Position joueur 1
int positionPlayer2=0;            //Position joueur 2

int scorePl1;                     //Score joueur 1
int scorePl2;                     //Score joueur 2

int ballX=128/2;                  //Position balle si écran 128X64
int ballY=64/2;                   //Position balle si écran 128X64

int ballSpeedX=2;
int ballSpeedY=1;

Adafruit_SSD1306 display(OLED_RESET);

void setup() {
  // put your setup code here, to run once:
   pinMode(led, OUTPUT);
   pinMode(btn, INPUT);
   Serial.begin(9600);
   display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
   display.clearDisplay();
   digitalWrite(led, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  controleurValue1=analogRead(controleur1);//Lecture potentiometre
  controleurValue2=analogRead(controleur2);//Lecture potentiometre

  positionPlayer1=controleurValue1*(46/1023);
  positionPlayer2=controleurValue2*(46/1023);

  buttonStatus=digitalRead(btn);    //Initialisation du statut

  //Lancement d'une partie
  if(buttonStatus==HIGH && gameStatus==0){
    gameStatus=1;
    delay(100);
  }
  //Remise à zéro du jeu
  else if(buttonStatus == HIGH && (gameStatus==1 || gameStatus==2)){
    gameStatus=0;
    scorePl1=0;
    scorePl2=0;
    ballX=128/2;
    ballY=64/2;
    delay(100);
  }

  //Création du menu
  if(gameStatus==0){
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(40,18);
    display.println("PONG");
    display.setTextSize(1);
    display.setCursor(32,38);
    display.println("Appuyez !");
    display.display();
    display.clearDisplay();
  }

  if(gameStatus==1){
    terrain(scorePl1,scorePl2);
  
    collision();
    ball();

    display.display();
    display.clearDisplay();
  }

  if(gameStatus == 2){
    terrain(scorePl1,scorePl2);

    display.setTextSize(1);
    display.setTextColor(WHITE);

    if (scorePl1 == 2) {
      display.setCursor(15, 30);
    } else if (scorePl2 == 2) {
      display.setCursor(77, 30);
    }
    display.println("Gagné !");
    display.display();
    display.clearDisplay();  
  }
}

//Collision(score)
void collision(){
  //Si touche haut ou bas
  if(ballY >= 62 || ballY <=0){
    ballSpeedY = ballSpeedY*(-1);
  }
  //Score si la balle est derriere le joueur
  if(ballX >= 126 || ballX <= 0){
    if(ballSpeedX > 0){
      scorePl1+=1;
      ballX=128/4;
    }
      if (ballSpeedX < 0) {
      scorePl2++;
      ballX = 128 / 4 * 3;
    }  
    if (scorePl1 == 2 || scorePl2 == 2) {
      gameStatus = 2;
    }
  }
    if (ballX >= 0 && ballX <= 2 && ballSpeedX < 0) {
    if (ballY > round(positionPlayer1) - 2 && ballY < round(positionPlayer1) + 18) {
      ballSpeedX *= -1;
    }
  }
 
  if (ballX >= 128-2-2 && ballX <= 128-2 && ballSpeedX > 0) {
    if (ballY > round(positionPlayer2) - 2 && ballY < round(positionPlayer2) + 18) {
      ballSpeedX *= -1;
    }

  }
}

void terrain(int sc1, int sc2){
  //Dessin du rectangle joueur
  display.fillRect(0,round(positionPlayer1),2,18,1);
  display.fillRect(126,round(positionPlayer1),2,18,1);

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(55, 0);
    display.print(scorePl1);
    display.print(":");
    display.print(scorePl2);

    display.fillRect(63, 12, 1, 5, 1);
    display.fillRect(63, 22, 1, 5, 1);
    display.fillRect(63, 32, 1, 5, 1);
    display.fillRect(63, 42, 1, 5, 1);
    display.fillRect(63, 52, 1, 5, 1);
    display.fillRect(63, 62, 1, 5, 1);
}

//Dessin de la balle
void ball(){
  display.fillRect(ballX,ballY,2,2,1);
  ballX+=ballSpeedX;
  ballY+=ballSpeedY;
}

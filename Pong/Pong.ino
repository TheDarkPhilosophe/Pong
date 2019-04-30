#include <Wire.h>
#include <Adafruit_GFX.h>         //Adrafruit GFX(librairie)
#include <Adafruit_SSD1306.h>     //Librairie pour écran
#define OLED_RESET 4

//Définition des entrées/sorties utilisées
// int controleur1=2;             //Analogique 2
// int controleur2=3;             //Analogique 3
int led=5;                        //Digital 5
int btn=6;                        //Digital 6

// Arduino pin Joysitck 1
const int SW1_pin = 2; // digital pin connected to switch output (bouton poussoir)
//const int X1_pin = 0; // analog pin connected to X output
const int Y1_pin = 8; // analog pin connected to Y output

// Arduino pin Joysitck 2
const int SW2_pin = 3; // digital pin connected to switch output (bouton poussoir)
//const int X2_pin = 0; // analog pin connected to X output
const int Y2_pin = 9; // analog pin connected to Y output

// Definition du materiel utilise
int tailleEcranX = 320;           // si écran 128X64
int tailleEcranY = 240;

//Définition des variables
int gameStatus =0;                //Si 0->Menu, 1->Jeu, 2->Termine
int buttonStatus=0;               //HIGH = Presse

int controleurValue1 = 520;
int controleurValue2 = 520;
int milieuJoystick = 512;          //Sauvegarde la valeur venant du potentiometre
            
float playerSpeed = 0.05;
int playerWidth = 10;
int positionPlayer1=tailleEcranY/2;//Position joueur 1
int positionPlayer2=tailleEcranY/2;//Position joueur 2

int scorePl1;                     //Score joueur 1
int scorePl2;                     //Score joueur 2
int scoreMax = 2;

int ballX=tailleEcranX/2;         //Position balle en X en fonction de la taille de l'ecran
int ballY=tailleEcranY/2;         //Position balle en Y

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
  controleurValue1=analogRead(Y1_pin);  //Lecture potentiometre
  controleurValue2=analogRead(Y2_pin);  //Lecture potentiometre

  if (controleurValue1<470 || controleurValue1>520) {  // valeur de repos du joystick -> on se trouve en dehors de cet interval
    int newPositionPlayer1 = positionPlayer1 + (int)(((float)controleurValue1-milieuJoystick)*playerSpeed);
    Serial.print("Y-axis1: ");
    Serial.println(controleurValue1);
    if (newPositionPlayer1>playerWidth/2 && newPositionPlayer1<tailleEcranY-playerWidth/2) {
      positionPlayer1 = newPositionPlayer1;
    }
    else {
      if (newPositionPlayer1<playerWidth/2) {
        positionPlayer1 = playerWidth/2;
      }
      else {
        positionPlayer1 = tailleEcranY-playerWidth/2;
      }
    }
    Serial.print("\n");
    Serial.print("player1 pos: ");
    Serial.println(positionPlayer1);
    Serial.print("\n\n");
  }

  if (controleurValue2<470 || controleurValue2>520) {  // valeur de repos du joystick -> on se trouve en dehors de cet interval
    int newPositionPlayer2 = positionPlayer2 + (int)(((float)controleurValue2-milieuJoystick)*playerSpeed);
    Serial.print("Y-axis2: ");
    Serial.println(controleurValue2);
    if (newPositionPlayer2>playerWidth/2 && newPositionPlayer2<tailleEcranY-playerWidth/2) {
      positionPlayer2 = newPositionPlayer2;
    }
    else {
      if (newPositionPlayer2<playerWidth/2) {
        positionPlayer2 = playerWidth/2;
      }
      else {
        positionPlayer2 = tailleEcranY-playerWidth/2;
      }
    }
    Serial.print("\n");
    Serial.print("player2 pos: ");
    Serial.println(positionPlayer2);
    Serial.print("\n\n");
  }

  buttonStatus=digitalRead(btn);             //Initialisation du statut

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
    ballX=tailleEcranX/2;
    ballY=tailleEcranY/2;
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

  if(gameStatus==1){  // si le jeu est en cours
    terrain(scorePl1,scorePl2);
  
    collision();
    ball();

    display.display();
    display.clearDisplay();
  }

  if(gameStatus == 2){  // correspond a la victoire de l'un des joueurs
    terrain(scorePl1,scorePl2);

    display.setTextSize(1);
    display.setTextColor(WHITE);

    if (scorePl1 == scoreMax) {  // le joueur 1 gagne
      int positionTexte = tailleEcranX/4-4;
      if (positionTexte < 0) {
        positionTexte = 0;
      }
      display.setCursor(positionTexte, tailleEcranY/2-1);  // de base (15, 30) pour un ecran (128, 64)
    
    } else if (scorePl2 == scoreMax) {  // le joueur 2 gagne
      int positionTexte = tailleEcranX/4*3-3;
      if (positionTexte + 7 >tailleEcranX) {  // le 7 correspond a la taille du message
        positionTexte = tailleEcranX-7;
      }
      display.setCursor(positionTexte, tailleEcranY/2-1);  // de base (77, 30) pour un ecran (128, 64)
    }
    display.println("Gagné !");
    display.display();
    display.clearDisplay();  
  }
}

//Collision(score)
void collision(){
  //Si touche haut ou bas
  if(ballY >= 62 || ballY <=0){  // TODO pourquoi pas <= 2 si on prend une marge
    ballSpeedY *= -1;
  }
  //Score si la balle est derriere le joueur
  if(ballX >= 126 || ballX <= 0){
    if(ballSpeedX > 0){
      scorePl1+=1;
      ballX=tailleEcranX/4;
    }
      if (ballSpeedX < 0) {
      scorePl2++;
      ballX = tailleEcranX / 4 * 3;
    }  
    if (scorePl1 == scoreMax || scorePl2 == scoreMax) {
      gameStatus = 2;
    }
  }
    if (ballX >= 0 && ballX <= 2 && ballSpeedX < 0) {
    if (ballY > round(positionPlayer1) - 2 && ballY < round(positionPlayer1) + 18) {  // TODO c'est quoi round ?
      ballSpeedX *= -1;
    }
  }
 
  if (ballX >= 128-2 && ballX <= 128-2 && ballSpeedX > 0) {
    if (ballY > round(positionPlayer2) - 2 && ballY < round(positionPlayer2) + 18) {
      ballSpeedX *= -1;
    }
  }
}

void terrain(int sc1, int sc2){
  //Dessin du rectangle joueur
  display.fillRect(0,round(positionPlayer1),2,18,1);
  display.fillRect(126,round(positionPlayer2),2,18,1);

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

//Dessin de la balle et deplacement
void ball(){
  display.fillRect(ballX,ballY,2,2,1);
  ballX+=ballSpeedX;
  ballY+=ballSpeedY;
}

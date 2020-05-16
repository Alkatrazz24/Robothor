#include <Servo.h>

int pos = 90; 

/*
 * Notre robot se déplace presque comme un humain
 * Tout d'abord on lui fait lever une jambe d'un des deux côtés
 * On fait ensuite avancer cette jambes et on la repose 
 * On répète l'action avec la jambe opposé
 * Le servo_1 gère le lever de jambes 
 * Le servo_2 gère le déplacement des jambes
 * La majorité des delay sont à 1500 pour pouvoir bien comprendre le programme lors de la simulation
 * Dans une optique de sortir le plus rapidement du labyrinthe ils seraient bien évidemment écourtés 
 */
/* Constantes pour les broches */

/*codage des 3 capteurs ultrasons */
/* Constantes pour les broches */
const byte T1 = 7; // Broche TRIGGER
const byte E1 = 6;    // Broche ECHO
const byte T2 = 9; // Broche TRIGGER
const byte E2 = 8;    // Broche ECHO
const byte T3 = 11; // Broche TRIGGER
const byte E3 = 10;    // Broche ECHO

/* Constantes pour le timeout */
const unsigned long MEASURE_TIMEOUT = 25000UL; // 25ms = ~8m à 340m/s

/* Vitesse du son dans l'air en mm/us */
const float SOUND_SPEED = 340.0 / 1000;

Servo servo_1;
Servo servo_2;

// INITIALISATION
// Seuil obtenu avec le calibrage
int seuil = 955;
// Etat des capteurs
int s_avant = 0;
int s_droite = 0;
int s_gauche = 0;
// Variable comptant les changements de direction
int compteur =0;
// Durees (en ms) des virages a 90 degres
int t_90_gauche = 3325;
int t_90_droite = 3870;
// Duree (en ms) des petits virages du suivi de ligne
int t_micro = 700;
// Durees (en ms) pour avancer avant les virages
int t_long = 2000;
int t_moyen = 1500;
int t_court = 1000;
// Vitesse de rotation
//( nombre de rotations par minute)
int i_90 = 60;
int i_avancer = 20;

bool est_a_droite_de_la_ligne;

void setup()
{
  Serial.begin(115200);
// capteur 1 //
  pinMode(T1, OUTPUT);
  digitalWrite(T1, LOW);
  pinMode(E1, INPUT);
 
// capteur 2 //
  pinMode(T2, OUTPUT);
  digitalWrite(T2, LOW); 
  pinMode(E2, INPUT);
 // capteur 3 //
  pinMode(T3, OUTPUT);
  digitalWrite(T3, LOW);
  pinMode(E3, INPUT);
  
  servo_1.attach(2);
  servo_2.attach(3);

}

void loop()
{
  sortir_labyrinthe();
}


int distance_gauche(){
  /* 1. Lance une mesure de distance en envoyant une impulsion HIGH de 10µs sur la broche TRIGGER */
  digitalWrite(T1, HIGH);
  delayMicroseconds(10);
  digitalWrite(T1, LOW);
  
  /* 2. Mesure le temps entre l'envoi de l'impulsion ultrasonique et son écho (si il existe) */
  long measure = pulseIn(E1, HIGH, MEASURE_TIMEOUT);
   
  /* 3. Calcul la distance à partir du temps mesuré */
  float distance1 = measure / 2.0 * SOUND_SPEED;
   
  Serial.print(F("Distance: "));
  Serial.print(distance1 / 10.0, 2);
  Serial.print(F("cm, "));
  
  return distance1;
}
int distance_devant(){
 
  digitalWrite(T2, HIGH);
  delayMicroseconds(10);
  digitalWrite(T2, LOW);
  
  
  long measure = pulseIn(E2, HIGH, MEASURE_TIMEOUT);
  float distance2 = measure / 2.0 * SOUND_SPEED;
   
  Serial.print(F("Distance: "));
  Serial.print(distance2 / 10.0, 2);
  Serial.print(F("cm, "));
  
  return distance2;
}
int distance_droite(){
  /* 1. Lance une mesure de distance en envoyant une impulsion HIGH de 10µs sur la broche TRIGGER */
  digitalWrite(T3, HIGH);
  delayMicroseconds(10);
  digitalWrite(T3, LOW);
  
  /* 2. Mesure le temps entre l'envoi de l'impulsion ultrasonique et son écho (si il existe) */
  long measure = pulseIn(E3, HIGH, MEASURE_TIMEOUT);
   
  /* 3. Calcul la distance à partir du temps mesuré */
  float distance3 = measure / 2.0 * SOUND_SPEED;
   
  Serial.print(F("Distance: "));
  Serial.print(distance3 / 10.0, 2);
  Serial.print(F("cm, "));
  
  return distance3;
}
void Avance(){
  delay(t_long);
  Avance_droite();
  Avance_gauche();
  delay(t_long);
}
/*
 * Les fonctionns suivante permettent respectivement d'avancer chacun des jambes indépendemments du reste
 * Et de tourner un peu à la manière d'un tank
 */
void Avance_gauche(){
  Leve_jambe_gauche();
  Avance_jambe_gauche();
  Pose_jambe_gauche();
}

void Avance_droite(){
  Leve_jambe_droite();
  Avance_jambe_droite();
  Pose_jambe_droite();
}

void tourner_a_droite(){
  delay(2000);
  Avance_gauche();
  delay(2000);
  Avance_jambe_droite();
  Avance_gauche();
  delay(2000);
  Avance_jambe_droite();
}

void tourner_a_gauche(){
  Avance_droite();
  Avance_jambe_gauche();
  Avance_droite();
  Avance_jambe_gauche();
}

/*
 * La fonction arrêt réinitialise la position des servo et et arrête la boucle
 */
void arret(){
    servo_1.write(90);
    servo_2.write(90);
    
}

/*
 * Fonctions du servo_1 
 * Dans les fonctions suivante on rajoute un delai de 2000ms afin de pouvoir bien observé le mouvement lors des simulations
 */
 
void Leve_jambe_droite(){
  for (pos = 90; pos <= 180; pos += 1) {
    servo_1.write(pos);
    delay(15); 
  }
  delay(t_long);
}
void Pose_jambe_droite(){
  for (pos = 180; pos >= 90; pos -= 1) {
    servo_1.write(pos);
    delay(15); 
  }
  delay(t_long);
}
void Leve_jambe_gauche(){
  for (pos = 90; pos >= 0; pos -= 1) {
    servo_1.write(pos);
    delay(15); 
  }
  delay(t_long);
}
void Pose_jambe_gauche(){
  for (pos = 0; pos <= 90; pos += 1) {
    servo_1.write(pos);
    delay(15); 
  }
  delay(t_long);
}


/*
 * Fonctions du servo_2
 */
void Avance_jambe_gauche(){
  for (pos = 90; pos >= 45; pos -= 1) {
    servo_2.write(pos);
    delay(15); 
  }
  delay(t_long);
}
void Avance_jambe_droite(){
  for (pos = 90; pos <= 135; pos += 1) {
    servo_2.write(pos);
    delay(15);
  }
  delay(t_long);
}

// FONCTIONS ALGORITHME
//On attribue à chaque variable la nouvelle valeur du capteur
void actualiser_capteurs (){
    s_avant = distance_devant();
    s_droite = distance_droite();
    s_gauche = distance_gauche();
    }
//tant que le robot ne détecte pas de mur devant lui mais qu’il longe un mur à sa gauche, on teste les 3 cas possibles
void algorithme(){
  actualiser_capteurs();
    while (( s_avant <seuil) && ( s_gauche < seuil )){
      if (s_droite <s_gauche ){
        tourner_a_gauche();
        delay (t_micro );
      }
      else{
        tourner_a_droite();
        delay( t_micro );
      }
      Avance();
      delay( t_micro );
      arret();
// Tester si le robot est sur le bord droit de la ligne
      if (s_droite <seuil ){
        est_a_droite_de_la_ligne = true;
      }
      else{
        est_a_droite_de_la_ligne = false;
      }
    }
}
//Maintenant on teste la raison de son arrêt
void sortir_labyrinthe(){
  algorithme();
  if ( s_gauche < seuil ){
    Avance();
    delay(t_micro );
    tourner_a_droite();
    delay( t_90_droite );
    compteur += 1;
  }
  else{
    Avance();
    delay (t_moyen );
    tourner_a_gauche();
    delay ( t_90_gauche );
    Avance();

    if ( est_a_droite_de_la_ligne ){
        delay (t_long );
    }
    else{
        delay (t_court );
    }
    compteur-=1;
  }
  arret();
  
}

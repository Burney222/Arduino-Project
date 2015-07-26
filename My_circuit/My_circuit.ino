

const int button1Pin = 2;  // pushbutton 1 pin
const int button2Pin = 3;  // pushbutton 2 pin
const int button3Pin = 7;  // pushbutton 3 pin

const int led1Pin = 12;    // LED 1 pin
const int led2Pin = 13;    // LED 2 pin

const int led3Pin_blue = 9;  // LED 3 pins
const int led3Pin_green = 10;
const int led3Pin_red  = 6;

const int buzzerPin = 5;   // buzzer pin

const int potioPin = 0;

int nButtons = 3;

int highscore;  //variable to hold the highscore of game 1


void setup()
{
  // Set up the pushbutton pins to be an input: INPUT_PULLUP to enable integrated pullup-resistors
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(button3Pin, INPUT_PULLUP);

  // Set up the LED pins to be an output:
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin_red, OUTPUT);
  pinMode(led3Pin_green, OUTPUT);
  pinMode(led3Pin_blue, OUTPUT);
  
  // Buzzer output
  pinMode(buzzerPin, OUTPUT);
  
  
  // Serial Output
  Serial.begin(9600);
  
  // Set highscore to zero
  highscore = 0;
  
  // "Randomise" the random-generator
  randomSeed(analogRead(1));
  
  //Lock
  boolean lock = true;
  lightLEDs(LOW, LOW, LOW);
  delay(2000);
  lightLEDs(HIGH, HIGH, HIGH);
  while(lock) {
    lock = locked();
  }
  
  
}


boolean locked() {
  int button1State, button2State, button3State;  // variables to hold the pushbutton states
  
  long unlock_numbers[3] = {1,2,3}; //unlock sequence
  int sequencelength = 3;
  
  //Get input from buttons
  boolean get_input = true;
  boolean wrong_input = false;
  int counter = 0; 
  
    
  while(get_input) {
    long buttonnumber = 0;
    //Read status of the buttons
    button1State = digitalRead(button1Pin);
    button2State = digitalRead(button2Pin);
    button3State = digitalRead(button3Pin);
  
    lightLEDs(button1State, button2State, button3State);
    
    if (button1State == LOW) {  //button1 was pushed
      buttonnumber = 1;
      tone(buzzerPin, frequency('c'), 500);
    }        
    else if (button2State == LOW) {  //button2 was pushed
      buttonnumber = 2;
      tone(buzzerPin, frequency('e'), 500);
    } 
    else if (button3State == LOW) {  //button3 was pushed
      buttonnumber = 3;
      tone(buzzerPin, frequency('g'), 500);
    }   
    

    if ( (button1State == LOW) || (button2State == LOW) || (button3State == LOW) ) {
      delay(500);  //if at least one button was pushed: wait
      lightLEDs(HIGH, HIGH, HIGH);
      delay(400);
    }
    

    if ( (buttonnumber != unlock_numbers[counter]) && (buttonnumber != 0) ) {  //wrong input
      wrong_input = true;
    }
    
    if(buttonnumber != 0) {  //any button was pushed
      counter++;
      if(counter >= sequencelength) get_input = false;
    }
  }
  
 if (wrong_input) {    //wrong input
    blink_tone_wronginput();
    delay(2000);
    return true;  //locked = true 
  }
  if (!wrong_input) {    //right input
    blinkrightinput();
    delay(2000);
    return false;   //not locked anymore
  }
  
  
}
  

// Minimal class to replace std::vector
template<typename Data>
class Vector {
  size_t d_size; // Stores no. of actually stored objects
  size_t d_capacity; // Stores allocated capacity
  Data *d_data; // Stores data
  public:
    Vector() : d_size(0), d_capacity(0), d_data(0) {}; // Default constructor
    Vector(Vector const &other) : d_size(other.d_size), d_capacity(other.d_capacity), d_data(0) { 
      d_data = (Data *)malloc(d_capacity*sizeof(Data));
      memcpy(d_data, other.d_data, d_size*sizeof(Data));
    }; // Copy constuctor
    ~Vector() { free(d_data); }; // Destructor
    Vector &operator=(Vector const &other) { 
      free(d_data); 
      d_size = other.d_size; 
      d_capacity = other.d_capacity; 
      d_data = (Data *)malloc(d_capacity*sizeof(Data)); 
      memcpy(d_data, other.d_data, d_size*sizeof(Data)); 
      return *this; }; // Needed for memory management
    void push_back(Data const &x) { 
      if (d_capacity == d_size) resize(); 
      d_data[d_size++] = x; 
    }; // Adds new value. If needed, allocates more space
    size_t size() const { return d_size; }; // Size getter
    Data const &operator[](size_t idx) const { return d_data[idx]; }; // Const getter
    Data &operator[](size_t idx) { return d_data[idx]; }; // Changeable getter
  private:
    void resize() { 
      d_capacity = d_capacity ? d_capacity*2 : 1;
      Data *newdata = (Data *)malloc(d_capacity*sizeof(Data));
      memcpy(newdata, d_data, d_size * sizeof(Data));
      free(d_data); d_data = newdata; 
    };// Allocates double the old space
};



//Check whether to light any of the LEDs
void lightLEDs(int button1State, int button2State, int button3State)
{
  // Buttons: LOW = pressed, HIGH = not pressed
  if (button1State == LOW)        // if button 1 is pressed
  {
    digitalWrite(led1Pin, HIGH);  // turn LED 1 on
  }
  else if (button1State == HIGH)
  {
    digitalWrite(led1Pin, LOW);
  }
  
  if (button2State == LOW)
  {
    digitalWrite(led2Pin, HIGH);  // turn the LED off
  }
  else if (button2State == HIGH)
  {
    digitalWrite(led2Pin, LOW);
  }	
  
  if (button3State == LOW)        // if button 1 is pressed
  {
    digitalWrite(led3Pin_green, HIGH);  // turn LED 1 on
  }
  else if (button3State == HIGH)
  {
    digitalWrite(led3Pin_green, LOW);
  }
}
  


//reset-function (not integrated at the moment)
//checks, if reset-condition is fulfilled and returns 1 (reset) or 0 (no reset)
int resetstatus(int button1State, int button2State)
{
  if ((button1State == LOW) && (button2State == LOW))  //if both buttons are pressed
  {
    for(int i = 0; i < 6; i++) {      //blink LEDs and return 1
      digitalWrite(led1Pin, HIGH);
      digitalWrite(led2Pin, HIGH);
      delay(100);
      digitalWrite(led1Pin, LOW);
      digitalWrite(led2Pin, LOW);
      delay(100);
    }
    return 1;
  }
  else
  {
    return 0;              //or do nothing and return 0
  }
}


//Blinking and/or tone patterns
void blinkrightinput() {
  for(int i = 0; i < 2; i++) {    
    digitalWrite(led3Pin_green, HIGH);
    delay(100);
    digitalWrite(led3Pin_green, LOW);
    delay(100);        //if everything was correct: blink yellow led and the game is on
  }
}

void blink_tone_wronginput() {
  digitalWrite(led1Pin, HIGH);
  tone(buzzerPin, frequency('f'), 300);  
  delay(200);
  digitalWrite(led1Pin, LOW);
  delay(100);
  digitalWrite(led1Pin, HIGH);
  tone(buzzerPin, frequency('c'), 1200);
  delay(1200);
  digitalWrite(led1Pin, LOW);
}

void blinknewgame() {
  for(int i = 0; i < 3; i++) {
    digitalWrite(led1Pin, HIGH);
    digitalWrite(led2Pin, HIGH);
    digitalWrite(led3Pin_green, HIGH);
    delay(100);
    digitalWrite(led1Pin, LOW);
    digitalWrite(led2Pin, LOW);
    digitalWrite(led3Pin_green, LOW);
    delay(100);
  }
  delay(700);
}


void blink_tone_newhighscore() {
  char notes[] = "cdef";
  int beats[] = {2,2,2,2};
  int songLength = 4;
  int tempo = 100;
  for (int k = 0; k < 3; k++) 
  {
    for (int i = 0; i < songLength; i++) // step through the song arrays
    {
      int duration = beats[i] * tempo;  // length of note/rest in ms
      if (i==3) duration *= 3;
      tone(buzzerPin, frequency(notes[i]), duration);
      showRainbow(i*60);
      delay(duration);              // wait for tone to finish
      delay(tempo/10);              // brief pause between notes
    }
    digitalWrite(led3Pin_red, LOW);
    digitalWrite(led3Pin_green, LOW);
    digitalWrite(led3Pin_blue, LOW);
  }
  
  delay(1500);
}
  
  
// Play melody
// takes string of notes and spaces (=rests) and array of beats (A "1" represents a quarter-note, 2 a half-note, etc.,)
// Note that you also need to provide the songLength (= length of notes and spaces = length of array of beats)
// Also note that spaces also need a beat!
// tempo is optional and describes the overall tempo of the melody
void Melody(char notes[], int beats[], int songLength, int tempo) {
  for (int i = 0; i < songLength; i++) // step through the song arrays
  {
    int duration = beats[i] * tempo;  // length of note/rest in ms
    
    if (notes[i] == ' ')          // is this a rest? 
    {
      delay(duration);            // then pause for a moment
    }
    else                          // otherwise, play the note
    {
      tone(buzzerPin, frequency(notes[i]), duration);
      delay(duration);            // wait for tone to finish
    }
    delay(tempo/10);              // brief pause between notes
  }
}


// Frequency function
int frequency(char note) 
{
  // This function takes a note character (a-g), and returns the
  // corresponding frequency in Hz for the tone() function.
  
  int i;
  const int numNotes = 8;  // number of notes we're storing
  
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int frequencies[] = {262, 294, 330, 349, 392, 440, 494, 523};
  
  for (i = 0; i < numNotes; i++)  // Step through the notes
  {
    if (names[i] == note)         // Is this the one?
    {
      return(frequencies[i]);     // Yes! Return the frequency
    }
  }
  return(0);  // We looked through everything and didn't find it,
              // but we still need to return a value, so return 0.
}


void showSpectrum(int wait /*delaytime in ms*/)
{
  int x;  // define an integer variable called "x"

  for (x = 0; x < 255; x++) {  //up
    showRainbow(x);  // Call showRainbow() with our new x
    delay(wait);   // Delay for 10 ms (1/100th of a second)
  }
  
  digitalWrite(led3Pin_red, LOW);
  digitalWrite(led3Pin_green, LOW);
  digitalWrite(led3Pin_blue, LOW);
}



void showRainbow(int color) {  //takes color-value from 0 to 254
  int Phase = color / 51;
  int Level = color % 51;
  int Red, Green, Blue;
  switch( Phase ) {
     case 0:
       Red   = 255;
       Green = Level * 5;
       Blue  = 0;
       break;

     case 1:
       Red   = 255 - Level * 5;
       Green = 255;
       Blue  = 0;
       break;

     case 2:
       Red   = 0;
       Green = 255;
       Blue  = Level * 5;
       break;

     case 3:
       Red   = 0;
       Green = 255 - Level * 5;
       Blue  = 255;
       break;

     case 4:
       Red   = Level * 5;
       Green = 0;
       Blue  = 255;
       break;

     default:
       Red = Green = Blue = 255;
   }
  analogWrite(led3Pin_red, Red);
  analogWrite(led3Pin_blue, Blue);
  analogWrite(led3Pin_green, Green);
}


//game function
//starts the single-player game and returns the score
int game(int button1State, int button2State, int button3State) {
  boolean game = true;
  int score = 0;  //score in this run of the game
  
  int difficulty = analogRead(potioPin)/20;  //Read difficulty from the potiontiometer

  long randomnumber;
  Vector<long> random_numbers;
  //Generate starting random numbers according to the difficulty
  for(int i = 0; i < difficulty; i++) {
    randomnumber = random(1, nButtons+1);
    random_numbers.push_back(randomnumber);
  }
  
  blinknewgame();
  Serial.println("\nNew Game");
  Serial.println("Difficulty: " + String(difficulty));
  while(game) {
    //Generate new random number and store it at the end of the random_numbers vector
    randomnumber = random(1, nButtons+1);
    random_numbers.push_back(randomnumber);
    
    //Play the sequence stored in random_numbers
    for(int i = 0; i < random_numbers.size(); i++) {
      if (random_numbers[i] == 1) {
        digitalWrite(led1Pin, HIGH);
        tone(buzzerPin, frequency('c'), 500);
      }
        
      else if (random_numbers[i] == 2)  {
        digitalWrite(led2Pin, HIGH);
        tone(buzzerPin, frequency('e'), 500);
      }
      
      else if (random_numbers[i] == 3)  {
        digitalWrite(led3Pin_green, HIGH);
        tone(buzzerPin, frequency('g'), 500);
      }
      
      delay(500);
      digitalWrite(led1Pin, LOW);
      digitalWrite(led2Pin, LOW);
      digitalWrite(led3Pin_green, LOW);
      delay(500);
    } 
    
    //Get input from buttons
    boolean get_input = true;
    boolean wrong_input = false;
    int counter = 0;
    
    while(get_input) {
      long buttonnumber = 0;
      //Read status of the buttons
      button1State = digitalRead(button1Pin);
      button2State = digitalRead(button2Pin);
      button3State = digitalRead(button3Pin);
      lightLEDs(button1State, button2State, button3State);
      
      if (button1State == LOW) {  //button1 was pushed
        buttonnumber = 1;
        tone(buzzerPin, frequency('c'), 500);
      }        
      else if (button2State == LOW) {  //button2 was pushed
        buttonnumber = 2;
        tone(buzzerPin, frequency('e'), 500);
      } 
      else if (button3State == LOW) {  //button3 was pushed
        buttonnumber = 3;
        tone(buzzerPin, frequency('g'), 500);
      }   
      
      if ( (button1State == LOW) || (button2State == LOW) || (button3State == LOW) ) {
        delay(500);  //if at least one button was pushed: wait
        lightLEDs(HIGH, HIGH, HIGH);
        delay(400);
      }
      
      if (buttonnumber == random_numbers[counter]) {  //correct input
        if (counter < random_numbers.size()-1) counter++;  //increase counter
        else get_input = false;
      }
      else if ( (buttonnumber != random_numbers[counter]) && (buttonnumber != 0) ) {  //wrong input
        Serial.println("Wrong input");
        get_input = false;
        wrong_input = true;
      }
      
    }  //end of getting the input-loop
    
    if (wrong_input) {    //wrong input
      blink_tone_wronginput();
      game = false;  
    }
    if (!wrong_input) {    //right input => continue playing
      blinkrightinput();
      score = random_numbers.size();
    }
    delay(1500);
    
    //leave this while-loop if game = false
  }
  Serial.println("GAME OVER"); //if something wrong was entered in the get_input-loop: light red led and the game is over
  Serial.println("SCORE: " + String(score));
  return score;
} 





//Loop = main-function
void loop()
{
  int button1State, button2State, button3State;  // variables to hold the pushbutton states
  int score;
  
  //Start game
  score = game(button1State, button2State, button3State); 
  if (score > highscore) {
    highscore = score;
    Serial.println("NEW HIGHSCORE!");
    blink_tone_newhighscore();
    
  }
   

}

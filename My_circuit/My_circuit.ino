

const int button1Pin = 2;  // pushbutton 1 pin
const int button2Pin = 3;  // pushbutton 2 pin
const int led1Pin = 12;    // LED 1 pin
const int led2Pin = 13;    // LED 2 pin

int nButtons = 2;


void setup()
{
  // Set up the pushbutton pins to be an input: INPUT_PULLUP to enable integrated pullup-resistors
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);

  // Set up the LED pins to be an output:
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  
  // "Randomise" the random-generator
  randomSeed(analogRead(0));
  
  // Serial Output
  Serial.begin(9600);
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
void lightLEDs(int button1State, int button2State)
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
}
  


//reset-function
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
    return 0;              //or do nothing an return 0
  }
}


void blinkrightinput() {
  for(int i = 0; i < 6; i++) {      
    digitalWrite(led2Pin, HIGH);
    delay(100);
    digitalWrite(led2Pin, LOW);
    delay(100);        //if everything was correct: blink yellow led and the game is on
  }
}

void blinkwronginput() {
  digitalWrite(led1Pin, HIGH);  
  delay(200);
  digitalWrite(led1Pin, LOW);
  delay(200);
  digitalWrite(led1Pin, HIGH);
  delay(1500);
  digitalWrite(led1Pin, LOW);
}

void blinknewgame() {
  for(int i = 0; i < 3; i++) {
    digitalWrite(led1Pin, HIGH);
    digitalWrite(led2Pin, HIGH);
    delay(100);
    digitalWrite(led1Pin, LOW);
    digitalWrite(led2Pin, LOW);
    delay(100);
  }
  delay(700);
}

//Loop = main-function
void loop()
{
  int button1State, button2State;  // variables to hold the pushbutton states
  int reset;  // variable to hold whether the reset condition was fulfilled or not

  //Read status of the LEDs
  button1State = digitalRead(button1Pin);
  button2State = digitalRead(button2Pin);


  //Start game1
  boolean game1 = true;
  
  long randomnumber;
  Vector<long> random_numbers;
  blinknewgame();
  if (game1) Serial.println("New Game");
  while(game1) {
    //Generate new random number and store it at the end of the random_numbers vector
    randomnumber = random(1, nButtons+1);
    random_numbers.push_back(randomnumber);
    
    //Play the sequence stored in random_numbers
    for(int i = 0; i < random_numbers.size(); i++) {
      if (random_numbers[i] == 1)       digitalWrite(led1Pin, HIGH);
      else if (random_numbers[i] == 2)  digitalWrite(led2Pin, HIGH);
      delay(500);
      digitalWrite(led1Pin, LOW);
      digitalWrite(led2Pin, LOW);
      delay(500);
    } 
    
    //Get input from buttons
    boolean get_input = true;
    boolean wrong_input = false;
    int counter = 0;
    
    while(get_input) {
      long buttonnumber = 0;
      //Read status of the LEDs
      button1State = digitalRead(button1Pin);
      button2State = digitalRead(button2Pin);
      lightLEDs(button1State, button2State);
      
      if ( (button1State == LOW) && (button2State == LOW) ) {  //if both buttons are pressed: wrong_input => gameover!
        Serial.println("Both buttons pressed simultaneously");
        get_input = false;
        wrong_input = true;
      }
      else if (button1State == LOW) buttonnumber = 1;  //only button1 was pushed
      else if (button2State == LOW) buttonnumber = 2;  //only button2 was pushed
      
      if ( (button1State == LOW) || (button2State == LOW) ) {
        delay(500);  //if at least one button was pushed: wait
        digitalWrite(led1Pin, LOW);
        digitalWrite(led2Pin, LOW);
        delay(400);
      }
      
      if (buttonnumber == random_numbers[counter]) {  //correct input
        Serial.println("Correct input");
        if (counter < random_numbers.size()-1) counter++;  //increase counter
        else get_input = false;
      }
      else if ( (buttonnumber != random_numbers[counter]) && (buttonnumber != 0) ) {  //wrong input
        Serial.println("Wrong input");
        get_input = false;
        wrong_input = true;
      }
      
    }  //end of getting the input-loop
    
    if (wrong_input) {
      blinkwronginput();
      game1 = false;  
      Serial.println("GAME OVER"); //if something wrong was entered in the get_input-loop: light red led and the game is over
    }
    if (!wrong_input) {
      blinkrightinput();
    }
    delay(1500);
  } 
    

}

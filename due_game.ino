/*
  Oscilloscope 2D Platformer

  JLab project that aims to create a 2D platformer with output displayed on an oscilloscope in XY mode

  created 3 Nov 2025
  modified 3 Dec 2025
  by Cameron Bryzek

  NES Controller Connection Code by Joseph Corleto, 2016: https://www.allaboutcircuits.com/projects/nes-controller-interface-with-an-arduino-uno/

  Music MIDI to Arduino Conversion by ShivamJoker, https://github.com/ShivamJoker/MIDI-to-Arduino
*/

#include <stdint.h>

#ifndef PROGMEM
#define PROGMEM
#endif

#ifndef pgm_read_word
#define pgm_read_word(addr) (*(const uint16_t *)(addr))
#endif

#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))

// music note definitions
#define Fb4 370
#define Cb3 139
#define Gb4 415
#define A4  440
#define B4  494
#define Fb2 92
#define E4  330
#define B2  123
#define Gb2 104
#define E2  82
#define Cb4 277
#define A2  110
#define Cb5 554
#define D3  147
#define E5  659
#define D5  587
#define A3  220
#define Fb3 185
#define D4  294
#define B3  247
#define Gb3 208
#define E3  165
#define Ab4 466
#define Db3 156
#define Db4 311
#define Db5 622
#define Ab2 117
#define F4  349
#define A5  880
#define B5  988
#define Cb6 1109
#define D6  1175
#define C5  523

// sfx
const uint16_t sfxJump[][3] PROGMEM = {
  { A5,  18, 0 },
  { B5,  18, 0 },
  { Cb6, 18, 0 },
  { D6,  18, 0 },
  { Cb6, 18, 0 },
};
#define SFX_JUMP_LEN (sizeof(sfxJump) / sizeof(sfxJump[0]))

const uint16_t sfxDeath[][3] PROGMEM = {
  { C5,  18, 0 },
  { D5,  18, 0 },
  { C5,  18, 0 },
  { B4,  18, 0 },
  { A4,  18, 0 },
};
#define SFX_DEATH_LEN (sizeof(sfxDeath) / sizeof(sfxDeath[0]))

struct SFXPlayer {
  uint8_t pin;
  const uint16_t (*score)[3];
  size_t len, i;
  uint8_t phase;  // 0 = ON, 1 = REST
  bool active;

  uint32_t seg_end_us;
  uint32_t halfPeriod_us;
  uint32_t nextToggle_us;
  bool level;
} sfx;

inline void sfxLow() {
  if (sfx.pin) digitalWrite(sfx.pin, LOW);
}

void sfxStart(uint8_t pin, const uint16_t (*score)[3], size_t len) {
  if (sfx.active) return;   // don't interrupt existing SFX
  sfx.pin   = pin;
  sfx.score = score;
  sfx.len   = len;
  sfx.i     = 0;
  sfx.phase = 0;
  sfx.active = true;
  sfx.level  = false;

  pinMode(pin, OUTPUT);
  sfxLow();

  uint16_t f  = pgm_read_word(&sfx.score[sfx.i][0]);
  uint16_t on = pgm_read_word(&sfx.score[sfx.i][1]);
  sfx.halfPeriod_us = f ? (uint32_t)(500000UL / f) : 0;
  uint32_t now = micros();
  sfx.nextToggle_us = now + sfx.halfPeriod_us;
  sfx.seg_end_us    = now + (on ? (uint32_t)on * 1000UL : 1000UL);
}

void sfxService() {
  if (!sfx.active) return;
  uint32_t now = micros();

  // square wave edges in ON segment
  if (sfx.phase == 0 && sfx.halfPeriod_us) {
    uint8_t edges = 4;
    while (edges && (int32_t)(now - sfx.nextToggle_us) >= 0) {
      sfx.level = !sfx.level;
      digitalWrite(sfx.pin, sfx.level ? HIGH : LOW);
      sfx.nextToggle_us += sfx.halfPeriod_us;
      edges--;
      now = micros();
    }
  }

  // segment timing
  if ((int32_t)(now - sfx.seg_end_us) >= 0) {
    if (sfx.phase == 0) {
      // finished ON segment -> REST
      sfx.phase = 1;
      sfxLow();
      sfx.halfPeriod_us = 0;
      uint16_t off = pgm_read_word(&sfx.score[sfx.i][2]);
      sfx.seg_end_us = now + (off ? (uint32_t)off * 1000UL : 1000UL);
    } else {
      // finished REST -> next note
      sfx.i++;
      if (sfx.i >= sfx.len) {
        sfxLow();
        sfx.active = false;
        return;
      }
      uint16_t f  = pgm_read_word(&sfx.score[sfx.i][0]);
      uint16_t on = pgm_read_word(&sfx.score[sfx.i][1]);
      sfx.halfPeriod_us = f ? (uint32_t)(500000UL / f) : 0;
      sfx.level = false;
      sfxLow();
      now = micros();
      sfx.nextToggle_us = now + sfx.halfPeriod_us;
      sfx.seg_end_us    = now + (on ? (uint32_t)on * 1000UL : 1000UL);
      sfx.phase = 0;
    }
  }
}

// Generated using https://github.com/ShivamJoker/MIDI-to-Arduino

// ---------------- NES CONTROLLER ----------------
const int A_BUTTON = 0;
const int B_BUTTON = 1;
const int SELECT_BUTTON = 2;
const int START_BUTTON = 3;
const int UP_BUTTON = 4;
const int DOWN_BUTTON = 5;
const int LEFT_BUTTON = 6;
const int RIGHT_BUTTON = 7;

byte nesRegister = 0;   // We will use this to hold current button states

//Inputs:
int nesData = 13;    // The data pin for the NES controller

//Outputs:
int nesClock = 11;    // The clock pin for the NES controller
int nesLatch = 12;    // The latch pin for the NES controller

// ---------------- PROJECT ----------------

// writing in terrain
int terrain[][2] = {
    {1,200},{2,40},{3,40},{4,40},{5,40},{6,40},{7,40},{8,40},{9,0},{10,0},{11,0},{12,40},{13,40},{14,40},{15,40},{16,40},{17,0},{18,0},{19,0},{20,0},{21,0},{22,40},{23,40},{24,40},{25,40},{26,40},{27,41},{28,42},{29,43},{30,44},{31,45},{32,46},{33,47},{34,48},{35,49},{36,50},{37,50},{38,50},{39,50},{40,85},{41,85},{42,85},{43,50},{44,50},{45,50},{46,50},{47,50},{48,85},{49,85},{50,85},{51,85},{52,0},{53,0},{54,0},{55,100},{56,100},{57,100},{58,100},{59,100},{60,100},{61,0},{62,0},{63,0},{64,120},{65,120},{66,120},{67,0},{68,0},{69,0},{70,0},{71,140},{72,140},{73,140},{74,140},{75,0},{76,0},{77,0},{78,0},{79,0},{80,0},{81,0},{82,100},{83,100},{84,100},{85,100},{86,100},{87,100},{88,0},{89,0},{90,0},{91,0},{92,0},{93,50},{94,50},{95,50},{96,50},{97,50},{98,0},{99,0},{100,0},{101,0},{102,85},{103,85},{104,85},{105,85},{106,84},{107,83},{108,82},{109,81},{110,80},{111,79},{112,78},{113,77},{114,76},{115,75},{116,74},{117,73},{118,72},{119,71},{120,70},{121,0},{122,0},{123,0},{124,70},{125,0},{126,0},{127,0},{128,0},{129,70},{130,0},{131,0},{132,0},{133,0},{134,95},{135,95},{136,95},{137,95},{138,95},{139,130},{140,130},{141,130},{142,130},{143,130},{144,0},{145,0},{146,0},{147,0},{148,0},{149,0},{150,95},{151,95},{152,95},{153,95},{154,95},{155,90},{156,85},{157,80},{158,75},{159,70},{160,65},{161,60},{162,0},{163,0},{164,0},{165,0},{166,0},{167,0},{168,50},{169,50},{170,50},{171,50},{172,50},{173,50},{174,50},{175,50},{176,50},{177,50},{178,50},{179,50},{180,200}
};
size_t terrain_length = sizeof(terrain) / sizeof(terrain[0]);

// inputs and outputs
int terrainPin = DAC1;  // terrain output pin
int playerPin = DAC0;  // player output pin

int joyPin = A0;     // joystick input
int sfxPin = 7;      // SFX output
const uint8_t clockPin = 2;  // input pin for trigger from function generator

// tunable parameters
int trigger_centre = 422;
int trigger_width = 200;
int display_width = 20;
int framerate = 6;
int g = 8;                  // gravity
int jump_strength = 30;
int player_height = 20;
const int coyoteMax = 3;    // implementing jumping coyote time (game design)
const int bufferMax = 3;
int scroll_max = 1;         // scrolling speed
int scroll_dir = 1;
int playerDrawPos = display_width / 2;
int max_height = 200;

// values to be used later in code
int joy_val = 0;
float loc = 3.0;
int t = 0;
int gnd;
int V_terrain;
int y_pos;        // player y coord
int y_vel = 0;    // vertical velocity
int coyoteFrames = 0;
int bufferFrames = 0;
int right_trigger = trigger_centre + trigger_width;
int left_trigger = trigger_centre - trigger_width;
bool prevPressed = false;
volatile bool trigger = false;

void clockISR() {
  trigger = true;
}

void setup() {
  Serial.begin(9600);

  pinMode(terrainPin, OUTPUT);
  pinMode(playerPin, OUTPUT);
  pinMode(joyPin, INPUT);
  pinMode(sfxPin, OUTPUT);
  digitalWrite(sfxPin, LOW);
  pinMode(clockPin, INPUT);

  gnd = terrain[(int)loc][1];    // start player on ground
  y_pos = gnd;
  y_vel = 0;

  prevPressed = (bitRead(nesRegister, B_BUTTON) == 0);
  attachInterrupt(digitalPinToInterrupt(clockPin), clockISR, RISING);

  // NES setup
  pinMode(nesData, INPUT);
  pinMode(nesClock, OUTPUT);
  pinMode(nesLatch, OUTPUT);
  digitalWrite(nesClock, LOW);
  digitalWrite(nesLatch, LOW);
}

void loop() {
  // if there's no trigger, then play audio
  while (!trigger) {
    sfxService();
  }
  trigger = false;

  // if there IS a trigger, move onto the rest of the code

  // read inputs
  joy_val = analogRead(joyPin);
  int rawJump = bitRead(nesRegister, B_BUTTON);
  bool pressed = (rawJump == LOW);
  nesRegister = readNesController();

  // Serial.print("gnd: ");     Serial.println(gnd);
  // Serial.print("player: ");  Serial.println(y_pos);
  // Serial.print("loc: ");     Serial.println(loc);

  // PLAYER LOGIC
  // ground & physics
  gnd = terrain[(int)loc][1];
  bool on_ground = (y_pos == gnd) && (y_vel <= 0);
  if(loc < 0) { loc = 0.0; }

  // if on the ground, give player all coyote frames, else decrease frames
  if (on_ground) coyoteFrames = coyoteMax;
  else if (coyoteFrames > 0) coyoteFrames--;

  // give the player a jumping buffer to allow them to jump while on an edge
  bool pressedEdge = pressed && !prevPressed;
  if (pressedEdge) bufferFrames = bufferMax;
  else if (bufferFrames > 0) bufferFrames--;

  // jumping mechanics
  if (bufferFrames > 0 && (on_ground || coyoteFrames > 0)) {
    y_vel = jump_strength;
    bufferFrames = 0;
    coyoteFrames = 0;
    sfxStart(sfxPin, sfxJump, SFX_JUMP_LEN); // play jump sfx
  } else { y_vel -= g; }

  // check player's next position
  int nextY = y_pos + y_vel;
  if (y_vel <= 0 && nextY <= gnd) {
    y_pos = gnd;
    y_vel = 0;
  } else { y_pos = nextY; }

  if (y_pos < 0) y_pos = 0;
  if (y_pos > max_height) { y_pos = max_height; y_vel = 0; }  // set max player height

  int intLoc  = (int)loc - display_width / 2;
  float fracLoc = loc - display_width / 2 - intLoc;

  // draw terrain forward
  for (int i = 0; i < display_width; i++) {
    int x1 = (intLoc + i) % terrain_length;
    int x2 = (intLoc + i + 1) % terrain_length;

    int y1 = terrain[x1][1];
    int y2 = terrain[x2][1];
    V_terrain = y1 + (int)((y2 - y1) * fracLoc);

    analogWrite(terrainPin, V_terrain);

    // timing consistencies
    uint32_t delayStart = micros();
    uint32_t totalDelay = framerate * 1000UL;
    while ((micros() - delayStart) < totalDelay) {
      sfxService();
      delayMicroseconds(50);
    }
  }

  // draw terrain backward
  for (int i = display_width - 1; i >= 0; i--) {
    int x1 = (intLoc + i) % terrain_length;
    int x2 = (intLoc + i + 1) % terrain_length;

    int y1 = terrain[x1][1];
    int y2 = terrain[x2][1];
    V_terrain = y1 + (int)((y2 - y1) * fracLoc);

    analogWrite(terrainPin, V_terrain);

    // draw player
    if (i == playerDrawPos) {
      analogWrite(playerPin, y_pos + player_height  - 0.94 * V_terrain);
    } else {
      analogWrite(playerPin, 0);
    }

    // play sfx
    uint32_t delayStart = micros();
    while ((micros() - delayStart) < (framerate * 1000UL)) {
      sfxService();
      delayMicroseconds(50);
    }
  }

  // move terrain
  t++;

  float scrollSpeed = 0.0;

  if (nextY < terrain[(int)loc + 1][1] - g - 1) {
    if (bitRead(nesRegister, LEFT_BUTTON) == 0) {
      scrollSpeed = -scroll_dir * scroll_max;
    }
  } else if (nextY < terrain[(int)loc - 1][1] - g - 1) {
    if (bitRead(nesRegister, RIGHT_BUTTON) == 0) {
      scrollSpeed = scroll_dir * scroll_max;
    }
  } else {
    if (bitRead(nesRegister, RIGHT_BUTTON) == 0) {
      scrollSpeed = scroll_dir * scroll_max;
    } else if (bitRead(nesRegister, LEFT_BUTTON) == 0) {
      scrollSpeed = -scroll_dir * scroll_max;
    }
  }

  loc += scrollSpeed;

  // loop terrain at edge
  /*while (loc >= terrain_length) loc -= terrain_length;
  while (loc < 0) loc += terrain_length;*/

  // if falls in a pit; reset the player to the start
  if (y_pos == 0) {
    loc = 3.0;
    sfx.active = false;  // stop any active SFX
    sfxStart(sfxPin, sfxDeath, SFX_DEATH_LEN);
  }

  prevPressed = pressed;
}

byte readNesController() {
  // Pre-load a variable with all 1's which assumes all buttons are not
  // pressed. But while we cycle through the bits, if we detect a LOW, which is
  // a 0, we clear that bit. In the end, we find all the buttons states at once.
  int tempData = 255;

  // Quickly pulse the nesLatch pin so that the register grab what it see on
  // its parallel data pins.
  digitalWrite(nesLatch, HIGH);
  digitalWrite(nesLatch, LOW);

  // Upon latching, the first bit is available to look at, which is the state
  // of the A button. We see if it is low, and if it is, we clear out variable's
  // first bit to indicate this is so.
  if (digitalRead(nesData) == LOW)
    bitClear(tempData, A_BUTTON);

  // Clock the next bit which is the B button and determine its state just like
  // we did above.
  digitalWrite(nesClock, HIGH);
  digitalWrite(nesClock, LOW);
  if (digitalRead(nesData) == LOW)
    bitClear(tempData, B_BUTTON);

  // Now do this for the rest of them!

  // Select button
  digitalWrite(nesClock, HIGH);
  digitalWrite(nesClock, LOW);
  if (digitalRead(nesData) == LOW)
    bitClear(tempData, SELECT_BUTTON);

  // Start button
  digitalWrite(nesClock, HIGH);
  digitalWrite(nesClock, LOW);
  if (digitalRead(nesData) == LOW)
    bitClear(tempData, START_BUTTON);

  // Up button
  digitalWrite(nesClock, HIGH);
  digitalWrite(nesClock, LOW);
  if (digitalRead(nesData) == LOW)
    bitClear(tempData, UP_BUTTON);

  // Down button
  digitalWrite(nesClock, HIGH);
  digitalWrite(nesClock, LOW);
  if (digitalRead(nesData) == LOW)
    bitClear(tempData, DOWN_BUTTON);

  // Left button
  digitalWrite(nesClock, HIGH);
  digitalWrite(nesClock, LOW);
  if (digitalRead(nesData) == LOW)
    bitClear(tempData, LEFT_BUTTON);

  // Right button
  digitalWrite(nesClock, HIGH);
  digitalWrite(nesClock, LOW);
  if (digitalRead(nesData) == LOW)
    bitClear(tempData, RIGHT_BUTTON);

  // After all of this, we now have our variable all bundled up
  // with all of the NES button states.*/
  return tempData;
}
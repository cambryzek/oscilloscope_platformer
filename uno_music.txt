/*
  Oscilloscope 2D Platformer Music

  Assistant code to the JLab project that aims to create a 2D platformer with output displayed on an oscilloscope in XY mode.
  This code specifically runs the music only on an Arduino UNO.

  created 3 Nov 2025
  modified 1 Dec 2025
  by Cameron Bryzek

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

// music tune
const uint16_t midi1[][3] PROGMEM = {
  { Fb4, 226, 1 },
  { Fb4, 454, 1 },
  { Gb4, 226, 1 },
  { A4, 226, 1 },
  { Cb3, 226, 1 },
  { B4, 226, 1 },
  { Fb2, 113, 1 },
  { A4, 113, 1 },
  { E4, 226, 1 },
  { B2, 454, 1 },
  { Gb2, 226, 1 },
  { E2, 226, 1 },
  { B2, 340, 115 },
  { Cb4, 113, 1 },
  { E4, 113, 1 },
  { Fb4, 226, 1 },
  { Cb3, 454, 1 },
  { Gb4, 226, 1 },
  { A4, 226, 1 },
  { Cb3, 226, 1 },
  { B4, 226, 1 },
  { A2, 113, 1 },
  { A4, 113, 1 },
  { Cb5, 226, 1 },
  { D3, 226, 1 },
  { Cb3, 226, 1 },
  { B2, 226, 1 },
  { A2, 226, 1 },
  { Cb3, 226, 1 },
  { Cb5, 226, 1 },
  { A2, 226, 1 },
  { E5, 226, 1 },
  { D3, 454, 1 },
  { D5, 113, 1 },
  { Cb5, 113, 1 },
  { B4, 113, 115 },
  { A4, 113, 115 },
  { Gb4, 226, 1 },
  { Cb3, 113, 115 },
  { Fb4, 226, 1 },
  { D3, 113, 115 },
  { E4, 226, 1 },
  { Cb3, 113, 115 },
  { Cb4, 226, 1 },
  { A2, 226, 1 },
  { Fb2, 226, 1 },
  { A2, 226, 1 },
  { Cb4, 113, 115 },
  { Cb4, 113, 1 },
  { E4, 113, 1 },
  { Fb4, 113, 115 },
  { Gb4, 113, 115 },
  { A4, 113, 115 },
  { Gb4, 113, 115 },
  { Fb4, 113, 115 },
  { B4, 113, 115 },
  { E4, 681, 228 },
  { E5, 226, 683 },
  { A4, 226, 1 },
  { Cb3, 454, 1 },
  { Gb4, 113, 1 },
  { Fb4, 113, 1 },
  { E4, 226, 1 },
  { Cb3, 226, 1 },
  { Fb4, 226, 1 },
  { Cb3, 113, 1 },
  { Cb4, 113, 1 },
  { A3, 681, 1 },
  { Fb3, 226, 1 },
  { D3, 226, 1 },
  { A3, 226, 1 },
  { Gb4, 226, 1 },
  { D3, 226, 1 },
  { A4, 226, 1 },
  { Cb3, 454, 1 },
  { Gb4, 113, 1 },
  { Fb4, 113, 1 },
  { E4, 113, 115 },
  { D4, 113, 115 },
  { Cb4, 113, 115 },
  { E4, 113, 115 },
  { A4, 226, 1 },
  { D3, 226, 1 },
  { Cb3, 226, 1 },
  { B2, 226, 1 },
  { B4, 226, 1 },
  { Cb3, 226, 1 },
  { B2, 226, 1 },
  { A2, 226, 1 },
  { Cb5, 226, 1 },
  { D3, 454, 1 },
  { B4, 226, 1 },
  { A4, 226, 1 },
  { Fb3, 113, 115 },
  { B4, 226, 1 },
  { Cb3, 113, 115 },
  { E5, 226, 1 },
  { D3, 113, 115 },
  { D5, 226, 1 },
  { Cb5, 226, 1 },
  { A4, 226, 1 },
  { A2, 226, 1 },
  { Fb2, 226, 1 },
  { A2, 226, 1 },
  { A3, 113, 115 },
  { A3, 113, 1 },
  { B3, 113, 1 },
  { Cb4, 113, 115 },
  { Cb4, 113, 1 },
  { D4, 113, 1 },
  { E4, 113, 115 },
  { E4, 113, 1 },
  { Gb4, 113, 1 },
  { B4, 113, 115 },
  { B4, 113, 1 },
  { D5, 113, 1 },
  { E5, 113, 115 },
  { E5, 113, 115 },
  { E5, 113, 115 },
  { E5, 113, 115 },
  { E5, 113, 115 },
  { Gb3, 226, 1 },
  { Fb3, 226, 1 },
  { E3, 226, 1 },
  { Gb4, 226, 1 },
  { Gb2, 454, 1 },
  { Ab4, 226, 1 },
  { B4, 226, 1 },
  { Db3, 226, 1 },
  { Cb5, 226, 1 },
  { Db3, 226, 1 },
  { Fb4, 226, 1 },
  { Fb2, 681, 1 },
  { Cb3, 454, 1 },
  { Db4, 226, 1 },
  { Fb4, 226, 1 },
  { Gb4, 226, 1 },
  { Gb2, 454, 1 },
  { Ab4, 226, 1 },
  { B4, 226, 1 },
  { Db3, 226, 1 },
  { Cb5, 226, 1 },
  { Db3, 113, 1 },
  { B4, 113, 1 },
  { Db5, 226, 1 },
  { Db3, 681, 1 },
  { Ab2, 454, 1 },
  { Db5, 454, 1 },
  { E5, 454, 1 },
  { Gb3, 226, 1 },
  { Db5, 113, 1 },
  { Cb5, 113, 1 },
  { B4, 226, 1 },
  { E3, 113, 115 },
  { Cb5, 226, 1 },
  { Cb3, 113, 115 },
  { B4, 226, 1 },
  { B2, 113, 115 },
  { Gb4, 226, 1 },
  { Gb2, 113, 115 },
  { Fb4, 454, 1 },
  { Db3, 454, 1 },
  { Gb4, 226, 1 },
  { Cb3, 454, 1 },
  { Ab4, 226, 1 },
  { B4, 226, 1 },
  { Ab2, 113, 115 },
  { Cb5, 226, 1 },
  { Ab2, 113, 115 },
  { B4, 226, 1 },
  { E3, 681, 1 },
  { E5, 908, 1 },
  { Cb5, 226, 1 },
  { E3, 454, 1 },
  { B4, 226, 1 },
  { A4, 226, 1 },
  { E3, 226, 1 },
  { B4, 226, 1 },
  { A2, 113, 1 },
  { A4, 113, 1 },
  { E4, 226, 1 },
  { A3, 454, 1 },
  { Fb3, 226, 1 },
  { D3, 226, 1 },
  { A3, 340, 115 },
  { Cb4, 113, 1 },
  { E4, 113, 1 },
  { Fb4, 226, 1 },
  { E3, 454, 1 },
  { Gb4, 226, 1 },
  { A4, 226, 1 },
  { E3, 226, 1 },
  { B4, 226, 1 },
  { Cb3, 113, 1 },
  { A4, 113, 1 },
  { Cb5, 226, 1 },
  { Fb3, 226, 1 },
  { E3, 226, 1 },
  { D3, 226, 1 },
  { Cb3, 226, 1 },
  { E3, 226, 1 },
  { Cb5, 226, 1 },
  { Cb3, 226, 1 },
  { E5, 226, 1 },
  { D3, 454, 1 },
  { D5, 113, 1 },
  { Cb5, 113, 1 },
  { B4, 113, 115 },
  { A4, 113, 115 },
  { Gb4, 226, 1 },
  { Cb3, 113, 115 },
  { Fb4, 226, 1 },
  { D3, 113, 115 },
  { E4, 226, 1 },
  { Cb3, 113, 115 },
  { Cb4, 226, 1 },
  { A2, 226, 1 },
  { Fb2, 226, 1 },
  { A2, 226, 1 },
  { Cb4, 113, 115 },
  { Cb4, 113, 1 },
  { E4, 113, 1 },
  { Fb4, 113, 115 },
  { Gb4, 113, 115 },
  { A4, 113, 115 },
  { Gb4, 113, 115 },
  { Fb4, 113, 115 },
  { B4, 113, 115 },
  { E4, 681, 228 },
  { E5, 226, 228 },
  { B2, 226, 1 },
  { E3, 226, 1 },
  { A4, 226, 1 },
  { Cb3, 454, 1 },
  { Gb4, 113, 1 },
  { Fb4, 113, 1 },
  { E4, 226, 1 },
  { Cb3, 226, 1 },
  { Fb4, 226, 1 },
  { Cb3, 113, 1 },
  { Cb4, 113, 1 },
  { A3, 681, 1 },
  { Fb3, 226, 1 },
  { D3, 226, 1 },
  { A3, 226, 1 },
  { Gb4, 226, 1 },
  { D3, 226, 1 },
  { A4, 226, 1 },
  { Cb3, 454, 1 },
  { Gb4, 113, 1 },
  { Fb4, 113, 1 },
  { E4, 113, 115 },
  { D4, 113, 115 },
  { Cb4, 113, 115 },
  { E4, 113, 115 },
  { A4, 226, 1 },
  { D3, 226, 1 },
  { Cb3, 226, 1 },
  { B2, 226, 1 },
  { B4, 226, 1 },
  { Cb3, 226, 1 },
  { B2, 226, 1 },
  { A2, 226, 1 },
  { Cb5, 226, 1 },
  { D3, 454, 1 },
  { B4, 226, 1 },
  { A4, 226, 1 },
  { Fb3, 113, 115 },
  { B4, 226, 1 },
  { Cb3, 113, 115 },
  { E5, 226, 1 },
  { D3, 113, 115 },
  { D5, 226, 1 },
  { Cb5, 226, 1 },
  { A4, 226, 1 },
  { A2, 226, 1 },
  { Fb2, 226, 1 },
  { A2, 226, 1 },
  { A3, 113, 115 },
  { A3, 113, 1 },
  { B3, 113, 1 },
  { Cb4, 113, 115 },
  { Cb4, 113, 1 },
  { D4, 113, 1 },
  { E4, 113, 115 },
  { E4, 113, 1 },
  { Gb4, 113, 1 },
  { B4, 113, 115 },
  { B4, 113, 1 },
  { D5, 113, 1 },
  { E5, 113, 115 },
  { E5, 113, 115 },
  { E5, 113, 115 },
  { E5, 113, 115 },
  { E5, 113, 115 },
  { Gb3, 226, 1 },
  { Fb3, 226, 1 },
  { E3, 226, 1 },
  { E5, 226, 1 },
  { E2, 454, 1 },
  { B4, 226, 1 },
  { Gb4, 226, 1 },
  { B2, 226, 1 },
  { E4, 226, 1 },
  { B2, 226, 1 },
  { Fb4, 226, 1 },
  { Fb2, 681, 1 },
  { Cb3, 454, 1 },
  { Db4, 226, 1 },
  { Fb4, 226, 1 },
  { Gb4, 226, 1 },
  { E2, 454, 1 },
  { Ab4, 226, 1 },
  { B4, 226, 1 },
  { B2, 226, 1 },
  { Cb5, 226, 1 },
  { B2, 113, 1 },
  { B4, 113, 1 },
  { Db5, 226, 1 },
  { Db3, 681, 1 },
  { Ab2, 454, 1 },
  { Db5, 454, 1 },
  { E5, 454, 1 },
  { Gb3, 226, 1 },
  { Db5, 113, 1 },
  { Cb5, 113, 1 },
  { B4, 226, 1 },
  { E3, 113, 115 },
  { Cb5, 226, 1 },
  { Cb3, 113, 115 },
  { B4, 226, 1 },
  { Fb3, 113, 115 },
  { Gb4, 226, 1 },
  { Db3, 113, 115 },
  { Fb4, 454, 1 },
  { Db3, 454, 1 },
  { Gb4, 226, 1 },
  { Cb3, 454, 1 },
  { Ab4, 226, 1 },
  { B4, 226, 1 },
  { Ab2, 113, 115 },
  { Cb5, 226, 1 },
  { Ab2, 113, 115 },
  { Ab4, 226, 1 },
  { Ab2, 681, 1 },
  { F4, 454, 1 },
  { Gb4, 454, 0 },
};

#define MIDI_LEN (sizeof(midi1) / sizeof(midi1[0]))

struct MusicPlayer {
  uint8_t pin;
  const uint16_t (*score)[3];
  size_t len;
  size_t i;
  uint8_t phase;      // 0 = start ON segment, 1 = rest, 2 = move to next note
  bool enabled;

  uint32_t due_us;
  bool initialized;

  uint32_t halfPeriod_us;
  uint32_t nextToggle_us;
  bool level;
} music;

void musicStart(uint8_t pin, const uint16_t (*score)[3], size_t len) {
  music.pin   = pin;
  music.score = score;
  music.len   = len;
  music.i     = 0;
  music.phase = 2;
  music.enabled    = true;
  music.initialized = false;
  music.due_us      = 0;
  music.halfPeriod_us = 0;
  music.nextToggle_us = 0;
  music.level = false;

  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void musicUpdate() {
  if (!music.enabled || music.len == 0) return;

  uint32_t now = micros();
  if (!music.initialized) {
    music.due_us       = now;
    music.nextToggle_us = now;
    music.initialized   = true;
  }

  // handles square wave toggling if in ON segment
  if (music.halfPeriod_us) {
    uint8_t edges = 4;  // safety limit
    while (edges && (int32_t)(now - music.nextToggle_us) >= 0) {
      music.level = !music.level;
      digitalWrite(music.pin, music.level ? HIGH : LOW);
      music.nextToggle_us += music.halfPeriod_us;
      edges--;
      now = micros();
    }
  }

  // handles note timing
  for (uint8_t steps = 0; steps < 4; ++steps) {
    if ((int32_t)(now - music.due_us) < 0) break;

    uint16_t f   = pgm_read_word(&music.score[music.i][0]);
    uint16_t on  = pgm_read_word(&music.score[music.i][1]); // ms
    uint16_t off = pgm_read_word(&music.score[music.i][2]); // ms

    if (music.phase == 0) {
      // start ON segment
      if (f) {
        music.halfPeriod_us = (uint32_t)(500000UL / f);
      } else {
        music.halfPeriod_us = 0;
        digitalWrite(music.pin, LOW);
      }
      music.level = false;
      digitalWrite(music.pin, LOW);
      now = micros();
      music.nextToggle_us = now + music.halfPeriod_us;
      music.due_us += (on ? (uint32_t)on * 1000UL : 1000UL);
      music.phase = 1;

    } else if (music.phase == 1) {
      // rest segment
      music.halfPeriod_us = 0;
      digitalWrite(music.pin, LOW);
      music.due_us += (off ? (uint32_t)off * 1000UL : 1000UL);
      music.phase = 2;

    } else {
      // next note
      music.i++;
      if (music.i >= music.len) music.i = 0;
      music.phase = 0;
    }
  }
}

// Generated using https://github.com/ShivamJoker/MIDI-to-Arduino


int musicPin = 4;      // music output

void setup() {
  pinMode(musicPin, OUTPUT);

  // start music
  musicStart(musicPin, midi1, MIDI_LEN);
}

void loop() {
  musicUpdate();
    
}
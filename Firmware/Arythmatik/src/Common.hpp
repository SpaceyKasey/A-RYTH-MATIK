#pragma once
#include <Arduino.h>
#include <Encoder.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "EEPROMManager.hpp"
#include "EncoderDirection.hpp"
#include "Config.hpp"

enum EuclidMenuEnum
{
    channel = 0,
    hits,
    offset,
    limit,
    mute,
    reset,
    save,
    last
};

enum class Mode
{
    Euclid = 0,
    Gate,
    Last,
    Setup // Setup mode special :)
};

enum class SetupMenuEnum
{
    Mode = 0,
    Invert,
    Save,
    Restart,
    Last
};

const PROGMEM static byte euc16[17][16] = { // euclidian rythm
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
    {1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0},
    {1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0},
    {1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0},
    {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
    {1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0},
    {1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1},
    {1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1},
    {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1},
    {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

const PROGMEM byte graph_x[6] = {0, 40, 80, 15, 55, 95}; // each chanel display offset
const PROGMEM byte graph_y[6] = {0, 0, 0, 32, 32, 32};   // each chanel display offset

const PROGMEM byte x16[16] = {15, 21, 26, 29, 30, 29, 26, 21, 15, 9, 4, 1, 0, 1, 4, 9}; // Vertex coordinates
const PROGMEM byte y16[16] = {0, 1, 4, 9, 15, 21, 26, 29, 30, 29, 26, 21, 15, 9, 4, 1}; // Vertex coordinates

// random assign
const byte PROGMEM hit_occ[6] = {0, 10, 20, 20, 40, 80};   // random change rate of occurrence
const byte PROGMEM off_occ[6] = {10, 20, 20, 30, 40, 20};  // random change rate of occurrence
const byte PROGMEM mute_occ[6] = {20, 20, 20, 20, 20, 20}; // random change rate of occurrence
const byte PROGMEM hit_rng_max[6] = {0, 14, 16, 8, 9, 16}; // random change range of max
const byte PROGMEM hit_rng_min[6] = {0, 13, 6, 1, 5, 10};  // random change range of max

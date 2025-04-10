# XIAO MIDI Synth Board

## Overview
This project is based on the **xiao_esp32s3** microcontroller and the **SAM2695** driver board. By using this library, you can implement a wide range of MIDI functionalities, such as adjusting instrument sounds, rhythms, and notes, supporting multi-track and drum beat playback.

## Features

**Change Instrument Sounds**  
   There are **127** different instrument sounds available. However, some sounds may have lower volume, which could result in less noticeable effects. Most sounds, however, are clear and suitable for various applications.

**Change Tempo**  
   You can modify the tempo of the music, allowing for flexible rhythm control and enabling playback at different speeds.

**Change Scale**  
   The pitch of the notes can be adjusted, allowing you to change between higher or lower notes according to your needs.

**Adjust Volume**  
   The volume of the sound can be adjusted to meet your requirements, making it adaptable to different environments and needs.

**Multi-track Playback**  
   Support for simultaneous playback of multiple tracks, where each track's tempo can be controlled independently via software.

**Chord Playback**  
   Supports chord playback, where chords are made up of multiple notes. The number of notes in the chord can be controlled via software, allowing for different melodies to be played.

**Drum Beat Playback**  
   Track 9 is designated as a specific drum track for playing drum beats, which can be customized and edited.

**State Machine Mode**  
   Utilizes a state machine to trigger different button functions based on the current state, allowing for more flexible and customizable operations.

**LED Indicators**  
   In the example program, three different states correspond to different LED blinking times for easy state differentiation.

**Software Serial**
   Before using Software Serial, please first install the **EspSoftwareSerial library**. 
   The installation process is as follows: Open the Arduino IDE, search for "EspSoftwareSerial" in the library manager on the left side, and then install it.
   This driver library provides methods for initializing software serial and example sketches. Please refer to the relevant examples for using software serial.


## Usage Instructions
1. Download and import this library into your development environment.
2. Connect the XIAO ESP32S3 to the SAM2695 driver board.
3. Modify the code as needed to adjust instrument sounds, tempo, scale, and other parameters.
4. Use the software serial port to send MIDI signals or control the LED indicators.

## Project Dependencies
- **xiao_esp32s3** microcontroller
- **SAM2695** driver board
- Arduino IDE or PlatformIO (depending on your development environment)

## License
This project is licensed under the [MIT License](LICENSE), allowing you to freely use, modify, and distribute it.



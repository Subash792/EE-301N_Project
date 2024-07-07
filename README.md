# Design and implementation of a model of an EVM

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Demo link](#Demo-link)
- [Team Members](#team-members)

## Overview
The project aimed at the design and inplementation of a model of an EVM. This project was aimed at learning the concepts of Microcontroller programming and embedded systems.  
The following compnents were used to create the model of the EVM:
1. Arduino UNO
2. (16x2) LCD display
3. Keypad interface

The microcontroller was programmed using Arduino IDE.
This repository contains the code files of the project. There are separate code files used to unit test the various elements of the device namely LCD display and keypad interface.
The main code file is EE-301N_project.ino

## Features
The model of the EVM has two modes:
1. Voting mode
2. Counting mode

In voting mode:
Each voter has a separate voter ID and password that he can use to validate his vote.
Every voter is allowed to vote only once.
The tally of the votes are stored in the EEPROM available in the ATMEGA 328p.

In counting mode:
The official has a special username and password that he can use to enter the counting mode and see a tally of votes.
For more features please take a look at the video demo of how to use the EVM.

## Demo Link
Please find the video link for the demo of our model. If you are not able to access it please send a mail to gantisubash@gmail.com 
 
https://drive.google.com/file/d/11n22xgnto96I5ypJpDzutBeLcmoM3VRl/view?usp=drivesdk

## Team Members

1. Subash Ganti
2. Sajan Swaroop Reddy

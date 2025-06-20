# Minigin

Minigin is a very small project using [SDL2](https://www.libsdl.org/) and [glm](https://github.com/g-truc/glm) for 2D c++ game projects. It is in no way a game engine, only a barebone start project where everything sdl related has been set up. It contains glm for vector math, to aleviate the need to write custom vector and matrix classes.

[![Build Status](https://github.com/avadae/minigin/actions/workflows/msbuild.yml/badge.svg)](https://github.com/avadae/msbuild/actions)
[![GitHub Release](https://img.shields.io/github/v/release/avadae/minigin?logo=github&sort=semver)](https://github.com/avadae/minigin/releases/latest)

# Goal

Minigin can/may be used as a start project for the exam assignment in the course [Programming 4](https://youtu.be/j96Oh6vzhmg) at DAE. In that assignment students need to recreate a popular 80's arcade game with a game engine they need to program themselves. During the course we discuss several game programming patterns, using the book '[Game Programming Patterns](https://gameprogrammingpatterns.com/)' by [Robert Nystrom](https://github.com/munificent) as reading material. 

# Disclaimer

Minigin is, despite perhaps the suggestion in its name, **not** a game engine. It is just a very simple sdl2 ready project with some of the scaffolding in place to get started. None of the patterns discussed in the course are used yet (except singleton which use we challenge during the course). It is up to the students to implement their own vision for their engine, apply patterns as they see fit, create their game as efficient as possible.

# Use

Either download the latest release of this project and compile/run in visual studio or, since students need to have their work on github too, they can use this repository as a template (see the "Use this template" button at the top right corner). There is no point in forking this project.

# Link 

[REPO](https://github.com/QuentinDemuynck-DAE/Prog4Ass1)

# Simple 2D Game Engine & Burger Time Demo 

A **Burger Time** remake serves as a demonstration for the component-based game engine made for Prog 4, which allows for modification of new gameplay mechanics and player vs. AI control.
  
## Features 

### Entity-Component System 

- **Game Objects**  
  
  Construct entities at run time by assembling various components.  

- **Flexible Composition**  

  Change behavior dynamically without the need for subclassing by adding and removing components.  

### Event & Observer Pattern  

- **Custom Event Types**  

  Create your own events with customizable data payloads for high flexibility.

- **Decoupled Messaging**  

  Emit and subscribe to game events through subjects and observers.  

### Finite State Machines  

- **Character & System States**  

  Create states with entry and exit callbacks.

- **Configurable Transitions**  

  Use events, timers, or user-defined conditions to trigger state changes.

### Command Pattern & Input Abstraction  

- **AI Controllers**  

  Create an AI control logic that translates and issues commands akin to human players, allowing seamless shift between manual and AI control.  

- **Multiple Controllers**  

  Attach any number of controller components (keyboard, gamepad, AI) to an entity.  

- **Rebindable Commands**  

  Assign physical inputs to actions such as `Left`, `Up`, `Shoot`, etc.

### Scene Management

- **Level & UI Scenes**

- Load/unload levels, menus.

### Sound System

- **Music & SFX**
-SAAS

## Burger Time Demo

A remake showcasing engine features:

- **Level Design**  
  3 of the original stages
- **Enemy AI**  
  Sausage, Egg, Pickle
- **Pepper Mechanics**  
  Sprinkle pepper to stun enemies.
- **Scene Transitions**  
  Title screens and levels

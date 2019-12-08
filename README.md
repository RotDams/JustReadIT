<img src="src/assets/logos/logo.png" alt="logo" width="75"/>

# JustReadIt

![epita](https://img.shields.io/badge/EPITA-project-brightgreen)
![S3](https://img.shields.io/badge/Semester-S3-important)
![C](https://img.shields.io/badge/Language-C-blueviolet)
![C version](https://img.shields.io/badge/Version-c99-informational)

JustReadIt is a text recognition tool developed as part of EPITA's 3rd semester project.

This software allows to extract a text from an image, and this through 3 processes.

- [x] A segmentation of the image into paragraphs, lines, words and characters.

- [x] A recognition of each character through a neural network.

# Getting Started

These instructions will get you a copy of the project up and running on your machine for testing purposes and production.

## Prerequisites

- [SDL 1](https://www.libsdl.org/download-1.2.php)

- [GTK 3](https://developer.gnome.org/gtk3/stable/)

- [SDL_image](https://www.libsdl.org/projects/SDL_image/)

- [SDL_gfx](http://www.ferzkopp.net/wordpress/2016/01/02/sdl_gfx-sdl2_gfx/)

## Installing

- Download the project from github

```
$ git clone https://github.com/RotDams/JustReadIT
```

- Go in the created folder, make and run

```
$ cd JustReadIT
$ make
```

- A JustReadIt executable should be created.

## Running

Our program currently offers two main functions, segmentation and xor.

### Complete OCR

The complete OCR can be start easily, and help you by showing an UI. You can start it by just typing:

```
$ ./JustReadIt
```

### Trainning

If you want, you can try the neural network. For it, just start the program with the -t flag.

```
$ ./JustReadIt -t
```
 
 Our function can also take 3 optional parameters.
 
 - `--save` (or `-s`): if specify, the code will save the data of neural networks after working.
 - `--load` (or `-l`): if specify, the code will load the saved data of neural networks before working.
 - `--fonts x` (or `-f x`): if specify, the code will set the number of font to x.
 


# Credits

JustReadIt was developed by `damien.champeyroux`, `quentin.briolant`, `adrien.barens` and `sami.beyhum`.

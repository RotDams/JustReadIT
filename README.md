<img src="src/assets/logos/logo.png" alt="logo" width="75"/>

# JustReadIt

![epita](https://img.shields.io/badge/EPITA-project-brightgreen)
![S3](https://img.shields.io/badge/Semester-S3-important)
![C](https://img.shields.io/badge/Language-C-blueviolet)
![C version](https://img.shields.io/badge/Version-c99-informational)

JustReadIt is a text recognition tool developed as part of EPITA's 3rd semester project.

This software allows to extract a text from an image, and this through 3 processes.

- [x] A segmentation of the image into paragraphs, lines, words and characters.

- [ ] A recognition of each character through a neural network.

# Getting Started

These instructions will get you a copy of the project up and running on your machine for testing purposes and production.

## Prerequisites

- [SDL 1](https://www.libsdl.org/download-1.2.php)

- [GTK 3](https://developer.gnome.org/gtk3/stable/)

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

### Segmentation

Segmentation is based on an image, so it is important to provide the link from the image to the program.
This is to be done via the path parameter.

```
$ ./JustReadIt --path public/my_image.bmp
```

*The keyword `--path` can be reduced to `-p`.*

You can also see the image during the segmentation. For this, you need to provide the `--dev` parameter. You can also just provide a `-d`.

```
$ ./JustReadIt --path public/my_image.bmp --dev
```

### XOR

As XOR is only a step in discovering neural networks, it is not automatically launched.
It is therefore necessary to use the `--xor` (or `-x`) flag.
 
 Our function can also take 3 optional parameters.
 
 - `--save` (or `-s`): if specify, the code will save the data of neural networks after working.
 - `--load` (or `-l`): if specify, the code will load the saved data of neural networks before working.
 - `--iteration x` (or `-i x`): if specify, the code will set the number of iteration to x. (default value: 1 000 000)
 


# Credits

JustReadIt was developed by `damien.champeyroux`, `quentin.briolant`, `adrien.barens` and `sami.beyhum`.

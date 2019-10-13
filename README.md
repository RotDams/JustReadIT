<img src="src/assets/logos/logo.png" alt="logo" width="75"/>

# JustReadIt

JustReadIt is a text recognition tool developed as part of EPITA's 3rd semester project.

This software allows to extract a text from an image, and this through 3 processes.

- A segmentation of the image into paragraphs, lines, words and characters.

- A recognition of each character through a neural network.

- A reconstruction of the word using a dictionary of existing words.

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

- Go in the created folder, make an run

```
$ cd JustReadIT
$ make
$ ./output.exe
```

# Credits

JustReadIt was developed by `damien.champeyroux`, `quentin.briolant`, `adrien.barens` and `sami.beyhum`.
<img src="src/assets/logos/logo.png" alt="logo" width="75"/>

# JustReadIt

![c version](https://camo.githubusercontent.com/50863320f4d71daf90b585eb2a1fe6a6c4715cd2/68747470733a2f2f696d672e736869656c64732e696f2f62616467652f632d7374616e646172642532306339392d626c75652e737667)
![version](https://img.shields.io/badge/Version-1.0.0-informational)

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

# ImgCompressor

## What it is ?

A C++17 implementation of k-mean algorithm for image compression.\
Using SFML for graphical part.

Compression result may vary as there is a random part.

## Building

This program is build with cmake and dependencies managed thanks to conan.

```sh
$> cmake -B build . && cmake --build build
```

## Usage

```sh
$> ./bin/app -h
Usage: app [--file=<path>] [--level=<level>] [-i]

Options:
  file      path of the source target
  level     number of color in the ouput image
  i         launch in interactive mode

Interactive Mode:
  mouse left button     increment the colors count
  mouse right button    decrement the colors count
  any key               save the picture
```

## Screenshots

Original image

![Original](./asset/butterfly.jpg "Original")

With 3 colors `Done in 0.023 seconds`

![3Colors](./asset/butterfly.jpg.compressed_3.png "3Colors")

With 10 colors `Done in 0.057 seconds`

![10Colors](./asset/butterfly.jpg.compressed_10.png "10Colors")

With 1000 colors `Done in 4.548 seconds`

![1000Colors](./asset/butterfly.jpg.compressed_1000.png "1000Colors")

# gifTer Animations
A Linux C++ program built to render small ASCII animations on a terminal.
Built by Ian Harvey.

## Building
Run the `make` command in the root folder of the project. Your program will be generated in the root folder. To clean up the directory, run the `make clean` command.

## Use
The program currently expects 2 arguments to be used.
1. File name
   - The name of the file you wish to display
2. Pixels Per Pixel
   - The amount of pixels (x) in a subsection square (x by x) you would like to compress to make the display of the file easier to see on the terminal

Overall, the command should look like `./asciiAnimate [FileName (string)] [PixelsPerPixel (int)]`

## Demonstration
Watch the video demonstration here
[![Video Demonstration](https://img.youtube.com/vi/f0_KpcjPcG4/maxresdefault.jpg)](https://www.youtube.com/watch?v=f0_KpcjPcG4)

## Supported File Types
Currently there is only one supported File type. If I chose to add more, you will see them show up here:
1. .gif File
   - [Giflib file information](https://giflib.sourceforge.net/whatsinagif/bits_and_bytes.html)
   - [Daubnet file information](https://www.daubnet.com/en/file-format-gif)
   - ### Unsupported behavior
     - Gif Disposal method 3
     - Gifs with interlacing

## Supported Terminals
Terminal Detection can be tricky (I think... Idk I didn't bother :/) so there is also only one type of supported terminal
1. xterm-256color
   - [xterm ANSI codes](https://www.ditig.com/256-colors-cheat-sheet)

## Supported Operating Systems
1. Linux


## Known Issues
Unfortunately, as I have begun testing this program on different files, I have come to notice some major issues (Segmentation faults, etc.) with certain files. I have not yet taken a look into and debugged what might be causing these issues, but intend to do so in the future. Unfortunately, for now, that means that certain files do not produce their expected output.

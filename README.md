# Terminal-ASCII-Animations
A Linux C++ program built to render small ASCII animations on a terminal.
Built by Ian Harvey.

## Building
Run the `make` command in the root folder of the project. Your program will be generated in the root folder. To clean up the directory, run the `make clean` command.

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

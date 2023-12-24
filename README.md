# Install Required Libraries
`sudo apt install libsdl2-2.0-0 libsdl2-dev libsdl2-ttf-2.0-0 libsdl2-ttf-dev`

# Compilation

While in the project folder,

`g++ src/*.cpp -I include/ -lSDL2 -lSDL2_ttf -o bin/doge`

# Run

`./bin/doge`
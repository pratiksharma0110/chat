# Compilation and Execution

## Install Required Libraries
`sudo apt install libsdl2-2.0-0 libsdl2-dev libsdl2-ttf-2.0-0 libsdl2-ttf-dev`

## Compilation

While in the project folder,

* First compile the server

`g++ src/networking/multiClientServ.cpp -o bin/server`

* Now compile the client

`g++ src/*.cpp src/networking/clienthandler.cpp -I include/ -lSDL2 -lSDL2_ttf -lSDL2_image -o bin/client

## Run

* Run the server

`./bin/server`

* Run the client
  
./bin/client

# Carcassonne

## Prerequisites

* To compile the project, SDL-1.2, SDL_image-1.2 and SDL_gfx are needed. It will not compile with SDL2 !
* To compile the doc, Doxygen (at least 1.8.3) is needed.

## Compiling

* To compile the project, go to the root and type `make`.
* To compile the tests, go to the root and type `make test`.
* To compile the doc, go to the root and type `make doc`.

## Installing

To install the project, go to the root and type `make install`. This will install the server and the clients in the directory *install*.

## Running

* To run the project, go to the *install* directory after you ran `make install` and run `./server [-g] [FILES]`. Option `-g` is to enable graphical interface and `FILES` are the paths of the clients you want to load.
  * ex: `./server ./client_random.so ./client_human.so`
  * ex2: `./server -g ./client_*.so`

* To run the tests, go to the *tst* directory after you ran `make test` and type `./test_X` where `X` is the test you want to run (ex: test_road).

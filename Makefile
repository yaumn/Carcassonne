export CC=gcc
export CFLAGS=-Wall -Wextra -std=c99
export ENABLE_GUI=1


.PHONY: build
build:
	@make -C src/server
	@make -C src/clients

.PHONY: test
test: 
	@make -C tst

.PHONY: install
install: build
	cp src/server/server install/
	cp src/clients/*.so install/

.PHONY: doc
doc:
	cd doc && doxygen Doxyfile ; cd ../

.PHONY: all
all: build test install doc

.PHONY: clean
clean:
	@make -C src/server clean
	@make -C src/clients clean
	rm -f src/common/*.o src/common/*~
	@make -C tst clean
	rm -f install/server install/*.so
	rm -rf doc/html doc/latex

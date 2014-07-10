all : makeall sldr.o

makeall:
	cd sldr0 && make
	cd sldr1 && make
	
sldr.o : sldr0/sldr0.o sldr1/sldr1.o
	cat sldr0/sldr0.o sldr1/sldr1.o > $@

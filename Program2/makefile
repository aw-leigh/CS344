CXX = gcc
CXXFLAGS = -lpthread

rooms: wilsoan6.buildrooms.c
	@${CXX} -o wilsoan6.buildrooms wilsoan6.buildrooms.c

game: wilsoan6.adventure.c
	@${CXX} -o wilsoan6.adventure wilsoan6.adventure.c ${CXXFLAGS}

clean:
	@rm -rf wilsoan6.buildrooms wilsoan6.adventure wilsoan6.r* currentTime.txt
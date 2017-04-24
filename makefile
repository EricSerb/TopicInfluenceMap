##########################################################
#
# Eric Serbousek
#
##########################################################

define HEADER

   ---------------------------------------------------------
   |                                                       |
   |             DBProj Program - Version 0.0.0            |
   |                                                       |
   |                                                       |
   |                   Eric Serbousek                      |
   |                                                       |
   |                                                       |
   ---------------------------------------------------------

   . . . building components . . .

endef

define FOOTER

  . . . components ready

endef

export HEADER FOOTER

##########################################################


CC = g++ -std=c++17
CCFLAGS = -Wall -pedantic -pthread -g
SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SRCS))
EXE := main

##########################################################


.SUFFIXES :
.PHONY : fresh clean cls hdr ftr semi_clean


##########################################################

all : fresh

$(EXE) : $(OBJS)
	$(CC) $(CCFLAGS) -o $@ $^

%.o : %.cpp
	$(CC) $(CCFLAGS) -c $<


##########################################################


fresh : clean cls hdr $(EXE) ftr semi_clean

clean :
	-@rm $(OBJS) $(EXE) 2>/dev/null || true

cls :
	@clear

hdr :
	@$(info $(HEADER))

ftr :
	@$(info $(FOOTER))

semi_clean :
	-@rm $(OBJS) 2>/dev/null || true

	
##########################################################

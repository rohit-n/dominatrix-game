SRC = $(wildcard *.cpp)
SRC += ../elib/qstring.cpp
SRC += ../elib/emisc.cpp
SRC += ../elib/zone.cpp

DEBUG=0
CFLAGS= -DSIN -DELIB_GAMEDLL -fPIC

ifeq ($(MINGW),1)
DLL_NAME := gamex86_64.dll
else
DLL_NAME := game.so
endif

ifeq ($(DEBUG),1)
CFLAGS += -g
else
CFLAGS += -O2
endif

ifeq ($(IS_HARD_LINKED),1)
SRC := $(filter-out pulserifle.cpp, $(SRC))
SRC := $(filter-out thrall.cpp, $(SRC))
SRC := $(filter-out bacrodai.cpp, $(SRC))
SRC := $(filter-out turret.cpp, $(SRC))
SRC := $(filter-out skeet.cpp, $(SRC))
SRC := $(filter-out speargun.cpp, $(SRC))
SRC := $(filter-out eonandpeon.cpp, $(SRC))
SRC := $(filter-out quantumd.cpp, $(SRC))
SRC := $(filter-out sniperrifle.cpp, $(SRC))
SRC := $(filter-out peon.cpp, $(SRC))
SRC := $(filter-out keys.cpp, $(SRC))
SRC := $(filter-out shotrocketlauncher.cpp, $(SRC))
SRC := $(filter-out shield.cpp, $(SRC))
SRC := $(filter-out launcher.cpp, $(SRC))
SRC := $(filter-out glowstick.cpp, $(SRC))
SRC := $(filter-out camgun.cpp, $(SRC))
SRC := $(filter-out lensflare.cpp, $(SRC))
SRC := $(filter-out heligun.cpp, $(SRC))
SRC := $(filter-out earthquake.cpp, $(SRC))
SRC := $(filter-out rocketlauncher.cpp, $(SRC))
SRC := $(filter-out bouncingbetty.cpp, $(SRC))
SRC := $(filter-out fists.cpp, $(SRC))
SRC := $(filter-out stungun.cpp, $(SRC))
SRC := $(filter-out silencer.cpp, $(SRC))
SRC := $(filter-out genericrocket.cpp, $(SRC))
SRC := $(filter-out ctf_turret.cpp, $(SRC))
SRC := $(filter-out magnum.cpp, $(SRC))
SRC := $(filter-out hammer.cpp, $(SRC))
SRC := $(filter-out mutanthands.cpp, $(SRC))
SRC := $(filter-out securityturret.cpp, $(SRC))
SRC := $(filter-out secgun.cpp, $(SRC))
SRC := $(filter-out box.cpp, $(SRC))
SRC := $(filter-out powerups.cpp, $(SRC))
SRC := $(filter-out animals.cpp, $(SRC))
SRC := $(filter-out reactiveshields.cpp, $(SRC))
SRC := $(filter-out shotgun.cpp, $(SRC))
DLL_NAME := game.a
endif

OBJ = $(patsubst %.cpp, %.o, $(SRC))

all: $(OBJ) q_shared.o
	$(CXX) $(OBJ) q_shared.o -shared -o $(DLL_NAME)

static: $(OBJ) q_shared.o
	$(AR) rcs $(DLL_NAME) $(OBJ) q_shared.o

%.o: %.cpp
	$(CXX) $< -c $(CFLAGS) -o $@

q_shared.o: q_shared.c
	$(CXX) q_shared.c -c $(CFLAGS) -o q_shared.o

clean:
	rm -f *.o
	rm -f $(DLL_NAME)
	rm -f ../elib/*.o

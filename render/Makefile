CXX = mpicxx
CC  = mpicc
LD  = mpicxx
F90  = mpif90

CUDA_TK  = /usr/local/cuda-5.5
#CUDA_TK  = /usr/local/cuda
CUDA_TK = $(HOME)/usr/local/cuda-5.5
CXXFLAGS = -Wall -fopenmp -std=c++11
CXXFLAGS += -Wno-reorder
CXXFLAGS += -Wno-write-strings
CXXFLAGS += -DMPICH_IGNORE_CXX_SEEK  -DMPICH_SKIP_MPICXX
CFLAGS =  -Wall

OPT += -O3 -g

CFLAGS += $(OPT)
CXXFLAGS += $(OPT)


NVCC=$(CUDATK)/bin/nvcc
NVCCFLAGS=-arch=sm_35

ENGINE=particles
ENGINE=smoke
ENGINE=density
#ENGINE=splotch


INCLUDE_FLAGS= -I./ -I./$(ENGINE)/ -I/export/scratch1/jbedorf/Bonsai/runtime/include/
#INCLUDE_FLAGS= -I./ -I./$(ENGINE)/
INCLUDE_FLAGS += -I$(CUDA_TK)/include 
INCLUDE_FLAGS += -I./IceT/include/
#LDFLAGS = -lGL -lGLU -lglut -lGLEW -fopenmp
#-L$(PWD)/lib/linux -lGLEW_x86_64
LDFLAGS = -L./ -L/usr/lib64/ -lGL -lGLU -lglut -fopenmp -lGLEW 
LDFLAGS += -L./IceT/lib -lIceTCore -lIceTGL  -lIceTMPI


ifeq ($(APPLEOSX),1)
	INCLUDE_FLAGS += -I/opt/local/include
	CXXFLAGS +=-Wno-deprecated-declarations 
	LDFLAGS = -framework OpenGL -L/opt/local/lib -lglut -lglew -fopenmp
endif

OBJ = main.o $(ENGINE)/renderloop.o anyoption.o $(ENGINE)/renderer.o paramgl.o
ifeq ($(ENGINE),smoke)
	OBJ += tr.o
	OBJ += smoke/GLSLProgram.o
	OBJ += smoke/SmokeShaders.o
	OBJ += smoke/framebufferObject.o
#	OBJ += smoke/depthSort.cu_o
	OBJ += smoke/depthSort.o
endif
ifeq ($(ENGINE),density)
	OBJ += tr.o
	OBJ += density/GLSLProgram.o
	OBJ += density/SmokeShaders.o
	OBJ += density/framebufferObject.o
#	OBJ += density/depthSort.cu_o
	OBJ += density/depthSort.o
endif
ifeq ($(ENGINE),splotch)
	OBJ += splotch/GLSLProgram.o
	OBJ += splotch/splotch.o
endif
ifeq ($(ENGINE),splotchGL)
	OBJ += splotch/GLSLProgram.o
	OBJ += splotchGL/splotch.o
endif

PROG= renderer

all: $(PROG)

$(PROG): $(OBJ)
	$(LD) $^ -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

%.cu_o: %.cu
	$(NVCC) $(NVCCFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

clean:
	/bin/rm -rf $(OBJ) $(PROG)

clean_bak:
	find . -name '*~' -exec /bin/rm -rf '{}' \;



$(OBJ): anyoption.h  paramgl.h  param.h  read_tipsy.h  RendererData.h  $(ENGINE)/renderloop.h  $(ENGINE)/renderer.h  tipsydefs.h  vector_math.h \
	splotch/Blending.h  splotch/GLSLProgram.h  splotch/MathArray.h  splotch/renderer.h  splotch/renderloop.h  splotch/Splotch.h  splotch/Texture.h  splotch/Vertex.h 



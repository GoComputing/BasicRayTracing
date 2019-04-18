# ************************ Configuración del Makefile ************************ #
.PHONY: clean mrproper all run
.SECONDARY:


# *********************************** Rutas ********************************** #
BIN = bin
SRC = src
INC = include
OBJ = obj
RES = res


# ************************* Variables de compilación ************************* #
CL = mpicxx
CLFLAGS = -O3
CXX = mpicxx
CXXFLAGS = -I$(INC) -Wall -Wextra -pedantic -c -g -O3
DEBUGGER = valgrind
DEBUGGER_FLAGS = 
EXECUTER = mpirun
EXECUTER_FLAGS = -np 4


# ********************************** Módulos ********************************* #
EXEC = $(BIN)/main

MODULOS  = $(OBJ)/geometria.o
MODULOS += $(OBJ)/herramientas.o
MODULOS += $(OBJ)/fisica.o
MODULOS += $(OBJ)/modelos.o
MODULOS += $(OBJ)/escena.o


# ******************************** Parámetros ******************************** #
PARAMETROS = $(RES)/PGM 1


# **************************** Reglas por defecto **************************** #
all: $(EXEC)


run: $(EXEC)
	$(EXECUTER) $(EXECUTER_FLAGS) ./$< $(PARAMETROS)

debug: $(EXEC)
	$(DEBUGGER) $(DEBUGGER_FLAGS) ./$< $(PARAMETROS)


# *************************** Reglas de compilación ************************** #
$(BIN)/%: $(OBJ)/%.o $(MODULOS)
	$(CL) $(CLFLAGS) -o $@ $< $(MODULOS)

$(OBJ)/%.o: $(SRC)/%.cpp $(wildcard $(INC)/*.h)
	$(CXX) $(CXXFLAGS) -o $@ $<


# ********************************* Limpieza ********************************* #
clean:
	-rm $(EXEC)

mrproper: clean
	-rm $(MODULOS)

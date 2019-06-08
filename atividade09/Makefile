all: 
	+@echo "Digite make serial ou make paralell" 

GCC = gcc 
OMP = -fopenmp
BIN = bin
OBJ = obj
INC = -Iinc/
SRC_DIR = /home/matt/Desktop/folder/Computacao_Paralela/atividade09/src
BIN_DIR = /home/matt/Desktop/folder/Computacao_Paralela/atividade09/bin

serial: txt folder app_serial
paralell: txt folder app_paralell

app_serial: main_serial matrix_op_serial
	@echo "Linking..."
	+@$(GCC) -o $(BIN)/app_serial $(OBJ)/main.o $(OBJ)/matrix_op.o 
	@echo "Binary and text files are ready in $(BIN)/app_serial"
	
main_serial:
	+@echo "Compile: $@"
	@$(GCC) src/main.c $(INC) -c -o $(OBJ)/main.o

matrix_op_serial:
	+@echo "Compile: $@"
	@$(GCC) src/matrix_op.c $(INC) -c -o $(OBJ)/matrix_op.o

app_paralell: main_paralell matrix_op_paralell
	@echo "Linking..."
	+@$(GCC) $(OMP) -o $(BIN)/app_paralell $(OBJ)/main.o $(OBJ)/matrix_op.o 
	@echo "Binary and text files are ready in $(BIN)/app_paralell"
	
main_paralell:
	+@echo "Compile: $@"
	@$(GCC) $(OMP) src/main.c $(INC) -c -o $(OBJ)/main.o

matrix_op_paralell:
	+@echo "Compile: $@"
	@$(GCC) $(OMP) src/matrix_op.c $(INC) -c -o $(OBJ)/matrix_op.o

txt:
	@touch $(SRC_DIR)/.mat_dot.txt

folder:
	@mkdir -p $(OBJ) $(BIN) 
	@cp $(SRC_DIR)/*.txt $(BIN_DIR) 
	@mv $(SRC_DIR)/.*.txt $(BIN_DIR) 
	
clean:
	@rm -rf $(OBJ)/* $(BIN)/* $(OBJ) $(BIN)

###############################################################################
all: txt folder app

GCC = mpicc
BIN = bin
OBJ = obj
INC = -Iinc/
SRC_DIR = /home/matt/Desktop/folder/Comp_paralela/atividade_05/src
BIN_DIR = /home/matt/Desktop/folder/Comp_paralela/atividade_05/bin

app: main matrix_op
	@echo "Linking..."
	+@$(GCC) -o $(BIN)/app $(OBJ)/main.o $(OBJ)/matrix_op.o 
	@echo "Binary and text files are ready in $(BIN)/app"
	
main:
	+@echo "Compile: $@"
	@$(GCC) src/$@.c $(INC) -c -o $(OBJ)/$@.o

matrix_op:
	+@echo "Compile: $@"
	@$(GCC) src/$@.c $(INC) -c -o $(OBJ)/$@.o

txt:
	@touch $(SRC_DIR)/.mat_dot.txt

folder:
	@mkdir -p $(OBJ) $(BIN)  
	@cp $(SRC_DIR)/*.txt $(BIN_DIR)
	@mv $(SRC_DIR)/.*.txt $(BIN_DIR) 
	
clean:
	@rm -rf $(OBJ)/* $(BIN)/* $(OBJ) $(BIN)

###############################################################################
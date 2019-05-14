all: txt folder app

GCC = gcc
BIN = bin
OBJ = obj
INC = -Iinc/
SRC_DIR = /home/matt/Desktop/folder/Computacao_Paralela/atividade_extra/src
BIN_DIR = /home/matt/Desktop/folder/Computacao_Paralela/atividade_extra/bin

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
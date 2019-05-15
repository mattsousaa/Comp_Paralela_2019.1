all: txt folder app

MPICC = mpicc
BIN = bin
OBJ = obj
INC = -Iinc/
SRC_DIR = /home/matt/Desktop/folder/Computacao_Paralela/atividade06/src
BIN_DIR = /home/matt/Desktop/folder/Computacao_Paralela/atividade06/bin

app: main matrix_op
	@echo "Linking..."
	+@$(MPICC) -o $(BIN)/app $(OBJ)/main.o $(OBJ)/matrix_op.o 
	@echo "Binary and text files are ready in $(BIN)/app"
	
main:
	+@echo "Compile: $@"
	@$(MPICC) src/$@.c $(INC) -c -o $(OBJ)/$@.o

matrix_op:
	+@echo "Compile: $@"
	@$(MPICC) src/$@.c $(INC) -c -o $(OBJ)/$@.o

txt:
	@touch $(SRC_DIR)/.mat_dot.txt

folder:
	@mkdir -p $(OBJ) $(BIN)  
	@cp $(SRC_DIR)/*.txt $(BIN_DIR)
	@mv $(SRC_DIR)/.*.txt $(BIN_DIR) 
	
clean:
	@rm -rf $(OBJ)/* $(BIN)/* $(OBJ) $(BIN)

###############################################################################
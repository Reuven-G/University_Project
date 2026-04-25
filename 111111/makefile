CC     = gcc
CFLAGS = -Wall -ansi -pedantic -g

TARGET = assembler

OBJS =  main.o              \
        preprocessor.o      \
        getFirstWord.o      \
        firstPass.o         \
        processLine.o       \
        analyzeRow.o        \
        parseOperands.o     \
        updateIC.o          \
        updateDC.o          \
        symbolTable.o       \
        addSymbol.o         \
        findSymbol.o        \
        printSymbolTable.o  \
        freeSymbolTable.o   \
        instructionTable.o  \
        findInstruction.o   \
        checkOperands.o     \
        getAddressingType.o \
        isRegister.o        \
        isRegisterName.o    \
        isValidLabel.o      \
        isCommandName.o     \
        isDirectiveName.o   \
        getdDirectiveType.o \
        handleData.o        \
        handleString.o      \
        handleExtern.o      \
        handleEntry.o       \
        skipWhiteChars.o    \
        isEmptyLine.o       \
        isComment.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean

CC     = gcc
CFLAGS = -Wall -ansi -pedantic -g

TARGET = assembler

OBJS = \
    main.o \
    \
    preprocessor.o \
    getFirstWord.o \
    \
    first_pass/firstPass.o \
    first_pass/updateIC.o \
    first_pass/updateDC.o \
    first_pass/parseOperands.o \
    first_pass/calcInstructionSize.o \
    first_pass/processLine.o \
    \
    analyzeRow/analyzeRow.o \
    \
    second_pass/secondPass.o \
    second_pass/encodeInstruction.o \
    second_pass/encodeOperand.o \
    \
    files/writeOb.o \
    files/writeEnt.o \
    files/writeExt.o \
    \
    symbolTable/symbolTable.o \
    symbolTable/addSymbol.o \
    symbolTable/findSymbol.o \
    symbolTable/printSymbolTable.o \
    symbolTable/freeSymbolTable.o \
    \
    instructions/instructionTable.o \
    instructions/findInstruction.o \
    instructions/checkOperands.o \
    \
    directives/getdDirectiveType.o \
    directives/handleData.o \
    directives/handleString.o \
    directives/handleExtern.o \
    directives/handleEntry.o \
    \
    addressing/getAddressingType.o \
    addressing/isRegister.o \
    \
    labels/isValidLabel.o \
    labels/isRegisterName.o \
    labels/isCommandName.o \
    labels/isDirectiveName.o \
    \
    utils/skipWhiteChars.o \
    utils/isEmptyLine.o \
    utils/isComment.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
	find . -name "*.am" -delete

.PHONY: clean

#ifndef MATRIX_MULT_TRANSLATOR_H
#define MATRIX_MULT_TRANSLATOR_H

#include "llvm/IR/Function.h"
#include "PimBackend.h"
#include <vector>

class MatrixMultTranslator {
public:
    MatrixMultTranslator();
    
    // Analyze a function to detect matrix multiplication patterns
    bool detectMatrixMultiplication(llvm::Function &function);
    
    // Generate PIM instructions for matrix multiplication
    std::vector<PimInstruction> generateInstructions();
    
private:
    // Matrix dimensions extracted from analysis
    int matrixARows;
    int matrixACols;
    int matrixBRows;
    int matrixBCols;
    int resultRows;
    int resultCols;
    
    // Memory layouts
    uint16_t matrixABaseAddress;
    uint16_t matrixBBaseAddress;
    uint16_t resultBaseAddress;
};

#endif // MATRIX_MULT_TRANSLATOR_H

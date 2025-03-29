#ifndef PIM_BACKEND_H
#define PIM_BACKEND_H

#include "llvm/IR/Module.h"
#include <vector>
#include <string>

// Define the PIM instruction format according to the ISA
struct PimInstruction {
    // Bits 18-17: Core Pointer
    uint8_t corePtr;
    
    // Bits 16-11: R/W Pointer or Opcode (depending on instruction type)
    uint8_t rwPtrOrOpcode;
    
    // Bit 10: Address Pointer
    bool addrPtr;
    
    // Bit 9: Read/Core Pointer
    bool readCorePtr;
    
    // Bits 8-0: Row Address
    uint16_t rowAddress;
    
    // For debugging/display
    std::string toString() const;
};

enum class PimInstructionType {
    MEMORY_ACCESS,
    LUT_PROGRAMMING,
    COMPUTE
};

class PimBackend {
public:
    PimBackend();
    ~PimBackend();
    
    bool processModule(llvm::Module &module);
    
    // Generate output file with PIM instructions
    bool writeOutput(const std::string &filename);
    
private:
    // Process matrix multiplication patterns
    bool processMatrixMultiplication(llvm::Function &function);
    
    // Generate memory access instructions
    std::vector<PimInstruction> generateMemoryAccessInstructions(
        uint16_t rowAddress, bool isRead);
    
    // Generate LUT programming instructions for matrix multiplication
    std::vector<PimInstruction> generateLutProgrammingInstructions();
    
    // Generate compute instructions
    std::vector<PimInstruction> generateComputeInstructions(uint8_t opcode);
    
    // The generated PIM instructions
    std::vector<PimInstruction> instructions;
};

#endif // PIM_BACKEND_H

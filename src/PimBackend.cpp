#include "PimBackend.h"
#include "MatrixMultTranslator.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include <fstream>

std::string PimInstruction::toString() const {
    std::string result;
    llvm::raw_string_ostream os(result);
    
    os << "Core Ptr: " << static_cast<int>(corePtr) 
       << ", RW/Opcode: " << static_cast<int>(rwPtrOrOpcode)
       << ", Addr Ptr: " << (addrPtr ? "1" : "0")
       << ", Read/Core Ptr: " << (readCorePtr ? "1" : "0")
       << ", Row Addr: " << rowAddress;
    
    return os.str();
}

PimBackend::PimBackend() {}

PimBackend::~PimBackend() {}

bool PimBackend::processModule(llvm::Module &module) {
    bool success = false;
    
    // Look for matrix multiplication functions
    for (auto &function : module) {
        if (processMatrixMultiplication(function)) {
            success = true;
        }
    }
    
    return success;
}

bool PimBackend::processMatrixMultiplication(llvm::Function &function) {
    MatrixMultTranslator translator;
    
    if (translator.detectMatrixMultiplication(function)) {
        std::vector<PimInstruction> matMulInstructions = translator.generateInstructions();
        
        // Add these instructions to our instruction list
        instructions.insert(instructions.end(), matMulInstructions.begin(), matMulInstructions.end());
        return true;
    }
    
    return false;
}

std::vector<PimInstruction> PimBackend::generateMemoryAccessInstructions(
    uint16_t rowAddress, bool isRead) {
    
    std::vector<PimInstruction> memInstructions;
    
    PimInstruction instr;
    instr.corePtr = 0;  // Not used for memory access
    instr.rwPtrOrOpcode = isRead ? 0b100000 : 0b010000;  // Example values for read/write
    instr.addrPtr = true;  // Using address
    instr.readCorePtr = isRead;  // Read or write
    instr.rowAddress = rowAddress;
    
    memInstructions.push_back(instr);
    
    return memInstructions;
}

std::vector<PimInstruction> PimBackend::generateLutProgrammingInstructions() {
    std::vector<PimInstruction> lutInstructions;
    
    // Example: Program LUTs for multiplication
    PimInstruction instr;
    instr.corePtr = 0b11;  // Core programming mode
    instr.rwPtrOrOpcode = 0b000100;  // Example opcode for multiply
    instr.addrPtr = false;
    instr.readCorePtr = true;
    instr.rowAddress = 0;  // Not used for LUT programming
    
    lutInstructions.push_back(instr);
    
    return lutInstructions;
}

std::vector<PimInstruction> PimBackend::generateComputeInstructions(uint8_t opcode) {
    std::vector<PimInstruction> computeInstructions;
    
    PimInstruction instr;
    instr.corePtr = 0b00;  // Not programming cores
    instr.rwPtrOrOpcode = opcode;
    instr.addrPtr = false;
    instr.readCorePtr = false;
    instr.rowAddress = 0;  // Not accessing memory
    
    computeInstructions.push_back(instr);
    
    return computeInstructions;
}

bool PimBackend::writeOutput(const std::string &filename) {
    std::ofstream outFile(filename);
    if (!outFile) {
        return false;
    }
    
    for (const auto &instr : instructions) {
        // Format the instruction as a binary string according to ISA
        uint32_t encodedInstr = 0;
        encodedInstr |= (static_cast<uint32_t>(instr.corePtr) & 0x3) << 17;
        encodedInstr |= (static_cast<uint32_t>(instr.rwPtrOrOpcode) & 0x3F) << 11;
        encodedInstr |= (instr.addrPtr ? 1 : 0) << 10;
        encodedInstr |= (instr.readCorePtr ? 1 : 0) << 9;
        encodedInstr |= (instr.rowAddress & 0x1FF);
        
        outFile << std::bitset<19>(encodedInstr) << " // " << instr.toString() << std::endl;
    }
    
    outFile.close();
    return true;
}

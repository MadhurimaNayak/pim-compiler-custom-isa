#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"

#include "PimBackend.h"

using namespace llvm;

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <IR file>\n";
        return 1;
    }

    // Parse LLVM IR file
    LLVMContext context;
    SMDiagnostic error;
    std::unique_ptr<Module> module = parseIRFile(argv[1], error, context);
    
    if (!module) {
        error.print(argv[0], errs());
        return 1;
    }

    std::cout << "Successfully loaded LLVM IR module: " << module->getName().str() << std::endl;
    
    // Initialize the PIM Backend
    PimBackend backend;
    
    // Process the module and generate PIM instructions
    if (!backend.processModule(*module)) {
        std::cerr << "Error processing module\n";
        return 1;
    }
    
    std::string outputFilename = "pim_instructions.out";
    if (backend.writeOutput(outputFilename)) {
        std::cout << "PIM instructions written to " << outputFilename << std::endl;
    } else {
        std::cerr << "Error writing PIM instructions to file\n";
        return 1;
    }
    
    std::cout << "PIM instruction generation complete\n";
    
    return 0;
}

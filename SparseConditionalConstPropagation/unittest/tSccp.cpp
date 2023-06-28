#include "../Sccp.h"
#include "llvm/IR/Function.h"
#include "llvm/AsmParser/Parser.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/SourceMgr.h"
#include <gtest/gtest.h>

namespace {

    std::unique_ptr<llvm::Module> parseIR(llvm::LLVMContext& aCtx, const char* aIr)
    {
        llvm::SMDiagnostic err;
        std::unique_ptr<llvm::Module> module = llvm::parseAssemblyString(aIr, err, aCtx);
        assert(module && "The IR can not be properly parse!");
        return module;
    }

    TEST(SparseConditionalConstPropagation, placeholder) 
    {
        EXPECT_TRUE(true);
    }

}


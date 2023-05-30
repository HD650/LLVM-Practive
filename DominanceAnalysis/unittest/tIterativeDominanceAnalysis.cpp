#include "../IterativeDominanceAnalysis.h"
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
        if(!module)
            assert(false);
        return module;
    }



    TEST(IterativeDominanceAnalysis, IfBranchMerged) {
        llvm::LLVMContext ctx;
        const char* input = R"(
            define double @foo(i32 %a, double %b, double %c)
            {
                bb_start:
                %condition = icmp sgt i32 %a, 50
                br i1 %condition, label %bb_then, label %bb_else
                bb_then:
                    %0 = fadd double %b, %c
                    br label %bb_end
                bb_else:
                    %1 = fsub double %b, %c
                    br label %bb_end
                bb_end:
                    %2 = phi double [%0, %bb_then], [%1, %bb_else]
                    ret double %2
            }
        )";
        std::unique_ptr<llvm::Module> parsedModule = parseIR(ctx, input);
        llvm::Function* foo = parsedModule->getFunction("foo");

        llvmPractice::IterativeDominanceAnalysis analysis(*foo);
    }


}


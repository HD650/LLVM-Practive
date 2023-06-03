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
        assert(module && "The IR can not be properly parse!");
        return module;
    }


    TEST(IterativeDominanceAnalysis, IfBranchMerged) 
    {
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

        llvm::Function::iterator fi = foo->begin();
        llvm::BasicBlock *bb_start = &*fi++;
        llvm::BasicBlock *bb_then = &*fi++;
        llvm::BasicBlock *bb_else = &*fi++;
        llvm::BasicBlock *bb_end = &*fi++;
        
        // bb_start
        EXPECT_TRUE(analysis.dominates(bb_start, bb_start));
        EXPECT_TRUE(analysis.dominates(bb_start, bb_then));
        EXPECT_TRUE(analysis.dominates(bb_start, bb_else));
        EXPECT_TRUE(analysis.dominates(bb_start, bb_end));
        // bb_then
        EXPECT_FALSE(analysis.dominates(bb_then, bb_start));
        EXPECT_TRUE(analysis.dominates(bb_then, bb_then));
        EXPECT_FALSE(analysis.dominates(bb_then, bb_else));
        EXPECT_FALSE(analysis.dominates(bb_then, bb_end));
        // bb_else
        EXPECT_FALSE(analysis.dominates(bb_else, bb_start));
        EXPECT_FALSE(analysis.dominates(bb_else, bb_then));
        EXPECT_TRUE(analysis.dominates(bb_else, bb_else));
        EXPECT_FALSE(analysis.dominates(bb_else, bb_end));
        // bb_end
        EXPECT_FALSE(analysis.dominates(bb_end, bb_start));
        EXPECT_FALSE(analysis.dominates(bb_end, bb_then));
        EXPECT_FALSE(analysis.dominates(bb_end, bb_else));
        EXPECT_TRUE(analysis.dominates(bb_end, bb_end));
    }

    TEST(IterativeDominanceAnalysis, UnreachableBB) 
    {
        llvm::LLVMContext ctx;
        // a test case from llvm-project/llvm/unittests/IR/DominatorTreeTest.cpp
        const char* input = R"(
            declare i32 @g()
            define void @foo(i32 %x) personality i32 ()* @g 
            {
                bb0:
                    %y1 = add i32 %x, 1
                    %y2 = add i32 %x, 1
                    %y3 = invoke i32 @g() to label %bb1 unwind label %bb2
                bb1:
                    %y4 = add i32 %x, 1
                    br label %bb4
                bb2:
                    %y5 = landingpad i32
                            cleanup
                    br label %bb4
                bb3:
                    %y6 = add i32 %x, 1
                    %y7 = add i32 %x, 1
                    ret void
                bb4:
                    %y8 = phi i32 [0, %bb2], [%y4, %bb1]
                    %y9 = phi i32 [0, %bb2], [%y4, %bb1]
                    ret void
            }
        )";
        std::unique_ptr<llvm::Module> parsedModule = parseIR(ctx, input);
        llvm::Function* foo = parsedModule->getFunction("foo");

        llvmPractice::IterativeDominanceAnalysis analysis(*foo);

        llvm::Function::iterator fi = foo->begin();
        llvm::BasicBlock *BB0 = &*fi++;
        llvm::BasicBlock *BB1 = &*fi++;
        llvm::BasicBlock *BB2 = &*fi++;
        llvm::BasicBlock *BB3 = &*fi++;
        llvm::BasicBlock *BB4 = &*fi++;
        
        // BB0
        EXPECT_TRUE(analysis.dominates(BB0, BB0));
        EXPECT_TRUE(analysis.dominates(BB0, BB1));
        EXPECT_TRUE(analysis.dominates(BB0, BB2));
        EXPECT_FALSE(analysis.dominates(BB0, BB3));
        EXPECT_TRUE(analysis.dominates(BB0, BB4));
        // BB1
        EXPECT_FALSE(analysis.dominates(BB1, BB0));
        EXPECT_TRUE(analysis.dominates(BB1, BB1));
        EXPECT_FALSE(analysis.dominates(BB1, BB2));
        EXPECT_FALSE(analysis.dominates(BB1, BB3));
        EXPECT_FALSE(analysis.dominates(BB1, BB4));
        // BB2
        EXPECT_FALSE(analysis.dominates(BB2, BB0));
        EXPECT_FALSE(analysis.dominates(BB2, BB1));
        EXPECT_TRUE(analysis.dominates(BB2, BB2));
        EXPECT_FALSE(analysis.dominates(BB2, BB3)); 
        EXPECT_FALSE(analysis.dominates(BB2, BB4));
        // BB3
        EXPECT_FALSE(analysis.dominates(BB3, BB0));
        EXPECT_FALSE(analysis.dominates(BB3, BB1));
        EXPECT_FALSE(analysis.dominates(BB3, BB2));
        EXPECT_TRUE(analysis.dominates(BB3, BB3));
        EXPECT_FALSE(analysis.dominates(BB3, BB4));
        // BB4
        EXPECT_FALSE(analysis.dominates(BB4, BB0));
        EXPECT_FALSE(analysis.dominates(BB4, BB1));
        EXPECT_FALSE(analysis.dominates(BB4, BB2));
        EXPECT_FALSE(analysis.dominates(BB4, BB3));
        EXPECT_TRUE(analysis.dominates(BB4, BB4));
    }

    TEST(IterativeDominanceAnalysis, Loops) 
    {
        llvm::LLVMContext ctx;
        const char* input = R"(
            define void @foo(i32 %in) {
                bb1:
                    %condition = icmp sgt i32 %in, 50
                    br i1 %condition, label %bb1, label %bb2
                bb2:
                    %a = phi i32 [0, %bb1], [1, %bb2]
                    %b = phi i32 [2, %bb1], [%a, %bb2]
                    br label %bb2
            }
        )";
        std::unique_ptr<llvm::Module> parsedModule = parseIR(ctx, input);
        llvm::Function* foo = parsedModule->getFunction("foo");

        llvmPractice::IterativeDominanceAnalysis analysis(*foo);

        llvm::Function::iterator fi = foo->begin();
        llvm::BasicBlock *BB1 = &*fi++;
        llvm::BasicBlock *BB2 = &*fi++;
        
        // BB1
        EXPECT_TRUE(analysis.dominates(BB1, BB1));
        EXPECT_TRUE(analysis.dominates(BB1, BB2));
        // BB2
        EXPECT_FALSE(analysis.dominates(BB2, BB1));
        EXPECT_TRUE(analysis.dominates(BB2, BB2));
    }

    TEST(IterativeDominanceAnalysis, Crossing) 
    {
        llvm::LLVMContext ctx;
        //          (A)
        //          / \
        //         /   \
        //       (B)   (C)
        //        |\    |
        //        |  \  |
        //        |    \|
        //       (D)   (E)
        const char* input = R"(
            define void @foo() {
                A:
                    br i1 undef, label %B, label %C
                B:
                    br i1 undef, label %D, label %E
                C:
                    br label %E
                D:
                    ret void
                E:
                    ret void
        }
        )";
        std::unique_ptr<llvm::Module> parsedModule = parseIR(ctx, input);
        llvm::Function* foo = parsedModule->getFunction("foo");

        llvmPractice::IterativeDominanceAnalysis analysis(*foo);

        llvm::Function::iterator fi = foo->begin();
        llvm::BasicBlock *A = &*fi++;
        llvm::BasicBlock *B = &*fi++;
        llvm::BasicBlock *C = &*fi++;
        llvm::BasicBlock *D = &*fi++;
        llvm::BasicBlock *E = &*fi++;
        
        // A
        EXPECT_TRUE(analysis.dominates(A, A));
        EXPECT_TRUE(analysis.dominates(A, B));
        EXPECT_TRUE(analysis.dominates(A, C));
        EXPECT_TRUE(analysis.dominates(A, D));
        EXPECT_TRUE(analysis.dominates(A, E));
        // B
        EXPECT_FALSE(analysis.dominates(B, A));
        EXPECT_TRUE(analysis.dominates(B, B));
        EXPECT_FALSE(analysis.dominates(B, C));
        EXPECT_TRUE(analysis.dominates(B, D));
        EXPECT_FALSE(analysis.dominates(B, E));
        // C
        EXPECT_FALSE(analysis.dominates(C, A));
        EXPECT_FALSE(analysis.dominates(C, B));
        EXPECT_TRUE(analysis.dominates(C, C));
        EXPECT_FALSE(analysis.dominates(C, D));
        EXPECT_FALSE(analysis.dominates(C, E));
        // D
        EXPECT_FALSE(analysis.dominates(D, A));
        EXPECT_FALSE(analysis.dominates(D, B));
        EXPECT_FALSE(analysis.dominates(D, C));
        EXPECT_TRUE(analysis.dominates(D, D));
        EXPECT_FALSE(analysis.dominates(D, E));
        // E
        EXPECT_FALSE(analysis.dominates(E, A));
        EXPECT_FALSE(analysis.dominates(E, B));
        EXPECT_FALSE(analysis.dominates(E, C));
        EXPECT_FALSE(analysis.dominates(E, D));
        EXPECT_TRUE(analysis.dominates(E, E));
    }
}


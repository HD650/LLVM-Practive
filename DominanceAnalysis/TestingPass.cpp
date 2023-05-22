#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "IterativeDominanceAnalysis.h"

using namespace llvm;

namespace llvmPractice {

	class TestingPass : public PassInfoMixin<TestingPass> {
		public:
		PreservedAnalyses run(Function &aFunction, FunctionAnalysisManager& aManager) {
			IterativeDominanceAnalysis dominanceAnalysis(aFunction);
			dominanceAnalysis.run();
			// all analyses results are still vaild
			return PreservedAnalyses::all();
		}
	};

	// registe the TestingPass pass in the pass manager
	llvm::PassPluginLibraryInfo getTestingPassPluginInfo() {
		return {LLVM_PLUGIN_API_VERSION, "TestingPass", LLVM_VERSION_STRING,
          [](PassBuilder &aPassBuilder) {
            aPassBuilder.registerPipelineParsingCallback(
                [](StringRef aName, llvm::FunctionPassManager &aManager,
                   ArrayRef<llvm::PassBuilder::PipelineElement>) {
                  if (aName == "TestingPass") {
                    aManager.addPass(TestingPass());
                    return true;
                  }
                  return false;
                });
          }};
	}

	extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
		return getTestingPassPluginInfo();
	}

} // namespace
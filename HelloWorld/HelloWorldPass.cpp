#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {

	class HelloWorld : public PassInfoMixin<HelloWorld> {
		public:
		PreservedAnalyses run(Function &aFunction, FunctionAnalysisManager& aManager) {
			// just print out all the name of the functions in this module
			errs() << aFunction.getName() << '\n';
			// all analyses results are still vaild
			return PreservedAnalyses::all();
		}
	};

	// registe the HelloWorld pass in the pass manager
	llvm::PassPluginLibraryInfo getHelloWorldPluginInfo() {
		return {LLVM_PLUGIN_API_VERSION, "HelloWorld", LLVM_VERSION_STRING,
          [](PassBuilder &aPassBuilder) {
            aPassBuilder.registerPipelineParsingCallback(
                [](StringRef aName, llvm::FunctionPassManager &aManager,
                   ArrayRef<llvm::PassBuilder::PipelineElement>) {
                  if (aName == "HelloWorld") {
                    aManager.addPass(HelloWorld());
                    return true;
                  }
                  return false;
                });
          }};
	}

	extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
		return getHelloWorldPluginInfo();
	}

} // namespace
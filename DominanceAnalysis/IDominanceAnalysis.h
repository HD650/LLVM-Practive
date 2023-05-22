#include "llvm/IR/BasicBlock.h"

namespace llvmPractice {

	class IDominanceAnalysis {
		public:
			// run the analysis
			virtual void run() = 0;
			// check whether a basic block dominates another
			virtual bool dominates(const llvm::BasicBlock *aBB1, const llvm::BasicBlock* aBB2) const = 0;
			// print the dominance graph/tree
			virtual void debugDump() const = 0; 

	};

} // namespace
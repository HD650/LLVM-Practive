#include "IDominanceAnalysis.h"
#include <unordered_map>
#include <set>

namespace llvmPractice {

	class IterativeDominanceAnalysis : public IDominanceAnalysis {
		public:
			// constructor
			IterativeDominanceAnalysis(llvm::Function & aFcn);
			// why call this analysis with no input
			IterativeDominanceAnalysis() = delete;
			// run the analysis
			virtual void run() override;
			// check whether a basic block dominates another
			virtual bool dominates(const llvm::BasicBlock *aBB1, const llvm::BasicBlock* aBB2) const override;
			// print the dominance graph/tree
			virtual void debugDump() const override;

		private:
			// use a ordered set to prevent nondeterministic behavior
			std::unordered_map<llvm::BasicBlock*, std::set<llvm::BasicBlock*>> fDominanceGraph;

	};

} // namespace
#include "IDominanceAnalysis.h"
#include "DominatorTree.h"
#include <unordered_map>
#include <set>
#include <list>
#include <unordered_set>
#include <deque>

namespace llvmPractice {

	// The Simple Fast Dominator Algorithm
	// Keith D. Cooper, Timothy J. Harvey, and Ken Kennedy
	// This is an iterative algorithm that computes the dominator tree
	// of a function. 
	// The key idea is dataflow analysis and immediate dominator

	class IterativeDominanceAnalysis : public IDominanceAnalysis {
		public:
			// constructor
			IterativeDominanceAnalysis(llvm::Function& aFcn);
			// why call this analysis with no input
			IterativeDominanceAnalysis() = delete;
			// run the analysis
			virtual void run() override;
			// check whether a basic block dominates another
			virtual bool dominates(const llvm::BasicBlock *aBB1, const llvm::BasicBlock* aBB2) const override;
			// print the dominance graph/tree
			virtual void debugDump() override;
			// the algorithm result is saved in a IDom map initially, now transcript it into a tree
			void generateDominatorTree();
			// check whether the result of the dominator tree is the same as the dominator map
			bool verify();

		private:
			// sort the basic blocks in the give function in reversed post order
			void generateReversePostOrder();
			void dfs(llvm::BasicBlock* aBB);
			// When computing the idom of D, we need to intersect the idom of B and C
			// the result will be A
			// Note that it's not implemented by set intersect, we are actually searching
			// for the common ancestor the B and C
			//           [A]
			//          /   \
			//         /     \
			//       [B]     [C]
			//        \       /
			//         \     /
			//           [D]
			llvm::BasicBlock* intersect(llvm::BasicBlock* aU, llvm::BasicBlock* aV);
			
			// immediate dominator info, a -> b means b is the idom of a
			std::unordered_map<llvm::BasicBlock*, llvm::BasicBlock*> fIDom;
			std::unordered_map<llvm::BasicBlock*, size_t> fBBRpoNumer;
			std::deque<llvm::BasicBlock*> fSortedBB;
			std::unordered_set<llvm::BasicBlock*> fVisitedBB;
			std::unique_ptr<DominatorTree> fDomTree;
			llvm::Function& fFcn;
			size_t fRpoNum;
	};

} // namespace

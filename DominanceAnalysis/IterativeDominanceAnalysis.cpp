#include "IterativeDominanceAnalysis.h"
#include "llvm/ADT/PostOrderIterator.h" // llvm::ReversePostOrderTraversal
#include "llvm/IR/Function.h"
#include "llvm/IR/CFG.h" // succ_begin succ_end
#include "llvm/Support/raw_ostream.h"

#include <unordered_set>

using namespace llvmPractice;

IterativeDominanceAnalysis::IterativeDominanceAnalysis(llvm::Function & aFcn) : fFcn(aFcn), fRpoNum(fFcn.getBasicBlockList().size())
{
	// since we will do a dataflow analysis, we need to traverse the basic blocks
	// of the function in reverse post order (predecessor always got visited before
	// successor)
	generateReversePostOrder();
	run();
	generateDominatorTree();
	assert(verify() && "Dominator tree does not match the result?!");
}

llvm::BasicBlock* IterativeDominanceAnalysis::intersect(llvm::BasicBlock* aU, llvm::BasicBlock* aV)
{
	while(aU != aV)
	{
		while(fBBRpoNumer[aU] > fBBRpoNumer[aV])
		{
			aU = fIDom[aU];
		}
		while(fBBRpoNumer[aV] > fBBRpoNumer[aU])
		{
			aV = fIDom[aV];
		}
	}
	return aU;
}

void IterativeDominanceAnalysis::run()
{
	// the initial state of the analysis, we know no information but the 
	// entry block dominates the entry block itself
	for(llvm::BasicBlock* bb : fSortedBB)
		fIDom[bb] = nullptr;
	fIDom[&fFcn.getEntryBlock()] = &fFcn.getEntryBlock();


	// flag to determins whether to end this iterative algorithm
	bool changed = true;

	while(changed)
	{
		changed = false;
		// traverse the cfg in reverse post order
		for(llvm::BasicBlock* bb : fSortedBB)
		{
			if(bb ==  &fFcn.getEntryBlock())
				continue;
			
			llvm::BasicBlock* u = nullptr;
			llvm::BasicBlock* v = nullptr;
			for(llvm::BasicBlock* pred : llvm::predecessors(bb))
			{
				// select a predecessor that has idom information
				if(!u)
				{
					if(fIDom[pred])
						u = pred;
					continue;
				}
				// select the other predecessor
				v = pred;

				// do intersect of the dominators of two predecessors
				if(fIDom[v])
					u = intersect(u, v);
			}

			// idom of bb is either its predecessor or the intersect of its predecessor
			if(u != fIDom[bb])
			{
				fIDom[bb] = u;
				changed = true;
			}
		}
	}

}

bool IterativeDominanceAnalysis::dominates(const llvm::BasicBlock *aBB1, const llvm::BasicBlock* aBB2) const
{
	return fDomTree->dominate(aBB1, aBB2);
}


void IterativeDominanceAnalysis::debugDump()
{
	for(llvm::BasicBlock* bb : fSortedBB)
	{
		llvm::BasicBlock* idom = fIDom[bb];
		llvm::errs() << "[" << bb->getName().str() << "]" << "-" << "[" << idom->getName().str() << "]" << "\n";
	}
}

void IterativeDominanceAnalysis::generateReversePostOrder()
{
	fVisitedBB.clear();
	// dfs the cfg and record the rpo
	dfs(&(fFcn.getEntryBlock()));
}

void IterativeDominanceAnalysis::dfs(llvm::BasicBlock* aBB)
{
	fVisitedBB.insert(aBB);

	for(llvm::BasicBlock* succ : llvm::successors(aBB))
	{
		if(fVisitedBB.find(succ) == fVisitedBB.end())
			dfs(succ);
	}

	fRpoNum -= 1;
	fBBRpoNumer[aBB] = fRpoNum;
	fSortedBB.push_front(aBB);
}

void IterativeDominanceAnalysis::generateDominatorTree()
{
	if(fDomTree)
		return;
	
	// entry BB is the root of the tree
	fDomTree = std::make_unique<DominatorTree>(&(fFcn.getEntryBlock()));
	for(std::pair<llvm::BasicBlock *const, llvm::BasicBlock*>& idom : fIDom)
	{
		fDomTree->addEdge(idom.second, idom.first);
	}

	fDomTree->updateDFSNum();
}

bool IterativeDominanceAnalysis::verify()
{
	if(!fDomTree)
		return false;
	
	fDomTree->debugDump();

	// check each Idom pair
	// this isn't the complete check, a complete check is O(N^2)
	for(std::pair<llvm::BasicBlock *const, llvm::BasicBlock*>& idom : fIDom)
	{
		if(!fDomTree->dominate(idom.second, idom.first))
			return false;
	}
	return true;
}
#include "llvm/IR/BasicBlock.h"

namespace llvmPractice {
    class DominatorTreeNode
    {
        public:
        
        DominatorTreeNode(llvm::BasicBlock* aBB, DominatorTreeNode* aIDom);
        DominatorTreeNode(llvm::BasicBlock* aBB); // for root node

        // the corresponding BB
        llvm::BasicBlock* fBB;
        
        // the predecessor of this node, the immediate dominator
        // of the BB this node holds
        DominatorTreeNode* fIDom;

        // the children of this node, small vector improve the space
        // efficiency of this container, why 4? llvm uses 4 in its implementation
        llvm::SmallVector<DominatorTreeNode*, 4> fChildren;
        
        // These 2 number could be used to quickly determine whether a
        // node is the descendant of the other node, say we have A and B
        // if DSFNumIn(A) > DSFNumIn(B) && DSFNumOut(A) < DSFNumOut(B)
        // then A is a descendant of B, thus A is dominated by B.
        // A conceptual proof is that, the bigger In num means a node is
        // deeper than the other or righter than the other. A smaller
        // Out num means a node is deeper than the other or lefter than
        // the other, the mix of those two could prove a node is descendant
        // of the other.
        // Without these 2 number, we need to traverse in the tree to 
        // check any dominance.

        // the order that the DFS first pushes this node into the stack
        unsigned int fDSFNumIn = ~0;
        // the order that the DFS pops out this node from the stack
        unsigned int fDSFNumOut = ~0;

    };

    // Implementation of the dominator tree
    // Note that Cooper's algorithm is an iterative algorithm which means
    // we could not construct the dominator tree on the fly, we have to wait
    // the algorithm converge and then construct the tree entirely
    // While Lengauer Tarjan algorithm could construct a tree on the fly, but 
    // Lengauer Tarjan algorithm needs a builder class to handle the construction
    // because it's quiet complex
    class DominatorTree
    {
        public:

        // construct the dominator tree with a root BB
        DominatorTree(llvm::BasicBlock* aBB);
        DominatorTree(); // for empty tree

        // check if A dominates B
        bool dominate(const llvm::BasicBlock* aDominator, const llvm::BasicBlock* aDominated);
        // check if A dominates B, A != B
        bool restrictedDominate(const llvm::BasicBlock* aDominator, const llvm::BasicBlock* aDominated);

        // recompute the DFS number of all the nodes if DFS numbers are invalid
        void updateDFSNum();

        // add an edge into the tree, indicates that A IDom B
        void addEdge(llvm::BasicBlock* aDominator, llvm::BasicBlock* aDominated);

        void debugDump() const;

        private:
        
        void printTree(const DominatorTreeNode* aNode, const std::string& prefix = "", bool isLeft = true) const;

        // the root of the tree, always the entry block
        // note that if we need to build a post dominance tree,
        // it could have multiple roots, since a function could have
        // multiple exit point but only one entry point
        DominatorTreeNode* fRoot;
        // need a map to efficiently find a tree node by a BB
        llvm::DenseMap<const llvm::BasicBlock*, std::unique_ptr<DominatorTreeNode>> fDomTreeNodes;

        // the flag indicates whether the dfs number of nodes up to date
        bool fIsDFSNumValid = false;
    };
}
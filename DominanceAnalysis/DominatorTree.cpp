#include "DominatorTree.h"

namespace llvmPractice {
    DominatorTreeNode::DominatorTreeNode(llvm::BasicBlock* aBB, DominatorTreeNode* aIDom) :
        fBB(aBB), fIDom(aIDom)
    {

    }

    DominatorTreeNode::DominatorTreeNode(llvm::BasicBlock* aBB) :
        fBB(aBB), fIDom(this)
    {

    }

    DominatorTree::DominatorTree(llvm::BasicBlock* aBB)
    {
        fDomTreeNodes[aBB] = std::move(std::make_unique<DominatorTreeNode>(aBB));
        fRoot = fDomTreeNodes[aBB].get();
    }

    DominatorTree::DominatorTree()
    {

    }

    void DominatorTree::addEdge(llvm::BasicBlock* aDominator, llvm::BasicBlock* aDominated)
    {
        // don't explicitly shows self dominance
        if(aDominated == aDominator)
            return;

        if(fDomTreeNodes.find(aDominator) == fDomTreeNodes.end())
        {
            fDomTreeNodes[aDominator] = std::move(std::make_unique<DominatorTreeNode>(aDominator));
        }
        if(fDomTreeNodes.find(aDominated) == fDomTreeNodes.end())
        {
            fDomTreeNodes[aDominated] = std::move(std::make_unique<DominatorTreeNode>(aDominated));
        }
        // dominator -> dominated
        fDomTreeNodes[aDominator]->fChildren.push_back(fDomTreeNodes[aDominated].get());
        // dominated -> dominator
        fDomTreeNodes[aDominated]->fIDom = fDomTreeNodes[aDominator].get();
        
        // need to compute DFS num since tree has changed
        fIsDFSNumValid = false;
    }

    void DominatorTree::updateDFSNum()
    {
        if(fIsDFSNumValid)
            return;
        
        if(!fRoot)
            return;
        
        // a stack based dfs

        // <tree node, next children of this tree node to visit next>
        std::vector<std::pair<DominatorTreeNode*, size_t>> workList;
        
        unsigned int DFSNum = 0;
        fRoot->fDSFNumIn = DFSNum++; // root in stack
        workList.push_back({fRoot, 0});

        while(!workList.empty())
        {
            DominatorTreeNode* curr = workList.back().first;
            size_t nextChild = workList.back().second;

            // this node has no more children to traverse, pop it
            if(nextChild > curr->fChildren.size())
            {
                curr->fDSFNumOut = DFSNum++; // this node out of stack
                workList.pop_back();
            }
            else // traverse next child
            {

                workList.back().second = ++nextChild;
                workList.push_back({curr->fChildren[nextChild-1], 0});
                workList.back().first->fDSFNumIn = DFSNum++; // new node in stack
            }
        }
    }

    bool DominatorTree::dominate(llvm::BasicBlock* aDominator, llvm::BasicBlock* aDominated)
    {
        if(aDominated == aDominator)
            return true; // self dominance
        return restrictedDominate(aDominator, aDominated);
    }
    
    bool DominatorTree::restrictedDominate(llvm::BasicBlock* aDominator, llvm::BasicBlock* aDominated)
    {
        assert(fDomTreeNodes.find(aDominator) != fDomTreeNodes.end());
        assert(fDomTreeNodes.find(aDominated) != fDomTreeNodes.end());
        DominatorTreeNode* aDominatorNode = fDomTreeNodes[aDominator].get();
        DominatorTreeNode* aDominatedNode = fDomTreeNodes[aDominated].get();
        
        // a.in > b.in means a is deeper than b or a is righter than b
        // b.out > a.out means a is deeper than b or a is lefter than b
        return aDominatorNode->fDSFNumIn > aDominatedNode->fDSFNumIn &&
               aDominatedNode->fDSFNumOut > aDominatorNode->fDSFNumOut;
    }

    void DominatorTree::debugDump() const
    {
        
    }
}
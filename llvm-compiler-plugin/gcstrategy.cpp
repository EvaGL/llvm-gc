#include "llvm/CodeGen/GCStrategy.h"
#include "llvm/CodeGen/GCMetadata.h"
#include "llvm/Support/Compiler.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Intrinsics.h"
#include <iostream>
using namespace llvm;

namespace {
    class LLVM_LIBRARY_VISIBILITY TestGC: public GCStrategy {
    private:
        Type* chainType;
        Type* chainPtr;
        Type* voidPtr;
        Type* intType;
    public:
        TestGC() {
            NeededSafePoints = 1 << GC::PostCall;
            UsesMetadata = true;
            CustomRoots = true;
        }

        virtual bool initializeCustomLowering(Module &M) {
            intType = Type::getInt8Ty(M.getContext());
            voidPtr = PointerType::getUnqual(intType);
            Type* elems[2] = {voidPtr, voidPtr};
            chainType = StructType::create(elems, "chainType");
            chainPtr = PointerType::getUnqual(chainType);
            
            for(Module::iterator F = M.begin(); F != M.end(); ++F) {
                if (F->hasGC() && strcmp(F->getGC(), "jblab-gc") == 0) {
                    // Creating stack map for functions
                    M.getOrInsertGlobal("__gc_" + (std::string) F->getName(), intType);
                }
            }
            GlobalVariable* chainBottom = new GlobalVariable(M, chainPtr, false, GlobalValue::ExternalLinkage, 0, "chainBottom");
            //M.getOrInsertGlobal("chainBottom", chainPtr);
            return true;
        }

        virtual bool performCustomLowering(Function &F) {
            Value* zero = ConstantInt::get(F.getContext(), APInt(32, 0));
            Value* one = ConstantInt::get(F.getContext(), APInt(32, 1));
            Value* firstElem[2] = {zero, zero};
            Value* secondElem[2] = {zero, one};
            BasicBlock& entry = F.getEntryBlock();
            Instruction* first = entry.begin();
            
            // Allocating space for chain instance on stack
            AllocaInst* chain = new AllocaInst(chainType,"chain", first);

            // Push metadata pointer
            GetElementPtrInst* metaPtr = GetElementPtrInst::Create(chain, firstElem, "meta_ptr", first);
            Value* meta = F.getParent()->getOrInsertGlobal("__gc_" + (std::string) F.getName(), intType);
            StoreInst* storeMeta = new StoreInst(meta, metaPtr, first);
            
            // Push previous chain instance
            GetElementPtrInst* prevPtr = GetElementPtrInst::Create(chain, secondElem, "prev_ptr", first);
            Value* chainBottom = F.getParent()->getOrInsertGlobal("chainBottom", chainPtr);
            BitCastInst* cast = new BitCastInst(chainBottom, voidPtr, "prev_void_ptr", first);
            if (F.getName() != "main") {
                StoreInst* storePrev = new StoreInst(cast, prevPtr, first);
            } else {
                // There is no previous chain for main function
                StoreInst* storePrev = new StoreInst(Constant::getNullValue(voidPtr), prevPtr, first);
            }
            
            // Store current chain in global variable
            StoreInst* storeCurrent = new StoreInst(chain, chainBottom, first);
            
            // Restoring chain before return
            for (Function::iterator BB = F.begin(); BB != F.end(); ++BB) {
                TerminatorInst* term = BB->getTerminator();
                if (!term) {
                    continue;
                }
                if (isa<ReturnInst>(term) || isa<ResumeInst>(term)) {
                    BitCastInst* prevCast = new BitCastInst(prevPtr, chainPtr, "parent", term);
                    StoreInst* restore = new StoreInst(prevCast, chainBottom, term);
                }
            }
            return true;
        }
    };

    GCRegistry::Add<TestGC> X("jblab-gc", "JBlab gc complier plugin");
}

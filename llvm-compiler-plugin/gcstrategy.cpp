#include "llvm/CodeGen/GCStrategy.h"
#include "llvm/CodeGen/GCMetadata.h"
#include "llvm/Support/Compiler.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/DerivedTypes.h"
#include <iostream>
using namespace llvm;

namespace {
    class LLVM_LIBRARY_VISIBILITY TestGC: public GCStrategy {
    public:
        TestGC() {
            NeededSafePoints = 1 << GC::PostCall;
            UsesMetadata = true;
            CustomRoots = true;
        }

        virtual bool initializeCustomLowering(Module &M) {
            for(Module::iterator F = M.begin(); F != M.end(); ++F) {
                if (F->hasGC() && strcmp(F->getGC(), "jblab-gc") == 0) {
                    // Creating stack map for functions
                    M.getOrInsertGlobal("__gc_" + (std::string) F->getName(), StructType::get(M.getContext()));
                }
            }
            return true;
        }

        virtual bool performCustomLowering(Function &F) {
            return false;
        }
    };

    GCRegistry::Add<TestGC> X("jblab-gc", "JBlab gc complier plugin");
}

#include "llvm/CodeGen/GCStrategy.h"
#include "llvm/CodeGen/GCMetadata.h"
#include "llvm/Support/Compiler.h"

using namespace llvm;

namespace {
    class LLVM_LIBRARY_VISIBILITY TestGC: public GCStrategy {
    public:
        TestGC() {
            NeededSafePoints = 1 << GC::PostCall;
            UsesMetadata = true;
        }
    };

    GCRegistry::Add<TestGC> X("jblab-gc", "Implementation of llvm gc");
}

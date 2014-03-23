#include "llvm/CodeGen/GCMetadataPrinter.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetLoweringObjectFile.h"
#include "llvm/Target/TargetFrameLowering.h"

using namespace llvm;

namespace {
    class LLVM_LIBRARY_VISIBILITY TestGCPrinter: public GCMetadataPrinter {
    public:
        void finishAssembly(AsmPrinter &ap) {
            MCStreamer &os = ap.OutStreamer;
            unsigned ptrSize = ap.TM.getDataLayout()->getPointerSize();
            unsigned align = 1;
            while (1 << align != ptrSize) {
                align++;
            }
            os.SwitchSection(ap.getObjFileLowering().getDataSection());
            for (iterator func = begin(); func != end(); ++func) {
                GCFunctionInfo &info = **func;
                ap.EmitAlignment(align);
                MCSymbol *label = os.getContext().GetOrCreateSymbol("__gc_" + info.getFunction().getName());
                os.EmitLabel(label);
                ap.EmitInt32(info.roots_size());
                size_t frameSize = info.getFrameSize();
                for (GCFunctionInfo::roots_iterator root = info.roots_begin(); root != info.roots_end(); ++root) {
                    int stackOffset = root->StackOffset;
                    if (stackOffset >= 0) {
                        stackOffset = stackOffset - (frameSize - 2 * ptrSize); 
                    } else {
		        stackOffset = stackOffset + 3 * ptrSize; // + rbp
		    }
                    ap.EmitInt32(stackOffset);
                }
            }
        }

        void beginAssembly(AsmPrinter &ap) {
        }
    };

    GCMetadataPrinterRegistry::Add<TestGCPrinter> X("jblab-gc", "Metadata printer for jblab gc");
}

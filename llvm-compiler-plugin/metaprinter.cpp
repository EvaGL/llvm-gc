#include "llvm/CodeGen/GCMetadataPrinter.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/Function.h"
#include "llvm/DataLayout.h"
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
                for (GCFunctionInfo::roots_iterator root = info.roots_begin(); root != info.roots_end(); ++root) {
                    ap.EmitInt32(root->StackOffset);
                }
            }
        }

        void beginAssembly(AsmPrinter &ap) {
        }
    };

    GCMetadataPrinterRegistry::Add<TestGCPrinter> X("jblab-gc", "Metadata printer for jblab gc");
}

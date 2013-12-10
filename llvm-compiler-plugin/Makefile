LEVEL := ../..
LIBRARYNAME = jblab-gc
LOADABLE_MODULE = 1

include $(LEVEL)/Makefile.common

PATCH = llvm-3.3.patch
patch:
	cp $(PATCH) $(LEVEL)/lib/Target/X86/
	cd $(LEVEL)/lib/Target/X86 && patch -p1 < $(PATCH) && rm $(PATCH) 
	cd $(LEVEL) && $(MAKE) && $(MAKE) install

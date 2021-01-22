SUBDIRS := $(dir $(wildcard ../cci/*/Makefile.unix ../cci/*/*/Makefile.unix))

TARGETS := all build run check clean
.PHONY: $(TARGETS)

$(TARGETS): %:
	@set -e ; for i in $(SUBDIRS); do \
		$(MAKE) -f Makefile.unix -s -C $$i FLAG_BATCH=1 $@ ; \
	done

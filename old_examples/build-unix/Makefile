export FLAG_BATCH = 1

SUBDIRS := $(wildcard ../ex?_*/ ../ex??_*/)
TARGETS := all clean cleanall check run

$(TARGETS):
	@test "x$$FLAG_BATCH" == x1 || set -e ; \
	for i in $(SUBDIRS); do \
		$(MAKE) -C $$i/build-unix $@ ; \
	done

#-- ensure CCI library build

all run check: lib-all
clean:         lib-clean
cleanall:      lib-cleanall

lib-%:
	$(MAKE) -f Makefile.gs_cci $(*)

.PHONY: $(TARGETS)


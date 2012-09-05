export FLAG_BATCH = 1

SUBDIRS = $(wildcard */)
TARGETS = all clean cleanall check run

$(TARGETS):
	@test "x$$FLAG_BATCH" == x1 || set -e ; \
	for i in $(SUBDIRS); do \
		$(MAKE) -C $$i $@ ; \
	done

.PHONY: $(TARGETS)

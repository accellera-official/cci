include $(top_srcdir)/config/Make-rules.examples

SUBDIRS = \
	cci

# install examples?
if SEPARATE_INSTALL_TREE
buildexamplesdir = $(examplesdir)

uninstall-hook:
	test ! -d "$(buildexamplesdir)" || ( set -e ; rmdir --ignore-fail-on-non-empty "$(buildexamplesdir)" )

endif # SEPARATE_INSTALL_TREE

## Taf!

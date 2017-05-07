DOXY_EXEC_PATH = /home/celia/r2_irc/G-2361-06-P1
DOXYFILE = /home/celia/r2_irc/G-2361-06-P1/Doxyfile
DOXYDOCS_PM = /home/celia/r2_irc/G-2361-06-P1/perlmod/DoxyDocs.pm
DOXYSTRUCTURE_PM = /home/celia/r2_irc/G-2361-06-P1/perlmod/DoxyStructure.pm
DOXYRULES = /home/celia/r2_irc/G-2361-06-P1/perlmod/doxyrules.make

.PHONY: clean-perlmod
clean-perlmod::
	rm -f $(DOXYSTRUCTURE_PM) \
	$(DOXYDOCS_PM)

$(DOXYRULES) \
$(DOXYMAKEFILE) \
$(DOXYSTRUCTURE_PM) \
$(DOXYDOCS_PM): \
	$(DOXYFILE)
	cd $(DOXY_EXEC_PATH) ; doxygen "$<"

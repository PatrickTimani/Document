all: 
	@make -C server
	@make -C client


.PHONY: doc
doc:
	rm -rf doc/html
	doxygen doc/Doxyfile


.PHONY: clean
clean:
	@make -C server clean
	@make -C client clean

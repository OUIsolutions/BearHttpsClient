install_dependencies:
	@darwin install

build:
	@darwin run_blueprint --target amalgamation_build
 
run_example:
	@gcc examples/${FILE}.c -o examples/${FILE} && ./examples/${FILE}

install_dependencies:
	@darwin install

build:
	@darwin run_blueprint --target one_build
 
run_example:
	@gcc examples/${FILE}.c -o examples/${FILE} && ./examples/${FILE}

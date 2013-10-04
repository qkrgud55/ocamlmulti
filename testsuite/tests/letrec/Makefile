BASEDIR=../..

POCAMLOPT=/home/phc/ocaml_internal/ocamlmulti/ocamlopt -cclib '-pthread' -phcr -nostdlib -I /home/phc/ocaml_internal/ocamlmulti/stdlib_r

backreferences: backreferences.ml
	$(POCAMLOPT) -index_file index backreferences.ml
	./a.out

class_1: class_1.ml
	$(POCAMLOPT) -index_file index class_1.ml
	./a.out

class_2: class_2.ml
	$(POCAMLOPT) -index_file index class_2.ml
	./a.out

evaluation_order_1: evaluation_order_1.ml
	$(POCAMLOPT) -index_file index evaluation_order_1.ml
	./a.out

evaluation_order_2: evaluation_order_2.ml
	$(POCAMLOPT) -index_file index evaluation_order_2.ml
	./a.out

evaluation_order_3: evaluation_order_3.ml
	$(POCAMLOPT) -index_file index evaluation_order_3.ml
	./a.out

float_block_1: float_block_1.ml
	$(POCAMLOPT) -index_file index float_block_1.ml
	./a.out

float_block_2: float_block_2.ml
	$(POCAMLOPT) -index_file index float_block_2.ml
	./a.out

lists: lists.ml
	$(POCAMLOPT) -index_file index lists.ml
	./a.out

mixing_value_closures_1: mixing_value_closures_1.ml
	$(POCAMLOPT) -index_file index mixing_value_closures_1.ml
	./a.out

mixing_value_closures_2: mixing_value_closures_2.ml
	$(POCAMLOPT) -index_file index mixing_value_closures_2.ml
	./a.out

mutual_functions: mutual_functions.ml
	$(POCAMLOPT) -index_file index mutual_functions.ml
	./a.out


init:
	cp index ../../../stdlib_r
	cd ../../.. ; make cross-phc ; cd -

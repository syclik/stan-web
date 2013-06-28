CC=clang++

help:
	@echo 'Help -- list of targets:'
	@echo '  update:    update git submodules'

.PHONY: update
update :
	@echo '------------------------------------------------------------'
	@echo 'Updating git submodules'
	git pull
	cd stan; git pull; cd ..


models/linear_model.hpp : stan/bin/stanc
	@echo 'building ' $@

stan/bin/stanc :
	@echo 'building stan/bin/stanc'

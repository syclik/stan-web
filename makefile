CC = clang++
STANC = stan/bin/stanc

help:
	@echo 'Help -- list of targets:'
	@echo '  update:    update git submodules'

.PHONY: update
update :
	@echo '------------------------------------------------------------'
	@echo '--- Updating git submodules'
	@echo ''
	git pull
	cd stan; git pull; cd ..

src/% : src/%.cpp models/%.hpp
	@echo '------------------------------------------------------------'
	@echo '--- Building $@'
	@echo ''


models/%.hpp : models/%.stan stan/bin/stanc
	@echo '------------------------------------------------------------'
	@echo '--- Buidling $@'
	@echo ''
	$(STANC) $< --o=$@ --no_main

$(STANC) :
	@echo 'building stan/bin/stanc'
	cd stan; make bin/stanc CC=$(CC) -j2

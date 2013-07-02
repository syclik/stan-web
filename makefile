CC = g++
STANC = stan/bin/stanc

EIGEN ?= lib/eigen_3.1.3
BOOST ?= lib/boost_1.53.0

CFLAGS = -I . -I stan/src -I stan/$(EIGEN) -I stan/$(BOOST) -Wall -DBOOST_RESULT_OF_USE_TR1 -DBOOST_NO_DECLTYPE -DBOOST_DISABLE_ASSERTS
LDLIBS = -L stan/bin -l stan

O = 3

help:
	@echo 'Help -- list of targets:'
	@echo '  update:    update git submodules'
	@echo '  build:     build all'



.PHONY: update
update :
	@echo '------------------------------------------------------------'
	@echo '--- Updating git submodules'
	@echo ''
	git pull
	cd stan; git pull; cd ..

MODELS := $(patsubst src/%.cpp,%,$(shell find src -type f -name '*.cpp'))

.PHONY: build
build : $(MODELS)


$(MODELS): % : src/%.cpp models/%.hpp $(STANC)
	@echo '------------------------------------------------------------'
	@echo '--- Building $@'
	@echo ''
	node-gyp configure build --verbose
#	$(LINK.c) $(CFLAGS) $(LDLIBS) -O$(O) $< $(OUTPUT_OPTION)


models/%.hpp : models/%.stan stan/bin/stanc
	@echo '------------------------------------------------------------'
	@echo '--- Buidling $@'
	@echo ''
	$(STANC) $< --o=$@ --no_main

$(STANC) :
	@echo '------------------------------------------------------------'
	@echo '--- building $(STANC)'
	cd stan; make bin/stanc CC=$(CC) O=$(O) -j2

clean :
	@echo '------------------------------------------------------------'
	@echo '--- cleaning project'
	rm -f $(shell find models -type f -name '*.hpp')
	$(eval SRC_BASE := $(patsubst src/%.cpp,src/%,$(shell find src -type f -name '*.cpp')))
	rm -f $(wildcard $(addsuffix .d,$(SRC_BASE)) \
                         $(addsuffix .o,$(SRC_BASE)) \
                         $(wildcard $(SRC_BASE)))
	rm -rf build	


.PHONY: build
build:
	west build -b qemu_cortex_a53 basic

.PHONY: run
run: build
	west build -t run

.PHONY: test
test:
	west twister -p qemu_cortex_a53 -T basic

.PHONY: clean
clean:
	rm -rf build twister-out*
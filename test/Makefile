TEST := test_a2A
TEST_OUT := $(foreach n, $(TEST), $(n).out)

CFLAGS := -static

all: $(TEST_OUT)

$(TEST_OUT):%.out: %.c
	$(CC) $(CFLAGS) $< -o $@

dir:
	mkdir -p $(BUSYBOX)/test_mod
	cp *.out $(BUSYBOX)/test_mod

clean:
	$(RM) $(TEST_OUT)
	$(RM) -rf $(BUSYBOX)/test_mod
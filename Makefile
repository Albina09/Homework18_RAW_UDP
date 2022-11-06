.PHONY := clean
object = client server
all: $(object)

$(object): %: %.c
		gcc $< -o $@ -g


.PHONY := clean
clean:
		rm -rf $(object)


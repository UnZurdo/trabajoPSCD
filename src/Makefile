OBJDIR := build

# Rules
all: $(OBJDIR)/foo

%: %.o
    $(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(OBJDIR)/%.o: %.c
    $(CC) -c $(CFLAGS) $(LDFLAGS) -o $@ $^

$(OBJDIR):
    mkdir -p $@

# Dependencies
$(OBJDIR)/foo: $(OBJDIR)/foo.o $(OBJDIR)/bar.o | $(OBJDIR)
$(OBJDIR)/foo.o $(OBJDIR)/bar.o: | $(OBJDIR)
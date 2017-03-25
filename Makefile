INCLUDES	+= ./
CFLAGS	+= -I$(INCLUDES)
OBJCFLAGS	+= $(CFLAGS)

OBJDIR	= obj

_DEPS	= sfxtree.h benchmark.h
_SOURCE	= sfxtree.c benchmark.c

DEPS	= $(_DEPS)
OBJ	= $(patsubst %, $(OBJDIR)/%, $(_SOURCE:.c=.o))

.PHONY:	all
.PHONY:	clean

all:	$(OBJDIR) naive mccreight ukkonen

$(OBJDIR):	
	mkdir -p $(OBJDIR)

naive:	$(OBJ) $(OBJDIR)/naive.o
	$(CC) -o $@ $^ $(CFLAGS)
	
mccreight:	$(OBJ) $(OBJDIR)/mccreight.o
	$(CC) -o $@ $^ $(CFLAGS)

ukkonen:	$(OBJ) $(OBJDIR)/ukkonen.o
	$(CC) -o $@ $^ $(CFLAGS)

$(OBJDIR)/%.o:	%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJDIR)/%.o:	%.m $(DEPS)
	$(CC) -c -o $@ $< $(OBJCFLAGS)

clean:
	rm -f $(OBJDIR)/*.o naive mccreight ukkonen

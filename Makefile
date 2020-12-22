
CC=gcc

CFLAGS=-O3 -Wall -g -pg

all : graphline gen_trainset lineset

/tmp/output_graphline.png : RENDER_RES=960x960

/tmp/output_graphline.png : graphline
	@./$^ $(RENDER_RES) | convert -size $(RENDER_RES) rgb:- -depth 32 $@

graphline : graphline.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)

lineset : square.o mse.o lineset.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)

gen_trainset : gen_trainset.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)

train.dat : gen_trainset
	@./$^ $@


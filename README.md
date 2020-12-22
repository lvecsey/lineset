
This program will take a set of training values (describing a line), and perform a gradient descent to determine the two weights that describe the line data.

Edit gen_trainset.c and change the weights[0] and weights[1] values.
These are parameters that define a line.

```console
make train.dat
```
Create a visual of the line

```console
make /tmp/output_graphline.png
feh --fullscreen /tmp/output_graphline.png
```

![Image of output](https://phrasep.com/~lvecsey/software/lineset/output_graphline.png)

Finally, run lineset to look through the training data file and converge on the weights. After it finishes it should match the weights that you started off with.

```console
./lineset trainset.dat
```

Example output:

Current nextw 7.27246e-08 0.00120735
Current weights 0.519999 0.261534
[33] MSE 7.84479e-07

This shows that the weights were found to be at an accurate level, after about 33 sweeps through the data.


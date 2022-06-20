# permutor
#### Multi-threaded unordered permutor

A test program designed to permute a string, using multiple threads, in order to find a given test value.
The permutation generation algorithim used in this program can be found [here](https://stackoverflow.com/a/2031708).

To switch between multi-threaded and single-threaded operation, simply
`#define SINGLE` or `#define MULTI`.

| Variable     | Description                                                                         |
| ------------ | ----------------------------------------------------------------------------------- |
| thread_count | The number of threads created. Operation will scale accordingly                     |
| solution     | The test value the program will search for. Operation will cease when found.        |
| key          | The string to permute                                                               |

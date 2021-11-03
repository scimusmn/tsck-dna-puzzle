# tsck-dna-puzzle/lighting

This is the code for the lighting controller of the TSCK DNA puzzle component.
It accepts Stele packets via UART at 115200 baud, and responds to "genome"
keys by setting the attached Soya disks to a pattern derived from the provided
genome (a six-character string containing 'A's, 'C's, 'G's, or 'T's).

## Building the tests

Clone and enter the repository, and then run

```bash
mkdir build
cd build
cmake ..
make test
```

This will create an executable called `test` that runs all of the tests.
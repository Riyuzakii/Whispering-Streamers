## Reverse Engineering the Stream Prefetcher for Profit 
to appear in [EuroS&P 2020](https://www.ieee-security.org/TC/EuroSP2020/)

Please refer the paper for more details and explanation of all experiments.

## How to build
All programs can be simply compiled with gcc : `gcc file.c -o file`

## How to run
Before running the code, there are several things that must be done to create an ideal noise-free system.

* Isolate one physical core from all processes. This can be done by adding the *isolcpus* kernel parameter in you grub file. Update your grub config and reboot.
* You'll also need to install msr-tools for linux in order to read/write to the model-specific registers for controlling the prefetchers. Running the script `prerun.sh` will disable all but the stream prefetcher for the first physical core (Edit it according to you isolated core number)
* For consistent rdtsc measurements:

    * Fix cpu-frequency for isolated core to max. [Link](https://unix.stackexchange.com/questions/424602/is-it-possible-to-set-a-constant-lowest-cpu-frequency-under-the-modern-pstate)
    * Don't reboot between readings
    
For specific programs (assuming first physical core is isolated):

1. taskset 0x1 <huge_page_binary> <hit/miss threshold is cycles></trigger line number>
    * `taskset 0x1 ./huge_page 150 60`
2. taskset 0x1 <rev_eng_experiments_bin> <hit/miss threshold is cycles> <attack/training stream on(1)/off(0)>
    * `taskset 0x1 ./rev_eng_experiments 150 1`
3. taskset 0x1 <tream_table_size_bin> <hit/miss threshold is cycles>
    * `taskset 0x1 ./stream_table_size 150`
4. For covert-channel:
    * taskset 0x1 ./recv <hit/miss threshold is cycles> </test line for received bit>
      * `taskset 0x1 ./recv 150 4`
    * `taskset 0x1 ./sender`

You can find out the hit/miss threshold using this [calibration tool](https://github.com/IAIK/cache_template_attacks).
Please refer the paper for more details and explanation of all experiments.
## TODO
* Add shared stream prefetcher experriment code
* Add link to the paper

#### *For any queries, ping me on [Linkedin](https://www.linkedin.com/in/aditya-rohan-95632a145)*

#### *This work was only possible due to the help and support of the co-authors [Biswa](https://www.cse.iitk.ac.in/users/biswap/) and [Prakhar](https://github.com/pkhrag)*

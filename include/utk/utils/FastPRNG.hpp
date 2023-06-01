#pragma once

namespace utk
{
    // Simple Linear Congruential Generator. 
    // For some reason, even though this is (more or less) the same as GCC, 
    // using rand/srand is 100 times slower on my machine... 
    template<uint32_t rng_a, uint32_t rng_c, uint32_t rng_m>
    struct FastRNGGenerator {
        FastRNGGenerator() :
                mstate(1u), mseed(1u) {
        }

        FastRNGGenerator(const uint32_t _seed) :
                mstate(_seed), mseed(_seed) {
        }

        void seed(const uint32_t _seed) {
            //~ mseed= _seed;
            mseed = hash(_seed);
            mstate = mseed;
        }

        void index(const int _index) {
            // advance
            // cf http://www.pcg-random.org, c++ implementation
            uint32_t cur_mul = rng_a;
            uint32_t cur_add = rng_c;
            uint32_t acc_mul = 1u;
            uint32_t acc_add = 0u;
            uint32_t delta = _index;
            while (delta > 0u) {
                if (delta & 1u) {
                    acc_mul = acc_mul * cur_mul;
                    acc_add = acc_add * cur_mul + cur_add;
                }

                //~ cur_add= (cur_mul+1u) * cur_add;
                cur_add = cur_mul * cur_add + cur_add;
                cur_mul = cur_mul * cur_mul;
                delta = delta >> 1u;
            }

            // advance current state
            // mstate= acc_mul * mstate + acc_add;

            // advance to sample index
            mstate = acc_mul * mseed + acc_add;
        }

        uint32_t operator()() {
            return sample();
        }

        uint32_t sample() {
            mstate = (mstate * rng_a + rng_c) % rng_m;
            return mstate;
        }

        double sample_double() {
            return sample() / double(rng_m);
        }

        uint32_t sample_range(const uint32_t range) {
            // Efficiently Generating a Number in a Range
            // cf http://www.pcg-random.org/posts/bounded-rands.html
            uint32_t divisor = rng_m / range;
            if (divisor == 0)
                return 0;

            while (true) {
                uint32_t x = sample() / divisor;
                if (x < range)
                    return x;
            }
        }

        protected:
            uint32_t mstate;
            uint32_t mseed;

            // cf http://www.burtleburtle.net/bob/hash/integer.html
            uint32_t hash(uint32_t a) {
                a = (a + 0x7ed55d16) + (a << 12);
                a = (a ^ 0xc761c23c) ^ (a >> 19);
                a = (a + 0x165667b1) + (a << 5);
                a = (a + 0xd3a2646c) ^ (a << 9);
                a = (a + 0xfd7046c5) + (a << 3);
                a = (a ^ 0xb55a4f09) ^ (a >> 16);
                return a;
        }
    };

    typedef FastRNGGenerator<1103515245u, 12345u, 1u << 31> RNG;
};
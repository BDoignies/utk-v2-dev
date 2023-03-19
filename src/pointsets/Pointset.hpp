/*
 * Hélène Perrier helene.perrier@liris.cnrs.fr
 * and David Coeurjolly david.coeurjolly@liris.cnrs.fr
 *
 * Copyright (c) 2018 CNRS Université de Lyon
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those
 * of the authors and should not be interpreted as representing official policies,
 * either expressed or implied, of the UTK project.
 */
#pragma once

#include <vector>
#include <algorithm>

namespace utk
{
    template<typename T>
    class Pointset
    {
    public:
        Pointset(unsigned int n, unsigned int d) 
        {
            D = d;
            N = d;
            data.resize(n * d);
        }

        T* Data()
        {
            return &data[0];
        }

        const T* Data() const
        {
            return &data[0];
        }

        T* operator[](unsigned int i) 
        {
            return &data[0] + i * D;
        }

        const T* operator[](unsigned int i) const
        {
            return &data[0] + i * D;
        }

        void swap(Pointset<T>& other)
        {
            std::swap(other.N, N);
            std::swap(other.D, D);
            std::swap(other.data, data);
        }

        unsigned int Npts() const
        {
            return N;
        }

        unsigned int Ndim() const
        {
            return D;
        }

        void Fill(T value = 0) const
        {
            std::fill(data.begin(), data.end(), value);
        }

        void Resize(unsigned int n, unsigned int d)
        {
            N = n;
            D = d;
            data.resize(N * D);
        }
    private:
        unsigned int N;
        unsigned int D;
        std::vector<T> data; 
    };
};

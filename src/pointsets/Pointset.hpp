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

#include <cstring> // memcpy
#include <vector>
#include <iostream>
#include <algorithm>

namespace utk
{
    // Pointset class

    // Just a wrapper around a T*
    // It can be set to be a view or hold its own memory
    // The copy/view mecanism is introduced for seemless interaction with numpy (and python protocol buffer)
    // This also allows to use the library with 'plain double pointers'.

    template<typename T>
    class Pointset
    {
    public:
        Pointset()
        {
            D = 0;
            N = 0;
            isView = false;
            data = nullptr;
        }

        Pointset(unsigned int n, unsigned int d) 
        {
            isView = false;
            data = nullptr;
            Resize(n, d);
        }

        static Pointset<T> View(T* data, unsigned int n, unsigned int d)
        {
            Pointset<T> pt;
            pt.isView = true;
            pt.data = data;

            return pt;
        }

        static Pointset<T> Copy(const T* data, unsigned int n, unsigned int d)
        {
            Pointset<T> pt(n, d);
            pt.isView = false;
            std::memcpy(pt.data, data, sizeof(T) * n * d);
            return pt;
        }

        // Do not risk taking a view here, just use a copy
        Pointset(const Pointset& other)
        {
            N = 0;
            D = 0;
            isView = false;
            data = nullptr;
            Resize(other.N, other.D);
            std::memcpy(data, other.data, sizeof(T) * N * D);
        }

        // Do not risk taking a view here, just use a copy
        Pointset& operator=(const Pointset& other)
        {
            if (this != &other)
            {
                D = 0;
                N = 0;
                isView = false;
                data = nullptr;
                Resize(other.N, other.D);
                std::memcpy(data, other.data, sizeof(T) * N * D);   
            }
            return *this;
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
            std::swap(other.isView, isView);
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
            std::fill(data, data + N * D, value);
        }

        void Resize(unsigned int n, unsigned int d)
        {
            // Always realloc if it previously was a view
            if (isView)
            {
                // isView is true, do not delete here
                D = d;
                N = n;
                isView = false;

                data = new T[N * D];
            }
            else
            {
                // Realloc only if size do not match
                if (n != N || d != D)
                {
                    // Safe to delete, isView == false here
                    if (data != nullptr)
                        delete[] data;

                    D = d;
                    N = n;
                    isView = false;

                    data = new T[N * D];
                }
            }
        }

        ~Pointset()
        {      
            if (data != nullptr && !isView)
                delete[] data;
        }
    private:
        bool isView;
        unsigned int N;
        unsigned int D;

        T* data;

        // Can not set data's pointer... Or maybe with custom allocators
        // Anyway keeping the pointer is simpler.
        // std::vector<T> data; 
    };
};

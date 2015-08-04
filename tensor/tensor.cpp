#include <iostream>
#include <initializer_list>
#include <cmath>
#include <sstream>

#include "tensor.h"


int main(int argc, char **argv) {
    unsigned int dims = 3;
    Tensor<int> t(dims, {3, 3, 3});
    t.set({2, 1, 0}, 3);

    std::cout << t;
/*
    std::cout << &t << std::endl;
    std::cout << static_cast< int (Tensor<int>::*)(const unsigned int*)const>(&Tensor<int>::get) << std::endl;
    //std::cout << &(Tensor<int>::get) << std::enld;

    std::cout << t.to_string() << std::endl;
*/
    return 0;
}

template <typename T>
Tensor<T>::Tensor(unsigned int num_dims, const unsigned INDEXING_DATA_TYPE *size, Initialization init, double value) : num_dims(num_dims){

    /* Perform deep copy of size array 
     *
     * Simultaneously calculates the total number of elements. This method for 
     * calculating the number of elements is safe. Negative values are not 
     * allowed, and if one of the dimensions is zero, the number of elements 
     * will be zero. 
     */
    unsigned long long num_elements = 1;
    this->size = new unsigned INDEXING_DATA_TYPE[num_dims];
    for (unsigned INDEXING_DATA_TYPE *i = this->size; i < this->size + num_dims; ++i) {
        *i = *(size++);
        num_elements *= *i;
    }

    this->num_elements = num_elements;
    degenerate = num_elements == 0 ? true : false;

    // Allocate data
    data = new T[num_elements];

    // Initiate with appropriate elements
    initiate(init, value);
}

template <typename T>
Tensor<T>::Tensor(unsigned int num_dims, const std::initializer_list<unsigned INDEXING_DATA_TYPE>& size, Initialization init, double value) : Tensor(num_dims, size.begin(), init, value) {
    // EMPTY
}

template <typename T>
Tensor<T>::Tensor(const Tensor& other) : Tensor(0, 0) {

}

template <typename T>
Tensor<T>::~Tensor() {
    delete[] data;
    delete[] size;
    delete[] displ;
}

template <typename T>
void Tensor<T>::initiate(Initialization init, double value) {

    /* Calculate displacement array and jump value*/
    displ = new unsigned long long[num_dims];
    jump = 0;
    for (unsigned int i = 0; i < num_dims; ++i) {
        unsigned long long el = 1;
        for (unsigned int j = num_dims - 1; j > i; --j) {
            el *= size[j];
        }
        displ[i] = el;
        jump += el;
    }

    /* Initiate with appropriate content */
    T *data_temp = data;
    switch (init) {
    case IDENTITY:
        if (!is_square()) {
            throw 1;
        }
        for (T *i = data; i < data + num_elements; ++i) {
            *i = 0;
        }
        for (unsigned INDEXING_DATA_TYPE i = 0; i < size[0]; ++i) {
            *data_temp = 1;
            data_temp += jump;
        }
        break;
    case ZERO:
        value = 0;
    case CONSTANT:
        for (T *i = data; i < data + num_elements; ++i) {
            *i = value;
        }
        break;
    default:
        throw -1;
    }
}

template <typename T>
unsigned long long Tensor<T>::index(const unsigned INDEXING_DATA_TYPE *pos) const {
    unsigned long long index = 0;
    for (unsigned int i = 0; i < num_dims; ++i) {
        index += displ[i] * (*pos++);
    }
    return index;
}

template <typename T>
bool Tensor<T>::is_square() const {
    for (unsigned int i = 1; i < num_dims; ++i) {
        if (size[i] != size[0]) {
            return false;
        }
    }
    return true;
}

template <typename T>
unsigned int Tensor<T>::get_num_dims() const {
    return num_dims;
}

template <typename T>
unsigned long long Tensor<T>::get_num_elements() const {
    return num_elements;
}

template <typename T>
unsigned INDEXING_DATA_TYPE Tensor<T>::get_dim_size(unsigned int dimension) const {
    return size[index];
}

template <typename T>
T Tensor<T>::get(const std::initializer_list<unsigned INDEXING_DATA_TYPE>& pos) const {
    return get(pos.begin);
}

template <typename T>
T Tensor<T>::get(const unsigned INDEXING_DATA_TYPE *pos) const {
    return data[index(pos)];
}

/* Printing is expensive! */
template <typename T>
std::string Tensor<T>::to_string(unsigned int num_chars_el) const {
    if (num_chars_el == 0) {

        /* Estimate the number of characters per element of type T
         * 
         * Assume T is of integer type. This is a good solution for all primitive 
         * datatypes, as integer types are the longest to print. For user-defined 
         * datatypes this may result in a too large or too small buffer. In this
         * case the number should always be specified when calling to_string(). 
         */
        double mul_factor = 2.40823996531; // log_10(2^(sizeof(T) * 8)) = 2.408 * sizeof(T)
        num_chars_el = static_cast<unsigned int>(mul_factor * sizeof(T) + 1.0);
    } 

    /* Calculate number of passes and such */
    unsigned long int num_blocks = 1;
    unsigned long int block_size = 0;
    if (num_dims > 2) {
        // Integer division is safe, since displ[0] is divisable by displ[num_dims - 3]. 
        block_size = displ[num_dims - 3];
        num_blocks = size[0] * displ[0] / block_size;
    }

    /* Calculate and allocate buffer size */
    unsigned long long num_chars = ((static_cast<unsigned long long>(num_chars_el) + 1) * (block_size + num_dims - 2) + 10) * num_blocks;
    std::string s;
    s.reserve(num_chars);

    /* Begin string building */
    std::ostringstream sstream;
    T *data_temp = data;
    unsigned long int n = 0;
    for (unsigned long int block = 0; block < num_blocks; ++block) {
        if (num_dims > 2) {
            if (block > 0) {
                s += '\n';
            }
            s += "Layer: ";
            sstream << (n / displ[0]);
            for (unsigned int k = 1; k < num_dims - 2; ++k) {
                sstream << '-' << ((n / displ[k]) % size[k]);
            }
            sstream << '\n';
        }

        /* Print one 2x2 block */
        for (unsigned int i = 0; i < size[num_dims - 2]; ++i) {
            for (unsigned int j = 0; j < size[num_dims - 1]; ++j) {
                sstream << *(data_temp++) << " ";
            }
            sstream << '\n';
        }

        // Append to string
        s += sstream.str();
        n += block_size;

        // Clear stream
        sstream.str(std::string());
    }
    return s;
}

template <typename T>
void Tensor<T>::set(const std::initializer_list<unsigned INDEXING_DATA_TYPE>& pos, const T value) {
    return set(pos.begin(), value);
}

template <typename T>
void Tensor<T>::set(const unsigned INDEXING_DATA_TYPE *pos, const T value) {
    data[index(pos)] = value;
}

template <typename S>
std::ostream& operator <<(std::ostream &out, const Tensor<S> &t) {
    return out << t.to_string();
}


#include <iostream>
#include <initializer_list>
#include <cmath>
#include <sstream>
#include <algorithm>

#include "tensor.h"

/*
int main(int argc, char **argv) {
    unsigned int dims = 3;
    Tensor<double> t(dims, {3, 3, 3}, Tensor<double>::Initialization::CONSTANT, 736.384);

    std::cout << t.get({0, 0, 0}) << std::endl;
    std::cout << t.get({1, 3, 2}) << std::endl;
*/
/*
    std::cout << "test\n";
    std::cout << t.to_string().length() << std::endl;
    std::cout << t.get_num_elements() << std::endl;
    std::cout << t.to_string();
*/
/*
    std::cout << &t << std::endl;
    std::cout << static_cast< int (Tensor<int>::*)(const unsigned int*)const>(&Tensor<int>::get) << std::endl;
    //std::cout << &(Tensor<int>::get) << std::enld;

    std::cout << t.to_string() << std::endl;
*/
/*
    return 0;
}
*/

template <typename T>
Tensor<T>::Tensor(unsigned int num_dims, const unsigned INDEXING_DATA_TYPE *size, Initialization init, double value) : num_dims(num_dims){

    /* Check for illegal constructor arguments before allocating data. This
     * is the only place the constructor should be allowed to throw.
     */
    validate_input(num_dims, size, init, value);


    /* Perform deep copy of size array
     *
     * Simultaneously calculates the total number of elements. This method for
     * calculating the number of elements is safe. Negative values are not
     * allowed, and if one of the dimensions is zero, the number of elements
     * will be zero.
     *
     * It is also safe if the number of dimensions is zero. This method will
     * correctly set the number of elements to 1 and allocate an empty size
     * array, which should never be accessed.
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


    level = 0;
    temp = data;

    // Initiate with appropriate elements
    initiate(init, value);
}

template <typename T>
Tensor<T>::Tensor(unsigned int num_dims, const std::initializer_list<unsigned INDEXING_DATA_TYPE>& size, Initialization init, double value) : Tensor(num_dims, size.begin(), init, value) {
    // EMPTY
}

/*
template <typename T>
Tensor<T>::Tensor(const Tensor& other) : Tensor(0, 0) {

}
*/

template <typename T>
Tensor<T>::~Tensor() {
    delete[] data;
    delete[] size;
    delete[] displ;
}

template <typename T>
void Tensor<T>::validate_input(unsigned int num_dims, const unsigned INDEXING_DATA_TYPE *size, Initialization init, double value) const {

    /* Check if all dimension sizes are equal; only then is the tensor
     * considered square and can be initiated to identity. All tensors
     * of dimension less than 2 is square by definition. 
     */
    if (init == Initialization::IDENTITY && num_dims >= 2) {
        for (unsigned int i = 1; i < num_dims; ++i) {
            if (size[i] != size[0]) {
                throw 1; // Todo: better exception handling
            }
        }
    }
    return;
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
        // Valid shape should already be asserted
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
        // Unknown error
        throw -1;
    }
}

template <typename T>
unsigned long long Tensor<T>::index(const unsigned INDEXING_DATA_TYPE *pos) const {

    /* Validate index while performing lookup */
    unsigned long long index = 0;
    unsigned INDEXING_DATA_TYPE *size_temp = size;
    unsigned long long *displ_temp = displ;
    for (const unsigned INDEXING_DATA_TYPE *i = pos; i < pos + num_dims; ++i) {
        if (*i >= *(size_temp++)) {
            throw 1; // Todo: better exception handling
        }
        index += *(displ_temp++) * *i;
    }
    return index;
}

/* Tensor of dimension less than 2 is considered square
 * by definition. 
 */
template <typename T>
bool Tensor<T>::is_square() const {
    if (num_dims >= 2) {
        for (unsigned int i = 1; i < num_dims; ++i) {
            if (size[i] != size[0]) {
                return false;
            }
        }
    }
    return true;
}

template <typename T>
bool Tensor<T>::is_degenerate() const {
    return degenerate;
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
    return size[dimension];
}

template <typename T>
T Tensor<T>::get(const std::initializer_list<unsigned INDEXING_DATA_TYPE>& pos) const {
    return get(pos.begin());
}

template <typename T>
T Tensor<T>::get(const unsigned INDEXING_DATA_TYPE *pos) const {
    return data[index(pos)];
}

/* Printing is expensive! */
template <typename T>
std::string Tensor<T>::to_string(unsigned int num_chars_el) const {
    if (is_degenerate()) {
        return "<Tensor object is degenerate>\n";
    }
    if (num_chars_el == 0) {

        /* Estimate the number of characters per element of type T
         *
         * Assume T is of integer type. This is a good solution for all primitive
         * datatypes, as integer types are the longest to print. For user-defined
         * datatypes this may result in a too large or too small buffer. In this
         * case the number should always be specified when calling to_string().
         * 
         * The estimate finds the number of characters needed to display an 
         * integer of known byte size. The typical number of characters per element
         * is then chosen as half this value.
         */
        double mul_factor = 2.40823996531; // log_10(2^(sizeof(T) * 8)) = 2.408 * sizeof(T)
        num_chars_el = static_cast<unsigned int>(mul_factor * sizeof(T) / 2.0  + 1.0);
    }

    /* Calculate number of passes and such */
    unsigned long long num_blocks = 1; // Valid for num_dims <= 2
    unsigned long long block_size = 1; // Valid for num_dims == 1
    if (num_dims > 2) {
        // Integer division is safe, since displ[0] is divisable by displ[num_dims - 3].
        block_size = displ[num_dims - 3];
        num_blocks = size[0] * displ[0] / block_size;
    } else {
        block_size = num_elements;
    }

    /* Calculate and allocate buffer size */
    unsigned long long num_chars = static_cast<unsigned long long>(num_chars_el) + 1;
    if (num_dims > 2) {
        num_chars = (num_chars * (block_size + num_dims - 2) + 10) * num_blocks;
    } else {
        num_chars = num_chars * block_size;
    }
    std::string s;
    s.reserve(num_chars);
    std::cout << num_chars << std::endl;

    /* Begin string building */
    std::ostringstream sstream;
    T *data_temp = data;
    if (num_dims <= 1) {
        for (; data_temp < data + block_size; ++data_temp) {
            sstream << *(data_temp) << " ";
        }
        sstream << '\n';
        s += sstream.str();
        return s;
    }

    unsigned long long n = 0;
    for (unsigned long long block = 0; block < num_blocks; ++block) {
        if (num_dims > 2) {
            if (block > 0) {
                sstream << '\n';
            }
            sstream << "Layer: ";
            sstream << (n / displ[0]);
            for (unsigned int k = 1; k < num_dims - 2; ++k) {
                sstream << '-' << ((n / displ[k]) % size[k]);
            }
            sstream << '\n';
        }

        /* Print one 2D block */
        for (unsigned INDEXING_DATA_TYPE i = 0; i < size[num_dims - 2]; ++i) {
            for (unsigned INDEXING_DATA_TYPE j = 0; j < size[num_dims - 1]; ++j) {
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

template <typename T>
void Tensor<T>::test_mul(Tensor<T> &t, Tensor<T> &r) {
    //Tensor<T> rr(2, {size[0], size[0]});
    unsigned int N = size[0];

    T *temp;
    for (unsigned int i = 0; i < N; ++i) {
        for (unsigned int j = 0; j < N; ++j) {
            /*
            *(r[i][j]) = *((*this)[i][0]) * *(t[0][j]);
            for (unsigned int k = 1; k < N; ++k) {
                *(r[i][j]) += *((*this)[i][k]) * *(r[k][j]);
            }
            */
            /*
            T &temp = *(r[i][j]);
            temp = *((*this)[i][0]) * *(t[0][j]);
            for (unsigned int k = 1; k < N; ++k) {
                temp += *((*this)[i][k]) * *(r[k][j]);
            }
            */

            /*
            temp = r.q_get(i, j);
            *temp = *(this->q_get(i, 0)) * *(t.q_get(0, j));
            for (unsigned int k = 1; k < N; ++k) {
                *temp += *(this->q_get(i, k)) * *(t.q_get(k, j));
            }
            */
            /*
            temp = rr.qq_get({i, j});
            *temp = *(this->qq_get({i, 0})) * *(t.qq_get({0, j}));
            for (unsigned int k = 1; k < N; ++k) {
                *temp += *(this->qq_get({i, k})) * *(t.qq_get({k, j}));
            }
            */
            temp = r.qqq_get({i, j});
            *temp = *(this->qqq_get({i, 0})) * *(t.qqq_get({0, j}));
            for (unsigned int k = 1; k < N; ++k) {
                *temp += *(this->qqq_get({i, k})) * *(t.qqq_get({k, j}));
            }

        }
    }
}

template <typename T>
Tensor<T>& Tensor<T>::operator [](INDEXING_DATA_TYPE i) {
    if (level == num_dims) {
        temp = data;
        level = 0;
    }
    temp += i * displ[level++];
    return *this;
}

template <typename T>
T& Tensor<T>::operator *() {
    return *temp;
}

template <typename T>
T* Tensor<T>::q_get(unsigned int i, unsigned int j) {
    return data + i * displ[0] + j;
}

template <typename T>
T* Tensor<T>::qq_get(const std::initializer_list<unsigned int>& pos) {
    unsigned long long *temp = displ;
    T *data_temp = data;
    for (unsigned int i : pos) {
        data_temp += i * *(temp++);
    }
    return data_temp;
}

template <typename T>
T* Tensor<T>::qqq_get(const std::initializer_list<unsigned int>& pos) {
    unsigned long long *temp = displ;
    T *data_temp = data;
    std::for_each(pos.begin(), pos.end(), 
        [temp, &data_temp] (unsigned int i) mutable {
            data_temp += i * *(temp++);
        }
    );
    return data_temp;
}

template <typename T>
template <typename ValueType>
Tensor<T>::iterator_template<ValueType>::iterator_template() {
    std::cout << "cstr itr\n";

    typedef typename Tensor<T>:: template iterator_template<ValueType>::pointer pointer;
    std::cout << typeid(pointer).name() << '\n';
}


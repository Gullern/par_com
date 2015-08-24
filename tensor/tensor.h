#ifndef TENSOR_H
#define TENSOR_H

#include <initializer_list>
#include <string>
#include <iterator>

#define INDEXING_DATA_TYPE int

template <typename T>
class Tensor {
public:
    /* Defines types of initialization of tensor elements */
    enum Initialization { ZERO, IDENTITY, CONSTANT };

    template <typename ValueType>
    class iterator_template : public std::iterator<std::random_access_iterator_tag, ValueType> {
    public:
        iterator_template();
    };
private:
    /* Data */
    T *data;

    /* Shape information */
    unsigned int num_dims;
    unsigned long long num_elements;
    unsigned INDEXING_DATA_TYPE *size;
    unsigned long long *displ;
    unsigned long long jump;
    bool degenerate;

    T *temp;
    unsigned int level;

    /* Non-const functions */
    void initiate(Initialization init, double value = 0);

    /* Const functions */
    unsigned long long index(const unsigned INDEXING_DATA_TYPE *pos) const;
    void validate_input(unsigned int num_dims, const unsigned INDEXING_DATA_TYPE *size, Initialization init, double value) const;

public:
    /* Constructors and destructors */
    Tensor(unsigned int num_dims, const unsigned INDEXING_DATA_TYPE *size, Initialization init = Initialization::ZERO, double value = 0);
    Tensor(unsigned int num_dims, const std::initializer_list<unsigned INDEXING_DATA_TYPE>& size, Initialization init = Initialization::ZERO, double value = 0);
    //Tensor(const Tensor& other);
    ~Tensor() noexcept;

    /* Const functions */
    bool is_square() const;
    bool is_degenerate() const;
    unsigned int get_num_dims() const;
    unsigned long long get_num_elements() const;
    unsigned INDEXING_DATA_TYPE get_dim_size(unsigned int dimension) const;
    T get(const std::initializer_list<unsigned INDEXING_DATA_TYPE>& pos) const;
    T get(const unsigned INDEXING_DATA_TYPE *pos) const;

    std::string to_string(unsigned int max_number_of_elements_per_character = 0) const;

    /* Non-const fnctions */
    void set(const std::initializer_list<unsigned INDEXING_DATA_TYPE>& pos, const T value);
    void set(const unsigned INDEXING_DATA_TYPE *pos, const T value);

    /* Operators */
    template <typename S>
    friend std::ostream& operator <<(std::ostream &out, const Tensor<S> &t);

    Tensor<T>& operator [](INDEXING_DATA_TYPE i);
    T& operator *();
    T* q_get(unsigned int i, unsigned int j);
    T* qq_get(const std::initializer_list<unsigned int> &pos);
    T* qqq_get(const std::initializer_list<unsigned int> &pos);
    /*
    Tensor& operator=(const Tensor& rhs);
    Tensor& operator+=(const Tensor& rhs);
    Tensor& operator-=(const Tensor& rhs);
    Tensor& operator*=(const Tensor& rhs);
    Tensor& operator+(const Tensor& rhs);
    Tensor& operator-(const Tensor& rhs);
    Tensor& operator*(const Tensor& rhs);
    */

    /* Math */
    //Tensor<T>* test_mul(Tensor &t);
    void test_mul(Tensor<T> &t, Tensor<T> &r);

    typedef iterator_template<T> iterator;
    typedef iterator_template<const T> const_iterator;
};

#include "tensor_impl.h"

#endif

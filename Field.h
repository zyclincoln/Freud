#ifndef FIELD_H
#define FIELD_H
#include <vector>
#include <QObject>
#include <algorithm>

template<typename T, size_t dim>
class Field
{
public:
    Field(){
        dim_ = 0;
    };

    Field(int w, int h):w_(w), h_(h),dim_(dim){
        data_.resize(dim, std::vector<T>(w*h, 0));
    }

    T sum() const{
        T s = 0;
        for(int d = 0; d < dim_; ++d){
            for(int i = 0; i < data_[d].size(); ++i)
                s += data_[d][i];
        }
        return s;
    }

    inline
    T operator()(int x, int y, int dim) const {
        return data_[dim][x+y*w_];
    }

    inline
    T& operator()(int x, int y, int dim) {
        return data_[dim][x+y*w_];
    }

    inline
    T operator()(int pos, int dim) const {
        return data_[dim][pos];
    }

    inline
    T& operator()(int pos, int dim){
        return data_[dim][pos];
    }

    inline
    int get_width() const{
        return w_;
    }

    inline
    int get_height() const{
        return h_;
    }

    inline
    int get_size() const {
        return w_*h_;
    }

//private:
public:
    std::vector<std::vector<T>> data_;
    int w_, h_;
    int dim_;
};

#endif // FIELD_H

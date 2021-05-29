#pragma once
#include "qcustomplot.h"
#include <stack>
#include <cmath>

struct DataDecimator
{
    using storage_t = QSharedPointer<QCPGraphDataContainer>;
    using const_iterator = storage_t::value_type::const_iterator;

    static storage_t douglas_peucker(const storage_t& storage, double d);
    static storage_t reumann_witkam(const storage_t& storage, double d);
    static storage_t llb(const storage_t& storage, double bucket_count);
    static storage_t largest_triangle(const storage_t& storage, double bucket);
    static storage_t longest_line(const storage_t& storage, double bucket);
    static storage_t opheim(const storage_t& storage, double d);
    static storage_t lang(const storage_t& storage, double d);
    static storage_t swinging_door(const storage_t& storage, double d);
};

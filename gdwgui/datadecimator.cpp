#include "datadecimator.h"
#include "util.h"

// opc.tcp://192.168.56.1:62456
DataDecimator::storage_t DataDecimator::douglas_peucker(const DataDecimator::storage_t &storage, double d)
{
    if(storage->isEmpty() || d == 0) {
        return storage_t { new storage_t::value_type(*storage) };
    }
    storage_t dec_storage { new storage_t::value_type };
    dec_storage->add(*storage->begin());
    std::stack<std::pair<const_iterator, const_iterator>> stack;
    stack.push({ storage->constBegin(), storage->constEnd() });
    while(!stack.empty()) {
        auto range = stack.top();
        auto last = std::prev(range.second);
        double k1 = last->value  - range.first->value;
        double k2 = last->key - range.first->key;
        double l  = std::sqrt(k1 * k1 + k2 * k2);
        double k3 = (last->key/l) * range.first->value - (range.first->key/l) * last->value;
        double maxDist = 0;
        const_iterator maxIt;
        for(auto it = std::next(range.first); it < last; ++it) {
            auto dist = std::abs(k1*(it->key/l) - k2*(it->value/l) + k3);
            if(dist > maxDist) {
                maxDist = dist;
                maxIt = it;
            }
        }
        stack.pop();
        //messageBox(QString::number(maxDist) + '\n' + QDateTime::fromTime_t(maxIt->mainKey()).toString());
        if(maxDist >= d) {
            stack.push({ maxIt, range.second });
            dec_storage->add(*maxIt);
            stack.push({ range.first, std::next(maxIt) });
        }
    }
    dec_storage->add(*std::prev(storage->end()));
    dec_storage->sort();
    return dec_storage;
}

DataDecimator::storage_t DataDecimator::largest_triangle(const storage_t &storage, double bucket)
{
    storage_t dec_storage { new storage_t::value_type };
    if(!storage->isEmpty())
    {
        int bucket_count = 500;
        int values_size = storage->size();
        int size = (values_size - 2) / bucket_count;
        const_iterator maxIt;
        const_iterator It{static_cast<const_iterator>(std::next(storage->begin()))};

        if(((values_size - 2) % bucket_count) > 0)
            ++bucket_count;

        dec_storage->add(*storage->constBegin());

        for(unsigned i = 0; i < bucket_count; ++i) {
            std::pair<double, double> mean_of_bucket{0, 0};
            auto It1 = std::next(It, size + 1);

            auto It_tmp = std::next(It1, size + 1);

            for(; It1 < It_tmp && It1 < storage->constEnd(); ++It1) {
                mean_of_bucket.first += It1->key;
                mean_of_bucket.second += It1->value;
            }

            mean_of_bucket.first /= size;
            mean_of_bucket.second /= size;

            double triangle_area = 0;
            double a_side = mean_of_bucket.first - dec_storage->constEnd()->key;
            double b_side = mean_of_bucket.second - dec_storage->constEnd()->value;

            It_tmp = std::next(It, size + 1);

            for(; It < It_tmp && It < storage->constEnd(); ++It) {
                double area = std::abs(b_side * It->key - a_side * It->value +
                                       mean_of_bucket.first * dec_storage->constEnd()->value -
                                       mean_of_bucket.second * dec_storage->constEnd()->key) / 2;
                if(area > triangle_area) {
                    triangle_area = area;
                    maxIt = It;
                }
            }
            dec_storage->add(*maxIt);
        }
        dec_storage->add(*storage->constEnd());
    }
    return dec_storage;
}

DataDecimator::storage_t DataDecimator::longest_line(const storage_t& storage, double bucket)
{
    storage_t dec_storage { new storage_t::value_type };
    if(!storage->isEmpty()) {
        int bucket_count = 500;
        int values_size = storage->size();
        int size = (values_size - 2) / bucket_count;
        const_iterator maxIt;
        const_iterator It{static_cast<const_iterator>(std::next(storage->begin()))};

        if(((values_size - 2) % bucket_count) > 0)
            ++bucket_count;

        dec_storage->add(*storage->constBegin());

        for(unsigned i = 0; i < bucket_count; ++i) {
            double side_length = 0;

            for(; It < std::next(It, size + 1) && It < storage->constEnd(); ++It) {
                double a_line = It->key - dec_storage->constEnd()->key;
                double b_line = It->value - dec_storage->constEnd()->value;
                double side_length_current = std::sqrt(a_line * a_line + b_line * b_line);

                if(side_length_current > side_length) {
                    side_length = side_length_current;
                    maxIt = It;
                    //dec_storage->add(*maxIt);
                }
            }
           dec_storage->add(*maxIt);
        }
        dec_storage->add(*storage->constEnd());
    }
    return dec_storage;
}

DataDecimator::storage_t DataDecimator::reumann_witkam(const DataDecimator::storage_t &storage, double d)
{
    storage_t dec_storage { new storage_t::value_type };
    if(storage->isEmpty()) {
        return dec_storage;
    }
    double ymax = storage->begin()->value;
    double ymin = storage->begin()->value;
    for(auto It = storage->begin(); It < storage->end(); ++It){
        if (It->value > ymax) ymax = It->value;
        if (It->value < ymin) ymin = It ->value;
    }
    double norm = (std::prev(storage->end())->key - storage->begin()->key) / (ymax - ymin);
    d = (std::prev(storage->end())->key - storage->begin()->key)  / 500;
    size_t look_ahead = std::max(static_cast<unsigned long>((std::prev(storage->end())->key - storage->begin()->key) / 300), 1ul);
    dec_storage->add(*storage->begin());
    auto start { storage->begin() };
    double x1 = start->key;
    double y1 = start->value * norm;
    double x2 = ++start->key;
    double y2 = ++start->value * norm;
    dec_storage->add(*start);
    for(auto It = std::next(start); It < storage->end(); ++It){
        double x = It->key;
        double y = It->value * norm;
        double dx = x2 - x1;
        double dy = y2 - y1;
        double h = std::abs( dx*(y - y1) - dy*(x - x1) ) / std::sqrt(dx*dx + dy*dy);
        if (h > d || std::distance(start, It) > look_ahead){
            start = It;
            x1 = x2;
            y1 = y2;
            x2 = x;
            y2 = y;
            dec_storage->add(*It);
        }
    }
    dec_storage->add(*std::prev(storage->end()));
    return dec_storage;
}


DataDecimator::storage_t DataDecimator::opheim(const DataDecimator::storage_t& storage, double d)
{
    if(storage->isEmpty() || d == 0) {
        return storage_t { new storage_t::value_type(*storage) };
    }
    storage_t dec_storage { new QCPGraphDataContainer };
    double ymax = storage->begin()->value;
    double ymin = storage->begin()->value;
    for(auto It = storage->begin(); It < storage->end(); ++It){
        if (It->value > ymax) ymax = It->value;
        if (It->value < ymin) ymin = It ->value;
    }
    double norm = (std::prev(storage->end())->key - storage->begin()->key) / (ymax - ymin);
    d = ((std::prev(storage->end())->key - storage->begin()->key) / 35);
    dec_storage->add(*storage->begin());
    auto st { storage->begin() };
    double x1 = st->key;
    double y1 = st->value * norm;
    bool f = false;
    for(auto It = ++st; It < std::prev(storage->end()); ++It) {
        double x = It->key;
        double y = It->value * norm;
        auto dx = x - x1;
        auto dy = y - y1;
        double l = std::sqrt(dx * dx + dy * dy);
        if(l < d) {
            st  = It;
            f   = true;
        }
        else if(f) {
            dec_storage->add(*st);
            x1  = st->key;
            y1  = st->value * norm;
            f   = false;
        }
        else {
            x1  = x;
            y1  = y;
            dec_storage->add(*It);
        }
    }
    dec_storage->add(*std::prev(storage->end()));
    return dec_storage;
 }

DataDecimator::storage_t DataDecimator::lang(const DataDecimator::storage_t& storage, double d)
{
    if(storage->isEmpty() || d == 0) {
        return storage_t { new storage_t::value_type(*storage) };
    }
    double ymax = storage->begin()->value;
    double ymin = storage->begin()->value;
    for(auto It = storage->begin(); It < storage->end(); ++It){
        if (It->value > ymax) ymax = It->value;
        if (It->value < ymin) ymin = It ->value;
    }
    double norm = (std::prev(storage->end())->key - storage->begin()->key) / (ymax - ymin);
    d = ((std::prev(storage->end())->key - storage->begin()->key) / 40);
    size_t look_ahead = std::max(static_cast<unsigned long>((std::prev(storage->end())->key - storage->begin()->key) / 600), 1ul);
    storage_t dec_storage { new storage_t::value_type };
    dec_storage->add(*storage->begin());
    auto finish { std::next(storage->begin(), look_ahead) };
    decltype(finish) nextIt;
    for(auto start = storage->begin(); finish < std::prev(storage->end()); start = nextIt) {
        double x1   = start->key;
        double y1   = start->value * norm;
        double dx   = finish->key - x1;
        double dy   = (finish->value - y1) * norm;
        double maxH = 0;
        nextIt = finish;
        for(auto it = ++start; it < finish; ++it) {
            double h = std::abs(dx * (it->value * norm - y1) - dy * (it->key - x1)) / std::sqrt(dx * dx + dy * dy);
            if (h > d && h > maxH){
                maxH    = h;
                nextIt  = it;
            }
        }
        finish  = std::next(finish, std::distance(start, nextIt) + 1);
        dec_storage->add(*nextIt);
    }
    dec_storage->add(*std::prev(storage->end()));
    return dec_storage;
}

DataDecimator::storage_t DataDecimator::swinging_door(const DataDecimator::storage_t& storage, double d)
{
    if(storage->isEmpty() || d == 0) {
        return storage_t { new storage_t::value_type(*storage) };
    }
    double ymax = storage->begin()->value;
    double ymin = storage->begin()->value;
    for(auto It = storage->begin(); It < storage->end(); ++It){
        if (It->value > ymax) ymax = It->value;
        if (It->value < ymin) ymin = It ->value;
    }
    double norm = (std::prev(storage->end())->key - storage->begin()->key) / (ymax - ymin);
    d = (std::prev(storage->end())->key - storage->begin()->key) / 20;
    storage_t dec_storage { new storage_t::value_type };
    dec_storage->add(*storage->begin());
    size_t h = (std::prev(storage->end())->key - storage->begin()->key) / 500;
    auto start = storage->begin();
    double xA   = start->key;
    double yA   = start->value * norm + d;
    double xB   = start->key;
    double yB   = start->value * norm - d;
    double xC   = std::next(start)->key;
    double yC   = std::next(start)->value * norm;
    for(auto it = std::next(storage->begin()); it < storage->end(); ++it) {
        if(it==std::next(start)){
            xC = std::next(it)->key;
            yC = std::next(it)->value * norm;
        }
        else{
            double x   = it->key;
            double y   = it->value * norm;
            double v1 = (xA - x) * (yB - yA) - (xB - xA) * (yA - y);
            double v2 = (xB - x) * (yC - yB) - (xC - xB) * (yB - y);
            double v3 = (xC - x) * (yA - yC) - (xA - xC) * (yC - y);
            if (!((v1 > 0 && v2 >0 && v3 >0) || (v1<0 && v2<0 && v3<0))){
                double k1, b1,k2,b2;
                if (y < yC){
                    k1=(y - yB) / (x - xB);
                    b1 = -k1 * xB + yB;
                    k2= (yC - yA) / (xC - xA);
                    b2 = -k2 * xA + yA;

                } else{
                    k1=(yC - yB) / (xC - xB);
                    b1 = -k1 * xB + yB;
                    k2= (y - yA) / (x - xA);
                    b2 = -k2 * xA + yA;

                }
                if ((b2 - b1) / (k1 - k2) < xA || std::abs(k1 - k2)<0.000000001 || std::distance(start, it) > h){
                    dec_storage->add(*it);
                    start = it;
                    xA = x;
                    yA = y + d;
                    xB = x;
                    yB = y - d;
                }
                else{
                    xC = (b2 - b1) / (k1 - k2);
                    yC = k1 * x + b1;
                }
            }
        }

    }
    dec_storage->add(*std::prev(storage->end()));
    return dec_storage;
}

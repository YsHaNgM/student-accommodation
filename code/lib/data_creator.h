#ifndef data_creator_h
#define data_creator_h

#include "basic_types.h"

namespace sas

{
    class data_creator
    {
    public:
        virtual ~data_creator() = default;
        virtual Incidence get_data() = 0;
    };
} // namespace sas

#endif
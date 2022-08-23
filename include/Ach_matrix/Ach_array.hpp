#ifndef __ACH_ARRAR_HPP__
#define __ACH_ARRAR_HPP__

#include <cstring>

namespace Ach_Net::inline Ach_array
{
    template <typename Type>
    class array
    {
    private:
        Type *arr;
        int length;

    public:
        array()
        {
            arr = nullptr;
            length = -1;
        }
        array(int size) : length(size)
        {
            arr = new Type[size];
            std::memset(arr, 0, length);
        }
        array(const array<Type> &t)
        {
            length = t.length;
            arr = new Type[length];
            for (int i = 0; i < length; ++i)
            {
                arr[i] = t.arr[i];
            }
        }
        ~array()
        {
            delete[] arr;
        }
        Type &operator[](int index)
        {
            if (index > length)
            {
                printf("Overflow!");
                abort();
            }
            return arr[index];
        }
    };

} // namespace Ach_Net::inline Ach_array

#endif
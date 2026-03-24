#ifndef SEGMENT_INDEX_EXCEPTION_H
#define SEGMENT_INDEX_EXCEPTION_H

#include <exception>

class SegmentIndexException : public std::exception {
public:
    const char* what() const noexcept {
        return "[ ERROR ]: Некорректный индекс сегмента корабля.\n";
    }
};

#endif SEGMENT_INDEX_EXCEPTION_H
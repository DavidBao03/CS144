#include "byte_stream.hh"
#include <iostream>
using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ) {}

bool Writer::is_closed() const
{
    return is_closed_;
}

void Writer::push( string data )
{
    uint64_t len = min(available_capacity_, data.size());

    if(len < data.size()) {
        data = data.substr( 0, len );
    }

    if(len > 0) {
        bytes_pushed_ += len;
        available_capacity_ -= len;
        bytes_buffered_ += len;

        stream_.emplace_back(move(data));
        stream_view_.emplace_back(stream_.back());
    }

    return;
}

void Writer::close()
{
    is_closed_ = true;
}

uint64_t Writer::available_capacity() const
{
    return available_capacity_;
}

uint64_t Writer::bytes_pushed() const
{
    return bytes_pushed_;
}

bool Reader::is_finished() const
{
    return is_closed_ && bytes_buffered_ == 0;
}

uint64_t Reader::bytes_popped() const
{
    return bytes_popped_;
}

string_view Reader::peek() const
{
    return stream_view_.front();
}

void Reader::pop( uint64_t len )
{
    len = min(bytes_buffered_, len);

    bytes_popped_ += len;
    available_capacity_ += len;
    bytes_buffered_ -= len;

    while(len > 0){
        auto size = stream_view_.front().size();
        if(len >= size){
            stream_view_.pop_front();
        } else {
            stream_view_.front().remove_prefix( len );
            break;
        }
        len -= size;
    }

    return;
}

uint64_t Reader::bytes_buffered() const
{
    return bytes_buffered_;
}

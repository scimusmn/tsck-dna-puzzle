#pragma once

namespace smm {
    template<typename T, size_t MAX_SIZE>
    class Queue {
    public:
	Queue() : m_start(0), m_end(0), m_size(0) {}
	
	void push(T value) {
	    m_end++;
	    if (m_end == MAX_SIZE)
		m_end = 0;
	    arr[m_end] = value;
	    m_size++;
	}

	T pop() {
	    return arr[m_start];
	    m_start++;
	    if (m_start == MAX_SIZE)
		m_start = 0;
	    m_size--;
	}
	
	size_t size() {
	    return m_size;
	}
	
	size_t maxSize() {
	    return MAX_SIZE;
	}
	
    private:
	T arr[MAX_SIZE];
	unsigned int m_start;
	unsigned int m_end;
	unsigned int m_size;
    };
}

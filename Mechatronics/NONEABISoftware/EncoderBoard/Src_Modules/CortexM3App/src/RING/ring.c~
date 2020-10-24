#include "ring.h"

int ring_buffer_init(ring_t *ring, void *buf, size_t buf_elements_size, size_t arrayByteSize){

  //Check passed pointers against null
  if((ring != NULL) && (buf != NULL)){
    //Check passed values
    if((buf_elements_size > 0) && (arrayByteSize > 0)){

      uint16_t n_elements = arrayByteSize/buf_elements_size;
      
      //Check nelements is a power of 2
      if(((n_elements-1) & n_elements) == 0){
	ring->s_elem = buf_elements_size;
	ring->n_elem = n_elements;
	ring->buf = buf;
	ring->head = 0;
	ring->tail = 0;
	return 0;
      }
    }
  }

  return -1;
}

int ring_buffer_full(ring_t *ring){

  return ((ring->head - ring->tail) == ring->n_elem)? 1:0;

}


int ring_buffer_empty(ring_t *ring){
  return ((ring->head - ring->tail) == 0)? 1:0;
}

inline void memcopy(void *destref, const void *srcref, size_t count){
  uint8_t *dest = (uint8_t*)destref;
  uint8_t *src = (uint8_t*)srcref;

  while(count--){
    *dest++ = *src++;
  }
}

int ring_buffer_put(ring_t *ring, const void *data){
  int status = -1;
  
  //Chek if can insert
  if(!ring_buffer_full(ring)){
    //Determine insertion position
    const size_t position = (ring->head & (ring->n_elem -1)) * ring->s_elem;
    //insert data at right position
    memcopy((uint8_t*)ring->buf + position,data,ring->s_elem);
    ring->head++;

    status = 0;
  }
  return status;  
}


int ring_buffer_get(ring_t *ring, void *data){
  int status = -1;
  
  //Chek if can insert
  if(!ring_buffer_empty(ring)){
    //Determine insertion position
    const size_t position = (ring->tail & (ring->n_elem -1)) * ring->s_elem;
    //insert data at right position
    memcopy(data, (uint8_t *)ring->buf + position, ring->s_elem);
    ring->tail++;

    status = 0;
    
  }
  return status;
}

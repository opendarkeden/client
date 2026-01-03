#ifndef BIT_RES_H
#define BIT_RES_H

#define BUFSIZE 4096

class Bit_Reserve {

private:

	unsigned int offset, totbit, buf_byte_idx;
	unsigned int *buf;
	unsigned int buf_bit_idx;
   int *putmask;

public:

	Bit_Reserve();
	~Bit_Reserve();

	unsigned int hsstell() { return(totbit); }
	unsigned int hgetbits(int N);
	void hputbuf(unsigned int val);

	void rewindNbits(int N);
	void rewindNbytes(int N);
};

#endif
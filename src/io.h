#ifndef _CHI_IO_H
#define _CHI_IO_H

#include "buffer.h"
#include "stream.h"
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>


namespace chi {

	class IoException : public StreamException {};

	class UnknownIoException : public IoException {

		int code;

	public:
		UnknownIoException() : code( errno ) {}

		const int& errorCode() const	{ return this->code; }
	};

	class FileStream : public virtual Stream {
	protected:
		int fd;

		FileStream( int fd = 0 ) : fd(fd) {}

	public:
		void close() {
			if ( ::close( this->fd ) != 0 )
				throw UnknownIoException();
		}
	};

	class ReadFileStream : public virtual FileStream, public virtual ReadStream {
	public:
		ReadFileStream() {}
		ReadFileStream( const char* filename ) {
			this->fd = open( filename, O_RDONLY );
			if ( this->fd == -1 )	throw UnknownIoException();
		}

		void read( BufferBase& buffer ) {
			if ( buffer.count() == 0 )	return;

			int read = ::read( this->fd, buffer.ptr(), buffer.count() );
			if ( read == -1 )	throw UnknownIoException();
			if ( read == 0 )	throw EndOfStreamException();

			if ( (chi::Size)read < buffer.count() )
				buffer.shrink( buffer.count() - read );
		}

		chi::Byte readByte() {
			chi::Byte byte;
			int read = ::read( this->fd, &byte, 1 );
			if ( read == -1 )	throw UnknownIoException();
			if ( read == 0 )	throw EndOfStreamException();

			return byte;
		}
	};

	class WriteFileStream : public virtual FileStream, public virtual WriteStream {
	public:
		WriteFileStream() {}

		chi::Size write( const chi::Buffer<>& buffer ) override {
			int written = ::write( this->fd, buffer.ptr(), buffer.size() );
			if ( written == -1 )	throw UnknownIoException();
			
			return written;
		}

		bool writeByte( Byte byte ) override {
			int written = ::write( this->fd, &byte, 1 );
			if ( written == -1 )	throw UnknownIoException();
			return written == 1;
		}
	};

	class SeekFileStream : public virtual FileStream, public virtual Seekable {
	public:
		Size position() const {
			off_t offset = lseek( this->fd, 0, SEEK_CUR );
			if ( offset == (off_t)-1 )	throw UnknownIoException();
			return offset;
		}

		void seek( Size position ) {
			off_t offset = lseek( this->fd, position, SEEK_SET );
			if ( offset == (off_t)-1 )	throw UnknownIoException();
		}

		void move( SSize position ) {
			off_t offset = lseek( this->fd, position, SEEK_CUR );
			if ( offset == (off_t)-1 )	throw UnknownIoException();
		}
	};

	class StderrStream : public WriteFileStream {
	public:
		StderrStream() : FileStream( 2 ) {}
	};

	class StdinStream : public ReadFileStream {
	public:
		StdinStream() : FileStream( 0 ) {}
	};

	class StdoutStream : public WriteFileStream {
	public:
		StdoutStream() : FileStream( 1 ) {}
	};

}

#endif//_CHI_IO_H
